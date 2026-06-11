#pragma once

#include "Instance.h"
#include <list>
#include "Camera.h"

#define MAX_LIGHTS 4

struct Light {
	Light()
	{

	}
	Light(glm::vec3 pos, glm::vec3 col, float intensity)
	{
		direction = pos;
		colour = col * intensity;
	}
	glm::vec3 direction;
	glm::vec3 colour;
};

class Scene
{
public:

	Scene(Camera* camera, Light* light, glm::vec3* ambientLight);
	~Scene();

	void draw();

	void addInstance(Instance* instance);
	Camera* getCamera() { return m_camera; }
	glm::vec3* getAmbientLight() { return m_ambientLight; }
	Light* getLight() { return m_light; }
	glm::vec2 getWindowSize();	

	int getNumLights() { return (int)m_pointLights.size(); }
	glm::vec3* getPointLightPositions() { return &m_pointLightPositions[0]; }
	glm::vec3* getPointLightColours() { return &m_pointLightColours[0]; }

	std::vector<Light>& getPointLights() { return m_pointLights; }

	Instance* getInstance(int index);

protected:

	Camera* m_camera;
	Light* m_light;
	glm::vec3* m_ambientLight;
	std::list<Instance*> m_instances; // could use an unordered map of lists to batch by shader

	std::vector<Light> m_pointLights;
	glm::vec3 m_pointLightPositions[MAX_LIGHTS];
	glm::vec3 m_pointLightColours[MAX_LIGHTS];
};

