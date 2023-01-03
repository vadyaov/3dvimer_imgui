#include "main.hpp"

static void fileBrowser(ImGui::FileBrowser &fileDialog, Settings& s, model *m) {
  fileDialog.Display();
  if (fileDialog.HasSelected()) {
    s.path = fileDialog.GetSelected();
    s.filename = getFilename(s.path);
    s.filenamePtr = s.filename.c_str();
    clearModelMemory(m);
    parseobj(s.path.c_str(), m);
    s.modelnamePtr = m->name;
    fileDialog.ClearSelected();
  }
}

static void ImGui_RenderSettings(Settings& s) {
  ImGui::SliderInt("Edge width", &s.linewidth, 1, 5);
  ImGui::SameLine();
  HelpMarker("CTRL + click to input value");
  ImGui::SliderFloat("Vertex Size", &s.pointsize, 0.0f, 10.0f);
  ImGui::SameLine();
  HelpMarker("CTRL + click to input value");

  ImGui::Checkbox("GL_TRIANGLES", &s.triangles);
  ImGui::SameLine();
  ImGui::Checkbox("GL_LINES", &s.lines);
  ImGui::SameLine();
  ImGui::Checkbox("GL_POINTS", &s.points);

  ImGui::RadioButton("GL_ORTHO", &s.perspective, 0);
  ImGui::SameLine();
  ImGui::RadioButton("GL_PERSPECTIVE", &s.perspective, 1);
}

static void ImGui_Colors(Settings &s) {
  ImGui::ColorEdit3("Background color", (float *)&s.clear_color);
  ImGui::ColorEdit3("Polygon color", (float *)&s.vertex_color);
  ImGui::ColorEdit3("Edge color", (float *)&s.edge_color);
  ImGui::ColorEdit3("Vertex Color", (float *)&s.point_color);
}

static void ImGui_Scale(model *m, Settings &s) {
  ImGui::SliderFloat("Scale coef", &s.addScale, 0.1f, 5.0f, "%.3f");
  if (s.addScale < 0) s.addScale = -1 / s.addScale;
  if (ImGui::Button("X-Scale")) scale(m, s.addScale, 1, 1);
  ImGui::SameLine();
  if (ImGui::Button("Y-Scale")) scale(m, 1, s.addScale, 1);
  ImGui::SameLine();
  if (ImGui::Button("Z-Scale")) scale(m, 1, 1, s.addScale);
  ImGui::Separator();
}

static void ImGui_Move(model *m, Settings &s) {
  ImGui::SliderFloat("Move range", &s.moveRange, 0.0f, 10.0f);
  ImGui::PushButtonRepeat(true);
  if (ImGui::ArrowButton("##left", ImGuiDir_Left)) move(m, -s.moveRange, 0, 0);
  ImGui::SameLine();
  if (ImGui::ArrowButton("##right", ImGuiDir_Right)) move(m, s.moveRange, 0, 0);
  ImGui::SameLine();
  if (ImGui::ArrowButton("##up", ImGuiDir_Up)) move(m, 0, s.moveRange, 0);
  ImGui::SameLine();
  if (ImGui::ArrowButton("##down", ImGuiDir_Down)) move(m, 0, -s.moveRange, 0);
  ImGui::SameLine();
  if (ImGui::Button("-Z")) move(m, 0, 0, -s.moveRange);
  ImGui::SameLine();
  if (ImGui::Button("+Z")) move(m, 0, 0, s.moveRange);
  ImGui::PopButtonRepeat();
  ImGui::Separator();
}

static void ImGui_Rotate(model *m, Settings &s) {
  ImGui::SliderAngle("Angle", &s.angle);
  ImGui::PushButtonRepeat(true);
  if (ImGui::Button("RotateX")) rotate(m, s.angle, 'x');
  ImGui::SameLine();
  if (ImGui::Button("RotateY")) rotate(m, s.angle, 'y');
  ImGui::SameLine();
  if (ImGui::Button("RotateZ")) rotate(m, s.angle, 'z');
  ImGui::PopButtonRepeat();
  ImGui::Separator();
}

static void ImGui_ColorSceme(Settings &s) {
  ImGui::SameLine();
  ImGui::RadioButton("Dark", &s.scheme, 0);
  ImGui::SameLine();
  ImGui::RadioButton("Light", &s.scheme, 1);
  if (!s.scheme)
    ImGui::StyleColorsDark();
  else
    ImGui::StyleColorsLight();
}

