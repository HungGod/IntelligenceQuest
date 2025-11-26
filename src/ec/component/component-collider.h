#pragma once  
#include "ec/component/gjk/component-gjk.h"  
#include <string>  
#include "ec/component/component-collider_mask.h"  
#include "ec/component/component-pathway.h"

namespace Component {  
   class Collider : public IComponent  
   {    
   public:
       uint8_t kind{};
       IGJK* shape{};
       bool moveable{};
       bool physical{};
       nlohmann::json data{};
       Component::Position* position{};
       Component::Float* scale{};

       void init(nlohmann::json json, Entity* game) override;  

       bool is_colliding(Component::Collider* other, Component::ColliderMask* mask)  
       {  
           if (!mask->is_collidable(kind, other->kind)) return false;  
           if (!shape->aabb(other->shape, *position, scale->val, *other->position, other->scale->val)) return false;  
           return shape->gjk_intersect(other->shape, *position, scale->val, *other->position, other->scale->val);  
       }  

       void collide_and_resolve(Component::Collider* other, Component::ColliderMask* mask, Component::Pathway* pathway) {  
           // Prevent self-collision
           if (this == other) return;
           
           if (is_colliding(other, mask)) {
                // Handle resolve commands (like warp, messages, etc.)
                if (other->data.contains("resolve")) {
                    std::string command_name = other->data["resolve"].get<std::string>();
                    nlohmann::json resolve {command_name, {data, other->data}};
                    pathway->message(resolve);
                }

                // Handle physical displacement - check if both are physical
                if (this->physical && other->physical) {
                    glm::vec2 piercing_vec = other->piercing_vec(this);
                   
                    // Apply displacement based on moveability
                    if (this->moveable && other->moveable) {
                        // Both moveable: split the displacement
                        *this->position += piercing_vec / 2.f;
                        *other->position -= piercing_vec / 2.f;
                    } else if (this->moveable) {
                        // Only this is moveable: full displacement
                        *this->position += piercing_vec;
                    } 
                }
               
           }
       }

       Rect get_rect() { 
           glm::vec2 min = shape->get_min(*position, scale->val);
           float w = shape->get_width(scale->val);
           float h = shape->get_height(scale->val);
           return Rect{ min.x, min.y, w, h };
       }

       glm::vec2 piercing_vec(Component::Collider* col_b)
       {
           return shape->resolve(col_b->shape, *position, scale->val, *col_b->position, col_b->scale->val);
       }

       std::string get_id() override { return "collider"; }
   };  
}