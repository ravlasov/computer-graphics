#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Shader.h"
#include "Camera.h"


Camera& camera = Camera::get_instance();
bool without_normal_mapping;
bool brick_plane = true;
GLuint planeVBO;
GLuint planeVAO;
GLuint planeEBO;

GLuint VAO;

GLuint lightingVAO;
GLuint texture;
GLuint plane_diff;
GLuint plane_white;
GLuint plane_norm;
GLuint plane_norm1;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if(key == GLFW_KEY_M && action == GLFW_PRESS) {
	without_normal_mapping = !without_normal_mapping;
    }
    if(key == GLFW_KEY_B && action == GLFW_PRESS) {
	brick_plane = !brick_plane;
    }
    if(key == GLFW_KEY_N && action == GLFW_PRESS) {
	std::swap(plane_norm, plane_norm1);
    }

    camera.key_callback(key, action);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera.mouse_callback(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.scroll_callback(xoffset, yoffset);
}


GLfloat vertices[] = {
     0.5f,  0.5f, 0.5f,
     0.5f, -0.5f, 0.5f,
     0.5f, -0.5f,-0.5f,
     0.5f,  0.5f,-0.5f,
    -0.5f,  0.5f,-0.5f,
    -0.5f, -0.5f,-0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f,  0.5f, 0.5f
};

GLuint indices[] = {
    0, 1, 3,
    1, 2, 3,
    2, 3, 5,
    3, 4, 5,
    4, 5, 6,
    6, 7, 4,

    0, 1, 6,
    0, 6, 7,
    0, 7, 4,
    0, 4, 5,
    1, 2, 5,
    1, 5, 6
};


GLfloat plane_vertices[] = {
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
};

GLuint plane_indices[] = {
    0, 1, 2,
    0, 2, 3
};


void loadTexture(const char* file, GLuint* buff){
    glGenTextures(1, buff);
    glBindTexture(GL_TEXTURE_2D, *buff);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height;
    unsigned char* image = SOIL_load_image(file, &width, &height, 0, SOIL_LOAD_RGB);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

}

void initBuffer(GLuint* buff, void* data, size_t size, GLuint type) {
    glGenBuffers(1, buff);
    glBindBuffer(type, *buff);
    glBufferData(type, size, data, GL_STATIC_DRAW);
}

void initVAO(GLuint* VAO, void* vertices, int vsize, void* indices, int esize, std::vector<int> asize, int step, std::vector<int> offset) {
    GLuint VBO, EBO;
    glGenVertexArrays(1, VAO);
    glBindVertexArray(*VAO);
    initBuffer(&VBO, vertices, vsize*sizeof(GLfloat), GL_ARRAY_BUFFER);
    initBuffer(&EBO, indices, esize*sizeof(GLfloat), GL_ELEMENT_ARRAY_BUFFER);
    for(int i=0; i<asize.size(); ++i) {
        glVertexAttribPointer(i, asize[i], GL_FLOAT, GL_FALSE, step * sizeof(GLfloat), (GLvoid*)(offset[i]*sizeof(GLfloat)));
        glEnableVertexAttribArray(i);
    }
    glBindVertexArray(0);
}

void init() {
    initVAO(&lightingVAO, vertices, 8*3, indices, 36, {3}, 3, {0});
    initVAO(&planeVAO, plane_vertices, 4*8, plane_indices, 6, {3, 2}, 8, {0, 6});
    initVAO(&VAO, plane_vertices, 4*8, plane_indices, 6, {3, 3, 2}, 8, {0, 3, 6});

    loadTexture("brickwall.jpg", &plane_diff);
    loadTexture("white.jpg", &plane_white);
    loadTexture("brickwall_normal.jpg", &plane_norm);
    loadTexture("normal.png", &plane_norm1);
}

void setCoordinates(Shader &shader, double side_ratio, glm::mat4 model)
{
    glm::mat4 view = camera.get_view();

    glm::mat4 projection = camera.get_projection(side_ratio);

    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
}


void draw(Shader &shader, GLfloat side_ratio, glm::mat4 model, GLuint VAO, int n) 
{
    setCoordinates(shader, side_ratio, model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, n, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void drawPlane(Shader& planeShader, GLuint planeVAO, glm::vec3 lightPos, int width, int height) {
    planeShader.Use();

    glActiveTexture(GL_TEXTURE2);
    if(brick_plane)
        glBindTexture(GL_TEXTURE_2D, plane_diff);
    else glBindTexture(GL_TEXTURE_2D, plane_white);
    glUniform1i(glGetUniformLocation(planeShader.Program, "material.diffuse"), 2);

    glActiveTexture(GL_TEXTURE3);
    if(!without_normal_mapping) {
        glBindTexture(GL_TEXTURE_2D, plane_norm);
    }
    else {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glUniform1i(glGetUniformLocation(planeShader.Program, "normal"), 3);

    planeShader.setVec3("viewPos", camera.get_position().x, camera.get_position().y, camera.get_position().z);

    planeShader.setFloat("material.shininess", 256.0f);
    planeShader.setVec3("material.specular", 0.2f, 0.2f, 0.2f);

    planeShader.setVec3("light.position", lightPos);
    planeShader.setVec3("light.ambient",  0.2f, 0.2f, 0.2f);
    planeShader.setVec3("light.diffuse",  0.5f, 0.5f, 0.5f);
    planeShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -2.0f, -1.0f));
    model = glm::scale(model, glm::vec3(2.0f));

    draw(planeShader, width/height, model, planeVAO, 3*2);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Lab5", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);

    init();
    Shader lightingShader("vertex.glsl", "lightingfragment.glsl");
    Shader shader("vertex.glsl", "fragment.glsl");
    Shader planeShader("planevertex.glsl", "planefragment.glsl");

    glEnable(GL_DEPTH_TEST);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);


    glm::vec3 lightPos(1.5f, -1.0f, 1.5f);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
	camera.do_movement();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.Use();

	glm::mat4 model(1.0);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
       
	draw(lightingShader, width/height, model, lightingVAO, 3*12);

	if(!without_normal_mapping)
	    drawPlane(planeShader, planeVAO, lightPos, width, height);
	else drawPlane(shader, VAO, lightPos, width, height);

	glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}
