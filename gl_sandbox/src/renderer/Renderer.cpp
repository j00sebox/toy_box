#include "pch.h"
#include "Renderer.h"

#include "GLError.h"
#include "events/EventList.h"

#include "mathz/Quaternion.h";

#include <glad/glad.h>

#define DEG_TO_RAD(x) (x / 180.f) * 3.1415f

Renderer::Renderer(int width, int height)
	: m_screen_width(width), m_screen_height(height),
	m_skybox("resources/skyboxes/above_the_clouds/")
{
	GL_CALL(glViewport(0, 0, width, height));

	GL_CALL(glEnable(GL_DEPTH_TEST));
	GL_CALL(glDepthFunc(GL_LEQUAL));
	GL_CALL(glFrontFace(GL_CCW));

	m_directional_light.normalize();

	m_camera = std::unique_ptr<Camera>(new Camera(width, height));

	// bind events
	//EventList::e_camera_move.bind_function(std::bind(&Renderer::update_camera_pos, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	float scaling_factor = 1.0f / tanf(DEG_TO_RAD(45.f) * 0.5f);
	float aspect_ratio = (float)m_screen_height / (float)m_screen_width;

	float q = m_far / (m_far - m_near);

	// create projection matrix
	m_perspective.set(
		aspect_ratio * scaling_factor, 0.f, 0.f, 0.f,
		0.f, scaling_factor, 0.f, 0.f,
		0.f, 0.f, q, 1.f,
		0.f, 0.f, -m_near * q, 0.f
	);

	m_orthographic.set(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f / (m_far - 0.1f), -m_near / (m_far - m_near),
		0.f, 0.f, 0.f, 1.f
	);

	m_scene.set_perspective(m_perspective);
	m_scene.load("resources/scenes/flying_high.scene");

	// airplane setup
	//m_airplane.load_mesh("resources/models/airplane_biplane/scene.gltf");

	//m_airplane.translate({ 0.f, 0.1f, -100.f });
	//m_airplane.scale(2.f);

	//m_airplane_shader.reset(new ShaderProgram(
	//	Shader("resources/shaders/texture2D/texture2D_vertex.shader", ShaderType::Vertex),
	//	Shader("resources/shaders/texture2D/texture2D_fragment.shader", ShaderType::Fragment)
	//));

	////m_airplane.attach_shader(m_airplane_shader);

	//m_airplane_shader->set_uniform_mat4f("u_model", m_airplane.get_transform());
	//m_airplane_shader->set_uniform_mat4f("u_projection", m_perspective);
	//m_airplane_shader->set_uniform_3f("u_light", m_directional_light);

	//// skybox
	//m_skybox_shader.reset(new ShaderProgram(
	//	Shader("resources/shaders/skybox/skybox_vertex.shader", ShaderType::Vertex),
	//	Shader("resources/shaders/skybox/skybox_fragment.shader", ShaderType::Fragment)
	//));

	//m_skybox.attach_shader(m_skybox_shader);

	
}

void Renderer::update(float elpased_time)
{
	m_camera->update(elpased_time);

	//m_airplane_shader->set_uniform_mat4f("u_view", m_camera->camera_look_at());
	//m_skybox_shader->set_uniform_mat4f("u_view", m_camera->look_at_no_translate());

	draw();
}

void Renderer::draw()
{
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	m_scene.draw(m_camera->camera_look_at(), m_camera->look_at_no_translate());
}

void Renderer::reset_view()
{
	m_camera->reset();
}


