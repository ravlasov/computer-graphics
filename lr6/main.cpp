#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
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
#include "Cube.h"
#include "toaster.h"
#include "axes.h"

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

bool catched = false;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera.mouse_callback(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.scroll_callback(xoffset, yoffset);
}

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
    if(key == GLFW_KEY_C && action == GLFW_PRESS){
	catched = !catched;
	if(catched){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPosCallback(window, mouse_callback);
            glfwSetScrollCallback(window, scroll_callback);
	}
	else{
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursorPosCallback(window, nullptr);
            glfwSetScrollCallback(window, nullptr);
	    camera.firstMouse = true;
	}
    }
    camera.key_callback(key, action);
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

void initVAO(GLuint* VAO, size_t type_size, void* vertices, int vsize, void* indices, int esize, std::vector<int> asize, int step, std::vector<int> offset) {
    GLuint VBO, EBO;
    glGenVertexArrays(1, VAO);
    glBindVertexArray(*VAO);
    initBuffer(&VBO, vertices, vsize*type_size, GL_ARRAY_BUFFER);
    initBuffer(&EBO, indices, esize*sizeof(GLuint), GL_ELEMENT_ARRAY_BUFFER);
    for(int i=0; i<asize.size(); ++i) {
        glVertexAttribPointer(i, asize[i], GL_FLOAT, GL_FALSE, step * type_size, (GLvoid*)(offset[i]*type_size));
        glEnableVertexAttribArray(i);
    }
    glBindVertexArray(0);
}


GLuint cube_VAO;
std::vector<float> points;
std::vector<unsigned int> cube_indices;
GLuint holes_VAO;
std::vector<float> holes;
std::vector<unsigned int> holes_indices;
GLuint buttons_VAO;
std::vector<float> buttons;
std::vector<unsigned int> buttons_indices;
GLuint axes_VAO;
std::vector<float> axes;
std::vector<unsigned int> axes_indices;

void generatePoints(int count){
    points.clear();
    cube_indices.clear();
    holes.clear();
    holes_indices.clear();
    buttons.clear();
    buttons_indices.clear();
    Toaster toaster;
    auto vec = toaster.getToastHoles(count);
    int n = 0;
    for(int i=0; i<vec.size(); ++i){
        for(int j=0; j<vec[i].size()/8; ++j){
            for(int k=0; k<8; ++k){
                holes.push_back(vec[i][8*j+k]);
            }
            if(j != 0){
                holes_indices.push_back(n-1);
                holes_indices.push_back(n);
            }
            ++n;
        }
    }
    vec = toaster.getButtons(count);
    n = 0;
    for(int i=0; i<vec.size(); ++i){
        for(int j=0; j<vec[i].size()/8; ++j){
            for(int k=0; k<8; ++k){
                buttons.push_back(vec[i][8*j+k]);
            }
            if(j != 0){
                buttons_indices.push_back(n-1);
                buttons_indices.push_back(n);
            }
            ++n;
        }
    }

    initVAO(&holes_VAO, sizeof(GLfloat), holes.data(), holes.size(), holes_indices.data(), holes_indices.size(), {3, 3, 2}, 8, {0, 3, 6});
    initVAO(&buttons_VAO, sizeof(GLfloat), buttons.data(), buttons.size(), buttons_indices.data(), buttons_indices.size(), {3, 3, 2}, 8, {0, 3, 6});
    Cube cube(count);
    points = cube.get_full();
    for(int i=0; i<points.size(); ++i) {
        if(i % 8 < 3 && abs(points[i]) > 1)
            std::cout << points[i] << "AUCH" << std::endl;
    }
    cube_indices = cube.get_indices();
    initVAO(&cube_VAO, sizeof(GLfloat), points.data(), points.size(), cube_indices.data(), cube_indices.size(), {3, 3, 2}, 8, {0, 3, 6});
    initVAO(&lightingVAO, sizeof(GLfloat), vertices, 8*3, indices, 36, {3}, 3, {0});
}

int fragm_count = 250;

void init() {
    Axes axe;
    auto vec = axe.cone(1000, 2, 0.5, 0.2);
    int n = 0;
    for(int i=0; i<vec.size(); ++i){
        for(int j=0; j<vec[i].size()/8; ++j){
            for(int k=0; k<8; ++k){
                axes.push_back(vec[i][8*j+k]);
            }
            if(j != 0){
                axes_indices.push_back(n-1);
                axes_indices.push_back(n);
            }
            ++n;
        }
        axes_indices.push_back(n-vec[i].size()/8);
        axes_indices.push_back(n-1);
    }
    initVAO(&axes_VAO, sizeof(GLfloat), axes.data(), axes.size(), axes_indices.data(), axes_indices.size(), {3, 3, 2}, 8, {0, 3, 6});

    generatePoints(fragm_count);
}

int isOrtho = 0;

