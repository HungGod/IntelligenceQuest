#include "ec/component/component-collider.h"
#include "ec/component/factory/component-factory-create_component_map.h"
#include <sstream>

void Component::Collider::init(nlohmann::json j, Entity* g)
{
    std::string gjk_type = j["shape"]["type"];
    std::string gjk_name = gjk_type + "-" + j["shape"]["parameters"].dump();
    Entity* colliders = g->get_child("Colliders");
    Component::ColliderMask* mask = g->get_child("Collider")->get_component<Component::ColliderMask>("mask");

    if (!colliders->has_component(gjk_name)) // if the collider doesn't exist create a new one
    {
        Factory::CreateComponentMap* create_component_map = g->get_component<Factory::CreateComponentMap>("create_component_map");
        IComponent* gjk = (*create_component_map)[gjk_type]();
        gjk->init(j["shape"]["parameters"], g);
        colliders->insert_component(gjk, gjk_name);
        shape = static_cast<IGJK*>(gjk);
    }
    else
        shape = colliders->get_component<IGJK>(gjk_name);
 
    moveable = j.value("movable", false);
    kind = mask->get_kind_from_string(j.value("mask", "physical"));
    data = j.value("data", nlohmann::json{});
    position = g->get_nested_component<Component::Position>(j["position"]);
    scale = g->get_nested_component<Component::Float>(j["scale"]);
}