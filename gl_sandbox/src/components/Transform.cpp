#include "pch.h"
#include "Transform.h"

#include "mathz/Quaternion.h"
#include "mathz/Misc.h"

#include <imgui.h>
#include <imgui_internal.h>

Transform::Transform()
{
	set_name("Transform");
}

void Transform::translate(const mathz::Vec3& pos)
{
	m_postion = pos;
	m_translate[3][0] = pos.x;	m_translate[3][1] = pos.y;	m_translate[3][2] = pos.z;
}

void Transform::scale(float s)
{
	m_scale[0][0] = s; m_scale[1][1] = s; m_scale[2][2] = s;
}

void Transform::rotate(float angle, const mathz::Vec3& axis)
{
	m_rotate_angle = angle;
	m_rotate_axis = axis;
	mathz::Quaternion q(mathz::radians(angle), axis);
	m_rotation = q.convert_to_mat();
}

mathz::Mat4 Transform::get_transform() const
{
	return  m_rotation * m_scale * m_translate;
}

void Transform::imgui_render()
{
	ImGui::Text(m_name);

	mathz::Vec3 position = get_position();
	ImGui::Text("\nPosition: ");
	ImGui::InputFloat("x", &position.x);
	ImGui::InputFloat("y", &position.y);
	ImGui::InputFloat("z", &position.z);
	translate(position);

	float angle = get_rotate_angle();
	mathz::Vec3 axis = get_rotate_axis();
	ImGui::Text("\nRotation: ");
	ImGui::InputFloat("angle", &angle);
	ImGui::SliderFloat("i", &axis.x, -1.f, 1.f);
	ImGui::SliderFloat("j", &axis.y, -1.f, 1.f);
	ImGui::SliderFloat("k", &axis.z, -1.f, 1.f);
	axis.normalize();
	rotate(angle, axis);
}
