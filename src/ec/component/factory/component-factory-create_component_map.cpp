#include "ec/component/factory/component-factory-create_component_map.h"

#include "ec/component/controller/component-controller-keyboard.h"

#include "ec/component/system/update/component-system-camera.h"
#include "ec/component/system/update/component-system-move.h"
#include "ec/component/system/render/component-system-camera_draw.h"
#include "ec/component/system/render/component-system-static_draw.h"
#include "ec/component/system/render/component-system-tilemap_draw.h"
#include "ec/component/system/update/component-system-animate.h"
#include "ec/component/system/update/component-system-animate_move.h"
#include "ec/component/system/update/component-system-npc_move.h"

#include "ec/component/gjk/component-gjk-line_segment.h"
#include "ec/component/gjk/component-gjk-circle.h"
#include "ec/component/gjk/component-gjk-point.h"
#include "ec/component/gjk/component-gjk-aabb.h"

#include "ec/component/material/component-material-sprite.h"
#include "ec/component/material/component-material-font.h"
#include "ec/component/component-collider_mask.h"
#include "ec/component/component-collider.h"
#include "ec/component/system/update/component-system-broad_phase_collision.h"

#include "ec/component/component-json_template_map.h"
#include "ec/component/system/update/component-system-insert_colliders.h"
#include "ec/component/system/update/component-system-clear_quad_tree.h"
#include "ec/component/system/update/component-system-controller_change_direction.h"
#include "ec/component/system/update/component-system-check_action.h"
#include "ec/component/system/update/component-system-execute_json.h"
#include "ec/component/system/render/component-system-text_draw.h"
#include "ec/component/system/render/component-system-static_block_draw.h"
#include "ec/component/system/update/component-system-combat-controller.h"
#include "ec/component/component-intelligence.h"

#include "ec/component/system/update/component-system-narrow_phase_collision.h"
#include "ec/component/gjk/component-gjk-ellipse.h"

#include "ec/component/src/component-src-texture_packer_gui.h"

#include "ec/component/block/component-block-dynamic_text.h"
#include "ec/component/block/component-block-text.h"
#include "ec/component/block/component-block-box.h"
#include "ec/component/block/component-block-options-horizontal.h"

#include "ec/component/system/render/component-system-camera_block_draw.h"
#include "ec/component/system/update/component-system-navigate-horizontal.h"
#include "ec/component/system/update/component-system-navigate-message.h"
#include "ec/component/component-cache.h"

#include "ec/component/system/update/component-system-navigate-vertical.h"
#include "ec/component/block/component-block-options-vertical.h"

#include "ec/component/src/component-src-transform.h"
#include "ec/component/system/update/component-system-combat-transformations.h"
#include "ec/component/system/render/component-system-zoom_camera_draw.h"
#include "ec/component/component-hitboxes.h"

template<typename T>
T* c_component() { return new T(); }

