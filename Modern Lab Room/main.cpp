

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "spotLight.h"
#include "sphere.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void drawCube(unsigned int &cubeVAO, Shader &lightingShader, glm::mat4 model, float r, float g, float b);

void room(unsigned int &cubeVAO, Shader &lightingShader);
void whiteboard(unsigned int &cubeVAO, Shader &lightingShader);
void teacherTable(unsigned int &cubeVAO, Shader &lightingShader);
void teacherChair(unsigned int &cubeVAO, Shader &lightingShader);
void studentDesk(unsigned int &cubeVAO, Shader &lightingShader, glm::mat4 parent);
void studentChair(unsigned int &cubeVAO, Shader &lightingShader, glm::mat4 parent);
void studentCPU(unsigned int &cubeVAO, Shader &lightingShader, glm::mat4 parent);
void studentMonitor(unsigned int &cubeVAO, Shader &lightingShader, glm::mat4 parent);
void airConditioner(unsigned int &cubeVAO, Shader &lightingShader);
void projector(unsigned int &cubeVAO, Shader &lightingShader);
void ceilingFan(unsigned int &cubeVAO, Shader &lightingShader, glm::mat4 parent, float angle);
void door(unsigned int &cubeVAO, Shader &lightingShader);
void teacherPerson(unsigned int &cubeVAO, Shader &lightingShader);
void studentPerson(unsigned int &cubeVAO, Shader &lightingShader, glm::mat4 parent);

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

// camera
Camera camera(glm::vec3(0.0f, 2.4f, -19.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(-5.0f, 5.0f, 9.0f),
    glm::vec3(5.0f, 5.0f, 9.0f),
    glm::vec3(-5.0f, 5.0f, -9.0f),
    glm::vec3(5.0f, 5.0f, -9.0f)};

PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z, // position
    0.05f, 0.05f, 0.05f,                                                          // ambient
    0.8f, 0.8f, 0.8f,                                                             // diffuse
    1.0f, 1.0f, 1.0f,                                                             // specular
    1.0f,                                                                         // k_c
    0.055f,                                                                       // k_l
    0.012f,                                                                       // k_q
    1                                                                             // light number
);

PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z, // position
    0.05f, 0.05f, 0.05f,                                                          // ambient
    0.8f, 0.8f, 0.8f,                                                             // diffuse
    1.0f, 1.0f, 1.0f,                                                             // specular
    1.0f,                                                                         // k_c
    0.055f,                                                                       // k_l
    0.012f,                                                                       // k_q
    2                                                                             // light number
);

PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z, // position
    0.05f, 0.05f, 0.05f,                                                          // ambient
    0.8f, 0.8f, 0.8f,                                                             // diffuse
    1.0f, 1.0f, 1.0f,                                                             // specular
    1.0f,                                                                         // k_c
    0.055f,                                                                       // k_l
    0.012f,                                                                       // k_q
    3                                                                             // light number
);

PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z, // position
    0.05f, 0.05f, 0.05f,                                                          // ambient
    0.8f, 0.8f, 0.8f,                                                             // diffuse
    1.0f, 1.0f, 1.0f,                                                             // specular
    1.0f,                                                                         // k_c
    0.055f,                                                                       // k_l
    0.012f,                                                                       // k_q
    4                                                                             // light number
);
// Projector Spotlight
glm::vec3 projectorLightPosition(-0.25f, 4.18f, 3.80f);
glm::vec3 whiteboardCenter(0.0f, 1.8f, 19.7f);

glm::vec3 projectorLightDirection =
    glm::normalize(whiteboardCenter - projectorLightPosition);

SpotLight projectorSpotLight(
    projectorLightPosition,
    projectorLightDirection,

    glm::vec3(0.0f),             // ambient
    glm::vec3(1.0f, 1.0f, 1.0f), // diffuse
    glm::vec3(1.0f, 1.0f, 1.0f), // specular

    1.0f,
    0.055f,
    0.012f,

    12.5f,
    18.0f);
// light settings
bool pointLightOn = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

// Fan on off
bool fanOn = true;
bool fanKeyPressed = false;
float fanAngle = 0.0f;

