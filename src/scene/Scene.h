#pragma once

#include "Window.h"
#include "Camera.h"
#include "entities/Skybox.h"
#include "SceneNode.h"
#include "LightManager.h"

#include "components/Fwd.h"

#include <map>
#include <queue>
#include <mathz/Vector.h>

class Entity;
class UniformBuffer;
struct RenderObject;

class Scene
{
public:
	Scene(Window* window);
	~Scene();

	void load(const char* scene);
	void save(const std::string& path);
	void init();
	void update(float elapsed_time);
	void add_primitive(const char* name);
	void window_resize(int width, int height);
	void reset_view();
	Camera* get_camera() { return m_camera.get(); }

	void set_background_colour(mathz::Vec4 colour);
	[[nodiscard]] const mathz::Vec4& get_background_colour() const { return m_clear_colour; }

private:
    void compile_shaders() const;

	// scene management
	void update_node(SceneNode& node, const Transform& parent_transform);
	void remove_node(SceneNode& node);
	SceneNode move_node(SceneNode& node);
	
	// imgui helpers
	void imgui_render(SceneNode& node);
	void display_components();

    Window* m_window_handle;
	std::shared_ptr<Camera> m_camera;
	std::unique_ptr<Skybox> m_skybox;
    std::unique_ptr<UniformBuffer> m_uniform_buffer;
	SceneNode root;
	std::queue<SceneNode*> m_nodes_to_remove;
	LightManager m_light_manager;
	std::vector<RenderObject> m_render_list;

	// imgui stuff
	SceneNode* m_selected_node = nullptr;
    SceneNode* m_drag_node = nullptr;
    SceneNode* m_drop_node = nullptr;
    mathz::Vec4 m_clear_colour = { 0.f, 0.f, 0.f, 1.f};
};

