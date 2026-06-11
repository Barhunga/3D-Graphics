#include "Scene.h"

#include "Application3D.h"

Scene::Scene(Camera* camera, Light* light, glm::vec3* ambientLight) :
	m_camera(camera), m_light(light), m_ambientLight(ambientLight)
{
}

Scene::~Scene()
{
	for (auto it = m_instances.begin(); it != m_instances.end(); it++)
		delete* it;
}

void Scene::draw()
{
	for (int i = 0; i < MAX_LIGHTS && i < m_pointLights.size(); i++)
	{
		m_pointLightPositions[i] = m_pointLights[i].direction;
		m_pointLightColours[i] = m_pointLights[i].colour;
	}

	for (Instance* instance : m_instances)
		instance->draw(this);

	//for (auto it = m_instances.begin(); it != m_instances.end(); it++)
	//{
	//	Instance* instance = *it;
	//	instance->draw(this);
	//}
}

void Scene::addInstance(Instance* instance)
{
	m_instances.push_back(instance);
}

glm::vec2 Scene::getWindowSize()
{
	return glm::vec2(Application3D::get()->getWindowWidth(), Application3D::get()->getWindowHeight());
}

Instance* Scene::getInstance(int index)
{
	std::list<Instance*>::iterator it = m_instances.begin();
	std::advance(it, index);
	return *it;
}
