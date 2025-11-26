#include "ec/component/component-collider.h"
#include "ec/component/factory/component-factory-create_component_map.h"
#include <sstream>
#include "json_to.h"    
#include "ec/component/component-quadtree.h"
#include "ec/component/component-template.h"
#include "templates.h"

void Component::Collider::init(nlohmann::json json, Entity* game)
{
    std::string gjk_type;
    try{
        gjk_type = json["shape"]["type"];
    }catch(std::exception e){
        Logger::error("Collider JSON needs shape: " + json.dump(1, '\t'), Logger::HIGH);
        return;
    }
    
    std::string gjk_name = gjk_type + "-" + json["shape"]["parameters"].dump();
    Entity* collision = game->get_child("Collision");
    Component::ColliderMask* mask = collision->get_component<Component::ColliderMask>("mask");

    if (!collision->has_component(gjk_name)) // if the collider doesn't exist create a new one
    {
        JsonTo json_to;
        IComponent* gjk = json_to.component(json["shape"], collision, game);
        shape = static_cast<IGJK*>(gjk);
    }
    else
        shape = collision->get_component<IGJK>(gjk_name);

    std::string mask_str = json.value("mask", "physical");
    kind = mask->get_kind_from_string(mask_str);
    physical = mask_str == "physical" || mask_str == "both" || mask_str == "player";
    data = json.value("data", nlohmann::json{});
    position = game->get_nested_component<Component::Position>(json["position"]);
    scale = game->get_nested_component<Component::Float>(json["scale"]);

    bool add_collider = json.value("add_collider", true);
    if (collision->has_child("QuadTree") && add_collider)
    {
        moveable = json.value("moveable", false);

        if (moveable)
        {
            Component::ColliderVector* colliders = collision->get_component<Component::ColliderVector>("moveable_colliders");
            colliders->push_back(this);
        }
        else{
            Component::ColliderVector* colliders = collision->get_component<Component::ColliderVector>("static_colliders");
            colliders->push_back(this);
        }
    }
}