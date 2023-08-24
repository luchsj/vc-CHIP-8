// Definition file for UI system.

// TODO: Change preprocessor directives based on active graphics API.
// cimgui
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#define CIMGUI_USE_OPENGL3
#define CIMGUI_USE_GLFW

#include "../lib/cimgui/cimgui.h"
#include "../lib/cimgui/generator/output/cimgui_impl.h"

#include "ui.h"

#include <stdlib.h>


typedef struct ui_t
{
	void* window;
}
ui_t;

ui_t* ui_init(void* window, char* glsl_version)
{
	ui_t* ret = malloc(sizeof(ui_t));

	ret->window = window;

	// Initialize ImGui.
	igCreateContext(NULL);
  	//ImGui_ImplGlfw_InitForOpenGL(ret->window, true);
  	//ImGui_ImplOpenGL3_Init(glsl_version);
	igStyleColorsDark(NULL);

	return ret;
}

void ui_render(ui_t* ui)
{
	igShowDemoWindow(NULL);
	//ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
}

void ui_destroy(ui_t* ui)
{
	// Shut down ImGui.
	//ImGui_ImplOpenGL3_Shutdown();
  	//ImGui_ImplGlfw_Shutdown();
  	igDestroyContext(NULL);
	
	// Free resources.
	free(ui->window);
}
