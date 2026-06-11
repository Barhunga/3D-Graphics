#pragma once

#include "Application.h"
#include <vector>
#include <glm/mat4x4.hpp>

#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"

#include "Scene.h"

class Application3D : public aie::Application {
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	// singleton pattern
	static Application3D* get() { return s_instance; }
	glm::vec2 getMousePosition() { return m_mousePosition; }
	glm::vec2 getMouseDelta() { return m_mousePosition - m_lastMousePosition; }

	// set up mouse input
	static void SetMousePosition(GLFWwindow* window, double x, double y);

protected:

	Camera m_camera;
	//glm::mat4	m_viewMatrix;
	//glm::mat4	m_projectionMatrix;

	aie::ShaderProgram m_shader;
	aie::ShaderProgram m_phongShader;
	aie::ShaderProgram m_normalMapShader;
	
	Mesh m_bunnyMesh;
	Mesh m_quadMesh;
	Mesh m_spearMesh;
	Mesh m_buddhaMesh;
	Mesh m_dragonMesh;
	Mesh m_lucyMesh;

	glm::vec2 m_mousePosition;
	glm::vec2 m_lastMousePosition;

	Light m_light;
	glm::vec3 m_ambientLight;

	static Application3D* s_instance;

	Scene* m_scene;
};