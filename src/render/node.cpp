#include "node.hpp"

Node::Node(const glm::vec3& s, const glm::quat& q, const glm::vec3& t)
        : local(s, q, t) {}

bool Node::addChild(entt::entity entity)
{
        if (size == MAX_CHILDREN)
        {
                const char* msg = "Child limit exceeded in Node::addChild()";
                Log::print(Log::render, Log::error, msg);
                return false;
        }

        children[size] = entity;
        size += 1;
        return true;
}

bool Node::removeChild(entt::entity entity)
{
        for (unsigned char i = 0; i < size; i++)
        {
                if (entity == children[i])
                {
                        for (unsigned char j = i; j < size - 1; j++)
                                children[j] = children[j + 1];

                        children[size - 1] = entt::null;
                        size -= 1;
                        return true;
                }
        }

        const char* msg = "Entity is not a child in Node::removeChild()";
        Log::print(Log::render, Log::error, msg);
        return false;
}

void Node::concatenate(entt::registry* registry, const Transform& parent)
{
        world = local * parent;

        for(unsigned char i = 0; i < size; i++)
        {
                Node& child = registry->get<Node>(children[i]);
                child.concatenate(registry, world);
        }
}