bool projectorOn = true;
bool projectorKeyPressed = false;

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
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lab Room", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
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
    // Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
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
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f};
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
        22, 23, 20};

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // ourShader.use();
    // lightingShader.use();

    // render loop
    // -----------
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

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // Room Lighting
        if (pointLightOn)
        {
            pointlight1.ambient = glm::vec3(0.14f);
            pointlight1.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
            pointlight1.specular = glm::vec3(0.90f);

            pointlight2.ambient = glm::vec3(0.14f);
            pointlight2.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
            pointlight2.specular = glm::vec3(0.90f);

            pointlight3.ambient = glm::vec3(0.14f);
            pointlight3.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
            pointlight3.specular = glm::vec3(0.90f);

            pointlight4.ambient = glm::vec3(0.14f);
            pointlight4.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
            pointlight4.specular = glm::vec3(0.90f);
        }
        else
        {
            pointlight1.ambient = glm::vec3(0.05f);
            pointlight1.diffuse = glm::vec3(0.35f);
            pointlight1.specular = glm::vec3(0.25f);

            pointlight2.ambient = glm::vec3(0.05f);
            pointlight2.diffuse = glm::vec3(0.35f);
            pointlight2.specular = glm::vec3(0.25f);

            pointlight3.ambient = glm::vec3(0.05f);
            pointlight3.diffuse = glm::vec3(0.35f);
            pointlight3.specular = glm::vec3(0.25f);

            pointlight4.ambient = glm::vec3(0.05f);
            pointlight4.diffuse = glm::vec3(0.35f);
            pointlight4.specular = glm::vec3(0.25f);
        }

        pointlight1.setUpPointLight(lightingShader);
        pointlight2.setUpPointLight(lightingShader);
        pointlight3.setUpPointLight(lightingShader);
        pointlight4.setUpPointLight(lightingShader);

        if (projectorOn)
            {
                projectorSpotLight.ambient = glm::vec3(0.0f);

                projectorSpotLight.diffuse =
                    glm::vec3(2.50f, 2.50f, 2.50f);

                projectorSpotLight.specular =
                    glm::vec3(1.0f, 1.0f, 1.0f);
            }
            else
            {
                projectorSpotLight.ambient = glm::vec3(0.0f);
                projectorSpotLight.diffuse = glm::vec3(0.0f);
                projectorSpotLight.specular = glm::vec3(0.0f);
            }

            projectorSpotLight.setUpSpotLight(lightingShader);
        projectorSpotLight.setUpSpotLight(lightingShader);
        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        // glm::mat4 view = basic_camera.createViewMatrix();
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

        // room
        room(cubeVAO, lightingShader);
        whiteboard(cubeVAO, lightingShader);
        teacherTable(cubeVAO, lightingShader);
        teacherChair(cubeVAO, lightingShader);
        airConditioner(cubeVAO, lightingShader);
        projector(cubeVAO, lightingShader);
        door(cubeVAO, lightingShader);
        teacherPerson(cubeVAO, lightingShader);

        if (fanOn)
        {
            fanAngle += 360.0f * deltaTime;

            if (fanAngle > 360.0f)
                fanAngle -= 360.0f;
        }

        // Left Ceiling Fan
        glm::mat4 leftFan = glm::mat4(1.0f);
        leftFan = glm::translate(leftFan, glm::vec3(-5.0f, 0.0f, 9.0f));
        ceilingFan(cubeVAO, lightingShader, leftFan, fanAngle);

        // Right Ceiling Fan
        glm::mat4 rightFan = glm::mat4(1.0f);
        rightFan = glm::translate(rightFan, glm::vec3(5.0f, 0.0f, 9.0f));
        ceilingFan(cubeVAO, lightingShader, rightFan, fanAngle);

        // Single Student
        // studentDesk(cubeVAO, lightingShader);
        // studentChair(cubeVAO, lightingShader);

        // 3 Rows x 3 Columns Student Setup
        for (int row = 0; row < 3; row++)
        {
            for (int col = 0; col < 3; col++)
            {
                glm::mat4 studentSet = glm::mat4(1.0f);

                float xOffset = -9.0f + col * 6.0f;
                float zOffset = 0.0f - row * 6.0f;

                studentSet = glm::translate(studentSet, glm::vec3(xOffset, 0.0f, zOffset));

                studentDesk(cubeVAO, lightingShader, studentSet);
                studentChair(cubeVAO, lightingShader, studentSet);
                studentCPU(cubeVAO, lightingShader, studentSet);
                studentMonitor(cubeVAO, lightingShader, studentSet);
                studentPerson(cubeVAO, lightingShader, studentSet);
            }
        }
        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        glm::vec3 panelPositions[] = {
            glm::vec3(-5.0f, 5.85f, 11.0f),
            glm::vec3(5.0f, 5.85f, 11.0f),
            glm::vec3(-5.0f, 5.85f, 0.0f),
            glm::vec3(5.0f, 5.85f, 0.0f),
            glm::vec3(-5.0f, 5.85f, -11.0f),
            glm::vec3(5.0f, 5.85f, -11.0f)};

        glm::vec3 panelColors[] = {
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f)};

        glBindVertexArray(lightCubeVAO);

        for (unsigned int i = 0; i < 6; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, panelPositions[i]);
            model = glm::scale(model, glm::vec3(3.2f, 0.08f, 0.75f));

            ourShader.setMat4("model", model);

            if (pointLightOn)
                ourShader.setVec3("color", panelColors[i]);
            else
                ourShader.setVec3("color", glm::vec3(0.20f));

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

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

