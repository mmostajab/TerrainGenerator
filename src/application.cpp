#ifdef WIN32
    #include <Windows.h>
#endif

#include "application.h"

// STD
#include <iostream>
#include <fstream>
#include <time.h>

// GL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

// Helper
#include "helper.h"

#include <omp.h>

// Static Members
GLFWwindow*				Application::m_window             = 0;
unsigned int			Application::m_width              = 0;
unsigned int			Application::m_height             = 0;
bool					    Application::m_controlKeyHold     = false;
bool					    Application::m_altKeyHold         = false;
bool					    Application::m_w_pressed          = false;
bool					    Application::m_s_pressed          = false;
bool					    Application::m_a_pressed          = false;
bool					    Application::m_d_pressed          = false;
bool					    Application::m_q_pressed          = false;
bool					    Application::m_e_pressed          = false;
bool              Application::m_mouse_left_drag    = false;
bool              Application::m_mouse_middle_drag  = false;
bool              Application::m_mouse_right_drag   = false;
Camera            Application::m_camera;


Application::Application() {
}

void Application::init(const unsigned int& width, const unsigned int& height) {

    m_width = width; m_height = height;

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    m_window = glfwCreateWindow(width, height, "Terrain Generator (Demo): Beta", NULL, NULL);
    if (!m_window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_window);

    glfwSetKeyCallback(m_window, key_callback);
    glfwSetWindowSizeCallback(m_window, WindowSizeCB);
    glfwSetCursorPosCallback(m_window, EventMousePos);
    glfwSetScrollCallback(m_window, EventMouseWheel);
    glfwSetMouseButtonCallback(m_window, (GLFWmousebuttonfun)EventMouseButton);
    glfwSetKeyCallback(m_window, (GLFWkeyfun)EventKey);

    // - Directly redirect GLFW char events to AntTweakBar
    glfwSetCharCallback(m_window, (GLFWcharfun)EventChar);

    if (glewInit() != GLEW_OK){
        std::cout << "cannot intialize the glew.\n";
        exit(EXIT_FAILURE);
    }

    init();

    glEnable(GL_DEPTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE0);
    //glEnable(GL_TEXTURE1);

    GLenum e = glGetError();
    //glDisable(GL_DEPTH);
    e = glGetError();
    //glDisable(GL_DEPTH_TEST);
    e = glGetError();
}

void Application::init() {
    GLenum e = glGetError();
    glClearColor(19.0f / 255.0f, 9.0f / 255.0f, 99.0f / 255.0f, 1.0f);
    glClearDepth(1.0f);
    e = glGetError();
}

bool Application::parseConfigFile(const char* config_filename){
  std::ifstream config_file(config_filename);
  if (!config_file)
    return false;

  char buffer[201];

  /*while (!config_file.eof())
  {

  }*/

  return true;
}

void Application::create() {
   compileShaders();

   //==================================
   // Create the Camera
   //==================================
   m_camera.SetMode(MODELVIEWER);
   //m_camera.SetMode(FREE);
   m_camera.SetPosition(glm::vec3(0.0f, 1.0f, 2.0f));
   m_camera.SetLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
   m_camera.SetClipping(0.01f, 100.0f);
   m_camera.SetFOV(60);
   m_camera.SetViewport(0, 0, m_width, m_height);
   m_camera.camera_scale = 0.01f;

   // ===================================================================
   // = Viewing, Projection, Lighting Mats! Common between everything
   // ===================================================================
   glGenBuffers(1, &m_transformation_buffer);
   glBindBuffer(GL_UNIFORM_BUFFER, m_transformation_buffer);
   glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

   glGenBuffers(1, &m_lighting_buffer);
   glBindBuffer(GL_UNIFORM_BUFFER, m_lighting_buffer);
   glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

   glGenBuffers(1, &m_general_buffer);
   glBindBuffer(GL_UNIFORM_BUFFER, m_general_buffer);
   glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

   // ===================================================================
   // = Generating the random terrain.
   // ===================================================================

   float rand_bound = 1.0f;
   boost::random::mt19937 rng;
   boost::random::uniform_real_distribution<> dist(-rand_bound, +rand_bound);
   rng.seed(clock());

   size_t nSeeds = 500;
   size_t npoints = 40;
   std::vector<glm::vec3> points(npoints);
   for (size_t i = 0; i < npoints; i++){
     points[i] = glm::vec3(-2.0f + 4 * static_cast<float>(i) / (npoints - 1), dist(rng), 0.0f);
   }

   curve.setControlPoints(points);
   std::vector<glm::vec4>* curve_points = curve.getSeedingPoints(nSeeds);

   terrain.setPoints(*curve_points);

   // Generating the sky
   sky.init(8.0f, 8.0f, "../../media/sky.tga");
}

