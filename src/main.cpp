#include "imgui/imgui.h"
#include "imgui/imfilebrowser.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

extern "C" {
#include "parseobj/parseobj.h"
#include "affinity/affinity.h"
}

#include "shaders/glshader.hpp"
#include "affinity/affinity.h"
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GL_SILENCE_DEPRECATION

static void glfw_error_callback(int error, const char* description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

// closing window by pressing ESC
static void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

static void startFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

static void render(ImVec4 &clear_color, GLFWwindow* window) {
  ImGui::Render();
  // changing model when resize window
  /* int display_w, display_h; */
  /* glfwGetFramebufferSize(window, &display_w, &display_h); */
  /* glViewport(0, 0, display_w, display_h); */
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

std::string getFilename(std::string& path) {
  return path.substr(path.find_last_of('/') + 1);
}

int main(int, char**) {
  
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    return 1;

#if defined(__APPLE_)
  // GL 3.2 + GLSL 150
  const char *glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2)
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
  // GL 3.0 + GLSL 130
  const char *glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // 3.0+ only
  /* glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); */
#endif

  GLFWwindow* window = glfwCreateWindow(1280, 720, "3DViewer_1.0", NULL, NULL);
  if (NULL == window) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  glewExperimental = true;
  if (glewInit() != GLEW_OK)
    return 1;

  // Setup dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsLight();
  
  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
  
  // Background color
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // Verteces Color
  ImVec4 vertex_color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // Create fileDialog for choosing .obj
  ImGui::FileBrowser fileDialog;
  fileDialog.SetTitle("Choose model");
  fileDialog.SetTypeFilters({".obj"});

  GLuint shaderProgram = LoadShader("shaders/versh.glsl", "shaders/fragm.glsl");
  int vertexColorLocation = glGetUniformLocation(shaderProgram, "my_color");
  glUseProgram(shaderProgram);

  model m;
  std::string path = "models/prism.obj";
  std::string filename = getFilename(path);
  const char *filenamePtr = filename.c_str();
  float zoom = 10.0f;
  float scaleX = 1.0, scaleY = 1.0, scaleZ = 1.0;
  float addScale = 0.0f;

  parseobj(path.c_str(), &m);

  const char *modelnamePtr = NULL;
  if (m.name != NULL) modelnamePtr = m.name;

  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  GLuint vertexVBO;
  glGenBuffers(1, &vertexVBO);

  GLuint linesVBO;
  glGenBuffers(1, &linesVBO);

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
               clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    processInput(window);


    // Start the Dear Imgui frame
    startFrame();

    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, m.allIndex * 3 * sizeof(float), &m.vertexArray[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, linesVBO);
    glBufferData(GL_ARRAY_BUFFER, m.lineIndex * 3 * sizeof(float), &m.linesArray[0], GL_STATIC_DRAW);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.7f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -zoom));

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
    glUniform4f(vertexColorLocation, vertex_color.x, vertex_color.y, vertex_color.z, vertex_color.w);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, m.allIndex * 3);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, linesVBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, m.lineIndex * 3);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    {
      ImGui::Begin("Settings");
      if (ImGui::Button("Browse obj"))
        fileDialog.Open();
      ImGui::Text("File:%s", filenamePtr);
      ImGui::Text("Model:%s", modelnamePtr);
      ImGui::SliderFloat("Zoom", &zoom, 80.0f, 5.0f);
      ImGui::InputFloat("Scale coef", &addScale, 0.01f, 1.0f, "%.3f");
      if (ImGui::Button("ScaleX"))
        scale(&m, scaleX + addScale, scaleY, scaleZ);
      ImGui::SameLine();
      if (ImGui::Button("ScaleY"))
        scale(&m, scaleX, scaleY + addScale, scaleZ);
      ImGui::SameLine();
      if (ImGui::Button("ScaleZ"))
        scale(&m, scaleX, scaleY, scaleZ + addScale);
      ImGui::SameLine();
      if (ImGui::Button("Reset Scale"))
        parseobj(path.c_str(), &m);
      ImGui::ColorEdit3("Background color", (float*)&clear_color);
      ImGui::ColorEdit3("Vertex color", (float*)&vertex_color);
      ImGui::End();
    }

    fileDialog.Display();

    if (fileDialog.HasSelected()) {
      path = fileDialog.GetSelected();

      filename = getFilename(path);
      filenamePtr = filename.c_str();

      free(m.vertexArray);
      free(m.linesArray);
      if (m.name) free(m.name);

      parseobj(path.c_str(), &m);

      modelnamePtr = m.name;

      fileDialog.ClearSelected();
    }

    // Rendering
    render(clear_color, window);

    glfwSwapBuffers(window);
    // Poll and handle events (inputs, window resize, etc.)
    glfwPollEvents();
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glDeleteBuffers(1, &vertexVBO);
  glDeleteProgram(shaderProgram);
  glDeleteVertexArrays(1, &VAO);

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