void drawCube(unsigned int &cubeVAO, Shader &lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 64.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
// Room
void room(unsigned int &cubeVAO, Shader &lightingShader)
{
    glm::mat4 model;

    // Floor
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-10.0f, -2.0f, -20.0f));
    model = glm::scale(model, glm::vec3(20.0f, 0.1f, 40.0f));
    //   drawCube(cubeVAO, lightingShader, model, 0.50f, 0.50f, 0.48f);

    // Ceiling
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-10.0f, 6.0f, -20.0f));
    model = glm::scale(model, glm::vec3(20.0f, 0.1f, 40.0f));
    drawCube(cubeVAO, lightingShader, model, 0.96f, 0.95f, 0.92f);

    // Left Wall
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-10.0f, -2.0f, -20.0f));
    model = glm::scale(model, glm::vec3(0.1f, 8.0f, 40.0f));
    drawCube(cubeVAO, lightingShader, model, 0.92f, 0.87f, 0.76f);

    // Right Wall
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(9.9f, -2.0f, -20.0f));
    model = glm::scale(model, glm::vec3(0.1f, 8.0f, 40.0f));
    drawCube(cubeVAO, lightingShader, model, 0.92f, 0.87f, 0.76f);

    // Front Wall
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-10.0f, -2.0f, 20.0f));
    model = glm::scale(model, glm::vec3(20.0f, 8.0f, 0.1f));
    drawCube(cubeVAO, lightingShader, model, 0.92f, 0.87f, 0.76f);

    // Back Wall
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-10.0f, -2.0f, -20.0f));
    model = glm::scale(model, glm::vec3(20.0f, 8.0f, 0.1f));
    drawCube(cubeVAO, lightingShader, model, 0.92f, 0.87f, 0.76f);
}

// Whiteboard
void whiteboard(unsigned int &cubeVAO, Shader &lightingShader)
{
    glm::mat4 model;

    // Whiteboard
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-5.0f, 0.25f, 19.7f));
    model = glm::scale(model, glm::vec3(10.0f, 3.5f, 0.15f));
    drawCube(cubeVAO, lightingShader, model, 0.92f, 0.92f, 0.90f);

    // Top Frame
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-5.15f, 3.75f, 19.65f));
    model = glm::scale(model, glm::vec3(10.3f, 0.15f, 0.2f));
    drawCube(cubeVAO, lightingShader, model, 0.12f, 0.12f, 0.12f);

    // Bottom Frame
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-5.15f, 0.10f, 19.65f));
    model = glm::scale(model, glm::vec3(10.3f, 0.15f, 0.2f));
    drawCube(cubeVAO, lightingShader, model, 0.12f, 0.12f, 0.12f);

    // Left Frame
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-5.15f, 0.25f, 19.65f));
    model = glm::scale(model, glm::vec3(0.15f, 3.5f, 0.2f));
    drawCube(cubeVAO, lightingShader, model, 0.12f, 0.12f, 0.12f);

    // Right Frame
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(5.0f, 0.25f, 19.65f));
    model = glm::scale(model, glm::vec3(0.15f, 3.5f, 0.2f));
    drawCube(cubeVAO, lightingShader, model, 0.12f, 0.12f, 0.12f);
}

// Teacher Computer Desk
void teacherTable(unsigned int &cubeVAO, Shader &lightingShader)
{
    glm::mat4 model;

    // Table Top
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-8.1f, 0.0f, 13.5f));
    model = glm::scale(model, glm::vec3(4.8f, 0.25f, 2.2f));
    drawCube(cubeVAO, lightingShader, model, 0.90f, 0.84f, 0.72f);

    // Front Left Leg
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-7.9f, -2.0f, 13.7f));
    model = glm::scale(model, glm::vec3(0.22f, 2.0f, 0.22f));
    drawCube(cubeVAO, lightingShader, model, 0.76f, 0.67f, 0.54f);

    // Front Right Leg
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-3.7f, -2.0f, 13.7f));
    model = glm::scale(model, glm::vec3(0.22f, 2.0f, 0.22f));
    drawCube(cubeVAO, lightingShader, model, 0.76f, 0.67f, 0.54f);

    // Back Left Leg
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-7.9f, -2.0f, 15.2f));
    model = glm::scale(model, glm::vec3(0.22f, 2.0f, 0.22f));
    drawCube(cubeVAO, lightingShader, model, 0.76f, 0.67f, 0.54f);

    // Back Right Leg
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-3.7f, -2.0f, 15.2f));
    model = glm::scale(model, glm::vec3(0.22f, 2.0f, 0.22f));
    drawCube(cubeVAO, lightingShader, model, 0.76f, 0.67f, 0.54f);

    // Back Support
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-7.7f, -1.1f, 15.15f));
    model = glm::scale(model, glm::vec3(3.8f, 0.15f, 0.12f));
    drawCube(cubeVAO, lightingShader, model, 0.70f, 0.62f, 0.50f);
}

