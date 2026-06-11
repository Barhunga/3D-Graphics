#include "Instance.h"

#include "Shader.h"
#include "Scene.h"

Instance::Instance(glm::mat4 transform, Mesh* mesh, aie::ShaderProgram* shader) : 
	m_transform(transform), m_mesh(mesh), m_shader(shader)
{
}

void Instance::draw(Scene* scene)
{
	// set the shader pipeline
	m_shader->bind();

	// bind transform and other uniforms
	auto pvm = scene->getCamera()->getProjectionMatrix(scene->getWindowSize().x, scene->getWindowSize().y) * 
			   scene->getCamera()->getViewMatrix() * 
			   m_transform;
	m_shader->bindUniform("ProjectionViewModel", pvm);
	m_shader->bindUniform("ModelMatrix", m_transform);

	m_shader->bindUniform("AmbientColour", *scene->getAmbientLight());
	m_shader->bindUniform("LightColour", scene->getLight()->colour);
	m_shader->bindUniform("LightDirection", scene->getLight()->direction); // for a sun, use normalized vector from it to the mesh
	m_shader->bindUniform("cameraPosition", scene->getCamera()->getPosition());

	if (m_mesh->getHasNormalMap()) {
		int numLights = scene->getNumLights();
		m_shader->bindUniform("numLights", numLights);
		m_shader->bindUniform("PointLightPosition", numLights, scene->getPointLightPositions());
		m_shader->bindUniform("PointLightColour", numLights, scene->getPointLightColours());
	}

	// draw mesh
	m_mesh->applyMaterial(m_shader);
	m_mesh->draw();
}

glm::mat4 Instance::makeTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale)
{
	return glm::translate(glm::mat4(1), position)
		* glm::rotate(glm::mat4(1), glm::radians(eulerAngles.z), glm::vec3(0, 0, 1))
		* glm::rotate(glm::mat4(1), glm::radians(eulerAngles.y), glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1), glm::radians(eulerAngles.x), glm::vec3(1, 0, 0))
		* glm::scale(glm::mat4(1), scale);
}

void Instance::setWorldPos(float x, float y, float z)
{
	m_transform[3][0] = x;
	m_transform[3][1] = y;
	m_transform[3][2] = z;
}

void Instance::setScale(float scale)
{
	//m_transform *= glm::mat4(scale, 0, 0, 0,
	//                         0, scale, 0, 0,
	//                         0, 0, scale, 0,
	//                         0, 0, 0, 1);
	setScale(glm::vec3(scale, scale, scale));
}

void Instance::setScale(float scaleX, float scaleY, float scaleZ)
{
	setScale(glm::vec3(scaleX, scaleY, scaleZ));
}

void Instance::setScale(glm::vec3 scale)
{
	m_transform = glm::scale(m_transform, scale);
}

void Instance::translate(float x, float y, float z)
{
	m_transform = glm::translate(m_transform, glm::vec3(x, y, z));
}

void Instance::rotate(float degrees, glm::vec3 axis)
{
	m_transform = glm::rotate(m_transform, glm::radians(degrees), glm::normalize(axis));
}
