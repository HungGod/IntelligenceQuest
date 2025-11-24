#include "ec/component/component-collider.h"
#include "ec/component/factory/component-factory-create_component_map.h"
#include <sstream>

void Component::Collider::init(nlohmann::json json, Entity* game)
{
    std::string gjk_type = json["shape"]["type"];
    std::string gjk_name = gjk_type + "-" + json["shape"]["parameters"].dump();
    Entity* colliders = game->get_child("Colliders");
    Component::ColliderMask* mask = game->get_child("Collider")->get_component<Component::ColliderMask>("mask");

    if (!colliders->has_component(gjk_name)) // if the collider doesn't exist create a new one
    {
        Factory::CreateComponentMap* create_component_map = game->get_component<Factory::CreateComponentMap>("create_component_map");
        IComponent* gjk = (*create_component_map)[gjk_type]();
        gjk->init(json["shape"]["parameters"], game);
        colliders->insert_component(gjk, gjk_name);
        shape = static_cast<IGJK*>(gjk);
    }
    else
        shape = colliders->get_component<IGJK>(gjk_name);
 
    moveable = json.value("moveable", false);
    kind = mask->get_kind_from_string(json.value("mask", "physical"));
    data = json.value("data", nlohmann::json{});
    position = game->get_nested_component<Component::Position>(json["position"]);
    scale = game->get_nested_component<Component::Float>(json["scale"]);
}