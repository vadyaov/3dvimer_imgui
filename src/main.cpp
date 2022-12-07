#include "imgui/imgui.h"
#include "imgui/imfilebrowser.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
extern "C" {
#include "parseobj/parseobj.h"
}
#include "shaders/glshader.hpp"
#include "affinity/affinity.hpp"
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // Create fileDialog for choosing .obj
  ImGui::FileBrowser fileDialog;
  fileDialog.SetTitle("Choose model");
  fileDialog.SetTypeFilters({".obj"});

  GLuint shaderProgram = LoadShader("shaders/versh.glsl", "shaders/fragm.glsl");
  glUseProgram(shaderProgram);

  model m;
  std::string path = "models/prism.obj";
  parseobj(path.c_str(), &m);

  std::string filename, modelname;
  const char *filenamePtr = filename.c_str();
  const char *modelnamePtr = modelname.c_str();

  /* glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f); */
  /* glm::mat4 trans = glm::mat4(1.0f); */
  /* trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0)); */
  /* trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5)); */
  /* vec = trans * vec; */
  /* std::cout << vec.x << vec.y << vec.z << std::endl; */

  /* GLuint transformLoc = glGetUniformLocation(shaderProgram, "transform"); */
  /* glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans)); */


  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  GLuint vertexVBO;
  glGenBuffers(1, &vertexVBO);

  /* think about why it need to be in while main loop, mb because new coord after
   * movements and rotation must be copied again to this vbo */

    // copy vertexes to vertexVBO
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, m.indexNumber * 3 * 3 * sizeof(float), &m.vertexArray[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    /* glBindBuffer(GL_ARRAY_BUFFER, vertexVBO); */

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  /* glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind */

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
               clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    processInput(window);

    // Start the Dear Imgui frame
    startFrame();

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
  model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -20.0f));
  GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
  glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, m.indexNumber * 3  * 3);

    {
      ImGui::Begin("Settings");
      if (ImGui::Button("Browse obj"))
        fileDialog.Open();
      ImGui::Text("File: ");
      ImGui::SameLine();
      ImGui::Text(filenamePtr);
      ImGui::Text("Model: ");
      ImGui::SameLine();
      ImGui::Text(modelnamePtr);
      ImGui::ColorEdit3("back color", (float*)&clear_color);
      ImGui::End();
    }

    fileDialog.Display();

    if (fileDialog.HasSelected()) {
      filename = fileDialog.GetSelected().string();
      filenamePtr = filename.c_str();

      free(m.vertexArray);
      if (m.name) free(m.name);
      m.vertexNumber = 0;
      m.indexNumber = 0;

      parseobj(filenamePtr, &m);

      modelname = m.name;
      modelnamePtr = modelname.c_str();

      filename = getFilename(filename);

      fileDialog.ClearSelected();
    }

    // Rendering
    render(clear_color, window);

    glfwSwapBuffers(window);
    // Poll and handle events (inputs, window resize, etc.)
    glfwPollEvents();
  }
  // GL_POINTS, GL_TRIANGLES, GL_LINE_STRIP --> primitives //

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