void Application::update(float time, float timeSinceLastFrame) {

  if (m_w_pressed) {
    wireframe = !wireframe;

    glPolygonMode(GL_FRONT, wireframe ? GL_LINE : GL_FILL);
    glPolygonMode(GL_BACK, wireframe ? GL_LINE : GL_FILL);

  }
  // ===============================================================
  // == Updating the camera, light and shader values information
  // ===============================================================
  m_camera.Update();
  m_camera.GetMatricies(m_projmat, m_viewmat, m_worldmat);

  const int m = 2, n = 2;
  int i = 0, j = 0;
  glm::mat4 scissor_mat;
  scissor_mat[0].x  = m; scissor_mat[0].y  = 0; scissor_mat[0].z  = 0; scissor_mat[0].w  = m - (2 * i + 1);
  scissor_mat[1].x  = 0; scissor_mat[1].y  = n; scissor_mat[1].z  = 0; scissor_mat[1].w  = n - (2 * j + 1);
  scissor_mat[2].x  = 0; scissor_mat[2].y  = 0; scissor_mat[2].z  = 1; scissor_mat[2].w = 0;
  scissor_mat[3].x  = 0; scissor_mat[3].y  = 0; scissor_mat[3].z  = 0; scissor_mat[3].w = 1;
  scissor_mat = glm::transpose(scissor_mat);



  m_inv_viewmat = glm::inverse(m_viewmat);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_transformation_buffer);
  glm::mat4* transform_matrices = (glm::mat4*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, 3 * sizeof(glm::mat4), GL_MAP_WRITE_BIT);
  transform_matrices[0] = m_projmat * scissor_mat;
  transform_matrices[1] = m_viewmat;
  transform_matrices[2] = m_worldmat;
  glUnmapBuffer(GL_UNIFORM_BUFFER);

  m_worldmat = glm::translate(glm::mat(1.0f), glm::vec3(10.0, 10.0 ,10.0f));

  std::cout << "World Mat\n";
  std::cout
  << m_worldmat[0].x << " " << m_worldmat[0].y << " " << m_worldmat[0].z << " " << m_worldmat[0].w << std::endl
  << m_worldmat[1].x << " " << m_worldmat[1].y << " " << m_worldmat[1].z << " " << m_worldmat[1].w << std::endl
  << m_worldmat[2].x << " " << m_worldmat[2].y << " " << m_worldmat[2].z << " " << m_worldmat[2].w << std::endl
  << m_worldmat[3].x << " " << m_worldmat[3].y << " " << m_worldmat[3].z << " " << m_worldmat[3].w << std::endl << std::endl << std::endl << std::endl;

  std::cout << "Proj Mat\n";
  std::cout
  << m_projmat[0].x << " " << m_projmat[0].y << " " << m_projmat[0].z << " " << m_projmat[0].w << std::endl
  << m_projmat[1].x << " " << m_projmat[1].y << " " << m_projmat[1].z << " " << m_projmat[1].w << std::endl
  << m_projmat[2].x << " " << m_projmat[2].y << " " << m_projmat[2].z << " " << m_projmat[2].w << std::endl
  << m_projmat[3].x << " " << m_projmat[3].y << " " << m_projmat[3].z << " " << m_projmat[3].w << std::endl << std::endl << std::endl << std::endl;

  // updating the lighting info
  glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_lighting_buffer);
  glm::vec4* light_info = (glm::vec4*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, 2 * sizeof(glm::vec4), GL_MAP_WRITE_BIT);
  //light_info[0] = glm::vec4(-light_dir[0], -light_dir[1], -light_dir[2], 0.0f);
  light_info[0] = glm::vec4(-1, -1, -1, 0.0f);
  light_info[1] = glm::vec4(m_camera.getPosition(), 1.0f);
  glUnmapBuffer(GL_UNIFORM_BUFFER);

  // Buffer 2 is reserved for the sample points of the Ambient Occlusion Rendering

  // updating the general information for every object
  glBindBufferBase(GL_UNIFORM_BUFFER, 3, m_general_buffer);
  glm::vec4* general_info = (glm::vec4*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, 2 * sizeof(glm::vec4), GL_MAP_WRITE_BIT);
  //general_info[0].x = *color_multiplier;
  //general_info[1].x = back_color[0]; general_info[1].y = back_color[1];   general_info[1].z = back_color[2];    general_info[1].w = 1.0f;
  general_info[0].x = 1.0f;
  general_info[1].x = 0.0f; general_info[1].y = 1.0f;   general_info[1].z = 0.0f;    general_info[1].w = 1.0f;
  glUnmapBuffer(GL_UNIFORM_BUFFER);

}