static void ImGui_Info(model *m, Settings &s) {
  ImGui::Text("File:%s", s.filenamePtr);
  ImGui::Text("Model:%s", s.modelnamePtr);
  ImGui::Text("Vertex:%zu", m->vertexNumber);
  ImGui::SameLine();
  ImGui::Text("\tIndex:%zu", m->indexNumber);
  ImGui::SameLine();
  ImGui::Text("\tEdges:%d", m->edges);
  ImGui::SameLine();
  HelpMarker("Include joint edjes of 2 triangles");
  ImGui::Separator();
}

int main(int, char **) {
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) return 1;

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

  GLFWwindow *window =
      glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3DViewer_1.0", NULL, NULL);
  if (NULL == window) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);  // Enable vsync

  glewExperimental = true;
  if (glewInit() != GLEW_OK) return 1;

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // Setup dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
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

  Camera cam;
  initCameraBasics(&cam);

  model m;
  Settings s;
  initSettings(&s, &cam);

  parseobj(s.path.c_str(), &m);
  s.modelnamePtr = m.name;

  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  GLuint triangleVBO;
  glGenBuffers(1, &triangleVBO);

  GLuint lineVBO;
  glGenBuffers(1, &lineVBO);

  GLuint pointVBO;
  glGenBuffers(1, &pointVBO);

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    glClearColor(s.clear_color.x * s.clear_color.w,
                 s.clear_color.y * s.clear_color.w,
                 s.clear_color.z * s.clear_color.w, s.clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float currentFrame = static_cast<float>(glfwGetTime());
    cam.deltaTime = currentFrame - cam.lastFrame;
    cam.lastFrame = currentFrame;

    processInput(window, &cam);
    /* std::cout << cam.cameraPos.x << " " << cam.cameraPos.y << " " << cam.cameraPos.z << "\n"; */

    // Start the Dear Imgui frame
    startFrame();

    glm::mat4 model = glm::mat4(1.0f);
    /* glm::mat4 view = glm::mat4(1.0f); */
    glm::mat4 projection;

    glm::mat4 view = glm::lookAt(cam.cameraPos, cam.cameraFront, cam.cameraUp);

    if (s.perspective) {
      projection = glm::perspective(glm::radians(50.0f),
                                    (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
                                    0.1f, 500.0f);
    } else {
      projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 500.0f);
    }

    makeMVP(model, view, projection, shaderProgram);
    if (s.triangles) {
      glUniform4f(vertexColorLocation, s.vertex_color.x, s.vertex_color.y,
                  s.vertex_color.z, s.vertex_color.w);
      draw(triangleVBO, m.allIndex, &m.trianglesArray[0], VAO, GL_TRIANGLES,
           s.linewidth, s.pointsize);
    }

    if (s.lines) {
      glUniform4f(vertexColorLocation, s.edge_color.x, s.edge_color.y,
                  s.edge_color.z, s.edge_color.w);
      draw(lineVBO, m.lineIndex, &m.linesArray[0], VAO, GL_LINES,
           s.linewidth, s.pointsize);
    }

    if (s.points) {
      glUniform4f(vertexColorLocation, s.point_color.x, s.point_color.y,
                  s.point_color.z, s.point_color.w);
      draw(pointVBO, m.vertexNumber, &m.vertexArray[0], VAO, GL_POINTS,
           s.linewidth, s.pointsize);
    }

    glDisableVertexAttribArray(0);
    ImGuiSettingsWindow(window, s, &m, &cam, fileDialog);

    // Rendering
    render(window);

    glfwSwapBuffers(window);
    // Poll and handle events (inputs, window resize, etc.)
    glfwPollEvents();
    if (glfwWindowShouldClose(window)) {
      cleanFile("savefile.vimer");
      saveSettings("savefile.vimer", &s, &cam);
      clearModelMemory(&m);
    }
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glDeleteBuffers(1, &pointVBO);
  glDeleteBuffers(1, &triangleVBO);
  glDeleteBuffers(1, &lineVBO);
  glDeleteProgram(shaderProgram);
  glDeleteVertexArrays(1, &VAO);

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

void HelpMarker(const char *desc) {
  ImGui::TextDisabled("(?)");
  if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort)) {
    ImGui::BeginTooltip();
    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
    ImGui::TextUnformatted(desc);
    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();
  }
}

void glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

