#ifndef CAMERA_H
#define CAMERA_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>

class Camera
{
public:
    static Camera& get_instance() {
        static Camera camera;
	return camera;
    }
    void do_movement();
    glm::vec3& get_position() { return cameraPos; }
    glm::mat4 get_view();
    glm::mat4 get_projection(double side_ratio);
    glm::mat4 get_ortho();
    void key_callback(int key, int action);
    void mouse_callback(double xpos, double ypos);
    void scroll_callback(double xoffset, double yoffset);

    bool firstMouse = true;
private:
    Camera() {}

    glm::vec3 cameraPos   = glm::vec3(3.0f, 3.0f,  3.0f);
    glm::vec3 cameraFront = glm::vec3(-1.0f, -1.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    bool keys[1024];

    GLfloat lastX = 400, lastY = 300;
    GLfloat yaw   = -90.0f;
    GLfloat pitch = 0.0f;
    GLfloat fov = 45.0f;
};

#endif
