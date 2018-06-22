
#include "imgui_horizon.hpp"

#include <switch.h>
#include <vector>

int width, height;
u32 *framebuf;

ImVec2 cpos;

ImFont* titleFont;
ImFont* regularFont;
ImFont* footerFont;
ImFont* smallFont;

ImGuiWindowFlags window_flags;

imgui_sw::SwOptions sw_options;

std::vector<uint32_t> pixel_buffer;

void HorizonInit()
{
    romfsInit();
    gfxInitDefault();
	framebuf = (u32*) gfxGetFramebuffer((u32*)&width, (u32*)&height);

	pixel_buffer = std::vector<uint32_t>(width * height, 0);

	ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)width, (float)height);
 	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	titleFont = io.Fonts->AddFontFromFileTTF("romfs:/Inter-UI-Regular.ttf", 48);
	regularFont = io.Fonts->AddFontFromFileTTF("romfs:/Inter-UI-Regular.ttf", 32);
	footerFont = io.Fonts->AddFontFromFileTTF("romfs:/Inter-UI-Regular.ttf", 28);
    smallFont = io.Fonts->AddFontFromFileTTF("romfs:/Inter-UI-Regular.ttf", 24);
	io.Fonts->Build();

	imgui_sw::bind_imgui_painting();
	imgui_sw::make_style_fast();

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0;
}

void HorizonExit()
{
    imgui_sw::unbind_imgui_painting();
	ImGui::DestroyContext();

	gfxExit();
	romfsExit();
}

bool HorizonLoop()
{
    if (!appletMainLoop())
        return false;

    hidScanInput();
    u32 kHeld = hidKeysHeld(CONTROLLER_P1_AUTO);
    JoystickPosition pos;
    hidJoystickRead(&pos, CONTROLLER_P1_AUTO, JOYSTICK_LEFT);

    if (kHeld & KEY_PLUS) return false;

    ImGuiIO& io = ImGui::GetIO();

    memset(io.NavInputs, 0, sizeof(io.NavInputs));
    #define MAP_BUTTON(NAV, BUTTON)       { if (kHeld & BUTTON) io.NavInputs[NAV] = 1.0f; }
    MAP_BUTTON(ImGuiNavInput_Activate,    KEY_A);
    MAP_BUTTON(ImGuiNavInput_Cancel,      KEY_B);
    MAP_BUTTON(ImGuiNavInput_Menu,        KEY_Y);
    MAP_BUTTON(ImGuiNavInput_Input,       KEY_X);
    MAP_BUTTON(ImGuiNavInput_DpadLeft,    KEY_DLEFT);
    MAP_BUTTON(ImGuiNavInput_DpadRight,   KEY_DRIGHT);
    MAP_BUTTON(ImGuiNavInput_DpadUp,      KEY_DUP);
    MAP_BUTTON(ImGuiNavInput_DpadDown,    KEY_DDOWN);
    MAP_BUTTON(ImGuiNavInput_FocusPrev,   KEY_L);
    MAP_BUTTON(ImGuiNavInput_FocusNext,   KEY_R);
    MAP_BUTTON(ImGuiNavInput_TweakSlow,   KEY_L);
    MAP_BUTTON(ImGuiNavInput_TweakFast,   KEY_R);
    MAP_BUTTON(ImGuiNavInput_LStickLeft,  KEY_LSTICK_LEFT);
    MAP_BUTTON(ImGuiNavInput_LStickRight, KEY_LSTICK_RIGHT);
    MAP_BUTTON(ImGuiNavInput_LStickUp,    KEY_LSTICK_UP);
    MAP_BUTTON(ImGuiNavInput_LStickDown,  KEY_LSTICK_DOWN);
    #undef MAP_BUTTON
    io.BackendFlags |= ImGuiBackendFlags_HasGamepad;

    io.DeltaTime = 1.0f / 60.0f;

    return true;
}


void HorizonMainWindow(const char* title)
{
    window_flags = 0;
    //window_flags |= ImGuiWindowFlags_MenuBar;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoScrollbar;
    window_flags |= ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowSize(ImVec2(width, height - FOOTER_HEIGHT), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowFocus();

    ImGui::PushFont(titleFont);
    ImGui::Begin(title, NULL, window_flags); //TODO Logo

    ImGui::PushFont(regularFont);
    
    ImGui::PushItemWidth(-1.0f);  
}

//TODO Add key entries
void HorizonFooter()
{
    ImGui::PopFont();

    //hacky drawing for header separator
    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(40, 88), ImVec2(1238, 89), IM_COL32(43,43,43,255));

    ImGui::End();
    
    ImGui::PopFont();

    //Footer

    ImGui::SetNextWindowSize(ImVec2(width, FOOTER_HEIGHT - 1), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0, height - FOOTER_HEIGHT));

    window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoScrollbar;
    window_flags |= ImGuiWindowFlags_NoTitleBar;

    ImGui::PushFont(titleFont);
    ImGui::Begin("", NULL, window_flags);

    //TODO Icons
    //TODO Calculate the footer metrics at runtime depending on its text instead of having them spat like that
    
    ImGui::PushFont(footerFont);

    ImGui::PushItemWidth(-1.0f);   

    ImGui::SameLine(width-550);  
    ImGui::SetCursorPosY(FOOTER_HEIGHT/2 - 14); //14 = font size
    ImGui::Text("LStick - Scroll\tDPAD - Navigate\tA - OK");

    ImGui::PopFont();

    //hacky drawing for footer separator
    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(40, height - FOOTER_HEIGHT + 3), ImVec2(1238, height - FOOTER_HEIGHT + 2), IM_COL32(43,43,43,255));

    ImGui::End();
    ImGui::PopFont();
}