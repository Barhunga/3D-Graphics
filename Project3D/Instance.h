#pragma once

#include "Mesh.h"
class Scene;

class Instance
{
public:

	Instance(glm::mat4 transform, Mesh* mesh, aie::ShaderProgram* shader);

	void draw(Scene* scene);

	glm::mat4 makeTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale);

	void setWorldPos(float x, float y, float z);
	void setScale(float scale);
	void setScale(float scaleX, float scaleY, float scaleZ);
	void setScale(glm::vec3 scale);
	void translate(float x, float y, float z);
	void rotate(float degrees, glm::vec3 axis);
	
	glm::mat4 getQuadTransform() { return m_transform; }
	glm::vec3 getWorldPos() { return m_transform[3]; }

	Mesh* getMesh() { return m_mesh; }

protected:

	glm::mat4 m_transform;
	Mesh* m_mesh;
	aie::ShaderProgram* m_shader;

};

