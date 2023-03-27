// Window manager. Controls window instances and user input.
#include <stdio.h>
#include <stdlib.h>

#include "wm.h"

// GL
#include "GL/glew.h"
#define GLEW_STATIC // Required for Windows executable

#include "cglm/cam.h"
#include "cglm/mat4.h"
#include "cglm/quat.h"
#include <GLFW/glfw3.h>

// Temp shaders
static const char* vertex_shader_text =
"#version 110\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 110\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

// Main window manager object
typedef struct wm_t
{
	GLFWwindow* window;
	uint32_t key_mask;

	GLuint vertex_buffer, vertex_shader, fragment_shader, program;
	GLint mvp_location, vpos_location, vcol_location;

} wm_t;

static const struct
{
	float x, y;
	float r, g, b;
} vertices[4] =
{
	{ -1.f, -1.f, 0.f, 0.f, 0.f }, 
	{ -1.f, 1.f, 0.f, 0.f, 1.f }, 
	{ 1.f, -1.f, 1.f, 0.f, 0.f }, 
	{ 1.f, 1.f, 1.f, 1.f, 1.f },
};

const struct
{
	int virtual_key;
	int vc_key;
}
k_key_map[]=
{
	{.virtual_key = GLFW_KEY_ESCAPE, .vc_key = k_key_esc,},
};

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
		case GLFW_KEY_ESCAPE:
			if(action == GLFW_PRESS)
				glfwSetWindowShouldClose(window, GLFW_TRUE);

		default:
			break;
	}
}

// TODO: Report size change on callback
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	return;
}

// Handles the initialization of GL-related buffers.
void init_gl(wm_t* wm)
{
	// Just need to create a quad that fills the screen.
	GLenum err;

	glGenBuffers(1, &wm->vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, &wm->vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	wm->vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(wm->vertex_shader, 1, &vertex_shader_text, NULL);
	glCompileShader(wm->vertex_shader);

	wm->fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(wm->fragment_shader, 1, &fragment_shader_text, NULL);
	glCompileShader(wm->fragment_shader);

	wm->program = glCreateProgram();
	glAttachShader(wm->program, wm->vertex_shader);
	glAttachShader(wm->program, wm->fragment_shader);
	glLinkProgram(wm->program);	

	wm->mvp_location = glGetUniformLocation(wm->program, "MVP");
	wm->vpos_location = glGetAttribLocation(wm->program, "vPos");
	wm->vcol_location = glGetAttribLocation(wm->program, "vCol");

	glEnableVertexAttribArray(wm->vpos_location);
	glVertexAttribPointer(wm->vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) 0);
	glEnableVertexAttribArray(wm->vcol_location);
	glVertexAttribPointer(wm->vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) (sizeof(float) * 2));
}

// Initializes window, GL, UI, input callbacks, etc.
wm_t* wm_init()
{
	wm_t* wm = malloc(sizeof(wm_t));
	if(!wm)
	{
		fprintf(stderr, "Failed to init wm\n");
		return NULL;
	}
	// Initialize key mask
	wm->key_mask = 0;

	// Initialize GLFW
	if(!glfwInit())
	{
		fprintf(stderr, "WM: GLFW init failure\n");
		return NULL;
	}

	wm->window = glfwCreateWindow(640, 480, "VC-CHIP-8", NULL, NULL);
	if(!wm->window)
	{
		glfwTerminate();
		fprintf(stderr, "WM: GLFW window creation failure\n");
		return NULL;
	}

	glfwMakeContextCurrent(wm->window);

	// Initialize GLEW
	GLenum err = glewInit();
	if(err != GLEW_OK)
	{
		fprintf(stderr, "WM: GLEW init failure: %s\n", glewGetErrorString(err));
		return NULL;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	//fprintf(stdout, "WM: GLEW %s init success\n", glewGetString(GLEW_VERSION));

	// Set GLFW values
	glfwSwapInterval(1);
	glfwSetKeyCallback(wm->window, key_callback);

	int width, height;
	glfwGetFramebufferSize(wm->window, &width, &height);
	glfwSetFramebufferSizeCallback(wm->window, framebuffer_size_callback);
	glViewport(0, 0, width, height);
	
	init_gl(wm);

	return wm;
}

int wm_should_close(wm_t* wm)
{
	return glfwWindowShouldClose(wm->window);
}

// Update the given window.
void wm_update(wm_t* wm)
{
	float ratio;
	int width, height;
	mat4 m, p, mvp;

	glfwGetFramebufferSize(wm->window, &width, &height);
	ratio = width / (float) height;

	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(glfwGetTime() / 10, 0.f, 0.f, .5f);

	glm_mat4_identity(m);
	versor q;
	//glm_quat(q, (float) glfwGetTime(), 0, 0, 90.0f);
	//glm_quat_rotate(m, q, m);
	glm_ortho(-ratio, ratio, -1.0f, 1.0f, 1.0f, -1.0f, p);
	glm_mat4_mul(m, p, mvp);

	glUseProgram(wm->program);
	glUniformMatrix4fv(wm->mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 1, 3);

	glfwSwapBuffers(wm->window);
	glfwPollEvents();

	GLenum err = glGetError();
	if(err != GL_NO_ERROR)
	{
		fprintf(stderr, "WM: GL error: %s\n", gluErrorString(err));
		return;
	}
}

// Takes a set of booleans and creates a GL texture from them.
void wm_get_texture_from_bools(int h, int w, bool** bools)
{
	vec3 color1 = {0, 0, 0};
	vec3 color2 = {1, 1, 1};
}

// Uninitialize window and free related resources.
void wm_terminate(wm_t* wm)
{
	// TODO: crashes when attempting to free window?
	glfwDestroyWindow(wm->window);

	//if(wm->window)
		//free(wm->window);

	glfwTerminate();
}
