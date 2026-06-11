#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GLFW/glfw3.h>
#include "imgui_glfw3.h"

//#include "Texture.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

Application3D* Application3D::s_instance;

Application3D::Application3D() {

}

Application3D::~Application3D() {

}

bool Application3D::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	s_instance = this;
	glfwSetCursorPosCallback(m_window, &Application3D::SetMousePosition);

	//// create simple camera transforms
	//m_viewMatrix = glm::lookAt(vec3(15, 15, 15), vec3(0, 2, 0), vec3(0, 1, 0)); // location, focus point
	//m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
	//									  getWindowWidth() / (float)getWindowHeight(),
	//									  0.1f, 1000.f);

	// load vertex shader from file
	m_shader.loadShader(aie::eShaderStage::VERTEX, "./Shaders/simple.vert.txt");
	m_phongShader.loadShader(aie::eShaderStage::VERTEX, "./Shaders/phong.vert.txt");
	m_normalMapShader.loadShader(aie::eShaderStage::VERTEX, "./Shaders/normalmap.vert.txt");
	// load fragment shader from file
	m_shader.loadShader(aie::eShaderStage::FRAGMENT, "./Shaders/simple.frag.txt");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "./Shaders/phong.frag.txt");
	m_normalMapShader.loadShader(aie::eShaderStage::FRAGMENT, "./Shaders/normalmap.frag.txt");

	if (m_shader.link() == false) {
		printf("Simple Shader Error: %s\n", m_shader.getLastError());
	}
	if (m_phongShader.link() == false) {
		printf("Phong Shader Error: %s\n", m_phongShader.getLastError());
	}
	if (m_normalMapShader.link() == false) {
		printf("Normal Map Shader Error: %s\n", m_normalMapShader.getLastError());
	}


	// create a 2x2 black-n-white checker texture
	// RED simply means one colour channel, i.e. grayscale
	aie::Texture texture2;
	unsigned char texelData[4] = { 0, 255, 255, 0 };
	texture2.create(2, 2, aie::Texture::RED, texelData);

	// set world light
	m_light.colour = { 1, 1, 1 };
	m_ambientLight = { 0.7f, 0.7f, 0.7f };
	m_light.direction = { 0, -1, 0 };

	// create our scene
	m_scene = new Scene(&m_camera, &m_light, &m_ambientLight);

	// red light on left
	m_scene->getPointLights().push_back(Light(vec3(-15, 3, -5), vec3(1, 0, 0), 50));
	// green light on right
	m_scene->getPointLights().push_back(Light(vec3(-15, 3, 5), vec3(0, 1, 0), 50));

	// create our meshes
	m_bunnyMesh.initialiseFromFile("stanford/bunny.obj");
	m_bunnyMesh.loadMaterial("stanford/bunny.mtl");
	m_bunnyMesh.loadTexture("textures/numbered_grid.tga");
	m_bunnyMesh.setMaterial(glm::vec3(0.8, 0.2, 0), glm::vec3(0.8, 0.8, 0), glm::vec3(1, 1, 1), 32);

	m_buddhaMesh.initialiseFromFile("stanford/buddha.obj");
	m_buddhaMesh.loadMaterial("stanford/buddha.mtl");
	m_buddhaMesh.loadTexture("textures/numbered_grid.tga");
	m_buddhaMesh.setMaterial(glm::vec3(0.8, 0.5, 0), glm::vec3(1, 0.6, 0), glm::vec3(1, 1, 1), 32);

	m_dragonMesh.initialiseFromFile("stanford/dragon.obj");
	m_dragonMesh.loadMaterial("stanford/dragon.mtl");
	m_dragonMesh.loadTexture("textures/numbered_grid.tga");
	m_dragonMesh.setMaterial(glm::vec3(0.8, 0.3, 0), glm::vec3(1.5, 1.5, 0), glm::vec3(1, 1, 1), 32);

	m_lucyMesh.initialiseFromFile("stanford/lucy.obj");
	m_lucyMesh.loadMaterial("stanford/lucy.mtl");
	m_lucyMesh.loadTexture("textures/numbered_grid.tga");
	m_lucyMesh.setMaterial(glm::vec3(0, 0.27, 0.3), glm::vec3(0, 0.8, 0.9), glm::vec3(1, 1, 1), 32);
	
	m_quadMesh.initialiseQuad();
	m_quadMesh.loadTexture("textures/numbered_grid.tga");
	m_quadMesh.setMaterial(glm::vec3(0.8, 0.2, 0), glm::vec3(0.8, 0.8, 0), glm::vec3(1, 1, 1), 32);

	m_spearMesh.initialiseFromFile("soulspear/soulspear.obj");
	m_spearMesh.loadMaterial("soulspear/soulspear.mtl");
	//m_quadMesh[2].setMaterial(glm::vec3(0), glm::vec3(0.8, 0.8, 0.8), glm::vec3(0.5, 0.5, 0.5), 20);

	// add instances of our meshes
	m_scene->addInstance(new Instance(glm::mat4(1), &m_bunnyMesh, &m_phongShader));
	int instanceIndex = 0;

	m_scene->addInstance(new Instance(glm::mat4(1), &m_buddhaMesh, &m_phongShader));
	m_scene->getInstance(++instanceIndex)->setWorldPos(0, 0, -15);
	m_scene->getInstance(instanceIndex)->rotate(-45, glm::vec3(0, 1, 0));

	m_scene->addInstance(new Instance(glm::mat4(1), &m_dragonMesh, &m_phongShader));
	m_scene->addInstance(new Instance(glm::mat4(1), &m_dragonMesh, &m_phongShader));
	m_scene->getInstance(++instanceIndex)->setWorldPos(9, 0, 12);
	m_scene->getInstance(instanceIndex)->rotate(-60, glm::vec3(0, 1, 0));
	m_scene->getInstance(++instanceIndex)->setWorldPos(10, 0, -10);
	m_scene->getInstance(instanceIndex)->rotate(45, glm::vec3(0, 1, 0));

	m_scene->addInstance(new Instance(glm::mat4(1), &m_lucyMesh, &m_phongShader));
	m_scene->getInstance(++instanceIndex)->setWorldPos(12, 0, 1);
	m_scene->getInstance(instanceIndex)->rotate(-45, glm::vec3(0, 1, 0));

	m_scene->addInstance(new Instance(glm::mat4(1), &m_quadMesh, &m_phongShader));
	// transform order is important
	m_scene->getInstance(++instanceIndex)->setWorldPos(0, 0, 10);
	m_scene->getInstance(instanceIndex)->rotate(225, glm::vec3(0, 1, 0));
	m_scene->getInstance(instanceIndex)->setScale(6, 1, 10);

	for (int i = 0; i < 7; i++) {
		m_scene->addInstance(new Instance(glm::mat4(1), &m_spearMesh, &m_normalMapShader));
		m_scene->getInstance(++instanceIndex)->setWorldPos(-10, 0, -9 + (i * 3));
		m_scene->getInstance(instanceIndex)->rotate(90, glm::vec3(0, 1, 0));
	}
	
	//// ALTERNATIVE QUAD INITILISATIONS (Vertex and Index)

	//// define 6 vertices for 2 triangles
	//Mesh::Vertex vertices[6];
	//vertices[0].position = { -0.5f, 0, 0.5f, 1 };
	//vertices[1].position = { 0.5f, 0, 0.5f, 1 };
	//vertices[2].position = { -0.5f, 0, -0.5f, 1 };

	//vertices[3].position = { -0.5f, 0, -0.5f, 1 };
	//vertices[4].position = { 0.5f, 0, 0.5f, 1 };
	//vertices[5].position = { 0.5f, 0, -0.5f, 1 };

	//m_quadMesh.initialise(6, vertices);


	//// define 4 vertices for 2 triangles
	//Mesh::Vertex vertices[4];
	//vertices[0].position = { -0.5f, 0, 0.5f, 1 };
	//vertices[1].position = { 0.5f, 0, 0.5f, 1 };
	//vertices[2].position = { -0.5f, 0, -0.5f, 1 };
	//vertices[3].position = { 0.5f, 0, -0.5f, 1 };

	//// set the normals for our up facing quad
	//for (Mesh::Vertex& vertex : vertices)
	//	vertex.normal = { 0, 1, 0, 0 };

	//unsigned int indices[6] = { 0, 1, 2, 2, 1, 3 }; // 0 2 1, 2 3 1 to turn upside down

	//m_quadMesh[1].initialise(4, vertices, 6, indices);

	////


	return true;
}

