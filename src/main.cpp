#include "imgui/imgui.h"
#include "imgui/imfilebrowser.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include "parseobj/parseobj.hpp"
#include <iostream>

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
  int display_w, display_h;
  glfwGetFramebufferSize(window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

std::string getFilename(std::string& path) {
  return path.substr(path.find_last_of('/') + 1);
}

int main(int, char**) {
  
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    return 1;

  const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
  const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    " FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

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
  
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // Create fileDialog for choosing .obj
  ImGui::FileBrowser fileDialog;
  fileDialog.SetTitle("Choose any model");
  fileDialog.SetTypeFilters({".obj"});


  /*---------------------------SHADERS------------------------------*/

  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  int success_v;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success_v);
  if (!success_v) {
    char infoLog[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  int success_f;
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success_f);
  if (!success_f) {
    char infoLog[512];
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::INDEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  int success_p;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success_p);
  if (!success_p) {
    char infoLog[512];
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  glUseProgram(shaderProgram);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  /*------------------------------------------------------------------*/

  modelData m;
  std::string path = "models/cube.obj";
  parseObj(path.c_str(), &m);

  std::string filename, modelname;
  const char *filenamePtr = filename.c_str();
  const char *modelnamePtr = modelname.c_str();

  std::cout << "SMOTRIM!" << std::endl;
  for (glm::vec3 i : m.vertexArray)
    std::cout << i.x << " " << i.y << " " << i.z << " " << std::endl;

  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  GLuint vertexVBO;
  glGenBuffers(1, &vertexVBO);

  /* think about why it need to be in while main loop, mb because new coord after
   * movements and rotation must be copied again to this vbo */

    // copy vertexes to vertexVBO
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, m.vertexArray.size() * sizeof(glm::vec3), &m.vertexArray[0], GL_STATIC_DRAW);

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

    glDrawArrays(GL_TRIANGLES, 0, m.vertexArray.size());

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

      m.vertexArray.clear();
      if (m.name) free(m.name);
      m.vertexNumber = 0;
      m.faceNumber = 0;

      parseObj(filenamePtr, &m);

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