void Application::draw() {

  glViewport(0, 0, m_width, m_height);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  terrain.draw();
  sky.draw();
}

void Application::run() {
  create();
  double start_time;
  double start_frame;
  start_time = start_frame = glfwGetTime();

  while (!glfwWindowShouldClose(m_window))
  {
    draw();
    glfwSwapBuffers(m_window);
    glfwPollEvents();

    start_frame = glfwGetTime();

    update(0.0f, 0.0f);
  }
}

void Application::shutdown() {
  glfwDestroyWindow(m_window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

Application::~Application() {
}

void Application::compileShaders() {
  Terrain::compileShader();
  Quad::compileShader();
}


void Application::EventMouseButton(GLFWwindow* window, int button, int action, int mods) {

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    m_mouse_left_drag   = true;

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    m_mouse_left_drag   = false;

  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    m_mouse_right_drag  = true;

  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    m_mouse_right_drag  = false;

  if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
    m_mouse_middle_drag = true;

  if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
    m_mouse_middle_drag = false;
}

void Application::EventMousePos(GLFWwindow* window, double xpos, double ypos) {

  if (m_controlKeyHold){
    if (m_mouse_left_drag){
      m_camera.Move2D(static_cast<int>(xpos), static_cast<int>(ypos));
    }
    if (m_mouse_right_drag){
      m_camera.OffsetFrustum(static_cast<int>(xpos), static_cast<int>(ypos));
    }
  }

  m_camera.SetMousePos(static_cast<int>(xpos), static_cast<int>(ypos));

}

void Application::EventMouseWheel(GLFWwindow* window, double xoffset, double yoffset) {
  m_camera.MoveForward(static_cast<int>(yoffset));
}

void Application::EventKey(GLFWwindow* window, int key, int scancode, int action, int mods) {

  if (action == GLFW_PRESS){
    if (m_controlKeyHold && key == GLFW_KEY_W)  m_w_pressed = true;
    if (m_controlKeyHold && key == GLFW_KEY_S)  m_s_pressed = true;
    if (m_controlKeyHold && key == GLFW_KEY_A)  m_a_pressed = true;
    if (m_controlKeyHold && key == GLFW_KEY_D)  m_d_pressed = true;
    if (m_controlKeyHold && key == GLFW_KEY_Q)  m_q_pressed = true;
    if (m_controlKeyHold && key == GLFW_KEY_E)  m_e_pressed = true;

    if (key == GLFW_KEY_LEFT_CONTROL)           m_controlKeyHold = true;
    if (key == GLFW_KEY_LEFT_ALT)               m_altKeyHold     = true;
  }

  if (action == GLFW_RELEASE){
    if (key == GLFW_KEY_W)  m_w_pressed = false;
    if (key == GLFW_KEY_S)  m_s_pressed = false;
    if (key == GLFW_KEY_A)  m_a_pressed = false;
    if (key == GLFW_KEY_D)  m_d_pressed = false;
    if (key == GLFW_KEY_Q)  m_q_pressed = false;
    if (key == GLFW_KEY_E)  m_e_pressed = false;

    if (key == GLFW_KEY_LEFT_CONTROL)           m_controlKeyHold    = false;
    if (key == GLFW_KEY_LEFT_ALT)               m_altKeyHold        = false;
  }
}

void Application::EventChar(GLFWwindow* window, int codepoint) {
}

// Callback function called by GLFW when window size changes
void Application::WindowSizeCB(GLFWwindow* window, int width, int height) {
  m_width = width; m_height = height;
  glViewport(0, 0, width, height);
}
void Application::error_callback(int error, const char* description) {
  fputs(description, stderr);
}
void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS){
    m_controlKeyHold = true;
  }

  if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE){
    m_controlKeyHold = false;
  }
}