// Teacher Chair
void teacherChair(unsigned int &cubeVAO, Shader &lightingShader)
{
    glm::mat4 model;

    // Seat
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-6.7f, -0.9f, 16.2f));
    model = glm::scale(model, glm::vec3(1.8f, 0.25f, 1.6f));
    drawCube(cubeVAO, lightingShader, model, 0.08f, 0.12f, 0.20f);

    // Back Rest
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-6.55f, -0.5f, 17.55f));
    model = glm::scale(model, glm::vec3(1.5f, 1.9f, 0.20f));
    drawCube(cubeVAO, lightingShader, model, 0.08f, 0.12f, 0.20f);

    // Left Arm
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-6.75f, -0.55f, 16.3f));
    model = glm::scale(model, glm::vec3(0.18f, 0.55f, 1.0f));
    drawCube(cubeVAO, lightingShader, model, 0.05f, 0.06f, 0.08f);

    // Right Arm
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-5.15f, -0.55f, 16.3f));
    model = glm::scale(model, glm::vec3(0.18f, 0.55f, 1.0f));
    drawCube(cubeVAO, lightingShader, model, 0.05f, 0.06f, 0.08f);

    // Front Left Leg
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-6.55f, -2.0f, 16.35f));
    model = glm::scale(model, glm::vec3(0.18f, 1.1f, 0.18f));
    drawCube(cubeVAO, lightingShader, model, 0.05f, 0.05f, 0.05f);

    // Front Right Leg
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-5.35f, -2.0f, 16.35f));
    model = glm::scale(model, glm::vec3(0.18f, 1.1f, 0.18f));
    drawCube(cubeVAO, lightingShader, model, 0.05f, 0.05f, 0.05f);

    // Back Left Leg
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-6.55f, -2.0f, 17.45f));
    model = glm::scale(model, glm::vec3(0.18f, 1.1f, 0.18f));
    drawCube(cubeVAO, lightingShader, model, 0.05f, 0.05f, 0.05f);

    // Back Right Leg
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-5.35f, -2.0f, 17.45f));
    model = glm::scale(model, glm::vec3(0.18f, 1.1f, 0.18f));
    drawCube(cubeVAO, lightingShader, model, 0.05f, 0.05f, 0.05f);
}

// student Desk
void studentDesk(unsigned int &cubeVAO, Shader &lightingShader, glm::mat4 parent)
{
    glm::mat4 model;

    model = parent;
    model = glm::translate(model, glm::vec3(1.5f, -0.55f, 7.0f));
    model = glm::scale(model, glm::vec3(3.4f, 0.2f, 2.0f));
    drawCube(cubeVAO, lightingShader, model, 0.82f, 0.66f, 0.46f);

    model = parent;
    model = glm::translate(model, glm::vec3(1.7f, -1.95f, 7.2f));
    model = glm::scale(model, glm::vec3(0.18f, 1.4f, 0.18f));
    drawCube(cubeVAO, lightingShader, model, 0.58f, 0.42f, 0.28f);

    model = parent;
    model = glm::translate(model, glm::vec3(4.5f, -1.95f, 7.2f));
    model = glm::scale(model, glm::vec3(0.18f, 1.4f, 0.18f));
    drawCube(cubeVAO, lightingShader, model, 0.58f, 0.42f, 0.28f);

    model = parent;
    model = glm::translate(model, glm::vec3(1.7f, -1.95f, 8.6f));
    model = glm::scale(model, glm::vec3(0.18f, 1.4f, 0.18f));
    drawCube(cubeVAO, lightingShader, model, 0.58f, 0.42f, 0.28f);

    model = parent;
    model = glm::translate(model, glm::vec3(4.5f, -1.95f, 8.6f));
    model = glm::scale(model, glm::vec3(0.18f, 1.4f, 0.18f));
    drawCube(cubeVAO, lightingShader, model, 0.58f, 0.42f, 0.28f);
}

// Student Chair
void studentChair(unsigned int &cubeVAO, Shader &lightingShader, glm::mat4 parent)
{
    glm::mat4 model;

    // Seat
    model = parent;
    model = glm::translate(model, glm::vec3(2.45f, -1.20f, 5.80f));
    model = glm::scale(model, glm::vec3(1.50f, 0.20f, 1.20f));
    drawCube(cubeVAO, lightingShader, model, 0.10f, 0.16f, 0.24f);

    // Back Rest
    model = parent;
    model = glm::translate(model, glm::vec3(2.55f, -1.00f, 5.60f));
    model = glm::scale(model, glm::vec3(1.30f, 1.25f, 0.18f));
    drawCube(cubeVAO, lightingShader, model, 0.10f, 0.16f, 0.24f);

    // Front Left Leg
    model = parent;
    model = glm::translate(model, glm::vec3(2.60f, -2.0f, 6.65f));
    model = glm::scale(model, glm::vec3(0.16f, 0.80f, 0.16f));
    drawCube(cubeVAO, lightingShader, model, 0.08f, 0.08f, 0.08f);

    // Front Right Leg
    model = parent;
    model = glm::translate(model, glm::vec3(3.55f, -2.0f, 6.65f));
    model = glm::scale(model, glm::vec3(0.16f, 0.80f, 0.16f));
    drawCube(cubeVAO, lightingShader, model, 0.08f, 0.08f, 0.08f);

    // Back Left Leg
    model = parent;
    model = glm::translate(model, glm::vec3(2.60f, -2.0f, 5.95f));
    model = glm::scale(model, glm::vec3(0.16f, 0.80f, 0.16f));
    drawCube(cubeVAO, lightingShader, model, 0.08f, 0.08f, 0.08f);

    // Back Right Leg
    model = parent;
    model = glm::translate(model, glm::vec3(3.55f, -2.0f, 5.95f));
    model = glm::scale(model, glm::vec3(0.16f, 0.80f, 0.16f));
    drawCube(cubeVAO, lightingShader, model, 0.08f, 0.08f, 0.08f);
}

