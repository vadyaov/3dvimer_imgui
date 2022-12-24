#pragma once

#include "imgui/imgui.h"
#include "imgui/imfilebrowser.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h> // will drag system opengl headers

extern "C" {
#include "core/parseobj.h"
#include "core/affinity.h"
}

#include "shaders/glshader.hpp"
#include <iostream>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GL_SILENCE_DEPRECATION
#define SCREEN_WIDTH 1280.0f
#define SCREEN_HEIGHT 720.0f

typedef struct ProgramState {
  float zoom;
  float addScale;
  float moveRange;
  float angle;
  int linewidth;
  bool triangles, lines;

  ImVec4 clear_color;
  ImVec4 vertex_color;
  ImVec4 edge_color;

  std::string path;
  std::string filename;
  const char *filenamePtr;
  const char *modelnamePtr;

  int scheme;
  int perspective;
} Settings;

std::string getFilename(std::string& path);
void render(ImVec4 &clear_color, GLFWwindow* window);
void startFrame();
void processInput(GLFWwindow *window);
void glfw_error_callback(int error, const char* description);
void HelpMarker(const char* desc);
void initSettings(Settings *s);
void makeMVP(glm::mat4& model, glm::mat4& view, glm::mat4& projection, GLuint shaderProgram);
void draw(GLuint VBO, size_t size, float *array, GLuint VAO, GLuint type, int linewidth);
void cleanFile(const char *str);
void saveSettings(const char *str, Settings *s);
void ImGuiSettingsWindow(Settings& s, model *m, ImGui::FileBrowser& fileDialog);
