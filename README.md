Latest working demo (https://wadsey.itch.io/intelligencequest)

A game where the protagonist gets more intelligent. (Me, hopefully)

## System Requirements
- **OpenGL 4.6** required (Linux and Windows distributions updated post 2014 should be compatible. MacOS is not.)

To check your OpenGL version, download the [OpenGL Extensions Viewer](https://www.realtech-vr.com/home/glview)

## How to Run
```bash
cmake -G Ninja -B build
cmake --build build
./build/src/IntelligenceQuest
```

## Dependencies Included
- GLAD (OpenGL 4.6)
- GLFW (latest)
- GLM
- miniaudio
- nlohmann::json
- stb_image