// Student CPU
void studentCPU(unsigned int &cubeVAO, Shader &lightingShader, glm::mat4 parent)
{
    glm::mat4 model;

    // CPU Body
    model = parent;
    model = glm::translate(model, glm::vec3(4.05f, -1.85f, 7.35f));
    model = glm::scale(model, glm::vec3(0.55f, 1.30f, 0.80f));
    drawCube(cubeVAO, lightingShader, model, 0.10f, 0.10f, 0.12f);

    // CPU Front Panel
    model = parent;
    model = glm::translate(model, glm::vec3(4.12f, -1.72f, 7.28f));
    model = glm::scale(model, glm::vec3(0.41f, 1.05f, 0.08f));
    drawCube(cubeVAO, lightingShader, model, 0.16f, 0.16f, 0.18f);

    // Power Button
    model = parent;
    model = glm::translate(model, glm::vec3(4.25f, -0.85f, 7.18f));
    model = glm::scale(model, glm::vec3(0.14f, 0.14f, 0.10f));
    drawCube(cubeVAO, lightingShader, model, 0.20f, 0.55f, 0.85f);
}

// Student Monitor
void studentMonitor(unsigned int &cubeVAO, Shader &lightingShader, glm::mat4 parent)
{
    glm::mat4 model;

    // Monitor Body
    model = parent;
    model = glm::translate(model, glm::vec3(2.45f, -0.15f, 8.25f));
    model = glm::scale(model, glm::vec3(1.55f, 1.05f, 0.12f));
    drawCube(cubeVAO, lightingShader, model, 0.05f, 0.05f, 0.06f);

    // Screen
    model = parent;
    model = glm::translate(model, glm::vec3(2.55f, -0.05f, 8.20f));
    model = glm::scale(model, glm::vec3(1.35f, 0.82f, 0.06f));
    drawCube(cubeVAO, lightingShader, model, 0.08f, 0.18f, 0.30f);

    // Monitor Stand
    model = parent;
    model = glm::translate(model, glm::vec3(3.15f, -0.48f, 8.30f));
    model = glm::scale(model, glm::vec3(0.15f, 0.35f, 0.15f));
    drawCube(cubeVAO, lightingShader, model, 0.10f, 0.10f, 0.10f);

    // Stand Base
    model = parent;
    model = glm::translate(model, glm::vec3(2.85f, -0.53f, 8.10f));
    model = glm::scale(model, glm::vec3(0.75f, 0.08f, 0.55f));
    drawCube(cubeVAO, lightingShader, model, 0.10f, 0.10f, 0.10f);
}

// Air Conditioner
void airConditioner(unsigned int &cubeVAO, Shader &lightingShader)
{
    glm::mat4 model;

    // AC Main Body
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-9.75f, 3.6f, 4.0f));
    model = glm::scale(model, glm::vec3(0.35f, 1.25f, 3.6f));
    drawCube(cubeVAO, lightingShader, model, 0.92f, 0.92f, 0.90f);

    // Front Panel
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-9.55f, 3.75f, 4.25f));
    model = glm::scale(model, glm::vec3(0.10f, 0.80f, 3.10f));
    drawCube(cubeVAO, lightingShader, model, 0.82f, 0.84f, 0.84f);

    // Air Outlet
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-9.48f, 3.55f, 4.65f));
    model = glm::scale(model, glm::vec3(0.08f, 0.20f, 2.30f));
    drawCube(cubeVAO, lightingShader, model, 0.18f, 0.18f, 0.18f);

    // Display
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-9.45f, 4.30f, 6.55f));
    model = glm::scale(model, glm::vec3(0.08f, 0.22f, 0.40f));
    drawCube(cubeVAO, lightingShader, model, 0.12f, 0.45f, 0.65f);
}

