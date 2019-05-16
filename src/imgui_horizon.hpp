#pragma once

#include <vector>
#include <cstdio>

#include <switch.h>

#include "imgui/imgui.h"
#include "imgui/imgui_sw.h"

extern int width, height;
extern u32* framebuf;
extern Framebuffer fb;

extern ImVec2 cpos;

extern ImFont* titleFont;
extern ImFont* regularFont;
extern ImFont* footerFont;
extern ImFont* smallFont;

extern ImGuiIO& io; 

extern ImGuiWindowFlags window_flags;

extern imgui_sw::SwOptions sw_options;

extern std::vector<uint32_t> pixel_buffer;

#define FOOTER_HEIGHT 75 //for now, changing this will probably break the footer

#define HORIZON_BEGIN_HEADER(control) \
	cpos = ImGui::GetCursorScreenPos(); \
	ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(cpos.x/2 + 1, cpos.y), ImVec2(width - cpos.x/2 - 1, cpos.y+1), IM_COL32(207,207,207,255)); \
	if(control) { \
		cpos = ImGui::GetCursorScreenPos(); \
		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(cpos.x/2 + 1, cpos.y), ImVec2(width - cpos.x/2 - 1, cpos.y + 1), IM_COL32(207,207,207,255)); \
		ImGui::Dummy(ImVec2(10, 20));

#define HORIZON_END_HEADER() \
	ImGui::Dummy(ImVec2(10, 20)); \
	} else { \
		cpos = ImGui::GetCursorScreenPos(); \
		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(cpos.x/2 + 1, cpos.y), ImVec2(width - cpos.x/2 - 1, cpos.y + 1), IM_COL32(207,207,207,255)); \
	} 

#define HORIZON_BEGIN_BUTTON(label) \
    ImGui::Dummy(ImVec2(10, 2)); \
	cpos = ImGui::GetCursorScreenPos(); \
	ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(cpos.x + 1, cpos.y), ImVec2(width - cpos.x - 1, cpos.y+1), IM_COL32(207,207,207,255)); \
	if (ImGui::Button(label, ImVec2(width - cpos.x*2, 65))) { 

#define HORIZON_END_BUTTON() \
	} \
	cpos = ImGui::GetCursorScreenPos(); \
	ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(cpos.x + 1, cpos.y+2), ImVec2(width - cpos.x - 1, cpos.y+3), IM_COL32(207,207,207,255));

#define HORIZON_BUTTON_CAPTION(text) \
    ImGui::Indent(40); \
    ImGui::Dummy(ImVec2(10, 10)); \
    ImGui::PushFont(smallFont); \
    ImGui::TextDisabled(text); \
    ImGui::Dummy(ImVec2(10, 40)); \
    ImGui::PopFont(); \
    ImGui::Unindent(40);

void HorizonInit();
void HorizonMainWindow(const char* title);
void HorizonFooter();
bool HorizonLoop();
void HorizonExit();
