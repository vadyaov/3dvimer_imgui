#ifndef _SRC_MAIN_CPP_
#define _SRC_MAIN_CPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>  // will drag system opengl headers

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

extern "C" {
#include "core/affinity.h"
#include "core/parseobj.h"
}

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>

#include "shaders/glshader.hpp"

//#include </Users/neelyarl/.brew/Cellar/sdl2/2.26.1/include/SDL2/SDL.h>
#include <SDL2/SDL_surface.h>

#include "imgui/imfilebrowser.h"

//#define GL_SILENCE_DEPRECATION
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

    bool firstMouse = true;
    float yaw;
    float pitch;
    float lastX;
    float lastY;
    float fov;
} Camera;

std::string getFilename(std::string &path);
void render(GLFWwindow *window);
void startFrame();
void processInput(GLFWwindow *window, Camera *cam);
void glfw_error_callback(int error, const char *description);
void HelpMarker(const char *desc);
void initSettings(Settings *s, Camera *cam);
void initCameraBasics(Camera *cam);
void makeMVP(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection,
             GLuint shaderProgram);
void draw(GLuint VBO, size_t size, float *array, GLuint VAO, GLuint type,
          int linewidth, float pointsize);
void cleanFile(const char *str);
void saveSettings(const char *str, Settings *s, Camera *cam);
void ImGuiSettingsWindow(GLFWwindow *window, Settings &s, model *m, Camera *cam,
                         ImGui::FileBrowser &fileDialog);
void makeScreenShot(GLFWwindow *window, Settings &s);
void clearModelMemory(model *m);

#endif // _SRC_MAIN_CPP_