void setCoordinates(Shader &shader, double side_ratio, glm::mat4 model)
{
    glm::mat4 view = camera.get_view();

//    glm::mat4 projection = camera.get_projection(side_ratio);
    glm::mat4 projection;
    if(isOrtho)
        projection = camera.get_ortho();
    else
	projection = camera.get_projection(side_ratio);

    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
}

void setLightings(Shader &shader, glm::vec3 *lightPos, glm::vec3 *color, float* intensity, int n){
    for(int i=0; i<n; ++i){
        std::string pref = "light[" + std::to_string(i) + "].";
        shader.setVec3(pref + "position", lightPos[i]);
        shader.setVec3(pref + "ambient",  0.2f, 0.2f, 0.2f);
        shader.setVec3(pref + "diffuse",  intensity[i]*color[i]);
        shader.setVec3(pref + "specular", intensity[i]*glm::vec3(1.0f, 1.0f, 1.0f));
    }
}

bool inv_draw = false;
glm::vec3 lightPos[] = {glm::vec3(-1.0f, 3.0f, 2.0f), glm::vec3(1.0f, -2.0f, -3.0f), glm::vec3(0.0f, 2.0f, -2.0f)};
glm::vec3 color[] = {glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f)};
float intensity[] = {1.0f, 1.0f, 1.0f};
int angle[] = {0, 0};
glm::vec3 translation(0.0f);
glm::vec3 scale(1.0f);

