#pragma once  
#include "ec/component/gjk/component-gjk.h"  
#include <string>  
#include "ec/component/collider_mask/component-collider_mask.h"  
#include "ec/component/component-pathway.h"

/*────────────────────────────   3.  Component::Collider  ───────────────────*/  
namespace Component {  
   class Collider : public IComponent  
   {    
   public:
       uint8_t kind{};
       bool moveable{};
       IGJK* shape{};
       nlohmann::json data{};
       Component::Position* position{};
       Component::Float* scale{};
       /* ------------------------ init ------------------------ */  
       void init(nlohmann::json j, Entity* g) override;  

       bool is_colliding(Component::Collider* other, Component::ColliderMask* mask)  
       {  
           if (!mask->is_collidable(kind, other->kind)) return false;  
           if (!shape->aabb(other->shape, *position, scale->val, *other->position, other->scale->val)) return false;  
           return shape->gjk_intersect(other->shape, *position, scale->val, *other->position, other->scale->val);  
       }  

       void collide_and_resolve(Component::Collider* other, Component::ColliderMask* mask, Component::Pathway* pathway) {  
           if (is_colliding(other, mask)) {

               std::string command_name = kind > other->kind ? data["resolve"].get<std::string>(): other->data["resolve"].get<std::string>(); // resolve based on kind priority, if equal use other

               nlohmann::json resolve{
                   command_name, { data, other->data }
               };
               pathway->message(resolve);
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
} // namespace Component