#pragma once

#include "imgui/imgui.h"
#include "imgui/imfilebrowser.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include </Users/neelyarl/.brew/Cellar/glew/2.2.0_1/include/GL/glew.h>
#include <glm/glm.hpp>
#include </Users/neelyarl/.brew/Cellar/glfw/3.3.8/include/GLFW/glfw3.h> // will drag system opengl headers

extern "C" {
#include "parseobj/parseobj.h"
#include "affinity/affinity.h"
}

#include "shaders/glshader.hpp"
#include "affinity/affinity.h"
#include <iostream>

#include </Users/neelyarl/.brew/Cellar/glm/0.9.9.8/include/glm/glm.hpp>
#include </Users/neelyarl/.brew/Cellar/glm/0.9.9.8/include/glm/gtc/matrix_transform.hpp>
#include </Users/neelyarl/.brew/Cellar/glm/0.9.9.8/include/glm/gtc/type_ptr.hpp>

#define GL_SILENCE_DEPRECATION
#define SCREEN_WIDTH 1280.0f
#define SCREEN_HEIGHT 720.0f

typedef struct Model {
  float zoom;
  float addScale;
  float moveRange;
  float angle;
  bool triangles, lines;
  int linewidth;
} Settings;

std::string getFilename(std::string& path);
static void render(ImVec4 &clear_color, GLFWwindow* window);
static void startFrame();
static void processInput(GLFWwindow *window);
static void glfw_error_callback(int error, const char* description);
static void HelpMarker(const char* desc);
static void initSettings(Settings *s);
