#pragma once

#include "Log.h"

#include "mathz/Matrix.h"

#include <string>
#include <unordered_map>

enum class ShaderType
{
	None = 0,
	Vertex,
	Fragment
};

struct Shader
{
	Shader(const char* _path, ShaderType _type) 
		: file_path(_path), type(_type)  {}

	unsigned int id = 0;
	std::string file_path;
	ShaderType type;
};

template<typename T>
concept is_shader = std::convertible_to<T, Shader>;

class ShaderProgram
{
public:
	template<is_shader ... Shaders>
	ShaderProgram(const Shaders& ... s)
	{
		create_program();

		m_shaders = { s ... };

		for (int i = 0; i < m_shaders.size(); ++i)
		{
			std::string src = load_shader(m_shaders[i]);

			create_shader(m_shaders[i], src);

			compile_shader(m_shaders[i].id);

			attach_shader(m_shaders[i].id);
		}

		link();
		delete_shaders();
	}

	ShaderProgram(ShaderProgram&& sp) noexcept;
	~ShaderProgram();

	void set_uniform_1f(const std::string& name, float x);
	void set_uniform_2f(const std::string& name, float x, float y);
	void set_uniform_3f(const std::string& name, const mathz::Vec3& vec);
	void set_uniform_4f(const std::string& name, const mathz::Vec4& vec);

	void set_uniform_mat4f(const std::string& name, const mathz::Mat4& mat);

	int get_uniform_loaction(const std::string& name);

	void bind() const;
	void unbind() const;

private:
	void create_program();
	std::string load_shader(const Shader& s) const;
	void create_shader(Shader& s, const std::string& src) const;
	void compile_shader(unsigned int id) const;
	void attach_shader(unsigned int id) const;
	void link();
	// also does a detach
	void delete_shaders(); 

	unsigned int m_program_id;
	std::vector<Shader> m_shaders;
	std::unordered_map<std::string, int> m_uniform_location_cache;
};

