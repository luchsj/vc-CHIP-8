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
"#version 150 core\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"attribute vec2 texcoord;\n"
"varying vec3 color;\n"
"varying vec2 TexCoord;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"	 TexCoord = texcoord;\n"
"}\n";

static const char* fragment_shader_text =
"#version 150 core\n"
"uniform sampler2D tex;\n"
"varying vec2 TexCoord;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = texture(tex, TexCoord);\n"
"}\n";

// Main window manager object
typedef struct wm_t
{
	GLFWwindow* window;
	uint32_t key_mask;

	GLuint vertex_buffer, vertex_shader, fragment_shader, program;

	// Shader parameters
	GLint mvp_location, vpos_location, vcol_location, texture;
} wm_t;

static const struct
{
	float x, y;
	float r, g, b;
	float s, t;
} vertices[4] =
{
	{ -1.f, -0.5f, 0.f, 0.f, 0.f, 0.f, 1.f}, 
	{ -1.f, 0.5f, 0.f, 0.f, 1.f , 0.f, 0.f}, 
	{ 1.f, -0.5f, 1.f, 0.f, 0.f, 1.f, 1.f}, 
	{ 1.f, 0.5f, 1.f, 1.f, 1.f, 1.f, 0.f},
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

static void render_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param)
{
	GLenum severity_selection = GL_DEBUG_SEVERITY_HIGH | GL_DEBUG_SEVERITY_MEDIUM;

	if ((severity & severity_selection) == 0)
		return;

	char* src;

	switch (source)
	{
		case GL_DEBUG_SOURCE_API:
			src = "API";
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			src = "Window System";
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			src = "Shader Compiler";
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			src = "Third-party App";
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			src = "User-generated";
		case GL_DEBUG_SOURCE_OTHER:
			src = "Other";
			break;
		default:
			src = "Unknown";
			break;
	}

	fprintf(stderr, "GL err: src: %s, msg: %s\n", src, message);
}

// TODO: Report size change on callback
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	return;
}

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW error: %s\n", description);
}

// Handles the initialization of GL-related buffers.
void init_gl(wm_t* wm)
{
	// Just need to create a quad that fills the screen.

	// Enable debug messages
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(render_error_callback, NULL);
	
	// Generate vertex buffers
	glGenBuffers(1, &wm->vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, wm->vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Compile shaders
	wm->vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(wm->vertex_shader, 1, &vertex_shader_text, NULL);
	glCompileShader(wm->vertex_shader);

	wm->fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(wm->fragment_shader, 1, &fragment_shader_text, NULL);
	glCompileShader(wm->fragment_shader);

	GLint vert_success, frag_success;
	glGetShaderiv(wm->vertex_shader, GL_COMPILE_STATUS, &vert_success);
	glGetShaderiv(wm->fragment_shader, GL_COMPILE_STATUS, &frag_success);

	if (vert_success == GL_FALSE)
	{
		GLint log_length = 0;
		glGetShaderiv(wm->vertex_shader, GL_INFO_LOG_LENGTH, &log_length);

		char* msg = "Vertex shader failed to compile:\n";
		glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_HIGH, sizeof(msg), msg);
		
		GLchar* error_log = malloc(log_length);
		glGetShaderInfoLog(wm->vertex_shader, log_length, NULL, error_log);

		glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_HIGH, log_length, msg);

		glDeleteShader(wm->vertex_shader);
		return;
	}

	if (frag_success == GL_FALSE)
	{
		GLint log_length = 0;
		glGetShaderiv(wm->fragment_shader, GL_INFO_LOG_LENGTH, &log_length);

		char* msg = "Fragment shader failed to compile:\n";
		glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_HIGH, sizeof(msg), msg);
		
		GLchar* error_log = malloc(log_length);
		glGetShaderInfoLog(wm->fragment_shader, log_length, NULL, error_log);

		glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_HIGH, log_length, msg);

		glDeleteShader(wm->fragment_shader);
		return;
	}

	wm->program = glCreateProgram();
	glAttachShader(wm->program, wm->vertex_shader);
	glAttachShader(wm->program, wm->fragment_shader);
	glLinkProgram(wm->program);	
	glUseProgram(wm->program);

	wm->mvp_location = glGetUniformLocation(wm->program, "MVP");
	wm->vpos_location = glGetAttribLocation(wm->program, "vPos");
	wm->vcol_location = glGetAttribLocation(wm->program, "vCol");
	wm->texture = glGetAttribLocation(wm->program, "texcoord");

	glEnableVertexAttribArray(wm->vpos_location);
	glVertexAttribPointer(wm->vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) 0);
	glEnableVertexAttribArray(wm->vcol_location);
	glVertexAttribPointer(wm->vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) (sizeof(float) * 2));
	glEnableVertexAttribArray(wm->texture);
	glVertexAttribPointer(wm->texture, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) (sizeof(float) * 5));
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
	glfwSetErrorCallback(glfw_error_callback);

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
	//wm_init_texture(wm, NULL, );

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
	glClearColor(0.1f, 0.1f, 0.1f, 1.f);

	glm_mat4_identity(m);
	//versor q;
	//glm_quat(q, (float) glfwGetTime(), 0, 0, 90.0f);
	//glm_quat_rotate(m, q, m);
	glm_ortho(-ratio, ratio, -1.0f, 1.0f, 1.0f, -1.0f, p);
	glm_mat4_mul(m, p, mvp);

	glUseProgram(wm->program);
	glUniformMatrix4fv(wm->mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
	//glDrawElements
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 1, 3);

	glfwSwapBuffers(wm->window);
	glfwPollEvents();
}

// Takes a set of floats representing RGB values and creates a GL texture from them.
// TODO: specification
void wm_init_texture(wm_t* wm, float* tex_data, int h, int w)
{
	// Test: b/w checkerboard
	float pixels[] =
	{
		0.0f, 0.0f, 0.0f,	1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,
	};

	GLuint texture;
	glGenTextures(1, &texture); // invalid value occurring here - possibly shader compilation issue
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Why does the specification specify that "border" has to be 0?
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_FLOAT, tex_data);	
	glUniform1i(glGetUniformLocation(wm->program, "tex"), 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//*id = texture;
	glGenerateMipmap(GL_TEXTURE_2D);

	GLenum err = glGetError();
	if(err != GL_NO_ERROR)
	{
		fprintf(stderr, "WM: Texture init failure: %s\n", gluErrorString(err));
		return;
	}
}

// TODO: function which replaces the texture at an existing ID

// Uninitialize window and free related resources.
void wm_terminate(wm_t* wm)
{
	// TODO: crashes when attempting to free window?
	glfwDestroyWindow(wm->window);

	//if(wm->window)
		//free(wm->window);

	glfwTerminate();
}
