//
//  main.cpp
//  3D Object Drawing
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "directionLight.h"
#include "sphere.h"
#include "spotLight.h"
#include "cube.h"
#include "stb_image.h"
#include "cylinder.h"
#include "curve.h"

#include <iostream>
#include <random>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;
float run_speed = 0.004f;
//initial values

float gun_x = 0.0f;
float gun_y = 0.7f;

float bullet_x;
float bullet_y;
float slope_Y;
float slope_X;

float balloon_x = 1.5f;
float balloon_y = 6.0f;
float balloon_z;
float bz = 0.0f;

float up_angle = 0.0f;
float right_angle = 0.0f;

bool first_block = true;
bool balloon = true;

// camera
float camera_z = 0.0f;
Camera camera(glm::vec3(0.0f, 1.0f, -6.5f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 10.0, eyeZ = 1.0;
float lookAtX = 0.0, lookAtY = 10.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);
SpotLight spotlight(
    0.0f, 1.0f, -3.0f,
    0.0f, -1.0f, 0.0f,
    .0f, .0f, .0f,
    .0f, .0f, .0f,
    0.0f, 0.0f, 0.0f,
    30.0f,
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f //k_q
);

DirectionLight directlight(
    0.0f, 1.0f, 0.0f,
    .0f, .0f, .0f,
    .0f, .0f, .0f,
    0.0f, 0.0f, 0.0f,
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f //k_q
);
// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(1.50f,  1.50f,  0.0f),
    glm::vec3(1.5f,  1.5f,  10.0f),
    glm::vec3(-1.5f,  1.5f,  0.0f),
    glm::vec3(-1.5f,  1.5f,  10.0f)
};
PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);