void Factory::CreateComponentMap::create()
{
	// component
	(*this)["Float"] = c_component<Component::Float>;
	(*this)["Bool"] = c_component<Component::Bool>;
	(*this)["Int"] = c_component<Component::Int>;
	(*this)["Json"] = c_component<Component::Json>;
	(*this)["Position"] = (*this)["Direction"] = c_component<Component::Position>;
	(*this)["KeyboardArray"] = c_component<Component::KeyboardArray>;
	(*this)["SystemVector"] = c_component<Component::SystemVector>;
	(*this)["Animation"] = c_component<Component::Animation>;
	(*this)["QuadTreeVector"] = c_component<Component::QuadTreeVector>;
	(*this)["QuadTree"] = c_component<Component::QuadTree>;
	(*this)["AnimationMap"] = c_component<Component::AnimationMap>;
	(*this)["Texture"] = c_component<Component::Texture>;
	(*this)["Shader"] = c_component<Component::Shader>;
	(*this)["Renderer"] = c_component<Component::Renderer>;
	(*this)["Tilemap"] = c_component<Component::Tilemap>;
	(*this)["Tileset"] = c_component<Component::Tileset>;
	(*this)["Pathway"] = c_component<Component::Pathway>;
	(*this)["PositionMap"] = c_component<Component::PositionMap>;
	(*this)["JsonMap"] = c_component<Component::JsonMap>;
	(*this)["LoadMap"] = c_component<Component::BoolMap>;
	(*this)["JsonTemplateMap"] = c_component<Component::JsonTemplateMap>;
	(*this)["String"] = c_component<Component::DavidString>;
	(*this)["Intelligence"] = (*this)["IQ"] = c_component<Component::Intelligence>;
	(*this)["Box"] = c_component<Component::Box>;
	(*this)["SystemCache"] = c_component<Component::SystemCache>;
	(*this)["Hitboxes"] = c_component<Component::Hitboxes>;
	(*this)["ColliderMask"] = c_component<Component::ColliderMask>;
	(*this)["Collider"] = c_component<Component::Collider>;

	// component src
	(*this)["Src"] = (*this)["Src::TexturePackerGUI"] = c_component<Src::TexturePackerGUI>;
	(*this)["SrcTemplate"] = (*this)["SrcTemp"] = (*this)["Src::Flex"] = (*this)["Src::Transform"] = c_component<Src::Transform>;
	(*this)["BitmapGlyph"] = (*this)["Glyph"] = (*this)["Src::BMFont"] = c_component<Src::BMFont>;

	// component material
	(*this)["Material"] = (*this)["Material::Sprite"] = c_component<Material::Sprite>;
	(*this)["Material::Color"] = c_component<Material::Font>;

	// component block
	(*this)["Block::Box"] = c_component<Block::Box>;
	(*this)["Block::DynamicText"] = (*this)["Block::DynamicTemp"] = c_component<Block::DynamicTemp>;
	(*this)["Block::Text"] = (*this)["Block::Words"] = c_component<Block::Words>;

	// component block option
	(*this)["Block::Options"] = (*this)["Block::Options::Horizontal"] = (*this)["Options::Horizontal"] = c_component<Options::Horizontal>;
	(*this)["Block::Options::Vertical"] = (*this)["Options::Vertical"] = c_component<Options::Vertical>;

	// component gjk
	(*this)["GJK::LineSegment"] = c_component<GJK::LineSegment>;
	(*this)["GJK::Circle"] = c_component<GJK::Circle>;
	(*this)["GJK::Point"] = c_component<GJK::Point>;
	(*this)["GJK::AABB"] = c_component<GJK::AABB>;
	(*this)["GJK::Ellipse"] = c_component<GJK::Ellipse>;

	// component controller
	(*this)["Controller::Keyboard"] = c_component<Controller::Keyboard>;

	// component system
	(*this)["System::Camera"] = c_component<System::Camera>;
	(*this)["System::CameraDraw"] = c_component<System::CameraDraw>;
	(*this)["System::Move"] = c_component<System::Move>;
	(*this)["System::StaticDraw"] = c_component<System::StaticDraw>;
	(*this)["System::TilemapDraw"] = c_component<System::TilemapDraw>;
	(*this)["System::Animate"] = c_component<System::Animate>;
	(*this)["System::AnimateMove"] = c_component<System::AnimateMove>;
	(*this)["System::CheckCollision"] = c_component<System::BroadPhaseCollision>;
	(*this)["System::NPCMove"] = c_component<System::NPCMove>;
	(*this)["System::InsertColliders"] = (*this)["System::AddColliders"] = c_component<System::InsertColliders>;
	(*this)["System::ClearQuadtree"] = (*this)["System::ClearQuadTree"] = c_component<System::ClearQuadTree>;
	(*this)["System::ControllerChangeDirection"] = c_component<System::ControllerChangeDirection>;
	(*this)["System::CheckAction"] = c_component<System::CheckAction>;
	(*this)["System::ExecuteJson"] = c_component<System::ExecuteJson>;
	(*this)["System::TextDraw"] = c_component<System::TextDraw>;
	(*this)["System::StaticBlockDraw"] = c_component<System::StaticBlockDraw>;
	(*this)["System::CameraBlockDraw"] = c_component<System::CameraBlockDraw>;
	(*this)["System::CombatController"] = c_component<System::CombatController>;
	(*this)["System::CombatTransformations"] = c_component<System::CombatTransformations>;
	(*this)["System::ZoomCameraDraw"] = c_component<System::ZoomCameraDraw>;
	(*this)["System::CombatCollisions"] = c_component<System::NarrowPhaseCollision>;


	// component system navigate
	(*this)["Navigate::Message"] = c_component<Navigate::Message>;
	(*this)["Navigate::Horizontal"] = c_component<Navigate::Horizontal>;
	(*this)["Navigate::Vertical"] = c_component<Navigate::Vertical>;
}
