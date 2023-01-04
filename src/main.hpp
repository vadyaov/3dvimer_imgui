#ifndef MAIN_HPP
#define MAIN_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

extern "C" {
#include "core/affinity.h"
#include "core/parseobj.h"
}

#include <SDL2/SDL_surface.h>

#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "imgui/imfilebrowser.h"
#include "shaders/glshader.hpp"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

typedef struct ProgramState {
  float addScale;
  float moveRange;
  float angle;
  float pointsize;
  int linewidth;
  bool triangles, lines, points;

  ImVec4 clear_color;
  ImVec4 vertex_color;
  ImVec4 edge_color;
  ImVec4 point_color;

  std::string path;
  std::string filename;
  const char *filenamePtr;
  const char *modelnamePtr;

  int scheme;
  int perspective;

  bool jpeg, bmp;
} Settings;

typedef struct CameraSettings {
  glm::vec3 cameraPos;
  glm::vec3 cameraFront;
  glm::vec3 cameraUp;

  float deltaTime;
  float lastFrame;
  float speed;
} Camera;

void render(GLFWwindow *);
void startFrame();
void processInput(GLFWwindow *, Camera *);
void glfw_error_callback(int, const char *);
void HelpMarker(const char *);
void initSettings(Settings *, Camera *);
void initCameraBasics(Camera *);
void makeMVP(glm::mat4 &, glm::mat4 &, glm::mat4 &, GLuint);
void draw(GLuint, size_t, float *, GLuint, GLuint, int, float);
void cleanFile(const char *);
void saveSettings(const char *, Settings *, Camera *);
void ImGuiSettingsWindow(GLFWwindow *, Settings &, model *, Camera *,
                         ImGui::FileBrowser &);
void makeScreenShot(GLFWwindow *, Settings &);
void clearModelMemory(model *);
std::string getFilename(std::string &);

void fileBrowser(ImGui::FileBrowser &, Settings &, model *);
void ImGui_Info(model *, Settings &);
void ImGui_ColorSceme(Settings &);
void ImGui_Rotate(model *, Settings &);
void ImGui_Move(model *, Settings &);
void ImGui_Scale(model *, Settings &);
void ImGui_Colors(Settings &);
void ImGui_RenderSettings(Settings &);

#endif /* MAIN_HPP */
