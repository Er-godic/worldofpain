e1 = Entity("quad1")
e2 = Entity("quad2")

s = Vec3(0.5, 0.5, 0.5)
q = Quat(45, 0, 0, 1)
t = Vec3(-0.5, 0.0, 0.0)

node = Node(s, q, t)

addNode(e1, node)
addNode(e2, node, e1)

--[[

Functions :

Entity(name) : 
return an entity with a provided name in scene registry

Vec3(x, y, z) : 
return a 3 dim vector. Default (0, 0, 0)

Quat(a, x, y, z) : 
return a quaternion given an angle in degrees and a rotation axis. Default (0, 0, 0, 1)

Node(Vec3 s, Quat q, Vec3 t) : 
return a node component that represents a scaling s, rotation q, and translation t

addNode(Entity entity, Node node, Entity parent) : 
adds node hierarchy to an entity.  The node transform is relative to a parent entity. If parent is left null or nill, the entity has a root node.

--]]
