#include <entt/entt.hpp>
#include "transform.hpp"

#define MAX_CHILDREN 10

// node component that allows transform hierarchy
struct Node
{
        unsigned char size = 0;
        std::array<entt::entity, MAX_CHILDREN> children{};
        Transform local;
        Transform world;

        Node(const glm::vec3&, const glm::quat&, const glm::vec3&);
        bool addChild(entt::entity);
        bool removeChild(entt::entity);
        void concatenate(entt::registry*, const Transform&);
};

// parent component that allows transform hierarchy
struct Parent
{
        entt::entity entity;
        Parent(entt::entity e) : entity(e) {}
};
