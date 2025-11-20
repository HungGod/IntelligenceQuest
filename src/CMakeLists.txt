# -----------------------------------------------------------------------------
# IQ_rewrite ‒ target definition only
# -----------------------------------------------------------------------------
# ‣ The *project()* statement lives in the root‑level CMakeLists.txt.
# ‣ All third‑party libraries (glfw, glad, glm, miniaudio) are created there and
#   linked in the root after this sub‑directory is processed, so we do not need
#   to reference them here.
# -----------------------------------------------------------------------------

# ❶--------------------------------------------------------------------------
# Source‐file organisation (kept only for IDE navigation)
# -----------------------------------------------------------------------------
# We keep your original variables (Commands, EC, …) because they make CLion’s
# Project pane tidy, but we strip the MSVC‑only source_group back‑slashes.
# -----------------------------------------------------------------------------

# --- Commands -----------------------------------------------------------------
set(Commands
        command-add_flag.h
        command-add_intelligence.h
        command-add_kerning.h
        command-backtrack.h
        command-change_state.h
        command-create_components.h
        command-delay_cycles.h
        command-delay_time.h
        command-exit_game.h
        command-init_quadtree.h
        command-load_files.h
        command-move.h
        command-navigate_textbox.h
        command-normalize_srcs.h
        command-parse_json_template.h
        command-pause.h
        command-play_animation.h
        command-print_gamestate.h
        command-repeated_event.h
        command-save_state.h
        command-series.h
        command-simultaneous.h
        command-smart_enough.h
        command-static_move.h
        command-update_components.h
        command-update_message.h
        command-update_navigate.h
        command-update_options.h
        command-update_speech_arrow.h
        command-zoom.h
        command.h)
source_group("Commands" FILES ${Commands})

set(Commands_resolve
        command-resolve-displace.h
        command-resolve-message.h)
source_group("Commands/resolve" FILES ${Commands_resolve})

# --- EC and sub‑folders -------------------------------------------------------
set(EC
        component-animation.h
        component-animation_map.h
        component-box.h
        component-cache.h
        component-font_map.h
        component-hitboxes.h
        component-intelligence.h
        component-json_template_map.h
        component-old_renderer.cpp
        component-old_renderer.h
        component-pathway.cpp
        component-pathway.h
        component-position_map.h
        component-renderer.h
        component-shader.cpp
        component-shader.h
        component-template.h
        component-texture.cpp
        component-texture.h
        component-tilemap.cpp
        component-tilemap.h
        component-tileset.h
        component-vec2.h
        component.h
        entity.h
        templates.h)
source_group("EC" FILES ${EC})

set(EC_Block
        component-block-animation.h
        component-block-box.h
        component-block-dynamic_text.h
        component-block-image.h
        component-block-text.h
        component-block.h)
source_group("EC/Block" FILES ${EC_Block})

set(EC_Block_Options
        component-block-options-horizontal.h
        component-block-options-vertical.h
        component-block-options.h)
source_group("EC/Block/Options" FILES ${EC_Block_Options})

set(EC_Collider
        component-collider.cpp
        component-collider.h
        component-collider_mask.h
        component-quadtree.h)
source_group("EC/Collider" FILES ${EC_Collider})

set(EC_Collider_GJK
        component-gjk-aabb.h
        component-gjk-circle.h
        component-gjk-ellipse.h
        component-gjk-line_segment.h
        component-gjk-point.h
        component-gjk-polygon.h
        component-gjk.h)
source_group("EC/Collider/GJK" FILES ${EC_Collider_GJK})

set(EC_Controller
        component-controller-keyboard.h
        component-controller.h)
source_group("EC/Controller" FILES ${EC_Controller})

set(EC_Factory
        component-factory-command_map.cpp
        component-factory-command_map.h
        component-factory-create_component_map.cpp
        component-factory-create_component_map.h
        component-factory.h)
source_group("EC/Factory" FILES ${EC_Factory})

set(EC_Material
        component-material-font.h
        component-material-sprite.h
        component-material.h)
source_group("EC/Material" FILES ${EC_Material})

