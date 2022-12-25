#include "main.hpp"

int main(int, char**) {
  
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    return 1;

#if defined(__APPLE__)
  // GL 3.2 + GLSL 150
  const char *glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
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

  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3DViewer_1.0", NULL, NULL);
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

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // Setup dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
  
  ImGui::FileBrowser fileDialog;
  fileDialog.SetTitle("Choose model");
  fileDialog.SetTypeFilters({".obj"});

  GLuint shaderProgram = LoadShader("shaders/versh.glsl", "shaders/fragm.glsl");
  GLuint vertexColorLocation = glGetUniformLocation(shaderProgram, "my_color");
  glUseProgram(shaderProgram);

  model m;
  Settings s;
  initSettings(&s);

  parseobj(s.path.c_str(), &m);
  s.modelnamePtr = m.name;

  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  GLuint vertexVBO;
  glGenBuffers(1, &vertexVBO);

  GLuint linesVBO;
  glGenBuffers(1, &linesVBO);

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    glClearColor(s.clear_color.x * s.clear_color.w, s.clear_color.y * s.clear_color.w,
               s.clear_color.z * s.clear_color.w, s.clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    processInput(window);

    // Start the Dear Imgui frame
    startFrame();

    glm::mat4 model = glm::mat4(1.0f);
    /* glm::mat4 view = glm::mat4(1.0f); */
    glm::mat4 projection;

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, s.zoom);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 view = glm::lookAt(cameraPos, cameraFront, cameraUp);

    if (s.perspective) {
      projection = glm::perspective(glm::radians(50.0f), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 500.0f);
      /* view = glm::translate(view, glm::vec3(0.0f, 0.0f, -s.zoom)); */
    } else {
      projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 500.0f);
    }

    if (s.triangles) {
      makeMVP(model, view, projection, shaderProgram);
      glUniform4f(vertexColorLocation, s.vertex_color.x, s.vertex_color.y, s.vertex_color.z, s.vertex_color.w);
      draw(vertexVBO, m.allIndex * 3, &m.vertexArray[0], VAO, GL_TRIANGLES, s.linewidth);
    }

    if (s.lines) {
      makeMVP(model, view, projection, shaderProgram);
      glUniform4f(vertexColorLocation, s.edge_color.x, s.edge_color.y, s.edge_color.z, s.edge_color.w);
      draw(linesVBO, m.lineIndex * 3, &m.linesArray[0], VAO, GL_LINES, s.linewidth);
    }

    glDisableVertexAttribArray(0);
    ImGuiSettingsWindow(s, &m, fileDialog);


    // Rendering
    render(s.clear_color, window);

    glfwSwapBuffers(window);
    // Poll and handle events (inputs, window resize, etc.)
    glfwPollEvents();
    if (glfwWindowShouldClose(window)) {
      cleanFile("savefile.vimer");
      saveSettings("savefile.vimer", &s);
    }
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

void HelpMarker(const char* desc) {
  ImGui::TextDisabled("(?)");
  if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort)) {
    ImGui::BeginTooltip();
    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
    ImGui::TextUnformatted(desc);
    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();
  }
}

