#ifndef TRANSFORM_H
#define	TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
class Transform
{
public:
	 Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f)) :
		m_pos(pos),
		m_rot(rot),
		m_scale(scale) {}


	inline glm::mat4 GetModel() const
	{
		glm::mat4 positionMatrix = glm::translate(m_pos); //Generates a 4x4
		glm::mat4 rotationXMatrix = glm::rotate(m_rot.x, glm::vec3(1, 0, 0));//roation of the X matrix
		glm::mat4 rotationYMatrix = glm::rotate(m_rot.y, glm::vec3(0, 1, 0));//roation of the Y matrix
		glm::mat4 rotationZMatrix = glm::rotate(m_rot.z, glm::vec3(0, 0, 1));//roation of the Z matrix
		glm::mat4 scaleMatrix = glm::scale(m_scale);

		glm::mat4 rotionMatrix = rotationZMatrix * rotationYMatrix * rotationXMatrix; //you have to do this backwards

		return positionMatrix * rotionMatrix * scaleMatrix;					//you have to do this backwards(will affect differently otherwise)
	}



	inline glm::vec3& GetPos() { return m_pos; }
	inline glm::vec3& GetRot() { return m_rot; }
	inline glm::vec3& GetScale() { return m_scale; }

	inline void SetPos(const glm::vec3& pos) { m_pos = pos; }
	inline void SetRot(const glm::vec3& rot) { m_rot = rot; }
	inline void SetScale(const glm::vec3& scale) { m_scale = scale; }

protected:
private:

	glm::vec3 m_pos;
	glm::vec3 m_rot;
	glm::vec3 m_scale;
};

#endif // !TRANSFORM_H