// Projector
void projector(unsigned int &cubeVAO, Shader &lightingShader)
{
    glm::mat4 model;

    // Ceiling Rod
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-0.1f, 4.8f, 3.0f));
    model = glm::scale(model, glm::vec3(0.20f, 1.2f, 0.20f));
    drawCube(cubeVAO, lightingShader, model, 0.20f, 0.20f, 0.20f);

    // Projector Body
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 4.0f, 2.3f));
    model = glm::scale(model, glm::vec3(2.0f, 0.65f, 1.5f));
    drawCube(cubeVAO, lightingShader, model, 0.82f, 0.82f, 0.80f);

    // Front Face
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-0.85f, 4.10f, 3.72f));
    model = glm::scale(model, glm::vec3(1.70f, 0.42f, 0.10f));
    drawCube(cubeVAO, lightingShader, model, 0.68f, 0.68f, 0.66f);

    // Lens
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-0.25f, 4.18f, 3.80f));
    model = glm::scale(model, glm::vec3(0.45f, 0.28f, 0.15f));
    drawCube(cubeVAO, lightingShader, model, 0.12f, 0.12f, 0.15f);

    // Indicator
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.55f, 4.20f, 3.82f));
    model = glm::scale(model, glm::vec3(0.12f, 0.12f, 0.10f));
    drawCube(cubeVAO, lightingShader, model, 0.10f, 0.80f, 0.10f);
}

// Ceiling Fan
void ceilingFan(unsigned int &cubeVAO, Shader &lightingShader, glm::mat4 parent, float angle)
{
    glm::mat4 model;

    // Rod
    model = parent;
    model = glm::translate(model, glm::vec3(-0.08f, 4.8f, -4.58f));
    model = glm::scale(model, glm::vec3(0.15f, 1.2f, 0.15f));
    drawCube(cubeVAO, lightingShader, model, 0.25f, 0.25f, 0.25f);

    // Rotation around fan center
    glm::mat4 rotateFan = parent;
    rotateFan = glm::translate(rotateFan, glm::vec3(0.0f, 4.62f, -4.5f));
    rotateFan = glm::rotate(rotateFan, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateFan = glm::translate(rotateFan, glm::vec3(0.0f, -4.62f, 4.5f));

    // Center Hub
    model = rotateFan;
    model = glm::translate(model, glm::vec3(-0.30f, 4.5f, -4.80f));
    model = glm::scale(model, glm::vec3(0.60f, 0.25f, 0.60f));
    drawCube(cubeVAO, lightingShader, model, 0.32f, 0.32f, 0.32f);

    // Right Blade
    model = rotateFan;
    model = glm::translate(model, glm::vec3(0.30f, 4.5f, -4.68f));
    model = glm::scale(model, glm::vec3(2.3f, 0.12f, 0.35f));
    drawCube(cubeVAO, lightingShader, model, 0.45f, 0.45f, 0.45f);

    // Left Blade
    model = rotateFan;
    model = glm::translate(model, glm::vec3(-2.60f, 4.5f, -4.68f));
    model = glm::scale(model, glm::vec3(2.3f, 0.12f, 0.35f));
    drawCube(cubeVAO, lightingShader, model, 0.45f, 0.45f, 0.45f);

    // Front Blade
    model = rotateFan;
    model = glm::translate(model, glm::vec3(-0.18f, 4.5f, -4.20f));
    model = glm::scale(model, glm::vec3(0.35f, 0.12f, 2.3f));
    drawCube(cubeVAO, lightingShader, model, 0.45f, 0.45f, 0.45f);

    // Back Blade
    model = rotateFan;
    model = glm::translate(model, glm::vec3(-0.18f, 4.5f, -7.10f));
    model = glm::scale(model, glm::vec3(0.35f, 0.12f, 2.3f));
    drawCube(cubeVAO, lightingShader, model, 0.45f, 0.45f, 0.45f);
}

// Door Frame
void door(unsigned int &cubeVAO, Shader &lightingShader)
{
    glm::mat4 model;

    // Dark Opening
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-9.70f, -2.0f, 4.0f));
    model = glm::scale(model, glm::vec3(0.20f, 5.0f, 3.0f));
    drawCube(cubeVAO, lightingShader, model, 0.02f, 0.02f, 0.02f);

    // Left Frame
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-9.55f, -2.0f, 3.8f));
    model = glm::scale(model, glm::vec3(0.25f, 5.2f, 0.18f));
    drawCube(cubeVAO, lightingShader, model, 0.18f, 0.10f, 0.05f);

    // Right Frame
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-9.55f, -2.0f, 7.0f));
    model = glm::scale(model, glm::vec3(0.25f, 5.2f, 0.18f));
    drawCube(cubeVAO, lightingShader, model, 0.18f, 0.10f, 0.05f);

    // Top Frame
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-9.55f, 3.0f, 3.8f));
    model = glm::scale(model, glm::vec3(0.25f, 0.20f, 3.38f));
    drawCube(cubeVAO, lightingShader, model, 0.18f, 0.10f, 0.05f);
}