void glfw_error_callback(int error, const char* description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

// closing window by pressing ESC
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void startFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void render(ImVec4 &clear_color, GLFWwindow* window) {
  ImGui::Render();
  int display_w, display_h;
  glfwGetFramebufferSize(window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

std::string getFilename(std::string& path) {
  return path.substr(path.find_last_of('/') + 1);
}

void initSettings(Settings *s) {
  FILE *f = fopen("savefile.vimer", "r");
  if (f) {
    char *line = NULL;
    size_t len = 0;
    size_t i = 0;
    float r = 0.0f, g = 0.0f, b = 0.0f, w = 0.0f;
    while (i < 13) {
      getline(&line, &len, f);
      if (0 == i) {
        s->zoom = std::atof(line);
      } else if (1 == i) {
        s->addScale = std::atof(line);
      } else if (2 == i) {
        s->moveRange = std::atof(line);
      } else if (3 == i) {
        s->angle = std::atof(line);
      } else if (4 == i) {
        s->linewidth = std::atoi(line);
      } else if (5 == i) {
        if (std::atoi(line) == 1)
          s->triangles = true;
        else
          s->triangles = false;
      } else if (6 == i) {
        if (std::atoi(line) == 1)
          s->lines = true;
        else
          s->lines = false;
      } else if (i == 7) {
        sscanf(line, "%f %f %f %f", &r, &g, &b, &w);
        s->clear_color = ImVec4(r, g, b, w);
      } else if (i == 8) {
        sscanf(line, "%f %f %f %f", &r, &g, &b, &w);
        s->vertex_color = ImVec4(r, g, b, w);
      } else if (i == 9) {
        sscanf(line, "%f %f %f %f", &r, &g, &b, &w);
        s->edge_color = ImVec4(r, g, b, w);
      } else if (i == 10) {
        for (size_t j = 0; line[j] != '\n' && line[j] != '\0'; j++)
          s->path += line[j];
          s->filename = getFilename(s->path);
          s->filenamePtr = s->filename.c_str();
      } else if (i == 11) {
        s->scheme = atoi(line);
      } else if (i == 12) {
        if (std::atoi(line) == 1)
          s->perspective = true;
        else
          s->perspective = false;
      }
      i++;
    }
    if (line) free(line);
    fclose(f);
  } else {
    std::cout << "No savefile.vimer in current directory\n";
  }
}

void makeMVP(glm::mat4& model, glm::mat4& view, glm::mat4& projection, GLuint shaderProgram) {
  GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
  glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void draw(GLuint VBO, size_t size, float *array, GLuint VAO, GLuint type, int linewidth) {
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), array, GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);

  glBindVertexArray(VAO);
  if (type == GL_LINES)
    glLineWidth(linewidth);
  glDrawArrays(type, 0, size);
}

void cleanFile(const char *str) {
  std::ofstream ofs;
  ofs.open(str, std::ofstream::out | std::ofstream::trunc);
  ofs.close();
}

void saveSettings(const char *str, Settings *s) {
  FILE *fp = fopen(str, "w");
  if (fp) {
    fprintf(fp, "%f\n", s->zoom);
    fprintf(fp, "%f\n", s->addScale);
    fprintf(fp, "%f\n", s->moveRange);
    fprintf(fp, "%f\n", s->angle);
    fprintf(fp, "%d\n", s->linewidth);
    fprintf(fp, "%d\n", s->triangles);
    fprintf(fp, "%d\n", s->lines);
    fprintf(fp, "%f %f %f %f\n", s->clear_color.x, s->clear_color.y, s->clear_color.z, s->clear_color.w);
    fprintf(fp, "%f %f %f %f\n", s->vertex_color.x, s->vertex_color.y, s->vertex_color.z, s->vertex_color.w);
    fprintf(fp, "%f %f %f %f\n", s->edge_color.x, s->edge_color.y, s->edge_color.z, s->edge_color.w);
    fprintf(fp, "%s\n", s->path.c_str());
    fprintf(fp, "%d\n", s->scheme);
    fprintf(fp, "%d\n", s->perspective);
    fclose(fp);
  }
}

void ImGuiSettingsWindow(Settings& s, model *m, ImGui::FileBrowser& fileDialog) {
  ImGui::Begin("Settings");
  if (ImGui::Button("Browse obj"))
    fileDialog.Open();

  ImGui::SameLine();
  ImGui::RadioButton("Dark", &s.scheme, 0); ImGui::SameLine();
  ImGui::RadioButton("Light", &s.scheme, 1);
  if (!s.scheme)
   ImGui::StyleColorsDark();
  else
    ImGui::StyleColorsLight();

  ImGui::Text("File:%s", s.filenamePtr);
  ImGui::Text("Model:%s", s.modelnamePtr);
  ImGui::Text("Vertex:%zu", m->vertexNumber);ImGui::SameLine();
  ImGui::Text("\tIndex:%zu", m->indexNumber);ImGui::SameLine();
  ImGui::Text("\tEdges:%d", m->edges);ImGui::SameLine();
  HelpMarker("Include joint edjes of 2 triangles");


  ImGui::Separator();

  ImGui::SliderFloat("Zoom", &s.zoom, 200.0f, 1.0f);

  ImGui::SliderFloat("Scale coef", &s.addScale, 0.1f, 5.0f, "%.3f");
  if (s.addScale < 0) s.addScale = -1 / s.addScale;
  if (ImGui::Button("ScaleX"))
    scale(m, s.addScale, 1, 1);
  ImGui::SameLine();
  if (ImGui::Button("ScaleY"))
    scale(m, 1, s.addScale, 1);
  ImGui::SameLine();
  if (ImGui::Button("ScaleZ"))
    scale(m, 1, 1, s.addScale);

  ImGui::Separator();

  ImGui::SliderFloat("Move range", &s.moveRange, 0.0f, 10.0f);
  ImGui::PushButtonRepeat(true);
  if (ImGui::ArrowButton("##left", ImGuiDir_Left))
    move(m, -s.moveRange, 0, 0);
  ImGui::SameLine();
  if (ImGui::ArrowButton("##right", ImGuiDir_Right))
    move(m, s.moveRange, 0, 0);
  ImGui::SameLine();
  if (ImGui::ArrowButton("##up", ImGuiDir_Up))
    move(m, 0, s.moveRange, 0);
  ImGui::SameLine();
  if (ImGui::ArrowButton("##down", ImGuiDir_Down))
    move(m, 0, -s.moveRange, 0);
  ImGui::SameLine();
  if (ImGui::Button("Farther(-Z)"))
    move(m, 0, 0, -s.moveRange);
  ImGui::SameLine();
  if (ImGui::Button("Closer(+Z)"))
    move(m, 0, 0, s.moveRange);
  ImGui::PopButtonRepeat();

  ImGui::Separator();

  ImGui::SliderAngle("Angle", &s.angle);
  ImGui::PushButtonRepeat(true);
  if (ImGui::Button("RotateX"))
    rotate(m, s.angle, 'x');
  ImGui::SameLine();
  if (ImGui::Button("RotateY"))
    rotate(m, s.angle, 'y');
  ImGui::SameLine();
  if (ImGui::Button("RotateZ"))
    rotate(m, s.angle, 'z');
  ImGui::PopButtonRepeat();

  ImGui::Separator();

  if (ImGui::Button("RESET ALL")) {
    free(m->vertexArray);
    free(m->linesArray);
    parseobj(s.path.c_str(), m);
  }
  ImGui::SameLine(); HelpMarker("Resets the position, scale, rotation");

  ImGui::ColorEdit3("Background color", (float*)&s.clear_color);
  ImGui::ColorEdit3("Vertex color", (float*)&s.vertex_color);
  ImGui::ColorEdit3("Edge color", (float*)&s.edge_color);

  ImGui::SliderInt("Edge width", &s.linewidth, 1, 5);
  ImGui::SameLine(); HelpMarker("CTRL + click to input value");

  ImGui::Checkbox("Triangles", &s.triangles);
  ImGui::SameLine();
  ImGui::Checkbox("Lines", &s.lines);

  ImGui::RadioButton("Ortho", &s.perspective, 0); ImGui::SameLine();
  ImGui::RadioButton("Perspective", &s.perspective, 1);

  if (ImGui::Button(".bmp")) {
    BYTE* pixels = new BYTE[3 * SCREEN_WIDTH * SCREEN_HEIGHT];
    glReadPixels(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    //
    // Convert to FreeImage format & save to file
    FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, SCREEN_WIDTH, SCREEN_HEIGHT, 3 * SCREEN_WIDTH, 24, 0x0000FF, 0xFF0000, 0x00FF00, false);
    FreeImage_Save(FIF_BMP, image, "C:/test.bmp", 0);

    // Free resources
    FreeImage_Unload(image);
    delete [] pixels;
  }

  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::End();

  fileDialog.Display();

  if (fileDialog.HasSelected()) {
    s.path = fileDialog.GetSelected();
    s.filename = getFilename(s.path);
    s.filenamePtr = s.filename.c_str();
    free(m->vertexArray);
    free(m->linesArray);
    if (m->name) free(m->name);
    parseobj(s.path.c_str(), m);
    s.modelnamePtr = m->name;
    fileDialog.ClearSelected();
  }

}
