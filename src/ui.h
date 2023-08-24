// Header file for UI system.

typedef struct ui_t ui_t;

// Initialize the UI system and return a new UI object.
ui_t* ui_init(void* window, char* glsl_version);

// Render a new frame for the UI.
void ui_render(ui_t* ui);

// Shut down the UI system and free all related resources.
// This should be called before the window manager is shut down.
void ui_destroy(ui_t* ui);
