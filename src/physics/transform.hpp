#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

// Mathematics for 3D Game Programming and Computer Graphics 3rd Ed by Eric Lengyel
// transformation component stored as two 3 dim vectors and a quaternion
// the order of transformation is "scale then rotate then translate"
struct Transform
{
	glm::vec3 t = glm::vec3(0.0, 0.0, 0.0); // translate
	glm::quat q = glm::quat(1.0, 0.0, 0.0, 0.0); // rotate
	glm::vec3 s = glm::vec3(1.0, 1.0, 1.0); // scale

	Transform() {}

	Transform(const glm::vec3& _t, const glm::quat& _q, const glm::vec3& _s)
		: t(_t), q(glm::normalize(_q)), s(_s) {}

	// concatenation rules  : q = q2q1, s = s2s1, t = t2 + q2(s2t1)q2^(-1) 
	Transform operator * (const Transform& trans)
	{
		Transform result;
		result.q = glm::normalize(q * trans.q);
		result.s = s * trans.s;
		result.t = t + q * (s * trans.t) * glm::inverse(q);
		return result;
	}

	glm::mat4 mat4() { return glm::translate(t) * glm::toMat4(q) * glm::scale(s); }

	static Transform interpolate(const Transform& trans1, const Transform& trans2, float L)
	{
		Transform result;
		result.s = glm::mix(trans1.s, trans2.s, L);
		result.t = glm::mix(trans1.t, trans2.t, L);
		result.q = glm::slerp(trans1.q, trans2.q, L);
		return result;
	}
};
