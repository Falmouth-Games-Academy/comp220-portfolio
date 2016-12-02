#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
//this is the camera class to set up where the viewport will be for the user.
class Camera
{
public:
	// Z-near and far -closetest distance we can see and to furthest
	Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar)
	{
		m_perspective = glm::perspective(fov, aspect, zNear, zFar);
		m_position = pos;
		m_forward = glm::vec3(0, 0, 1);//lookinmg straight into the scren
		m_up = glm::vec3(0, 1, 0);	//harcoding this 
	}
	//set up giders for rotating with forward and up 

	inline glm::mat4 GetViewProjection()const
	{
		//view projection
		return m_perspective * glm::lookAt(m_position, m_position + m_forward, m_up);
	}

protected:
private:
	glm::mat4 m_perspective;
	glm::vec3 m_position;
	glm::vec3 m_forward;	//represents what eveer is forward looking
	glm::vec3 m_up;			//represents what ever up
};


#endif // !CAMERA_H_INCLUDED
