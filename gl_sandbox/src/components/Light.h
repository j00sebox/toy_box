#pragma once

#include "Component.h"

#include "mathz/Vector.h"

class Light : public Component
{
public:
	Light();

	virtual void on_remove() override {};
	[[nodiscard]] const char* get_name() const override { return "Light"; }
	[[nodiscard]] const char* get_type() const override { return typeid(Light).name(); }
	void parse(json info) override;
	void imgui_render() override;

	void set_colour(mathz::Vec4 col) { m_colour = col; }
	[[nodiscard]] const mathz::Vec4& get_colour() const { return m_colour; }

protected:
	mathz::Vec4 m_colour;
};

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	void set_direction(const mathz::Vec3& dir) { m_direction = dir; m_direction.normalize(); }
	[[nodiscard]] const mathz::Vec3& get_direction() const { return m_direction; }

	void on_remove() override;
	[[nodiscard]] const char* get_name() const override { return "Directional Light"; }
	[[nodiscard]] const char* get_type() const override { return typeid(DirectionalLight).name(); }
	void parse(json info) override;
	void imgui_render() override;

private:
	mathz::Vec3 m_direction;
};

class PointLight : public Light
{
public:
	PointLight();
	void set_radius(float rad) { m_radius = rad; }
	void set_range(float range) { m_range = range; }
	[[nodiscard]] int get_index() const { return m_index; }
	[[nodiscard]] float get_radius() const { return m_radius; }
	[[nodiscard]] float get_range() const { return m_range; }

	void on_remove() override;
	[[nodiscard]] const char* get_name() const override { return "Point Light"; }
	[[nodiscard]] const char* get_type() const override { return typeid(PointLight).name(); }
	void parse(json info) override;
	void imgui_render() override;

private:
	static int m_point_light_count;
	int m_index;
	float m_radius = 1.f;
	float m_range = 10.f;
};