void draw(Shader &shader, int draw_type, GLfloat side_ratio, glm::mat4 rotation, glm::mat4 model, GLuint VAO, int n, glm::vec3 diffuse) 
{
    shader.Use();

    shader.setBool("inv_draw", inv_draw);
    shader.setVec3("material.diffuse", diffuse);
    shader.setVec3("viewPos", camera.get_position().x, camera.get_position().y, camera.get_position().z);

    shader.setFloat("material.shininess", 256.0f);
    shader.setVec3("material.specular", 0.2f, 0.2f, 0.2f);

    setLightings(shader, lightPos, color, intensity, 3);

    shader.setMat4("rotation", rotation);

    setCoordinates(shader, side_ratio, model);

    glBindVertexArray(VAO);
    glDrawElements(draw_type, n, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void drawLighting(Shader &shader, float side_ratio, glm::vec3 lightPos, glm::vec3 color, float intensity, GLuint lightingVAO, int n) {
    glm::mat4 model(1.0);
    glm::mat4 rotation(1.0);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    draw(shader, GL_TRIANGLE_STRIP, side_ratio, rotation, model, lightingVAO, n, intensity * color);
}

void drawCube(Shader &shader, float side_ratio, GLuint VAO, int n) {
    glm::mat4 model(1.0);
    glm::mat4 rotation(1.0);
    model = glm::translate(model, translation);
    model = glm::rotate(model, glm::radians(90.0f+angle[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(1.0f*angle[1]), glm::vec3(0.0f, 1.0f, 0.0f));
    rotation = glm::rotate(rotation, glm::radians(90.0f+angle[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    rotation = glm::rotate(rotation, glm::radians(1.0f*angle[1]), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f, 0.7f, 0.5f));
    model = glm::scale(model, glm::vec3(scale.x, scale.z, scale.y));
    draw(shader, GL_LINES, side_ratio, rotation, model, cube_VAO, n, glm::vec3(0.7f, 0.7f, 0.7f));
}

void drawHoles(Shader &shader, float side_ratio, GLuint VAO, int n) {
    glm::mat4 model(1.0);
    glm::mat4 rotation(1.0);
    model = glm::translate(model, translation);
    model = glm::rotate(model, glm::radians(-90.0f+angle[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(1.0f*angle[1]), glm::vec3(0.0f, -1.0f, 0.0f));
    rotation = glm::rotate(rotation, glm::radians(-90.0f+angle[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    rotation = glm::rotate(rotation, glm::radians(1.0f*angle[1]), glm::vec3(0.0f, -1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.3f, 0.5f, 0.5f));
    model = glm::scale(model, glm::vec3(scale.x, scale.z, scale.y));
    draw(shader, GL_LINES, side_ratio, rotation, model, VAO, n, glm::vec3(0.3f, 0.3f, 0.3f));

    model = glm::mat4(1.0);
    model = glm::translate(model, translation);
    model = glm::rotate(model, glm::radians(-90.0f+angle[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(1.0f*angle[1]), glm::vec3(0.0f, -1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    rotation = glm::rotate(rotation, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(0.3f, 0.5f, 0.5f));
    model = glm::scale(model, glm::vec3(scale.x, scale.z, scale.y));
    draw(shader, GL_LINES, side_ratio, rotation, model, VAO, n, glm::vec3(0.3f, 0.3f, 0.3f));
}

void drawButtons(Shader &shader, float side_ratio, GLuint VAO, int n) {
    glm::mat4 model(1.0);
    glm::mat4 rotation(1.0);
    float sin0 = sin(glm::radians(1.0f*angle[0]));
    float cos0 = cos(glm::radians(1.0f*angle[0]));
    float sin1 = sin(glm::radians(1.0f*angle[1]));
    float cos1 = cos(glm::radians(1.0f*angle[1]));
    float x = -0.25f * scale[0];
    float y = -0.25f * scale[1];
    float z = 0.4f * scale[2];
    x = x*cos1 - y*sin1;
    y = (-0.25f * scale[0])*sin1 + y*cos1;
    z = y*sin0 + z*cos0;
    y = y*cos0 - (0.4f * scale[2])*sin0;
    rotation = glm::rotate(rotation, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotation = glm::rotate(rotation, glm::radians(1.0f*angle[0]), glm::vec3(0.0f, 0.0f, -1.0f));
    rotation = glm::rotate(rotation, glm::radians(1.0f*angle[1]), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, translation + glm::vec3(x, y, z));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(1.0f*angle[0]), glm::vec3(0.0f, 0.0f, -1.0f));
    model = glm::rotate(model, glm::radians(1.0f*angle[1]), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.3f, 0.5f, 0.5f));
    model = glm::scale(model, glm::vec3(scale.z, scale.y, scale.x));
    draw(shader, GL_LINES, side_ratio, rotation, model, VAO, n, glm::vec3(0.3f, 0.3f, 0.3f));
}

void drawAxes(Shader &shader, float side_ratio){
    glm::mat4 rotation(1.0f);
    glm::mat4 model(1.0f);
    draw(shader, GL_LINES, side_ratio, rotation, model, axes_VAO, axes_indices.size(), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    draw(shader, GL_LINES, side_ratio, rotation, model, axes_VAO, axes_indices.size(), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    draw(shader, GL_LINES, side_ratio, rotation, model, axes_VAO, axes_indices.size(), glm::vec3(0.0f, 0.0f, 1.0f));
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Lab6", glfwGetPrimaryMonitor(), nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    // Setup Dear ImGui context
IMGUI_CHECKVERSION();
ImGui::CreateContext();
ImGuiIO &io = ImGui::GetIO();
// Setup Platform/Renderer bindings
ImGui_ImplGlfw_InitForOpenGL(window, true);
ImGui_ImplOpenGL3_Init("#version 330");
// Setup Dear ImGui style
ImGui::StyleColorsDark();

    glViewport(0, 0, width, height);

    init();
    Shader lightingShader("vertex.glsl", "lightingfragment.glsl");
    Shader shader("vertex.glsl", "fragment.glsl");

    glEnable(GL_DEPTH_TEST);

    glfwSetKeyCallback(window, key_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    //glfwSetScrollCallback(window, scroll_callback);



    bool last = false;

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

	// feed inputs to dear imgui, start new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


	camera.do_movement();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	for(int i=0; i<3; ++i)
	    drawLighting(lightingShader, width/height, lightPos[i], color[i], intensity[i], lightingVAO, 3*12);

	drawCube(shader, width/height, cube_VAO, cube_indices.size());

	drawHoles(shader, width/height, holes_VAO, holes_indices.size());
	drawButtons(shader, width/height, buttons_VAO, buttons_indices.size());

	glUseProgram(0);
	drawAxes(lightingShader, width/height);


    // render your GUI
    ImGui::Begin("Settings");
    ImGui::SliderInt("fragmentation", &fragm_count, 20, 500);
    bool recalc = ImGui::Button("recalc");
    if(recalc)
	generatePoints(fragm_count);
    ImGui::RadioButton("perspective", &isOrtho, 0);
    ImGui::RadioButton("ortho", &isOrtho, 1);
    ImGui::Checkbox("draw invisible faces", &inv_draw);
    //if(!recalc && last)
	//generatePoints(fragm_count);
    //else last = recalc;

    ImGui::SliderFloat3("position", glm::value_ptr(translation), -2.0, 2.0);
    ImGui::SliderFloat3("scale", glm::value_ptr(scale), 0.0, 5.0);
    ImGui::SliderInt2("rotation", angle, -180, 180);
    for(int i=0; i<3; ++i){
        ImGui::Text("lighting №%d", i);
	std::string pos_label = "position " + std::to_string(i);
	std::string color_label = "color " + std::to_string(i);
	std::string intensity_label = "intensity " + std::to_string(i);
        ImGui::SliderFloat3(pos_label.c_str(), glm::value_ptr(lightPos[i]), -2.0, 2.0);
        ImGui::ColorEdit3(color_label.c_str(), glm::value_ptr(color[i]));
        ImGui::SliderFloat(intensity_label.c_str(), &intensity[i], 0.0, 1.0);
    }    
    ImGui::End();

    // Render dear imgui into screen
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(window);
    }
    glfwTerminate();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    return 0;
}
