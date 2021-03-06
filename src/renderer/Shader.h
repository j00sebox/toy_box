#pragma once

#include <mathz/Matrix.h>

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
	explicit ShaderProgram(const Shaders& ... s)
	{
		create_program();

		m_shaders = { s ... };

		for (Shader& shader : m_shaders)
		{
			std::string src = load_shader(shader);

			create_shader(shader, src);

			compile_shader(shader.id);

			attach_shader(shader.id);
		}

		link();
		delete_shaders();
		get_active_uniforms();
	}

	ShaderProgram(ShaderProgram&& sp) noexcept;
	~ShaderProgram();

	void set_uniform_1i(const std::string& name, int i);
	void set_uniform_1f(const std::string& name, float x);
	void set_uniform_2f(const std::string& name, float x, float y);
	void set_uniform_3f(const std::string& name, const mathz::Vec3& vec);
	void set_uniform_4f(const std::string& name, const mathz::Vec4& vec);
	void set_uniform_mat4f(const std::string& name, const mathz::Mat4& mat);

	[[nodiscard]] int get_uniform_loaction(const std::string& name);
	[[nodiscard]] const std::vector<std::string>& get_shader_locations() const { return m_shader_locations; } // TODO: Remove later

	void bind() const;
	void unbind() const;

private:
	void create_program();
	std::string load_shader(const Shader& s);
	void create_shader(Shader& s, const std::string& src) const;
	void compile_shader(unsigned int id) const;
	void attach_shader(unsigned int id) const;
	void link();
	// also does a detach
	void delete_shaders(); 
	void get_active_uniforms() const;

	unsigned int m_program_id = 0;
	std::vector<Shader> m_shaders;
	std::vector<std::string> m_shader_locations;
	std::unordered_map<std::string, int> m_uniform_location_cache;
};

class ShaderLib
{
public:
	static void add(const std::string& name, ShaderProgram&& sp);
	static std::shared_ptr<ShaderProgram> get(const std::string& name);
	static size_t get_num() { return m_shaders.size(); }
	static bool exists(const std::string& name);
	static std::string find(const std::shared_ptr<ShaderProgram>& s);
	static void release();

private:
	static std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> m_shaders;

    friend class Material;
	friend class SceneSerializer;
};