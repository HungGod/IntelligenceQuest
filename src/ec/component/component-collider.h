#pragma once  
#include "ec/component/gjk/component-gjk.h"  
#include <string>  
#include <vector>
#include <utility>
#include "ec/component/component-collider_mask.h"  
#include "ec/component/component-pathway.h"
#include "rect.h"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace Component {  
   class Collider : public IComponent  
   {    
   Rect min_rect;

   public:
       uint8_t kind{};
       std::vector<IGJK*> shapes{};
       Component::Vector2D* velocity{};
       bool physical{};
       bool add_collider{};

       nlohmann::json data{};
       Component::Vector2D* position{};
       Component::ValTemplate<float>* scale{};

       Rect get_rect() { 
          return Rect {min_rect.x * scale->val + position->x, min_rect.y * scale->val + position->y, min_rect.w * scale->val, min_rect.h * scale->val};
        }

       void init(nlohmann::json json, Entity* game) override;  

       std::vector<std::pair<IGJK*, IGJK*>> is_colliding(Component::Collider* other, Component::ColliderMask* mask)  
       {  
           if (!mask->is_collidable(kind, other->kind)) return {};  
           if (!aabb(other)) return {};  
           return gjk_collide(other);  
       }

       bool aabb(Component::Collider* other)
       {
           return get_rect().x < other->get_rect().x + other->get_rect().w &&
                  get_rect().x + get_rect().w > other->get_rect().x &&
                  get_rect().y < other->get_rect().y + other->get_rect().h &&
                  get_rect().y + get_rect().h > other->get_rect().y;
       }

       std::vector<std::pair<IGJK*, IGJK*>> gjk_collide(Component::Collider* other)
       {
            std::vector<std::pair<IGJK*, IGJK*>> colliding_shapes;
            for (IGJK* shape : shapes)
            {
                for (IGJK* other_shape : other->shapes)
                {
                    if (shape->gjk_intersect(other_shape, *position, scale->val, *other->position, other->scale->val))
                    {
                        colliding_shapes.push_back({shape, other_shape});
                    }
                }
            }
            return colliding_shapes;
       }

       void collide_and_resolve(Component::Collider* other, Component::ColliderMask* mask, Component::Pathway* pathway, Component::ValTemplate<float>* delta_time); 

       glm::vec2 piercing_vec(Component::Collider* col_b)
       {
           glm::vec2 result{0.f, 0.f};
           for (IGJK* shape_a : shapes) {
               for (IGJK* shape_b : col_b->shapes) {
                   result += shape_a->resolve(shape_b, *position, scale->val, *col_b->position, col_b->scale->val);
               }
           }
           return result;
       }

       std::string get_id() override { return "collider"; }
   };  
}