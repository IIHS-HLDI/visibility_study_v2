#include "SceneWindow.h"



SceneWindow::SceneWindow(const std::string windowName,
        std::shared_ptr<ApplicationState> appState) : 
    appState(appState),
    canvas(appState) {
    /*view(appState) {*/


    isHovered = false;
    isFocused = false;

    this->windowName = windowName;
    this->width = 0;
    this->height = 0;

}


// resize framebuffer
void SceneWindow::rescale()
{
	/*glBindTexture(GL_TEXTURE_2D, tex);*/
	/*glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);*/
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);*/
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/
	/*glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);*/
	/**/
	/*glBindRenderbuffer(GL_RENDERBUFFER, rbo);*/
	/*glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);*/
	/*glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);*/
	/*   glViewport(0,0,width,height);*/
	/*   view.setWidth(width);*/
	/*   view.setHeight(height);*/
}

void SceneWindow::layout() {
	
	ImGui::Begin(windowName.c_str());

    /*static ImVec2 scrolling(0.0f, 0.0f);*/

    /*static float zoom = 1.0f;*/

	/*ImVec2 availableSize = ImGui::GetWindowContentRegionMax();*/

	/*position = ImGui::GetWindowPos();*/
    canvas.layout();
	/*this->width = availableSize.x;*/
	/*this->height = availableSize.y;*/
	/**/
	/*   ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!*/
	/*   ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available*/
	/*   if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;*/
	/*   if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;*/
	/*   ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);*/
	/**/
	/**/
	/*   ImGuiIO& io = ImGui::GetIO();*/
	/**/
	/*   ImDrawList* drawList = ImGui::GetWindowDrawList();*/
	/**/
	/*   drawList->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50,50,50,255));*/
	/**/
	/*   drawList->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));*/
	/**/
	/**/
	/*   ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);*/
	/*   const bool is_hovered = ImGui::IsItemHovered(); // Hovered*/
	/*   const bool is_active = ImGui::IsItemActive();   // Held*/
	/*   const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin*/
	/*   const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);*/
	/**/
	/*   // Pan (we use a zero mouse threshold when there's no context menu)*/
	/*   // You may decide to make that threshold dynamic based on whether the mouse is hovering something etc.*/
	   /*const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;*/
	/*   const float mouse_threshold_for_pan = 0.0f;*/
	/*   if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))*/
	/*   {*/
	/*       scrolling.x += io.MouseDelta.x;*/
	/*       scrolling.y += io.MouseDelta.y;*/
	/*   }*/
	/**/
	   /*const float mouse_threshold_for_pan = 0.0f;*/
	   /*if (is_active && )*/
	   /*{*/
	/*   if (abs(io.MouseWheel) > 0 &&*/
	/*           (zoom + io.MouseWheel/300) < 5 && */
	/*           0 < (zoom + io.MouseWheel/500)) {*/
	/*       scrolling.x += io.MousePos.x;*/
	/*       scrolling.y += io.MousePos.y;*/
	/*       zoom += io.MouseWheel/300;*/
	/*   }*/
	   /*}*/
	/**/
	/**/
	/*   // Draw grid + all lines in the canvas*/
	/*   drawList->PushClipRect(canvas_p0, canvas_p1, true);*/
	   /*if (opt_enable_grid)*/
	   /*{*/
	   /*const float GRID_STEP = 64.0f*zoom;*/
	/**/
	/*   const auto GRAY = IM_COL32(200, 200, 200, 40);*/
	/**/
	/*   const float GRID_STEP = 64.0*zoom;*/
	/*   for (float x = scrolling.x; x < canvas_sz.x; x += GRID_STEP) {*/
	/*     drawList->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), GRAY);*/
	/*   }*/
	/**/
	/*   for (float y = scrolling.y; y < canvas_sz.y; y += GRID_STEP) {*/
	/*     drawList->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), GRAY);*/
	/*   }*/
	/**/
	/**/
	   /*for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP) {*/
	   /*    drawList->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));*/
	   /*}*/
	   /**/
	   /*for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP) {*/
	   /*    drawList->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40)); */
	   /*}*/
	/**/
	/*   drawList->PopClipRect();*/



	/*scene.setViewportWidth(availableSize.x);*/
	/*scene.setViewportHeight(availableSize.y);*/

	/*rescale();*/
	/*ImGui::Image((ImTextureID)static_cast<uintptr_t>(tex),*/
	/*	ImGui::GetContentRegionAvail(),*/
	/*	ImVec2(0, 1),*/
	/*	ImVec2(1, 0));*/
	
    /*glBindFramebuffer(GL_FRAMEBUFFER, fbo);*/
    /*glBindTexture(GL_TEXTURE_2D,tex);*/
    /*glBindRenderbuffer(GL_RENDERBUFFER, rbo);*/
	/*scene.render();*/
    /*glClearColor(0.0, 0.0, 0.0, 1.0);*/
    /*glClear(GL_COLOR_BUFFER_BIT);*/
    /*view.draw();*/
    /*glBindFramebuffer(GL_FRAMEBUFFER, 0);*/
    /*glBindTexture(GL_TEXTURE_2D,0);*/
    /*glBindRenderbuffer(GL_RENDERBUFFER, 0);*/
	

	ImGui::End();
}

void SceneWindow::close() {
	/*scene.close();*/

	/*glDeleteFramebuffers(1, &fbo);*/
	/*glDeleteTextures(1, &tex);*/
	/*glDeleteRenderbuffers(1, &rbo);*/
}
