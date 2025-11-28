#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <array>
#include <vector>
#include <glm/ext/matrix_clip_space.hpp>
#include "ec/component/component-renderer.h"
#include "ec/entity.h"
#include <sstream>
#include "game.h"
#include "ec/component/system/component-system.h"
#include "ec/component/component-pathway.h"
#include "ec/component/factory/component-factory-command_map.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

//Miniaudio docs https://miniaud.io/docs/examples/simple_playback.html

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void APIENTRY gl_debug_output(GLenum source, GLenum type, unsigned int id, GLenum severity,
    GLsizei length, const char* message, const void* userParam);
void key_callback(GLFWwindow* window, int key, int scan_code, int action, int mode);
void audio_callback(ma_device* p_device, void* p_output, const void* p_input, ma_uint32 frame_count);

constexpr auto max_keys = 1024;
constexpr auto game_filepath = "res/data/game.json";
Component::Template<std::array<bool, max_keys>>* keys;
Entity* e_game;

int main()
{
    Game game;
    e_game = game.init(game_filepath);
    keys = e_game->get_component<Component::Template<std::array<bool, max_keys>>>("keys");

    auto sc_width = e_game->get_component<Component::ValTemplate<float>>("width")->val;
    auto sc_height = e_game->get_component<Component::ValTemplate<float>>("height")->val;

    auto window_title = e_game->get_component<Component::ValTemplate<std::string>>("window_title")->val;

    auto& delta_time = e_game->get_component<Component::ValTemplate<float>>("delta_time")->val;
    auto& exit = e_game->get_component<Component::ValTemplate<bool>>("exit")->val;

    auto& render = *e_game->get_component<Component::Template<std::vector<ISystem*>>>("render");
    auto& update = *e_game->get_component<Component::Template<std::vector<ISystem*>>>("update");

    auto& pathway = *e_game->get_component<Component::Pathway>("pathway");
    auto command_map = e_game->get_component<Factory::CommandMap>("command_map");

    ma_result result;
    ma_decoder decoder;
    ma_device_config deviceConfig;
    ma_device device;
   
    result = ma_decoder_init_file("res/sfx/test_song.mp3", NULL, &decoder);
    if (result != MA_SUCCESS) {
        printf("Could not load file: %s\n", "res/sfx/test_song.mp3");
        return -2;
    }

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate = decoder.outputSampleRate;
    deviceConfig.dataCallback = audio_callback;
    deviceConfig.pUserData = &decoder;

    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
        printf("Failed to open playback device.\n");
        ma_decoder_uninit(&decoder);
        return -3;
    }

    if (ma_device_start(&device) != MA_SUCCESS) {
        printf("Failed to start playback device.\n");
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        return -4;
    }

    if (!glfwInit())
        Logger::error("failed to initialize glfw!", Logger::HIGH);

    // opengl version = major.minor
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    // use the deprecated functions? (CORE_PROFILE = yes, COMPATABILITY = no)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // show debug output with glfw
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    auto window = glfwCreateWindow((int)sc_width, (int)sc_height, window_title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(window);
    // make the window resizable and scale the renderer
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // set up so keyboard can interact with window
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        Logger::error("failed to initialize glad!", Logger::HIGH);

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(gl_debug_output, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    // normalize window to work on other devices
    glViewport(0, 0, (GLsizei)sc_width, (GLsizei)sc_height);


    //glDisable(GL_CULL_FACE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);         
    glFrontFace(GL_CCW);

    // set up alpha channel to display images beneath it.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    GLfloat last_frame = 0.0f;

    // game loop
    while (!glfwWindowShouldClose(window))
    {
        // calculate delta time
        auto current_frame = static_cast<GLfloat>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        
        for (auto u : update)
            u->execute();

        // clear screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (auto r : render)
            r->execute();

        pathway.navigate(e_game, command_map);
        if (exit)
            glfwSetWindowShouldClose(window, GL_TRUE);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);

    glfwTerminate();
    return 0;
}

void audio_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }

    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

    (void)pInput;
}

void key_callback(GLFWwindow* window, int key, int scan_code, int action, int mode)
{
    if (key >= 0 && key < max_keys)
    {
        // update our global keyboard object
        if (action == GLFW_PRESS)
            (* keys)[key] = GL_TRUE;
        else if (action == GLFW_RELEASE)
            (* keys)[key] = GL_FALSE;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    constexpr float TARGET_W = 800.0f;
    constexpr float TARGET_H = 640.0f;
    constexpr float TARGET_AR = TARGET_W / TARGET_H;          // 1.25

    float windowAR = static_cast<float>(width) / height;

    int vpX = 0, vpY = 0, vpW = width, vpH = height;

    if (windowAR > TARGET_AR) {
        // Window is *wider* than the target → pillar-box left/right
        vpH = height;
        vpW = static_cast<int>(height * TARGET_AR);
        vpX = (width - vpW) / 2;
    }
    else {
        // Window is *taller* than the target → letter-box top/bottom
        vpW = width;
        vpH = static_cast<int>(width / TARGET_AR);
        vpY = (height - vpH) / 2;
    }

    glViewport(vpX, vpY, vpW, vpH);

}

void APIENTRY gl_debug_output(GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char* message,
    const void* userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::stringstream ss;
    ss << "OpenGL error (" << id << "): " << message << std::endl;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             ss << "SOURCE: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   ss << "SOURCE: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: ss << "SOURCE: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     ss << "SOURCE: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     ss << "SOURCE: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           ss << "SOURCE: Other"; break;
    } ss << std::endl;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               ss << "TYPE: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: ss << "TYPE: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  ss << "TYPE: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         ss << "TYPE: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         ss << "TYPE: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              ss << "TYPE: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          ss << "TYPE: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           ss << "TYPE: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               ss << "TYPE: Other"; break;
    } ss << std::endl;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         ss << "GL SEVERITY: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       ss << "GL SEVERITY: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          ss << "GL SEVERITY: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: ss << "GL SEVERITY: notification"; break;
    }

    Logger::error(ss.str(), Logger::HIGH);
}
