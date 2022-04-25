#pragma once

#include "Shader.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "Texture.h"

#include "Camera.h"

#include "math/Quaternion.h"

struct RendererUpdates
{
	float dx, dy;
	math::Vec3 pos;
};

class Renderer
{
public:
	Renderer(int width, int height);
	void update(float elpased_time);

	void move_cam_forward(float f);
	void move_cam_right(float r);
	void update_camera_rot(float dx, float dy);
	void update_camera_pos(const math::Vec3& pos);
	void update_camera_pos(float x, float y, float z);

	void reset_view();
	
private:
	void draw();

	int m_screen_width, m_screen_height;

	float m_near = 0.1f, m_far = 1000.f;

	std::unique_ptr<Camera> m_camera = std::make_unique<Camera>();

	std::shared_ptr<VertexArray> m_cube_va;
	std::shared_ptr<ShaderProgram> m_cube_shader;
	std::shared_ptr<IndexBuffer> m_cube_ib;
	VertexBuffer m_cube_vb;

	std::shared_ptr<VertexArray> m_skybox_va;
	std::shared_ptr<ShaderProgram> m_skybox_shader;
	std::shared_ptr<IndexBuffer> m_skybox_ib;
	VertexBuffer m_skybox_vb;

	Texture2D m_lava_texure;
	CubeMap m_skybox_texture;
	
	math::Mat4 m_perspective;
	math::Mat4 m_orthographic;

	//test
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	unsigned int cubemapTexture;
};

