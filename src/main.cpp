// ImGui Test on Nintendo Switch

#include <switch.h>
#include <vector>
#include <cstdio>

#include "imgui/imgui.h"
#include "imgui/imgui_sw.h"

#include "imgui_horizon.hpp"
#define FB_WIDTH  1280
#define FB_HEIGHT 720

int main(int argc, char* argv[])
{
	socketInitializeDefault();

	//nxlinkStdio();	

	HorizonInit();

	while (HorizonLoop()) {

		ImGui::NewFrame();

		// Retrieve the framebuffer
        u32 stride;
		framebuf = (u32*) framebufferBegin(&fb, &stride);
		//Main window
		HorizonMainWindow("ImGui Nintendo Switch Theme");

		HORIZON_BEGIN_HEADER(ImGui::CollapsingHeader("About this demo"))
			ImGui::SetItemDefaultFocus();
			ImGui::TextWrapped("Theme created by natinusala, based on imgui-switch by carstene1ns. Fonts from precisionui, courtesy of relatived.");
		HORIZON_END_HEADER()

		HORIZON_BEGIN_HEADER(ImGui::CollapsingHeader("Controls"))
			HORIZON_BEGIN_BUTTON("Show popup")
			HORIZON_END_BUTTON()

			HORIZON_BEGIN_BUTTON("Useless button")
			HORIZON_END_BUTTON()
			HORIZON_BUTTON_CAPTION("This button doesn't do anything.")

			HORIZON_BEGIN_BUTTON("Will this one do something ?")
			HORIZON_END_BUTTON()
			
		HORIZON_END_HEADER()

		HorizonFooter();

		ImGui::Render();

		// fill the gap between the two windows
		std::fill_n(pixel_buffer.data(), width * height, 0xFFFFFFFFu);

		// overlay the GUI
		paint_imgui(pixel_buffer.data(), width, height, sw_options);

		// draw to screen
		for (u32 i = 0; i < FB_HEIGHT; i++)
			for (u32 j = 0; j < FB_WIDTH; j++) {
				u32 pos = i * stride / sizeof(u32) + j;
				framebuf[pos] = pixel_buffer.data()[pos];
			}

		// We're done rendering, so we end the frame here.
        framebufferEnd(&fb);
	}

	HorizonExit();

	socketExit();

	return 0;
}
