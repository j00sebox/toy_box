#include "pch.h"
#include "Material.h"

#include "Texture.h"
#include "Shader.h"

#include "ImGuiHelper.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <nlohmann/json.hpp>

using namespace nlohmann;

//TODO: remove later
#include "GLError.h"
#include <glad/glad.h>

void Material::load(const std::string* const textures)
{
    m_textures[0] = std::make_unique<Texture2D>(Texture2D(textures[0]));
    m_textures[1] = std::make_unique<Texture2D>(Texture2D(textures[1]));
    m_textures[2] = std::make_unique<Texture2D>(Texture2D(textures[2]));
    m_textures[3] = std::make_unique<Texture2D>(Texture2D(textures[3]));

    m_custom = false;
}

void Material::bind() const
{
    m_shader->set_uniform_1i("u_custom", m_custom);

    if (m_custom)
    {
        m_shader->set_uniform_4f("u_flat_colour", m_colour);
        m_shader->set_uniform_1f("u_metallic", m_metallic);
        m_shader->set_uniform_1f("u_roughness", m_roughness);
    }
    else
    {
//        for (unsigned int i = 0; i < 4; ++i)
//        {
//            m_textures[i]->bind(i);
//        }

        GL_CALL(glActiveTexture(GL_TEXTURE0));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, m_textures[0]->get_id()));

        GL_CALL(glActiveTexture(GL_TEXTURE1));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, m_textures[1]->get_id()));

        GL_CALL(glActiveTexture(GL_TEXTURE2));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, m_textures[2]->get_id()));

        GL_CALL(glActiveTexture(GL_TEXTURE3));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, m_textures[3]->get_id()));
    }

    m_shader->bind();
}

void Material::unbind() const
{
    if (!m_custom)
    {
        for (const auto& m_texture : m_textures)
        {
            m_texture->unbind();
        }
    }
    
    m_shader->unbind();
}

void Material::imgui_render()
{
    static std::string combo_preview = ShaderLib::find(m_shader);
    if (ImGui::BeginCombo("Shader", combo_preview.c_str()))
    {
        for (auto [name, shader_ptr] : ShaderLib::m_shaders)
        {
            const bool is_selected = (name == combo_preview);
            if (ImGui::Selectable(name.c_str(), is_selected))
            {
                combo_preview = name;
                m_shader = ShaderLib::get(combo_preview);
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::Checkbox("Custom", &m_custom);

    if (m_custom)
    {
        float colour[4] = {
            m_colour.x,
            m_colour.y,
            m_colour.z,
            m_colour.w
        };

        ImGui::ColorEdit4("Base Colour", colour);

        m_colour.x = colour[0];
        m_colour.y = colour[1];
        m_colour.z = colour[2];
        m_colour.w = colour[3];

        ImGui::SliderFloat("Metallic", &m_metallic, 0.f, 1.f);
        ImGui::SliderFloat("Roughness", &m_roughness, 0.f, 1.f);
    }
    else
    {
        ImGui::Text("\nBase Colour\n");
        texture_viewer(m_textures[0]->get_id(), m_textures[0]->get_width(), m_textures[0]->get_height());

        ImGui::Text("\nMetallic Roughness\n");
        texture_viewer(m_textures[1]->get_id(), m_textures[1]->get_width(), m_textures[1]->get_height());

        ImGui::Text("\nNormal Map\n");
        texture_viewer(m_textures[2]->get_id(), m_textures[2]->get_width(), m_textures[2]->get_height());

        ImGui::Text("\nOcclusion Map\n");
        texture_viewer(m_textures[3]->get_id(), m_textures[3]->get_width(), m_textures[3]->get_height());
    }
}

void Material::serialize(json& accessor) const
{
}
