#pragma once
#include <span>
#include "ec/component/system/component-system.h"
#include "ec/component/controller/component-controller.h"
#include "ec/component/gjk/component-gjk.h"
#include "ec/component/component-pathway.h"
#include "ec/component/component-template.h"
#include "ec/component/component-vec2.h"

namespace System {

    class NarrowPhaseCollision : public ISystem
    {
        struct Fighter {
            Component::Vector2D* velocity{};
            std::vector<Component::Collider*> hitboxes{};
            Component::ColliderVector* impulse{};
            nlohmann::json       id;
        };

        std::vector<Fighter> fighters_;
        Component::Pathway* pathway_{};
        Component::ColliderMask* mask_{};

    public:
        /* ---------------------------------------------------------------- init */
        void init(nlohmann::json j, Entity* g) override
        {
            for (auto& jf : j["fighters"]) {
                Entity* physical_colliders = g->get_nested_child(jf["hitboxes"]);
                fighters_.push_back({
                    g->get_nested_component<Component::Vector2D>(jf["velocity"]),
                    physical_colliders->casted_component_list<Component::Collider>(),
                    g->get_nested_component<Component::ColliderVector>(jf["impulse"]),
                    jf["id"]
                    });
            }
            mask_ = g->get_child("Collision")->get_component<Component::ColliderMask>("mask");
            pathway_ = g->get_component<Component::Pathway>("pathway");
        }

        /* --------------------------------------------------------------- execute */
        void execute() override
        {
            const std::size_t n = fighters_.size();

            /* ───────── 1. attacks → physical hit  (first hit wins) ───────── */
            for (std::size_t i = 0; i < n; ++i)          // attacker
            {
                Fighter& A = fighters_[i];

                for (Component::Collider* atk : *A.impulse)
                {
                    for (std::size_t j = 0; j < n; ++j)  // defender
                    {
                        if (i == j) continue;
                        Fighter& B = fighters_[j];

                        for (Component::Collider* col : B.hitboxes)
                        {
                            atk->collide_and_resolve(col, mask_, pathway_);
                        }
                    }
                }
            }

            /* ───────── 2. physical ↔ physical resolution (all pairs) ───────── */
            std::vector<glm::vec2> resolve_per_fighter(n, glm::vec2(0));
            std::vector<int>       hits_per_fighter(n, 0);

            for (std::size_t i = 0; i < n; ++i)
            {
                Fighter& A = fighters_[i];

                for (std::size_t j = i + 1; j < n; ++j)   // start at i+1 → no duplicates
                {
                    Fighter& B = fighters_[j];

                    for (Component::Collider* pa : A.hitboxes)
                    {
                        for (Component::Collider* pb : B.hitboxes)
                        {
                            pa->collide_and_resolve(pb, mask_, pathway_);
                        }
                }
            }

            /* ───────── 3. apply averaged displacement once per fighter ─────── */
            for (std::size_t i = 0; i < n; ++i)
            {
                if (hits_per_fighter[i] == 0) continue;
                glm::vec2 avg = resolve_per_fighter[i] / static_cast<float>(hits_per_fighter[i]);
                fighters_[i].velocity->x += avg.x;
                fighters_[i].velocity->y += avg.y;
            }
        }
    }


        std::string get_id() override { return "system-narrow_phase_collision"; }
    };
} // namespace System
