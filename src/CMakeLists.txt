# -----------------------------------------------------------------------------
# IntelligenceQuest ‒ target definition only
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
        command/command-jump.h
        command/command-add_flag.h
        command/command-add_intelligence.h
        command/command-add_kerning.h
        command/command-backtrack.h
        command/command-change_state.h
        command/command-create_components.h
        command/command-delay_cycles.h
        command/command-delay_time.h
        command/command-exit_game.h
        command/command-init_quadtree.h
        command/command-load_files.h
        command/command-move.h
        command/command-navigate_textbox.h
        command/command-normalize_srcs.h
        command/command-parse_json_template.h
        command/command-pause.h
        command/command-play_animation.h
        command/command-print_gamestate.h
        command/command-repeated_event.h
        command/command-save_state.h
        command/command-series.h
        command/command-simultaneous.h
        command/command-smart_enough.h
        command/command-static_move.h
        command/command-update_components.h
        command/command-update_message.h
        command/command-update_navigate.h
        command/command-update_options.h
        command/command-update_speech_arrow.h
        command/command-zoom.h
        command/command-resolve-displace.h
        command/command-resolve-message.h
        command/command.h)
source_group("Commands" FILES ${Commands})

# --- EC and sub‑folders -------------------------------------------------------
set(EC
        ec/component/component-animation.h
        ec/component/component-animation_map.h
        ec/component/component-box.h
        ec/component/component-cache.h
        ec/component/component-font_map.h
        ec/component/component-hitboxes.h
        ec/component/component-intelligence.h
        ec/component/component-json_template_map.h
        ec/component/component-pathway.cpp
        ec/component/component-pathway.h
        ec/component/component-position_map.h
        ec/component/component-renderer.h
        ec/component/component-shader.cpp
        ec/component/component-shader.h
        ec/component/component-template.h
        ec/component/component-texture.cpp
        ec/component/component-texture.h
        ec/component/component-tilemap.cpp
        ec/component/component-tilemap.h
        ec/component/component-tileset.h
        ec/component/component-vec2.h
        ec/component/component-collider.cpp
        ec/component/component-collider.h
        ec/component/component-collider_mask.h
        ec/component/component-quadtree.h
        ec/component/component.h
        ec/entity.h)
source_group("EC" FILES ${EC})

set(EC_Block
        ec/component/block/component-block-animation.h
        ec/component/block/component-block-box.h
        ec/component/block/component-block-dynamic_text.h
        ec/component/block/component-block-image.h
        ec/component/block/component-block-text.h
        ec/component/block/component-block.h
        ec/component/block/component-block-options-horizontal.h
        ec/component/block/component-block-options-vertical.h
        ec/component/block/component-block-options.h)
source_group("EC/Block" FILES ${EC_Block})

set(EC_GJK
        ec/component/gjk/component-gjk-aabb.h
        ec/component/gjk/component-gjk-circle.h
        ec/component/gjk/component-gjk-ellipse.h
        ec/component/gjk/component-gjk-line_segment.h
        ec/component/gjk/component-gjk-point.h
        ec/component/gjk/component-gjk-polygon.h
        ec/component/gjk/component-gjk.h)
source_group("EC/GJK" FILES ${EC_GJK})

set(EC_Controller
        ec/component/controller/component-controller-keyboard.h
        ec/component/controller/component-controller.h)
source_group("EC/Controller" FILES ${EC_Controller})

set(EC_Factory
        ec/component/factory/component-factory-command_map.cpp
        ec/component/factory/component-factory-command_map.h
        ec/component/factory/component-factory-create_component_map.cpp
        ec/component/factory/component-factory-create_component_map.h
        ec/component/factory/component-factory.h)
source_group("EC/Factory" FILES ${EC_Factory})

set(EC_Material
        ec/component/material/component-material-font.h
        ec/component/material/component-material-sprite.h
        ec/component/material/component-material.h)
source_group("EC/Material" FILES ${EC_Material})

set(EC_Src
        ec/component/src/component-src-bm_font.h
        ec/component/src/component-src-flex.h
        ec/component/src/component-src-texture_packer_gui.h
        ec/component/src/component-src-transform.h
        ec/component/src/component-src.h)
source_group("EC/Src" FILES ${EC_Src})

set(EC_System
        ec/component/system/component-system.h)
source_group("EC/System" FILES ${EC_System})

set(EC_System_Render
        ec/component/system/render/component-system-camera_block_draw.h
        ec/component/system/render/component-system-camera_draw.h
        ec/component/system/render/component-system-static_block_draw.h
        ec/component/system/render/component-system-static_draw.h
        ec/component/system/render/component-system-text_draw.h
        ec/component/system/render/component-system-tilemap_draw.h
        ec/component/system/render/component-system-zoom_camera_draw.h)
source_group("EC/System/Render" FILES ${EC_System_Render})

set(EC_System_Update
        ec/component/system/update/component-system-animate.h
        ec/component/system/update/component-system-animate_move.h
        ec/component/system/update/component-system-broad_phase_collision.h
        ec/component/system/update/component-system-camera.h
        ec/component/system/update/component-system-check_action.h
        ec/component/system/update/component-system-controller_change_direction.h
        ec/component/system/update/component-system-execute_json.h
        ec/component/system/update/component-system-move.h
        ec/component/system/update/component-system-npc_move.h
        ec/component/system/update/component-system-combat-controller.h
        ec/component/system/update/component-system-combat-gravity_physics.h
        ec/component/system/update/component-system-combat-transformations.h
        ec/component/system/update/component-system-narrow_phase_collision.h
        ec/component/system/update/component-system-navigate-fight.h
        ec/component/system/update/component-system-navigate-horizontal.h
        ec/component/system/update/component-system-navigate-message.h
        ec/component/system/update/component-system-navigate-vertical.h
        ec/component/system/update/component-system-navigate.h)
source_group("EC/System/Update" FILES ${EC_System_Update})

# --- Misc headers / sources ---------------------------------------------------
set(Header_Files
        fixed_splay_array.h
        game.h
        glyph.h
        json_template.h
        logger.h
        rect.h
        ring.h
        splay_array.h
        switch_ring.h
        text.h
        json_to.h)
source_group("Headers" FILES ${Header_Files})

set(Source_Files
        game.cpp
        "../include/glad/src/glad.c"
        logger.cpp
        json_to.cpp
        main.cpp)
source_group("Source" FILES ${Source_Files})

# Collect everything into a single variable
set(ALL_FILES
        ${Commands}
        ${EC}
        ${EC_Block}
        ${EC_GJK}
        ${EC_Controller}
        ${EC_Factory}
        ${EC_Material}
        ${EC_Src}
        ${EC_System}
        ${EC_System_Render}
        ${EC_System_Update}
        ${Header_Files}
        ${Source_Files})

# ❷--------------------------------------------------------------------------
# Target definition (no Windows‑only helpers)
# -----------------------------------------------------------------------------
add_executable(IntelligenceQuest ${ALL_FILES})

# ❸--------------------------------------------------------------------------
# Include directories – just the local folder; third‑party dirs come from the
# root‑level INTERFACE libraries (glad, glm, etc.)
# -----------------------------------------------------------------------------

target_include_directories(IntelligenceQuest PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# ❹--------------------------------------------------------------------------
# Compile definitions / options common to all platforms (keep MSVC bits guarded)
# -----------------------------------------------------------------------------

if(MSVC)
    target_compile_definitions(IntelligenceQuest PRIVATE _CONSOLE UNICODE _UNICODE)
    # You may add MSVC‑specific flags here if you need to keep Windows builds.
endif()