// light settings
bool pointLightOn = true;
bool directionLighton = true;
bool spotlighton = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;
bool shoot = false;
float bullet_z = -5.2f;
bool toggle = true;
float l = 0;
// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;
int score = 0;
int life = 5;
class Balloon
{
public:
    float speed;
    float x = 0.0, y = 6.0, z = 0.0;
    Sphere bb = Sphere();
};
void func(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Balloon b[])
{
    float baseHeight = 0.1;
    float width = 10;
    float length = 15;

    //base-1
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.1, 0.5, 0.3);


    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix;


    //wall
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-5.5f, 0.0f, 6.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(12.1f, 6.0f, 0.1f));
    model = alTogether * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0);


    //gun
    Cylinder gun = Cylinder();
    translateMatrix = glm::translate(identityMatrix, glm::vec3(gun_x, gun_y, -5.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.8f, 0.01f));
    glm::mat4 rotUp = glm::rotate(identityMatrix, glm::radians(up_angle - 90.0f), glm::vec3(1.0, 0.0, 0.0));
    glm::mat4 rotRight = glm::rotate(identityMatrix, glm::radians(right_angle), glm::vec3(0.0, 0.0, 1.0));
    model = alTogether * translateMatrix * rotUp * rotRight * scaleMatrix;
    gun.drawCylinder(lightingShader, model, 0.027, 0.451, 0.529);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(gun_x - 0.025, gun_y, -5.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.8f, 0.05f));
    rotUp = glm::rotate(identityMatrix, glm::radians(up_angle - 90.0f), glm::vec3(1.0, 0.0, 0.0));
    rotRight = glm::rotate(identityMatrix, glm::radians(right_angle), glm::vec3(0.0, 0.0, 1.0));
    model = alTogether * translateMatrix * rotUp * rotRight * scaleMatrix;
    gun.drawCylinder(lightingShader, model, 0.027, 0.451, 0.529);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(gun_x - 0.05, gun_y - 0.05, -5.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.07f, 0.07f, 0.35f));
    rotUp = glm::rotate(identityMatrix, glm::radians(up_angle), glm::vec3(1.0, 0.0, 0.0));
    rotRight = glm::rotate(identityMatrix, glm::radians(right_angle), glm::vec3(0.0, 1.0, 0.0));
    model = alTogether * translateMatrix * rotUp * rotRight * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 1.0, 0.0, 0.0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(gun_x - 0.075, gun_y + 0.025, -5.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.3f, 0.1f));
    rotUp = glm::rotate(identityMatrix, glm::radians(up_angle - 180.0f), glm::vec3(1.0, 0.0, 0.0));
    rotRight = glm::rotate(identityMatrix, glm::radians(-right_angle), glm::vec3(0.0, 1.0, 0.0));
    model = alTogether * translateMatrix * rotUp * rotRight * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 1.0, 0.8, 0.9);
    float bx = 0.0, by = 0.0;

    //shoot
    if (shoot)
    {
        bz += 0.2f;
        bx = slope_X * bz;
        by = slope_Y * bz;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(bullet_x + bx, bullet_y + by, bullet_z + bz));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
        model = alTogether * translateMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 0.5);
    }

    float bux = bullet_x + bx, buy = bullet_y + by, buz = bullet_z + bz;



    for (int i = 0; i < 5; i++)
    {
        float distance = sqrt((b[i].x - bux) * (b[i].x - bux) + (b[i].y - buy) * (b[i].y - buy) + (b[i].z - buz) * (b[i].z - buz));

        if (distance <= 0.6)
        {
            score++;
            toggle = true;
            shoot = false;
            bullet_z = -5.2f;
            bz = 0.0;
            std::random_device rd;  // Seed for the random number engine
            std::mt19937 gen(rd());  // Mersenne Twister engine
            std::uniform_real_distribution<float> dis(5.5f, 6.0f);
            b[i].z = dis(gen);
            std::uniform_real_distribution<float> dis2(i * 1.2 - 3, i * 1.2 - 3 + 0.6);
            b[i].x = dis2(gen);
            l = 0.005 * (score / 5);
            std::uniform_real_distribution<float> dis3(0.0015+l, 0.0025+l);
            b[i].speed = dis3(gen);
            std::uniform_real_distribution<float> dis4(6.0f, 6.5f);
            b[i].y = dis4(gen);
        }
    }


    if (bullet_z + bz > 7.0f)
    {
        shoot = false;
        bullet_z = -5.2f;
        bz = 0.0;
    }





}

int main()
{
    // glfw: initialize and configure
    // ------------------------------

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
   // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    vector<float>curve_idices = { {
-0.7950, 1.8600, 5.1000,
-0.7950, 1.8400, 5.1000,
-0.7850, 1.8050, 5.1000,
-0.7800, 1.7800, 5.1000,
-0.7750, 1.7500, 5.1000,
-0.7650, 1.7200, 5.1000,
-0.7650, 1.6650, 5.1000,
-0.7650, 1.6100, 5.1000,
-0.7650, 1.5750, 5.1000,
-0.7600, 1.5200, 5.1000,
-0.7650, 1.4500, 5.1000,
-0.7800, 1.3850, 5.1000,
-0.7900, 1.3300, 5.1000,
-0.8100, 1.2600, 5.1000,
-0.8250, 1.1950, 5.1000,
-0.8600, 1.1150, 5.1000,
-0.8800, 1.0500, 5.1000,
-0.8850, 1.0250, 5.1000,
-0.8850, 0.9900, 5.1000,
-0.8600, 0.9750, 5.1000,
-0.8250, 0.9500, 5.1000,
-0.7950, 0.9450, 5.1000,
-0.7700, 0.9200, 5.1000,
-0.7150, 0.9300, 5.1000,
-0.6750, 0.9300, 5.1000,
-0.6500, 0.9300, 5.1000,
-0.6200, 0.9300, 5.1000,
    } };
    vector<float>Fish = {

            {
    -0.4350, 1.4000, 5.1000,
    -0.5000, 1.4800, 5.1000,
    -0.6050, 1.5500, 5.1000,
    -0.7300, 1.5850, 5.1000,
    -0.8450, 1.5550, 5.1000,
    -0.9200, 1.5000, 5.1000,
    -1.0300, 1.4150, 5.1000,
    -1.1300, 1.3050, 5.1000,
    -1.2650, 1.2250, 5.1000,
    -1.3900, 1.1450, 5.1000,
    -1.5050, 1.0900, 5.1000,
    -1.6200, 1.0200, 5.1000,
    -1.6350, 0.9050, 5.1000,
    -1.5400, 0.8200, 5.1000,
    -1.3650, 0.7700, 5.1000,
    -1.2200, 0.7550, 5.1000,
    -1.0850, 0.7500, 5.1000,
    -0.9050, 0.7300, 5.1000,
    -0.7900, 0.7400, 5.1000,
    -0.6550, 0.7350, 5.1000,
    -0.4700, 0.6950, 5.1000,
    -0.3800, 0.6400, 5.1000,
    -0.3050, 0.5800, 5.1000,
    -0.2150, 0.5350, 5.1000,
    -0.1250, 0.5350, 5.1000,
    -0.0300, 0.5500, 5.1000
    }
    };
unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Sphere sphere = Sphere();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");

    string diffuseMapPath = "grass.jpg";
    string specularMapPath = "grass.jpg";

    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //ourShader.use();
    //lightingShader.use();

    // render loop
    // -----------

    std::random_device rd;  // Seed for the random number engine
    std::mt19937 gen(rd());  // Mersenne Twister engine
    std::uniform_real_distribution<float> dis(5.5f, 6.0f);
    float randomValue = dis(gen);
    balloon_z = randomValue;
    
    Balloon b[5];
    for (int i = 0; i < 5; i++)
    {
        //cout << "here" << endl;

        std::uniform_real_distribution<float> dis4(6.0f, 6.5f);
        b[i].y = dis4(gen);
        std::random_device rd;  // Seed for the random number engine
        std::mt19937 gen(rd());  // Mersenne Twister engine
        std::uniform_real_distribution<float> dis(5.5f, 6.0f);
        b[i].z = dis(gen);
        std::uniform_real_distribution<float> dis2(i * 1.2 - 3, i * 1.2 - 3 + 0.6);
        b[i].x = dis2(gen);
        std::uniform_real_distribution<float> dis3(0.0015+l, 0.0025+l);
        b[i].speed = dis3(gen);
    }

    cout << camera.Position[0] << " " << camera.Position[1] << " " << camera.Position[2];
    camera.Position[2] = -8;
    int count = 0;
    // score cubes
    diffuseMapPath = "Numbers/0.jpg";
    specularMapPath = "Numbers/0.jpg";
    unsigned int diffMap9 = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap9 = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dcube0 = Cube(diffMap9, specMap9, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "Numbers/1.jpg";
    specularMapPath = "Numbers/1.jpg";
    diffMap9 = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap9 = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dcube1 = Cube(diffMap9, specMap9, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "Numbers/2.jpg";
    specularMapPath = "Numbers/2.jpg";
    diffMap9 = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap9 = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dcube2 = Cube(diffMap9, specMap9, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "Numbers/3.jpg";
    specularMapPath = "Numbers/3.jpg";
    diffMap9 = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap9 = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dcube3 = Cube(diffMap9, specMap9, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "Numbers/4.jpg";
    specularMapPath = "Numbers/4.jpg";
    diffMap9 = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap9 = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dcube4 = Cube(diffMap9, specMap9, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "Numbers/5.jpg";
    specularMapPath = "Numbers/5.jpg";
    diffMap9 = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap9 = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dcube5 = Cube(diffMap9, specMap9, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "Numbers/6.jpg";
    specularMapPath = "Numbers/6.jpg";
    diffMap9 = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap9 = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dcube6 = Cube(diffMap9, specMap9, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "Numbers/7.jpg";
    specularMapPath = "Numbers/7.jpg";
    diffMap9 = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap9 = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dcube7 = Cube(diffMap9, specMap9, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "Numbers/8.jpg";
    specularMapPath = "Numbers/8.jpg";
    diffMap9 = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap9 = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dcube8 = Cube(diffMap9, specMap9, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "Numbers/9.jpg";
    specularMapPath = "Numbers/9.jpg";
    diffMap9 = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap9 = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dcube9 = Cube(diffMap9, specMap9, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    // lifes 
    diffuseMapPath = "Numbers/1l.jpg";
    specularMapPath = "Numbers/1l.jpg";
    diffMap9 = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap9 = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube lcube1 = Cube(diffMap9, specMap9, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "Numbers/2l.jpg";
    specularMapPath = "Numbers/2l.jpg";
    diffMap9 = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap9 = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube lcube2 = Cube(diffMap9, specMap9, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "Numbers/3l.jpg";
    specularMapPath = "Numbers/3l.jpg";
    diffMap9 = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap9 = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube lcube3 = Cube(diffMap9, specMap9, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "Numbers/4l.jpg";
    specularMapPath = "Numbers/4l.jpg";
    diffMap9 = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap9 = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube lcube4 = Cube(diffMap9, specMap9, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "Numbers/5l.jpg";
    specularMapPath = "Numbers/5l.jpg";
    diffMap9 = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap9 = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube lcube5 = Cube(diffMap9, specMap9, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "Numbers/game over.jpg";
    specularMapPath = "Numbers/game over.jpg";
    diffMap9 = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap9 = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube game_over = Cube(diffMap9, specMap9, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath2 = "Numbers/game over.jpg";
    string specularMapPath2 = "Numbers/game over.jpg";
    unsigned int diffMap2 = loadTexture(diffuseMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap2 = loadTexture(specularMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube2 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    
    diffuseMapPath = "curve.jpg";
    specularMapPath = "curve.jpg";
    diffMap9 = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap9 = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Curve  curve = Curve(Fish, diffMap9, specMap9, 5);
    

    while (!glfwWindowShouldClose(window))
    {

        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //camera.Position = glm::vec3(0.0f, 1.0f, -7.5f);



        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);

        directlight.setUpDirectionLight(lightingShader);

        spotlight.setUpSpotLight(lightingShader);

        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();

        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        func(cubeVAO, lightingShader, model, b);

        //curve
        if (life > 0) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.99f, 3.5f, -1.7f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(.2f, 2.2f, .5f));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(130.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            glm::mat4 tmp = glm::mat4(1.0f); 
            tmp = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

            curve.draw(lightingShader, tmp, glm::vec3(1.0f, 0.0f, 0.0f));
        }
        


        //glBindVertexArray(cubeVAO);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        //balloon_y -= 0.001f;
        glm::mat4 modelForSphere = glm::mat4(1.0f);
        //translateMatrix = glm::translate(model, glm::vec3(balloon_x, balloon_y, balloon_z));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8, 1.0, 1.0));
        //modelForSphere = translateMatrix * scaleMatrix;
        //sphere.drawSphere(lightingShader, modelForSphere);


        //if (balloon_y <= -1.0f)
        //{
        //    balloon_y = 6.0f;
        //    std::random_device rd;  // Seed for the random number engine
        //    std::mt19937 gen(rd());  // Mersenne Twister engine
        //    std::uniform_real_distribution<float> dis(4.0f, 6.0f);
        //    balloon_z = dis(gen);
        //    std::uniform_real_distribution<float> dis2(-1.0f, 1.0f);
        //    balloon_x = dis2(gen);
        //}


        if (life <= 0) {

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-4.65f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(9.2f, 4.6f, 0.1f));
            glm::mat4 modelForGameOver = model * translateMatrix * scaleMatrix;
            cube2.drawCubeWithTexture(lightingShaderWithTexture, modelForGameOver);
            
        }

        //balloon 1



        for (int i = 0; i < 5; i++)
        {
            //cout << "draw" << endl;
            b[i].y -= b[i].speed;
            translateMatrix = glm::translate(model, glm::vec3(b[i].x, b[i].y, b[i].z));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8, 1.0, 1.0));
            modelForSphere = translateMatrix * scaleMatrix;
            b[i].bb.drawSphere(lightingShader, modelForSphere);

            if (b[i].y <= -.05f)
            {
                std::uniform_real_distribution<float> dis4(6.0f, 6.5f);
                b[i].y = dis4(gen);
                std::random_device rd;  // Seed for the random number engine
                std::mt19937 gen(rd());  // Mersenne Twister engine
                std::uniform_real_distribution<float> dis(5.5f, 6.0f);
                b[i].z = dis(gen);
                std::uniform_real_distribution<float> dis2(i * 1.2 - 3, i * 1.2 - 3 + 0.6);
                b[i].x = dis2(gen);
                std::uniform_real_distribution<float> dis3(0.0015+l, 0.0025+l);
                b[i].speed = dis3(gen);
                life--;
                life = max(life, 0);
            }
        }
        // display score and life
        float rotateAngle_Xx = -10;
        float rotateAngle_Yy = 110;
        float rotateAngle_Zz = -30;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.1f, 3.35f, 0.01f));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Xx), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Yy), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Zz), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.01f, 0.55f, .4f));
        glm::mat4 digitCube1 = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

        translateMatrix = glm::translate(identityMatrix, glm::vec3(2.69f, 3.35f, 0.01f));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Xx), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Yy), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Zz), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.01f, 0.55f, .4f));
        glm::mat4 digitCube2 = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

        translateMatrix = glm::translate(identityMatrix, glm::vec3(2.29f, 3.35f, 0.01f));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Xx), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Yy), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Zz), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.01f, 0.55f, .4f));
        glm::mat4 digitCube3 = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

        rotateAngle_Xx = -10;
        rotateAngle_Yy = 70;
        rotateAngle_Zz = -30;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.99f, 3.35f, 0.01f));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Xx), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Yy), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Zz), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.01f, 0.55f, .4f));
        glm::mat4 digitCube4 = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

        //cube1.drawCubeWithTexture(lightingShaderWithTexture, digitCube1);
        if (life == 1)
        {
            lcube1.drawCubeWithTexture(lightingShaderWithTexture, digitCube4);
        }
        else  if (life == 2)
        {
            lcube2.drawCubeWithTexture(lightingShaderWithTexture, digitCube4);
        }
        else  if (life == 3)
        {
            lcube3.drawCubeWithTexture(lightingShaderWithTexture, digitCube4);
        }
        else  if (life == 4)
        {
            lcube4.drawCubeWithTexture(lightingShaderWithTexture, digitCube4);
        }
        else  if (life == 5)
        {
            lcube5.drawCubeWithTexture(lightingShaderWithTexture, digitCube4);
        }

            int d1, d2, d3;
            d3 = score % 10;
            d2 = (score / 10) % 10;
            d1 = (score / 100) % 10;

            // 1st digit
            if (d1 == 0)
            {
                dcube0.drawCubeWithTexture(lightingShaderWithTexture, digitCube1);
            }
            else  if (d1 == 1)
            {
                dcube1.drawCubeWithTexture(lightingShaderWithTexture, digitCube1);
            }
            else  if (d1 == 2)
            {
                dcube2.drawCubeWithTexture(lightingShaderWithTexture, digitCube1);
            }
            else  if (d1 == 3)
            {
                dcube3.drawCubeWithTexture(lightingShaderWithTexture, digitCube1);
            }
            else  if (d1 == 4)
            {
                dcube4.drawCubeWithTexture(lightingShaderWithTexture, digitCube1);
            }
            else  if (d1 == 5)
            {
                dcube5.drawCubeWithTexture(lightingShaderWithTexture, digitCube1);
            }
            else  if (d1 == 6)
            {
                dcube6.drawCubeWithTexture(lightingShaderWithTexture, digitCube1);
            }
            else  if (d1 == 7)
            {
                dcube7.drawCubeWithTexture(lightingShaderWithTexture, digitCube1);
            }
            else  if (d1 == 8)
            {
                dcube8.drawCubeWithTexture(lightingShaderWithTexture, digitCube1);
            }
            else  if (d1 == 9)
            {
                dcube9.drawCubeWithTexture(lightingShaderWithTexture, digitCube1);
            }


            //2nd digit

            if (d2 == 0)
            {
                dcube0.drawCubeWithTexture(lightingShaderWithTexture, digitCube2);
            }
            else  if (d2 == 1)
            {
                dcube1.drawCubeWithTexture(lightingShaderWithTexture, digitCube2);
            }
            else  if (d2 == 2)
            {
                dcube2.drawCubeWithTexture(lightingShaderWithTexture, digitCube2);
            }
            else  if (d2 == 3)
            {
                dcube3.drawCubeWithTexture(lightingShaderWithTexture, digitCube2);
            }
            else  if (d2 == 4)
            {
                dcube4.drawCubeWithTexture(lightingShaderWithTexture, digitCube2);
            }
            else  if (d2 == 5)
            {
                dcube5.drawCubeWithTexture(lightingShaderWithTexture, digitCube2);
            }
            else  if (d2 == 6)
            {
                dcube6.drawCubeWithTexture(lightingShaderWithTexture, digitCube2);
            }
            else  if (d2 == 7)
            {
                dcube7.drawCubeWithTexture(lightingShaderWithTexture, digitCube2);
            }
            else  if (d2 == 8)
            {
                dcube8.drawCubeWithTexture(lightingShaderWithTexture, digitCube2);
            }
            else  if (d2 == 9)
            {
                dcube9.drawCubeWithTexture(lightingShaderWithTexture, digitCube2);
            }
            // digit 3
            if (d3 == 0)
            {
                dcube0.drawCubeWithTexture(lightingShaderWithTexture, digitCube3);
            }
            else  if (d3 == 1)
            {
                dcube1.drawCubeWithTexture(lightingShaderWithTexture, digitCube3);
            }
            else  if (d3 == 2)
            {
                dcube2.drawCubeWithTexture(lightingShaderWithTexture, digitCube3);
            }
            else  if (d3 == 3)
            {
                dcube3.drawCubeWithTexture(lightingShaderWithTexture, digitCube3);
            }
            else  if (d3 == 4)
            {
                dcube4.drawCubeWithTexture(lightingShaderWithTexture, digitCube3);
            }
            else  if (d3 == 5)
            {
                dcube5.drawCubeWithTexture(lightingShaderWithTexture, digitCube3);
            }
            else  if (d3 == 6)
            {
                dcube6.drawCubeWithTexture(lightingShaderWithTexture, digitCube3);
            }
            else  if (d3 == 7)
            {
                dcube7.drawCubeWithTexture(lightingShaderWithTexture, digitCube3);
            }
            else  if (d3 == 8)
            {
                dcube8.drawCubeWithTexture(lightingShaderWithTexture, digitCube3);
            }
            else  if (d3 == 9)
            {
                dcube9.drawCubeWithTexture(lightingShaderWithTexture, digitCube3);
            }
            toggle = false;

        

        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        float cr, cg, cb;
        if (pointLightOn)
        {
            cr = 0.8f;
            cg = 0.8f;
            cb = 0.8f;
        }
        else
        {
            cr = 0.0f;
            cg = 0.0f;
            cb = 0.0f;
        }
        // point lights


        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(cr, cg, cb));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);
        lightingShaderWithTexture.setMat4("projection", projection);
        lightingShaderWithTexture.setMat4("view", view);

        lightingShaderWithTexture.use();
        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);



        //grass
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-10.5f, 0.1f, -8.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(20.0f, 0.2f, 20.0f));
        glm::mat4 modelMatrixForContainer3 = translateMatrix * scaleMatrix;
        cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer3);

        //translateMatrix = glm::translate(identityMatrix, glm::vec3(2.05f, 0.1f, grass2));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.2f, 10.0f));
        //glm::mat4 modelMatrixForContainer4 = translateMatrix * scaleMatrix;
        //cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer4);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 22.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        //gun_y += 0.003f;

        camera.ProcessKeyboard(FORWARD, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        //gun_y -= 0.003f;
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        // gun_x += 0.003f;
        camera.ProcessKeyboard(LEFT, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        // gun_x -= 0.003f;
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (life <= 0) {
            shoot = false;
        }
        else if (!shoot)
        {
            shoot = true;
            bullet_x = gun_x;
            bullet_y = gun_y;
            slope_X = tan(right_angle * 3.1416 / 180.0);
            slope_Y = -1 * tan(up_angle * 3.1416 / 180.0);
        }

    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        up_angle += 0.25f;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        up_angle -= 0.25f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        right_angle += 0.35f;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        right_angle -= 0.35f;
    }

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (directionLighton)
        {
            directlight.turnOff();
            directionLighton = !directionLighton;
        }
        else
        {
            directlight.turnOn();
            directionLighton = !directionLighton;
        }
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            pointlight1.turnOff();
            pointlight2.turnOff();
            pointlight3.turnOff();
            pointlight4.turnOff();
            pointLightOn = !pointLightOn;
        }
        else
        {
            pointlight1.turnOn();
            pointlight2.turnOn();
            pointlight3.turnOn();
            pointlight4.turnOn();
            pointLightOn = !pointLightOn;
        }
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if (spotlighton)
        {
            spotlight.turnOff();
            spotlighton = !spotlighton;
        }
        else
        {
            spotlight.turnOn();
            spotlighton = !spotlighton;
        }
    }
    else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        if (ambientToggle)
        {
            pointlight1.turnAmbientOff();
            pointlight2.turnAmbientOff();
            pointlight3.turnAmbientOff();
            pointlight4.turnAmbientOff();
            directlight.turnAmbientOff();
            spotlight.turnAmbientOff();
            ambientToggle = !ambientToggle;
        }
        else
        {
            pointlight1.turnAmbientOn();
            pointlight2.turnAmbientOn();
            pointlight3.turnAmbientOn();
            pointlight4.turnAmbientOn();
            directlight.turnAmbientOn();
            spotlight.turnAmbientOn();
            ambientToggle = !ambientToggle;
        }
    }
    else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        if (diffuseToggle)
        {
            pointlight1.turnDiffuseOff();
            pointlight2.turnDiffuseOff();
            pointlight3.turnDiffuseOff();
            pointlight4.turnDiffuseOff();
            directlight.turnDiffuseOff();
            spotlight.turnDiffuseOff();
            diffuseToggle = !diffuseToggle;
        }
        else
        {
            pointlight1.turnDiffuseOn();
            pointlight2.turnDiffuseOn();
            pointlight3.turnDiffuseOn();
            pointlight4.turnDiffuseOn();
            directlight.turnDiffuseOn();
            spotlight.turnDiffuseOn();
            diffuseToggle = !diffuseToggle;
        }
    }
    else if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {
        if (specularToggle)
        {
            pointlight1.turnSpecularOff();
            pointlight2.turnSpecularOff();
            pointlight3.turnSpecularOff();
            pointlight4.turnSpecularOff();
            directlight.turnSpecularOff();
            spotlight.turnSpecularOff();
            specularToggle = !specularToggle;
        }
        else
        {
            pointlight1.turnSpecularOn();
            pointlight2.turnSpecularOn();
            pointlight3.turnSpecularOn();
            pointlight4.turnSpecularOn();
            directlight.turnSpecularOn();
            spotlight.turnSpecularOn();
            specularToggle = !specularToggle;
        }
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}