// Teacher
void teacherPerson(unsigned int &cubeVAO, Shader &lightingShader)
{
    glm::mat4 model;

    // Body
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-6.35f, -0.15f, 16.35f));
    model = glm::scale(model, glm::vec3(0.95f, 1.35f, 0.60f));
    drawCube(cubeVAO, lightingShader, model, 0.45f, 0.16f, 0.30f);

    // Neck
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-6.05f, 1.15f, 16.45f));
    model = glm::scale(model, glm::vec3(0.32f, 0.25f, 0.32f));
    drawCube(cubeVAO, lightingShader, model, 0.82f, 0.62f, 0.48f);

    // Head
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-6.22f, 1.35f, 16.32f));
    model = glm::scale(model, glm::vec3(0.68f, 0.78f, 0.62f));
    drawCube(cubeVAO, lightingShader, model, 0.82f, 0.62f, 0.48f);

    // Hair Top
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-6.28f, 2.02f, 16.28f));
    model = glm::scale(model, glm::vec3(0.80f, 0.18f, 0.72f));
    drawCube(cubeVAO, lightingShader, model, 0.10f, 0.05f, 0.03f);

    // Hair Left
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-6.30f, 1.35f, 16.30f));
    model = glm::scale(model, glm::vec3(0.16f, 0.85f, 0.68f));
    drawCube(cubeVAO, lightingShader, model, 0.10f, 0.05f, 0.03f);

    // Hair Right
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-5.60f, 1.35f, 16.30f));
    model = glm::scale(model, glm::vec3(0.16f, 0.85f, 0.68f));
    drawCube(cubeVAO, lightingShader, model, 0.10f, 0.05f, 0.03f);

    // Left Eye
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-6.05f, 1.75f, 16.28f));
    model = glm::scale(model, glm::vec3(0.08f, 0.09f, 0.05f));
    drawCube(cubeVAO, lightingShader, model, 0.03f, 0.03f, 0.03f);

    // Right Eye
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-5.78f, 1.75f, 16.28f));
    model = glm::scale(model, glm::vec3(0.08f, 0.09f, 0.05f));
    drawCube(cubeVAO, lightingShader, model, 0.03f, 0.03f, 0.03f);

    // Mouth
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-5.94f, 1.52f, 16.27f));
    model = glm::scale(model, glm::vec3(0.18f, 0.05f, 0.05f));
    drawCube(cubeVAO, lightingShader, model, 0.45f, 0.08f, 0.08f);

    // Left Upper Arm
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-6.55f, 0.75f, 16.38f));
    model = glm::rotate(model, glm::radians(18.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.22f, 0.75f, 0.22f));
    drawCube(cubeVAO, lightingShader, model, 0.45f, 0.16f, 0.30f);

    // Right Upper Arm
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-5.45f, 0.75f, 16.38f));
    model = glm::rotate(model, glm::radians(18.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.22f, 0.75f, 0.22f));
    drawCube(cubeVAO, lightingShader, model, 0.45f, 0.16f, 0.30f);

    // Left Hand
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-6.55f, 0.18f, 15.55f));
    model = glm::scale(model, glm::vec3(0.26f, 0.14f, 0.26f));
    drawCube(cubeVAO, lightingShader, model, 0.82f, 0.62f, 0.48f);

    // Right Hand
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-5.45f, 0.18f, 15.55f));
    model = glm::scale(model, glm::vec3(0.26f, 0.14f, 0.26f));
    drawCube(cubeVAO, lightingShader, model, 0.82f, 0.62f, 0.48f);

    // Lower Body
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-6.30f, -0.75f, 16.35f));
    model = glm::scale(model, glm::vec3(0.85f, 0.60f, 0.80f));
    drawCube(cubeVAO, lightingShader, model, 0.12f, 0.13f, 0.18f);

    // Left Thigh
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-6.20f, -0.85f, 15.65f));
    model = glm::scale(model, glm::vec3(0.30f, 0.30f, 0.90f));
    drawCube(cubeVAO, lightingShader, model, 0.12f, 0.13f, 0.18f);

    // Right Thigh
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-5.75f, -0.85f, 15.65f));
    model = glm::scale(model, glm::vec3(0.30f, 0.30f, 0.90f));
    drawCube(cubeVAO, lightingShader, model, 0.12f, 0.13f, 0.18f);
}

