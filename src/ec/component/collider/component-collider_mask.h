#pragma once
#include "ec/component/component.h"
#include "logger.h"
#include <bitset>

namespace Component {
    class ColliderMask : public IComponent
    {
        static constexpr uint8_t MAX_IDS = 64u;
        std::bitset<MAX_IDS> mask_[MAX_IDS];
        std::unordered_map<std::string, uint8_t> mask_map_;
        uint8_t id_{};
    public:

        void init(nlohmann::json json, Entity* game) override
        {
            mask_map_.clear();
            id_ = 0u;

            for (const std::string& id : json["ids"]) {
                mask_map_[id] = id_++;
            }

            for (const auto& [key, value] : json["collide_map"].items()) {
                unsigned a = mask_map_[key];
                if (value.is_array()) {
                    for (const std::string& b_id : value) {
                        mask_[a].set(mask_map_[b_id]);
                    }
                }
                else {
                    mask_[a].set(mask_map_[value.get<std::string>()]);
                }
            }
        }

        unsigned get_kind_from_string(std::string kind)
        {
            return mask_map_[kind];
        }

        bool is_collidable(unsigned A, unsigned B) const
        {
            if (A >= MAX_IDS || B >= MAX_IDS) {
                Logger::error("Component::ColliderMask::is_collidable, A and B are out of bounds", Logger::HIGH);
            }
            return mask_[A][B];
        }

        std::string get_id() override { return "collider_mask"; }
    };
}