void Application3D::shutdown() {

	Gizmos::destroy();
	delete m_scene;
}

void Application3D::update(float deltaTime) {

	// query time since application started
	float time = getTime();

	// rotate light
	static bool lightToggleRotate = false;
	static bool lightToggleNight = true;
	static float lightRotateSpeed = 1.f;
	static float phase = 4.8; // instead of time for a constistant smooth beginning
	if (lightToggleRotate) {

		phase += deltaTime;

		float verticalDirection = glm::sin(phase * lightRotateSpeed);
		if (!lightToggleNight) 
			verticalDirection = -glm::abs(verticalDirection);

		m_light.direction = glm::normalize(vec3(glm::cos(phase * lightRotateSpeed),
												verticalDirection, 0));
	}

	//// rotate camera
	//m_viewMatrix = glm::lookAt(vec3(glm::sin(time) * 20, 15, glm::cos(time) * 20),
	//						   vec3(0, 2, 0), vec3(0, 1, 0));

	m_camera.update(deltaTime, m_window);
	m_lastMousePosition = m_mousePosition;

	// inputs
	
	// move mesh
	if (glfwGetKey(m_window, GLFW_KEY_UP))
		m_scene->getInstance(0)->translate(deltaTime * 4.f, 0, 0);
	if (glfwGetKey(m_window, GLFW_KEY_DOWN))
		m_scene->getInstance(0)->translate(-deltaTime * 4.f, 0, 0);
	if (glfwGetKey(m_window, GLFW_KEY_LEFT))
		m_scene->getInstance(0)->translate(0, 0, -deltaTime * 4.f);
	if (glfwGetKey(m_window, GLFW_KEY_RIGHT))
		m_scene->getInstance(0)->translate(0, 0, deltaTime * 4.f);

	// toggle rotate light
	static bool keyHeldR = false;
	if (glfwGetKey(m_window, GLFW_KEY_R) && !keyHeldR) {
		keyHeldR = true;
		lightToggleRotate = !lightToggleRotate;
	}
	else if (!glfwGetKey(m_window, GLFW_KEY_R))
		keyHeldR = false;

	// change light rotate speed
	if (glfwGetKey(m_window, GLFW_KEY_E)) {
		lightRotateSpeed += 0.02;
		phase *= (lightRotateSpeed - 0.02) / lightRotateSpeed; // adjust sin position for change of frequency
	}

	if (glfwGetKey(m_window, GLFW_KEY_Q)) {
		lightRotateSpeed -= 0.02;
		phase *= (lightRotateSpeed + 0.02) / lightRotateSpeed;
	}

	// toggle night
	static bool keyHeldF = false;
	if (glfwGetKey(m_window, GLFW_KEY_F) && !keyHeldF) {
		keyHeldF = true;
		lightToggleNight = !lightToggleNight;
	}
	else if (!glfwGetKey(m_window, GLFW_KEY_F))
		keyHeldF = false;


	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// demonstrate a few shapes
	Gizmos::addAABBFilled(vec3(0, 0, -6), vec3(1), vec4(0, 0.5f, 1, 0.25f));
	Gizmos::addSphere(vec3(5, 0, 5), 1, 8, 8, vec4(1, 0, 0, 0.5f));
	Gizmos::addRing(vec3(5, 0, -5), 1, 1.5f, 8, vec4(0, 1, 0, 1));
	Gizmos::addDisk(vec3(-5, 0, 5), 1, 16, vec4(1, 1, 0, 1));
	Gizmos::addArc(vec3(-5, 0, -5), 0, 2, 1, 8, vec4(1, 0, 1, 1));

	mat4 t = glm::rotate(mat4(1), time, glm::normalize(vec3(1, 1, 1)));
	t[3] = vec4(-2, 0, 0, 1);
	Gizmos::addCylinderFilled(vec3(0, 0, -6), 0.5f, 1, 5, vec4(0, 1, 1, 1), &t);

	// demonstrate 2D gizmos
	Gizmos::add2DAABB(glm::vec2(getWindowWidth() / 2, 100),
					  glm::vec2(getWindowWidth() / 2 * (fmod(getTime(), 3.f) / 3), 20),
					  vec4(0, 1, 1, 1));

	ImGui::Begin("Light Settings");
	ImGui::DragFloat3("Light Direction", &m_light.direction[0], 0.1f, -1.0f, 1.0f);
	ImGui::DragFloat3("Ambient Light",	 &m_ambientLight[0],	0.1f,  0.0f, 2.0f);
	ImGui::DragFloat3("Light Colour",	 &m_light.colour[0],	0.1f,  0.0f, 2.0f);
	ImGui::End();

	ImGui::Begin("Mesh Lighting");
	//ImGui::DragFloat3("Ambiant",	   &m_bunnyMesh.getAmbient()[0],   0.1f,  0.0f, 1.0f);
	//ImGui::DragFloat3("Diffuse",	   &m_bunnyMesh.getDiffuse()[0],   0.1f,  0.0f, 1.0f);
	//ImGui::DragFloat3("Specular",	   &m_bunnyMesh.getSpecular()[0],  0.1f,  0.0f, 1.0f);
	ImGui::DragFloat("Bunny Specular", m_bunnyMesh.getSpecularPower(), 1.f,   0.0f, 32.0f);
	ImGui::DragFloat("Spear Specular", m_spearMesh.getSpecularPower(), 1.f,   0.0f, 32.0f);
	ImGui::End();

	ImGui::Begin("Controls");
	ImGui::Text("WASD           Move the camera");
	ImGui::Text("Right click    Rotate the camera");
	ImGui::Text("Arrow Keys     Move the bunny");
	ImGui::Text("R              Toggle Rotate Light");
	ImGui::Text("Q/E            Adjust Rotation Speed");
	ImGui::Text("F              Toggle Never Night");
	ImGui::End();

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application3D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// draw 3D gizmos
	Gizmos::draw(m_camera.getProjectionMatrix((float)getWindowWidth(), (float)getWindowHeight()) * m_camera.getViewMatrix());

	// draw 2D gizmos using an orthogonal projection matrix (or screen dimensions)
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());

	m_scene->draw();
}

void Application3D::SetMousePosition(GLFWwindow* window, double x, double y)
{
	s_instance->m_mousePosition.x = (float)x;
	s_instance->m_mousePosition.y = (float)y;	
}