// Student
void studentPerson(unsigned int &cubeVAO, Shader &lightingShader, glm::mat4 parent)
{
    glm::mat4 model;

    // Body
    model = parent;
    model = glm::translate(model, glm::vec3(2.70f, -1.00f, 6.05f));
    model = glm::scale(model, glm::vec3(1.05f, 1.35f, 0.65f));
    drawCube(cubeVAO, lightingShader, model, 0.12f, 0.38f, 0.24f);

    // Head
    model = parent;
    model = glm::translate(model, glm::vec3(2.88f, 0.30f, 6.05f));
    model = glm::scale(model, glm::vec3(0.70f, 0.72f, 0.65f));
    drawCube(cubeVAO, lightingShader, model, 0.72f, 0.48f, 0.30f);

    // Left Arm
    model = parent;
    model = glm::translate(model, glm::vec3(2.45f, -0.55f, 6.45f));
    model = glm::rotate(model, glm::radians(-35.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.25f, 0.95f, 0.25f));
    drawCube(cubeVAO, lightingShader, model, 0.12f, 0.38f, 0.24f);

    // Right Arm
    model = parent;
    model = glm::translate(model, glm::vec3(3.55f, -0.55f, 6.45f));
    model = glm::rotate(model, glm::radians(-35.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.25f, 0.95f, 0.25f));
    drawCube(cubeVAO, lightingShader, model, 0.12f, 0.38f, 0.24f);

    // Left Hand
    model = parent;
    model = glm::translate(model, glm::vec3(2.42f, -0.70f, 7.15f));
    model = glm::scale(model, glm::vec3(0.28f, 0.18f, 0.35f));
    drawCube(cubeVAO, lightingShader, model, 0.72f, 0.48f, 0.30f);

    // Right Hand
    model = parent;
    model = glm::translate(model, glm::vec3(3.52f, -0.70f, 7.15f));
    model = glm::scale(model, glm::vec3(0.28f, 0.18f, 0.35f));
    drawCube(cubeVAO, lightingShader, model, 0.72f, 0.48f, 0.30f);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (rotateAxis_X)
            rotateAngle_X -= 0.1;
        else if (rotateAxis_Y)
            rotateAngle_Y -= 0.1;
        else
            rotateAngle_Z -= 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        translate_Y += 0.001;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        translate_Y -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        translate_X += 0.001;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        translate_X -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        translate_Z += 0.001;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        translate_Z -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        scale_X += 0.001;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
        scale_X -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        scale_Y += 0.001;
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        scale_Y -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        scale_Z += 0.001;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        scale_Z -= 0.001;

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        rotateAngle_X += 0.1;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        rotateAngle_Y += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 1.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        rotateAngle_Z += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 1.0;
    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        eyeX += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        eyeX -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        eyeZ += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        eyeZ -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    // if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    // {
    //     eyeY += 2.5 * deltaTime;
    //     basic_camera.changeEye(eyeX, eyeY, eyeZ);
    // }
    // if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    // {
    //     eyeY -= 2.5 * deltaTime;
    //     basic_camera.changeEye(eyeX, eyeY, eyeZ);
    // }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    camera.Position.y += 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.Position.y -= 2.5f * deltaTime;

    // Fan ON/OFF - F key
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !fanKeyPressed)
    {
        fanOn = !fanOn;
        fanKeyPressed = true;
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
    {
        fanKeyPressed = false;
    }
    // Projector ON/OFF
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !projectorKeyPressed)
    {
        projectorOn = !projectorOn;
        projectorKeyPressed = true;
    }

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE)
    {
        projectorKeyPressed = false;
    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
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

    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        pointlight1.ambient += 0.1f;
        pointlight2.ambient += 0.1f;
        pointlight3.ambient += 0.1f;
        pointlight4.ambient += 0.1f;
    }

    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        pointlight1.ambient -= 0.1f;
        pointlight2.ambient -= 0.1f;
        pointlight3.ambient -= 0.1f;
        pointlight4.ambient -= 0.1f;
    }

    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        pointlight1.diffuse += 0.1f;
        pointlight2.diffuse += 0.1f;
        pointlight3.diffuse += 0.1f;
        pointlight4.diffuse += 0.1f;
    }

    if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        pointlight1.diffuse -= 0.1f;
        pointlight2.diffuse -= 0.1f;
        pointlight3.diffuse -= 0.1f;
        pointlight4.diffuse -= 0.1f;
    }

    if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {
        pointlight1.specular += 0.1f;
        pointlight2.specular += 0.1f;
        pointlight3.specular += 0.1f;
        pointlight4.specular += 0.1f;
    }

    if (key == GLFW_KEY_7 && action == GLFW_PRESS)
    {
        pointlight1.specular -= 0.1f;
        pointlight2.specular -= 0.1f;
        pointlight3.specular -= 0.1f;
        pointlight4.specular -= 0.1f;
    }

    // Light ON/OFF
    // Light bright / dim - L
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            // Dim light
            pointlight1.diffuse = glm::vec3(0.35f);
            pointlight2.diffuse = glm::vec3(0.35f);
            pointlight3.diffuse = glm::vec3(0.35f);
            pointlight4.diffuse = glm::vec3(0.35f);

            pointlight1.specular = glm::vec3(0.25f);
            pointlight2.specular = glm::vec3(0.25f);
            pointlight3.specular = glm::vec3(0.25f);
            pointlight4.specular = glm::vec3(0.25f);
        }
        else
        {
            // Full light
            pointlight1.diffuse = glm::vec3(0.8f);
            pointlight2.diffuse = glm::vec3(0.8f);
            pointlight3.diffuse = glm::vec3(0.8f);
            pointlight4.diffuse = glm::vec3(0.8f);

            pointlight1.specular = glm::vec3(1.0f);
            pointlight2.specular = glm::vec3(1.0f);
            pointlight3.specular = glm::vec3(1.0f);
            pointlight4.specular = glm::vec3(1.0f);
        }

        pointLightOn = !pointLightOn;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    // Look around only while holding LEFT mouse button
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS)
    {
        firstMouse = true;
        return;
    }

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
        return;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
