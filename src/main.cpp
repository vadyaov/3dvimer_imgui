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

static void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
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

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  /*------------------------------------------------------------------*/

  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
  };

  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  
  /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */

  std::string filename, modelname;
  const char *filenamePtr = filename.c_str();
  const char *modelnamePtr = modelname.c_str();

  modelData m;
  std::string path = "models/diamond.obj";
  parseObj(path.c_str(), &m);

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    // Poll and handle events (inputs, window resize, etc.)
    processInput(window);

    // Start the Dear Imgui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

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

      parseObj(filenamePtr, &m);

      modelname = m.name;
      modelnamePtr = modelname.c_str();

      filename = getFilename(filename);

      fileDialog.ClearSelected();
    }

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
