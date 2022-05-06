#pragma once

#include <string>

#include "Texture.h"
#include "VertexArray.h"
#include "Buffer.h"
#include "GLTFLoader.h"

#include "components/IComponent.h"

#include "mathz/Vector.h"

struct Vertex
{
	mathz::Vec3 positon;
	mathz::Vec3 normal;
	mathz::Vec2<float> st;
};

class Mesh : public IComponent
{
public:
	Mesh() = default;
	Mesh(Mesh&& mesh) noexcept;

	void load(GLTFLoader loader);
	void bind() const;
	[[nodiscard]] unsigned int get_index_count() const { return m_indices_count; }

	virtual void on_remove() {};
	const char* get_type() const override { return typeid(Mesh).name(); }
	void parse(json mesh) override {};
	void imgui_render() override;

private:
	std::vector<mathz::Vec3> floats_to_vec3(const std::vector<float>& flts) const;
	std::vector<mathz::Vec2<float>> floats_to_vec2(const std::vector<float>& flts) const;

	std::string m_path;
	unsigned int m_indices_count = 0;
	VertexArray m_va;
	std::vector<Texture2D> m_textures;
};

