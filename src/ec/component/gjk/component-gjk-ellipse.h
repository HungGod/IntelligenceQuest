#pragma once
#include "ec/component/gjk/component-gjk.h"
#include "ec/component/component-template.h"
#include <glm/glm.hpp>
#include "ec/entity.h"
#include <cmath>

namespace GJK {

    class Ellipse : public IGJK
    {
        glm::vec2        centre_{ 0.f };
        float            rx_{ 0.f }, ry_{ 0.f };

        /* analytic support function for axis-aligned ellipse
           p = C + (r_x r_y) / √((r_y d_x)² + (r_x d_y)²) · d                     */
        static glm::vec2 support_axis(glm::vec2 C, glm::vec2 r, glm::vec2 dir)
        {
            if (dir == glm::vec2{ 0 }) return C;
            glm::vec2 d = glm::normalize(dir);
            float k = (r.x * r.y) /
                std::sqrt((r.y * d.x) * (r.y * d.x) +
                    (r.x * d.y) * (r.x * d.y));
            return C + k * d;
        }

    public:
        /* ---------------- init ---------------- */
        void init(nlohmann::json j, Entity* g) override
        {
            rx_ = j["rx"];               // half-width
            ry_ = j["ry"];               // half-height
            centre_ = { j["cx"], j["cy"] };
        }

        /* ---------------- IGJK interface ---------------- */
        glm::vec2 get_center(glm::vec2 position, float sc) override
        {
            return position + centre_ * sc;
        }

        glm::vec2 support(glm::vec2 dir, glm::vec2 position, float sc) override
        {
            return support_axis(get_center(position, sc), { rx_ * sc, ry_ * sc }, dir);
        }

        glm::vec2 get_min(glm::vec2 position, float sc) override
        {
            return { position.x + centre_.x * sc - rx_ * sc,
                    position.y + centre_.y * sc - ry_ * sc};
        }

        float get_width(float sc) override { return rx_ * 2.f * sc; }
        float get_height(float sc) override { return ry_ * 2.f * sc; }

        glm::vec2 resolve(IGJK* B, glm::vec2 A_pos, float A_sc, glm::vec2 B_pos, float B_sc) override
        {
            glm::vec2 n = glm::normalize(B->get_center(B_pos, B_sc) - get_center(A_pos, A_sc));
            glm::vec2 p = B->support(-n, B_pos, B_sc) - support(n, A_pos, A_sc);
            return glm::dot(p, -n) * n;
        }

        std::string get_id() override { return "gjk-ellipse"; }
    };

} // namespace GJK