set(EC_Src
        component-src-bm_font.h
        component-src-flex.h
        component-src-texture_packer_gui.h
        component-src-transform.h
        component-src.h)
source_group("EC/Src" FILES ${EC_Src})

set(EC_System
        component-system.h)
source_group("EC/System" FILES ${EC_System})

set(EC_System_Render
        component-system-camera_block_draw.h
        component-system-camera_draw.h
        component-system-static_block_draw.h
        component-system-static_draw.h
        component-system-text_draw.h
        component-system-tilemap_draw.h
        component-system-zoom_camera_draw.h)
source_group("EC/System/Render" FILES ${EC_System_Render})

set(EC_System_Update
        component-system-animate.h
        component-system-animate_move.h
        component-system-broad_phase_collision.h
        component-system-camera.h
        component-system-check_action.h
        component-system-clear_quad_tree.h
        component-system-controller_change_direction.h
        component-system-execute_json.h
        component-system-insert_colliders.h
        component-system-move.h
        component-system-npc_move.h)
source_group("EC/System/Update" FILES ${EC_System_Update})

set(EC_System_Update_Combat
        component-system-combat-controller.h
        component-system-combat-gravity_physics.h
        component-system-combat-transformations.h
        component-system-combat_ability_factory.h
        component-system-combat_collisions.h)
source_group("EC/System/Update/Combat" FILES ${EC_System_Update_Combat})

set(EC_System_Update_Combat_Navigate
        component-system-navigate-fight.h)
source_group("EC/System/Update/Combat/Navigate" FILES ${EC_System_Update_Combat_Navigate})

set(EC_System_Update_Navigate
        component-system-navigate-horizontal.h
        component-system-navigate-message.h
        component-system-navigate-vertical.h
        component-system-navigate.h)
source_group("EC/System/Update/Navigate" FILES ${EC_System_Update_Navigate})

# --- Misc headers / sources ---------------------------------------------------
set(Header_Files
        fixed_splay_array.h
        game.h
        glyph.h
        json.hpp
        json_template.h
        logger.h
        rect.h
        ring.h
        splay_array.h
        stb_image.h
        switch_ring.h
        text.h)
source_group("Headers" FILES ${Header_Files})

set(Json_To
        json_to.cpp
        json_to.h)
source_group("Json To" FILES ${Json_To})

set(Resource_Files
        ../res/shaders/sprite.fs
        ../res/shaders/sprite.vs)
source_group("res" FILES ${Resource_Files})

set(Source_Files
        game.cpp
        "../include/glad/src/glad.c"
        logger.cpp
        main.cpp)
source_group("Source" FILES ${Source_Files})

# Collect everything into a single variable
set(ALL_FILES
        ${Commands}
        ${Commands_resolve}
        ${EC}
        ${EC_Block}
        ${EC_Block_Options}
        ${EC_Collider}
        ${EC_Collider_GJK}
        ${EC_Controller}
        ${EC_Factory}
        ${EC_Material}
        ${EC_Src}
        ${EC_System}
        ${EC_System_Render}
        ${EC_System_Update}
        ${EC_System_Update_Combat}
        ${EC_System_Update_Combat_Navigate}
        ${EC_System_Update_Navigate}
        ${Header_Files}
        ${Json_To}
        ${Resource_Files}
        ${Source_Files})

# ❷--------------------------------------------------------------------------
# Target definition (no Windows‑only helpers)
# -----------------------------------------------------------------------------
add_executable(IQ_rewrite ${ALL_FILES})

# ❸--------------------------------------------------------------------------
# Include directories – just the local folder; third‑party dirs come from the
# root‑level INTERFACE libraries (glad, glm, etc.)
# -----------------------------------------------------------------------------

target_include_directories(IQ_rewrite PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# ❹--------------------------------------------------------------------------
# Compile definitions / options common to all platforms (keep MSVC bits guarded)
# -----------------------------------------------------------------------------

if(MSVC)
    target_compile_definitions(IQ_rewrite PRIVATE _CONSOLE UNICODE _UNICODE)
    # You may add MSVC‑specific flags here if you need to keep Windows builds.
endif()
