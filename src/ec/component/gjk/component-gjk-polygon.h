#pragma once
#include <vector>
#include <limits>
#include <glm/glm.hpp>
#include "ec/component/gjk/component-gjk.h"
#include "logger.h"

namespace GJK {

    class Polygon : public IGJK {
        std::vector<glm::vec2> verts_;

        // cached local values for fast queries
        glm::vec2 local_center_{ 0.f };
        glm::vec2 local_min_{ 0.f };
        float     local_width_{ 0.f };
        float     local_height_{ 0.f };
    public:
        Polygon() = default;

        void init(nlohmann::json j, Entity* game) override {
            verts_.clear();
            if (!j.is_array()) {
                Logger::error("Polygon JSON must be an array", Logger::HIGH);
                return;
            }
            for (auto& vj : j) {
                verts_.emplace_back(vj["x"].get<float>(), vj["y"].get<float>());
            }
            if (verts_.size() < 3) {
                Logger::error("Polygon needs at least 3 vertices", Logger::HIGH);
            }

            // compute centroid
            glm::vec2 sum{ 0.f };
            for (auto& v : verts_) sum += v;
            local_center_ = sum / float(verts_.size());

            // compute AABB
            float minX = std::numeric_limits<float>::infinity();
            float minY = std::numeric_limits<float>::infinity();
            float maxX = -std::numeric_limits<float>::infinity();
            float maxY = -std::numeric_limits<float>::infinity();
            for (auto& v : verts_) {
                minX = std::min(minX, v.x);
                minY = std::min(minY, v.y);
                maxX = std::max(maxX, v.x);
                maxY = std::max(maxY, v.y);
            }
            local_min_ = { minX, minY };
            local_width_ = maxX - minX;
            local_height_ = maxY - minY;
        }

        // world‐space centroid
        glm::vec2 get_center(glm::vec2 position, float sc) override {
            return position + local_center_ * sc;
        }

        // farthest point in direction dir
        glm::vec2 support(glm::vec2 dir, glm::vec2 position, float sc) override {
            float best = -std::numeric_limits<float>::infinity();
            glm::vec2 best_pt{};
            for (auto& v : verts_) {
                glm::vec2 w = position + v * sc;
                float d = glm::dot(w, dir);
                if (d > best) {
                    best = d;
                    best_pt = w;
                }
            }
            return best_pt;
        }

        // AABB minimum corner
        glm::vec2 get_min(glm::vec2 position, float sc) override {
            return position + local_min_ * sc;
        }

        float get_width(float sc) override {
            return local_width_ * sc;
        }

        float get_height(float sc) override {
            return local_height_ * sc;
        }

        // simple penetration along center‐to‐center axis
        glm::vec2 resolve(IGJK* B,
            glm::vec2 A_pos, float A_sc,
            glm::vec2 B_pos, float B_sc) override
        {
            glm::vec2 cA = get_center(A_pos, A_sc);
            glm::vec2 cB = B->get_center(B_pos, B_sc);
            glm::vec2 n = cA - cB;
            float d2 = glm::dot(n, n);
            if (d2 < GJK_EPSILON) {
                // fallback: first edge normal
                glm::vec2 e = verts_[1] - verts_[0];
                n = glm::normalize(glm::vec2(e.y, -e.x));
            }
            else {
                n *= 1.0f / std::sqrt(d2);
            }

            glm::vec2 As = support(n, A_pos, A_sc);
            glm::vec2 Bs = B->support(-n, B_pos, B_sc);
            float pen = glm::dot(As - Bs, n);
            return n * pen;
        }

        std::string get_id() override { return "gjk-polygon"; }
    };

} // namespace GJK
