#pragma once
#include <array>
#include <glm/glm.hpp>
#include "ec/component/component.h"
#include "logger.h"
#include "ec/component/component-vec2.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>  

/*────────────────────────────   1.  Robust GJK  ────────────────────────────*/
constexpr int   GJK_MAX_ITER = 32;
constexpr float GJK_EPSILON = 1e-6f;
constexpr int   GJK_MAX_SIMPLEX = 3;

class IGJK : public IComponent
{
    static glm::vec2 triple(glm::vec2 a, glm::vec2 b, glm::vec2 c)
    {
        auto a3 = glm::vec3(a, 0.f);
        auto b3 = glm::vec3(b, 0.f);
        auto c3 = glm::vec3(c, 0.f);
        return glm::vec2(glm::cross(glm::cross(a3, b3), c3));
    }

public:
    virtual glm::vec2 get_center(glm::vec2 position, float sc) = 0;
    virtual glm::vec2 support(glm::vec2 dir, glm::vec2 position, float sc) = 0;
    virtual glm::vec2 get_min(glm::vec2 position, float sc) = 0;
    virtual float     get_width(float sc) = 0;
    virtual float     get_height(float sc) = 0;
    virtual glm::vec2 resolve(IGJK* B, glm::vec2 A_pos, float A_sc, glm::vec2 B_pos, float B_sc) { return {}; }

    bool gjk_intersect(IGJK* B, glm::vec2 A_pos, float A_sc, glm::vec2 B_pos, float B_sc)
    {
        IGJK* A = this;
        std::array<glm::vec2, GJK_MAX_SIMPLEX> simplex;
        int n = 0;

        // initial direction
        glm::vec2 dir = B->get_center(B_pos, B_sc) - A->get_center(A_pos, A_sc);
        if (dir == glm::vec2{ 0.f }) dir = { 1.f, 0.f };

        // first support point
        simplex[n++] = B->support(dir, B_pos, B_sc) - A->support(-dir, A_pos, A_sc);
        dir = -simplex[0];

        for (int iter = 0; iter < GJK_MAX_ITER; ++iter)
        {
            glm::vec2 p = B->support(dir, B_pos, B_sc) - A->support(-dir, A_pos, A_sc);
            if (glm::dot(p, dir) < GJK_EPSILON)
                return false;   // no intersection

            simplex[n++] = p;

            if (n == 2)  // line segment case
            {
                const glm::vec2 a = simplex[1];
                const glm::vec2 b = simplex[0];
                const glm::vec2 ab = b - a;
                const glm::vec2 ao = -a;

                dir = triple(ab, ao, ab);
                if (glm::length2(dir) < GJK_EPSILON)
                {
                    // fallback: pick any perpendicular to AB
                    dir = glm::vec2(ab.y, -ab.x);
                }
            }
            else if (n == 3)  // triangle case
            {
                const glm::vec2 a = simplex[2];
                const glm::vec2 b = simplex[1];
                const glm::vec2 c = simplex[0];
                const glm::vec2 ab = b - a;
                const glm::vec2 ac = c - a;
                const glm::vec2 ao = -a;

                const glm::vec2 abp = triple(ac, ab, ab);
                if (glm::dot(abp, ao) > 0.f)
                {
                    simplex[0] = simplex[1];
                    n = 2;
                    dir = abp;
                    continue;
                }
                const glm::vec2 acp = triple(ab, ac, ac);
                if (glm::dot(acp, ao) > 0.f)
                {
                    n = 2;
                    dir = acp;
                    continue;
                }
                return true;  // origin is inside the triangle
            }
        }
        return false;  // give up after max iterations
    }

    bool aabb(IGJK* B, glm::vec2 A_pos, float A_sc, glm::vec2 B_pos, float B_sc)
    {
        const glm::vec2 minA = get_min(A_pos, A_sc);
        const glm::vec2 minB = B->get_min(B_pos, B_sc);
        return  minA.x < minB.x + B->get_width(B_sc) &&
            minA.x + get_width(A_sc)  > minB.x &&
            minA.y < minB.y + B->get_height(B_sc) &&
            minA.y + get_height(A_sc) > minB.y;
    }
};
