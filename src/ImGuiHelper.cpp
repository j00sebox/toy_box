#include "pch.h"
#include "ImGuiHelper.h"

#include <imgui.h>

void coloured_label(const char* label, ImVec4 colour, ImVec2 size)
{
    ImGui::PushStyleColor(ImGuiCol_Button, colour);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colour);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, colour);
    ImGui::Button(label, size);
    ImGui::PopStyleColor(3);
}

void texture_viewer(unsigned int texture_id, float texture_width, float texture_height)
{
    ImGuiIO& io = ImGui::GetIO();
    {
        ImGui::Text("%.0fx%.0f", texture_width, texture_height);
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 uv_min = ImVec2(0.0f, 0.0f);
        ImVec2 uv_max = ImVec2(1.0f, 1.0f);
        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
        ImGui::Image((ImTextureID)texture_id, ImVec2(texture_width * 0.05f, texture_height * 0.05f), uv_min, uv_max, tint_col, border_col);
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            float region_sz = 32.0f;
            float region_x = io.MousePos.x - pos.x - region_sz * 0.5f;
            float region_y = io.MousePos.y - pos.y - region_sz * 0.5f;
            float zoom = 4.0f;
            if (region_x < 0.0f) { region_x = 0.0f; }
            else if (region_x > texture_width - region_sz) { region_x = texture_width - region_sz; }
            if (region_y < 0.0f) { region_y = 0.0f; }
            else if (region_y > texture_height - region_sz) { region_y = texture_height - region_sz; }
            ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
            ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
            ImVec2 uv0 = ImVec2((region_x) / texture_width, (region_y) / texture_height);
            ImVec2 uv1 = ImVec2((region_x + region_sz) / texture_width, (region_y + region_sz) / texture_height);
            ImGui::Image((ImTextureID)texture_id, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, tint_col, border_col);
            ImGui::EndTooltip();
        }
    }
}