// closing window by pressing ESC
void processInput(GLFWwindow *window, Camera *cam) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  float cameraSpeed = static_cast<float>(cam->speed * cam->deltaTime);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      cam->cameraPos += cameraSpeed * cam->cameraFront;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      cam->cameraPos -= cameraSpeed * cam->cameraFront;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      cam->cameraPos -= glm::normalize(glm::cross(cam->cameraFront, cam->cameraUp)) * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      cam->cameraPos += glm::normalize(glm::cross(cam->cameraFront, cam->cameraUp)) * cameraSpeed;
}

void startFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void render(GLFWwindow *window) {
  ImGui::Render();
  int display_w, display_h;
  glfwGetFramebufferSize(window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

std::string getFilename(std::string &path) {
  return path.substr(path.find_last_of('/') + 1);
}

void initSettings(Settings *s, Camera *cam) {
  FILE *f = fopen("savefile.vimer", "r");
  if (f) {
    char *line = NULL;
    size_t len = 0;
    size_t i = 0;
    float r = 0.0f, g = 0.0f, b = 0.0f, w = 0.0f;
    while (i < 21) {
      getline(&line, &len, f);
      if (0 == i) {
        s->addScale = std::atof(line);
      } else if (1 == i) {
        s->moveRange = std::atof(line);
      } else if (2 == i) {
        s->angle = std::atof(line);
      } else if (3 == i) {
        s->pointsize = std::atof(line);
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
      } else if (7 == i) {
        if (std::atoi(line) == 1)
          s->points = true;
        else
          s->points = false;
      } else if (8 == i) {
        sscanf(line, "%f %f %f %f", &r, &g, &b, &w);
        s->clear_color = ImVec4(r, g, b, w);
      } else if (9 == i) {
        sscanf(line, "%f %f %f %f", &r, &g, &b, &w);
        s->vertex_color = ImVec4(r, g, b, w);
      } else if (10 == i) {
        sscanf(line, "%f %f %f %f", &r, &g, &b, &w);
        s->edge_color = ImVec4(r, g, b, w);
      } else if (11 == i) {
        sscanf(line, "%f %f %f %f", &r, &g, &b, &w);
        s->point_color = ImVec4(r, g, b, w);
      } else if (12 == i) {
        for (size_t j = 0; line[j] != '\n' && line[j] != '\0'; j++)
          s->path += line[j];
        s->filename = getFilename(s->path);
        s->filenamePtr = s->filename.c_str();
      } else if (13 == i) {
        s->scheme = atoi(line);
      } else if (14 == i) {
        if (std::atoi(line) == 1)
          s->perspective = true;
        else
          s->perspective = false;
      } else if (15 == i) {
        if (std::atoi(line) == 1)
          s->bmp = true;
        else
          s->bmp = false;
      } else if (16 == i) {
        if (std::atoi(line) == 1)
          s->jpeg = true;
        else
          s->jpeg = false;
      } else if (17 == i) {
        sscanf(line, "%f %f %f", &cam->cameraPos.x, &cam->cameraPos.y, &cam->cameraPos.z);
      } else if (18 == i) {
        sscanf(line, "%f %f %f", &cam->cameraFront.x, &cam->cameraFront.y, &cam->cameraFront.z);
      } else if (19 == i) {
        sscanf(line, "%f %f %f", &cam->cameraUp.x, &cam->cameraUp.y, &cam->cameraUp.z);
      } else if (20 == i) {
        sscanf(line, "%f", &cam->speed);
      }
      i++;
    }
    if (line) free(line);
    fclose(f);
  } else {
    std::cout << "No savefile.vimer in current directory\n";
  }
}

void initCameraBasics(Camera *cam) {
  cam->deltaTime = 0.0f;
  cam->lastFrame = 0.0f;
  cam->firstMouse = true;
  cam->yaw = -90.0f;
  cam->pitch = 0.0f;
  cam->lastX = (float)SCREEN_WIDTH / 2.0f;
  cam->lastY = (float)SCREEN_HEIGHT / 2.0f;
  cam->fov = 45.0f;
}

void makeMVP(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection,
             GLuint shaderProgram) {
  GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
  glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void draw(GLuint VBO, size_t size, float *array, GLuint VAO, GLuint type,
          int linewidth, float pointsize) {
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, size * 3 * sizeof(float), array, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, (void *)0);

  glBindVertexArray(VAO);
  glPointSize(pointsize);
  glLineWidth(linewidth);
  glDrawArrays(type, 0, size);
}

void cleanFile(const char *str) {
  std::ofstream ofs;
  ofs.open(str, std::ofstream::out | std::ofstream::trunc);
  ofs.close();
}

void saveSettings(const char *str, Settings *s, Camera *cam) {
  FILE *fp = fopen(str, "w");
  if (fp) {
    fprintf(fp, "%f\n", s->addScale);
    fprintf(fp, "%f\n", s->moveRange);
    fprintf(fp, "%f\n", s->angle);
    fprintf(fp, "%f\n", s->pointsize);
    fprintf(fp, "%d\n", s->linewidth);
    fprintf(fp, "%d\n", s->triangles);
    fprintf(fp, "%d\n", s->lines);
    fprintf(fp, "%d\n", s->points);
    fprintf(fp, "%f %f %f %f\n", s->clear_color.x, s->clear_color.y,
            s->clear_color.z, s->clear_color.w);
    fprintf(fp, "%f %f %f %f\n", s->vertex_color.x, s->vertex_color.y,
            s->vertex_color.z, s->vertex_color.w);
    fprintf(fp, "%f %f %f %f\n", s->edge_color.x, s->edge_color.y,
            s->edge_color.z, s->edge_color.w);
    fprintf(fp, "%f %f %f %f\n", s->point_color.x, s->point_color.y,
            s->point_color.z, s->point_color.w);
    fprintf(fp, "%s\n", s->path.c_str());
    fprintf(fp, "%d\n", s->scheme);
    fprintf(fp, "%d\n", s->perspective);
    fprintf(fp, "%d\n", s->bmp);
    fprintf(fp, "%d\n", s->jpeg);
    fprintf(fp, "%f %f %f\n", cam->cameraPos.x, cam->cameraPos.y, cam->cameraPos.z);
    fprintf(fp, "%f %f %f\n", cam->cameraFront.x, cam->cameraFront.y, cam->cameraFront.z);
    fprintf(fp, "%f %f %f\n", cam->cameraUp.x, cam->cameraUp.y, cam->cameraUp.z);
    fprintf(fp, "%f", cam->speed);
    fclose(fp);
  }
}

void ImGuiSettingsWindow(GLFWwindow *window, Settings &s, model *m, Camera *cam,
                         ImGui::FileBrowser &fileDialog) {
  ImGui::Begin("Settings");
  if (ImGui::Button("Browse obj"))
    fileDialog.Open();

  ImGui_ColorSceme(s);
  ImGui_Info(m, s);
  ImGui::SliderFloat("Camera Speed", &cam->speed, 1.0f, 10.0f);
  ImGui_Scale(m, s);
  ImGui_Move(m, s);
  ImGui_Rotate(m, s);

  if (ImGui::Button("RESET ALL")) {
    clearModelMemory(m);
    parseobj(s.path.c_str(), m);
    s.modelnamePtr = m->name;
    cam->cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    cam->cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cam->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
  }
  ImGui::SameLine();
  HelpMarker("Resets the position, scale, rotation");

  ImGui_Colors(s);
  ImGui_RenderSettings(s);
  if (ImGui::Button("ScreenShot"))
    makeScreenShot(window, s);

  ImGui::SameLine();
  ImGui::Checkbox("BMP", &s.bmp);
  ImGui::SameLine();
  ImGui::Checkbox("JPEG", &s.jpeg);

  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::End();

  fileBrowser(fileDialog, s, m);
}

void makeScreenShot(GLFWwindow *window, Settings &s) {
  int height = 0;
  int width = 0;
  glfwGetFramebufferSize(window, &width, &height);

  SDL_Surface *temp = SDL_CreateRGBSurface(
      SDL_SWSURFACE, width, height, 24, 0x000000FF, 0x0000FF00, 0x00FF0000, 0);
  if (temp == NULL) glfw_error_callback(1, "CreateRGBSurface failed");

  char *pixels = (char*)calloc(width * height * 3, sizeof(char));
  if (pixels) {
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
  for (int i = 0; i < height; i++)
    std::memcpy(((char *)temp->pixels) + temp->pitch * i,
                pixels + 3 * width * (height - i - 1), width * 3);
  if (s.bmp) SDL_SaveBMP(temp, "ScreenShot.bmp");
  if (s.jpeg) SDL_SaveBMP(temp, "ScreenShot.jpeg");
  SDL_FreeSurface(temp);
  free(pixels);
  }
}

void clearModelMemory(model *m) {
    if (m->trianglesArray) free(m->trianglesArray);
    if (m->linesArray) free(m->linesArray);
    if (m->vertexArray) free(m->vertexArray);
    if (m->name) free(m->name);
}
