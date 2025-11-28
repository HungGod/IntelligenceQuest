#include "ec/component/component-collider.h"
#include "ec/component/gjk/component-gjk-circle.h"
#include <sstream>
#include <limits>
#include "json_to.h"    
#include "ec/component/component-quadtree.h"
#include "ec/component/component-template.h"
#include "templates.h"

void Component::Collider::init(nlohmann::json json, Entity* game)
{
    Entity* collision = game->get_child("Collision");
    Component::ColliderMask* mask = collision->get_component<Component::ColliderMask>("mask");

    auto add_shape = [&](const nlohmann::json& shape_json) {
        std::string gjk_type = shape_json["type"];
        std::string gjk_name = gjk_type + "-" + shape_json["parameters"].dump();
        
        if (!collision->has_component(gjk_name)) {
            JsonTo json_to;
            IComponent* gjk = json_to.component(shape_json, collision, game);
            shapes.push_back(static_cast<IGJK*>(gjk));
        }
        else {
            shapes.push_back(collision->get_component<IGJK>(gjk_name));
        }
    };

    if (json.contains("shape")) {
        add_shape(json["shape"]);
        min_rect.x = shapes.back()->get_min({0.0f, 0.0f}, 1.0f).x;
        min_rect.y = shapes.back()->get_min({0.0f, 0.0f}, 1.0f).y;
        min_rect.w = shapes.back()->get_width(1.0f);
        min_rect.h = shapes.back()->get_height(1.0f);
    }
    else if (json.contains("shapes")) {
        float min_x = std::numeric_limits<float>::max();
        float min_y = std::numeric_limits<float>::max();
        float max_x = std::numeric_limits<float>::lowest();
        float max_y = std::numeric_limits<float>::lowest();
        
        for (const auto& shape : json["shapes"]) {
            add_shape(shape);
            glm::vec2 shape_min = shapes.back()->get_min({0.0f, 0.0f}, 1.0f);
            float shape_w = shapes.back()->get_width(1.0f);
            float shape_h = shapes.back()->get_height(1.0f);
            
            min_x = std::min(min_x, shape_min.x);
            min_y = std::min(min_y, shape_min.y);
            max_x = std::max(max_x, shape_min.x + shape_w);
            max_y = std::max(max_y, shape_min.y + shape_h);
        }
        
        min_rect.x = min_x;
        min_rect.y = min_y;
        min_rect.w = max_x - min_x;
        min_rect.h = max_y - min_y;
    }
    else Logger::error("Collider JSON needs shape or shapes: " + json.dump(1, '\t'), Logger::HIGH);

    std::string mask_str = json.value("mask", "physical");
    kind = mask->get_kind_from_string(mask_str);
    physical = mask_str == "physical" || mask_str == "both" || mask_str == "player";
    data = json.value("data", nlohmann::json{});
    position = game->get_nested_component<Component::Position>(json["position"]);
    scale = game->get_nested_component<Component::Float>(json["scale"]);
    if (json.contains("velocity"))
        velocity = game->get_nested_component<Component::Velocity>(json["velocity"]);
    else
        velocity = nullptr;
    add_collider = json.value("add_collider", true);
}

void Component::Collider::collide_and_resolve(Component::Collider* other, Component::ColliderMask* mask, Component::Pathway* pathway, Component::Float* delta_time) {  
    // Prevent self-collision
    if (this == other) return;
    
    auto colliding_shape_pairs = is_colliding(other, mask);
    if (!colliding_shape_pairs.empty()) {

         if (other->data.contains("commands")) {
             if (other->data.contains("cooldown")){
                 double cooldown = other->data["cooldown"];
                 double curr_time = glfwGetTime();
                 if (!this->data.contains("time_stamp") || curr_time - this->data["time_stamp"].get<double>() >= cooldown)
                 {
                     this->data["time_stamp"] = curr_time;
                     pathway->message(other->data["commands"]);
                 }
             }
             else
                 pathway->message(other->data["commands"]);
         }

         // Handle physical displacement
         if (this->physical && other->physical) {
             std::vector<glm::vec2> piercing_vecs;
             for (auto [shape, other_shape] : colliding_shape_pairs) {
                 piercing_vecs.push_back(other_shape->resolve(shape, *other->position, other->scale->val, *position, scale->val));
             }

            glm::vec2 result{0.f, 0.f};
            if (piercing_vecs.size() == 2) {
                glm::vec2 pv1 = piercing_vecs[0];
                glm::vec2 pv2 = piercing_vecs[1];
                float dot = glm::dot(glm::normalize(pv1), glm::normalize(pv2));
                
                if (dot <= 0.0f) {
                    glm::vec2 temp = glm::abs(pv1 + pv2);
                    *this->position -= *this->velocity * delta_time->val * glm::vec2{temp.x <= temp.y, temp.y < temp.x};
                    if (other->velocity != nullptr) {
                        *other->position += *other->velocity * delta_time->val * glm::vec2{temp.x <= temp.y, temp.y < temp.x};
                    }
                    return;
                } else {
                    result = pv1 + pv2;
                }
            } else {
                for (const glm::vec2& pv : piercing_vecs) {
                    result += pv;
                }
            }

            if (other->velocity != nullptr) {
                *this->position += result / 2.0f;
                *other->position -= result / 2.0f;
            }
            else
                *this->position += result;
        }
    }
}