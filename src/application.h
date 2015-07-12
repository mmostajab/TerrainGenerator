#ifndef __APPLICATION_H__
#define __APPLICATION_H__

// GL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// Boost
#include <boost/random.hpp>

// STD
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

// TerrainGenerator
#include "Camera.h"
#include "Terrain.h"
#include "BezierCurve.h"
#include "Quad.h"

#define GRID_X_SIZE 100
#define GRID_Y_SIZE 100

#define MAX_FRAMEBUFFER_WIDTH 2048
#define MAX_FRAMEBUFFER_HEIGHT 2048

class Application {
public:
    Application();

    void init(const unsigned int& width, const unsigned int& height);
    void init();
    void run();
    void shutdown();

    ~Application();

private:
    void create();
    void update(float time, float elapsedTime);
    void draw();
    bool parseConfigFile(const char* config_filename);
    
    void compileShaders();

    GLuint simple_program;

private:
    static void EventMouseButton(GLFWwindow* window, int button, int action, int mods);
    static void EventMousePos(GLFWwindow* window, double xpos, double ypos);
    static void EventMouseWheel(GLFWwindow* window, double xoffset, double yoffset);
    static void EventKey(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void EventChar(GLFWwindow* window, int codepoint);

    // Callback function called by GLFW when window size changes
    static void WindowSizeCB(GLFWwindow* window, int width, int height);
    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static bool m_controlKeyHold;
    static bool m_altKeyHold;
    static bool m_w_pressed, m_s_pressed, m_a_pressed, m_d_pressed, m_q_pressed, m_e_pressed;
    static bool m_mouse_left_drag, m_mouse_middle_drag, m_mouse_right_drag;

private:
    static GLFWwindow*    m_window;
    GLuint                m_transformation_buffer, m_lighting_buffer, m_general_buffer;
    static unsigned int   m_width, m_height;
    bool                  wireframe;

    static Camera         m_camera;
    glm::mat4             m_projmat, m_viewmat, m_worldmat, m_inv_viewmat;
    Terrain               terrain;
    BezierCurve           curve;
    Quad                  sky;
};

#endif
