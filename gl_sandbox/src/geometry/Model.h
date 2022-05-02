#pragma once

#include "Mesh.h"
#include "Shader.h"

#include "mathz/Matrix.h"
#include "mathz/Quaternion.h"

class Model
{
public:
	virtual void draw() const;
	void translate(const mathz::Vec3& pos);
	void scale(float s);
	void rotate(float angle, const mathz::Vec3& axis);

	void load_mesh(const std::string& file_path);
	void set_shader(const std::string& name);
	[[nodiscard]] const std::string& get_shader() const { return m_shader_name; }

	void set_name(const std::string& name);
	[[nodiscard]] const std::string& get_name() const { return m_name; }

	[[nodiscard]] const mathz::Vec3& get_position() const { return m_postion; }
	[[nodiscard]] const mathz::Vec3& get_rotate_axis() const { return m_rotate_axis; }
	[[nodiscard]] float get_rotate_angle() const { return m_rotate_angle; }
	[[nodiscard]] mathz::Mat4 get_transform() const;

private:
	std::string m_name;
	std::vector<Mesh> m_meshes;
	std::string m_shader_name;

	mathz::Vec3 m_postion;
	mathz::Vec3 m_rotate_axis;
	float m_rotate_angle;

	mathz::Mat4 m_scale;
	mathz::Mat4 m_rotation;
	mathz::Mat4 m_translate;
};