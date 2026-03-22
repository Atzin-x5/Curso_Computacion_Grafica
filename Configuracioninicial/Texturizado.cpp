//Ugalde Santos Atzin
//319057399
//Practica 7 Texturizado
//22/03/2026

#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"
#include "Camera.h"


// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Cubo-Dado-Atzin", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    // CUBO: 6 caras, 4 vertices cada una = 24 vertices en total

    GLfloat vertices[] =
    {
        // ---- CARA FRONTAL 
        
        -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.25f, 0.333f,  // 0: inf-izq
         0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.50f, 0.333f,  // 1: inf-der
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.50f, 0.667f,  // 2: sup-der
        -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.25f, 0.667f,  // 3: sup-izq

        // ---- CARA TRASERA 
       
         0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.75f, 0.333f,  // 4
        -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.00f, 0.333f,  // 5
        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.00f, 0.667f,  // 6
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.75f, 0.667f,  // 7

         // ---- CARA IZQUIERDA 
        
         -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.00f, 0.333f,  // 8
         -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.25f, 0.333f,  // 9
         -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.25f, 0.667f,  // 10
         -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.00f, 0.667f,  // 11

         // ---- CARA DERECHA 
         
          0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.50f, 0.333f,  // 12
          0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.75f, 0.333f,  // 13
          0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.75f, 0.667f,  // 14
          0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.50f, 0.667f,  // 15

          // ---- CARA SUPERIOR
          
          -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.25f, 0.667f,  // 16
           0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.50f, 0.667f,  // 17
           0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.50f, 1.00f,   // 18
          -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.25f, 1.00f,   // 19

          // ---- CARA INFERIOR 
         
          -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.25f, 0.000f,  // 20
           0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.50f, 0.000f,  // 21
           0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.50f, 0.333f,  // 22
          -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.25f, 0.333f,  // 23
    };

    // 6 caras x 2 triangulos x 3 indices = 36 indices
    GLuint indices[] =
    {
        // Frontal
        0, 1, 3,   1, 2, 3,
        // Trasera
        4, 5, 7,   5, 6, 7,
        // Izquierda
        8, 9, 11,  9, 10, 11,
        // Derecha
        12, 13, 15, 13, 14, 15,
        // Superior
        16, 17, 19, 17, 18, 19,
        // Inferior
        20, 21, 23, 21, 22, 23,
    };

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Atributo: Posicion (location 0) — 3 floats
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Atributo: Color (location 1) — 3 floats
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Atributo: Coordenadas de textura (location 2) — 2 floats
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // Cargar textura
    GLuint texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Esta linea ya esta correcta segun lo indicado
    image = stbi_load("images/paisaje.png", &textureWidth, &textureHeight, &nrChannels, 0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    if (image)
    {
        // Detectar si la imagen tiene canal alfa (RGBA) o no (RGB)
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(image);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lampShader.Use();

        // Transformaciones de camara
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera.GetZoom(),
            (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT,
            0.1f, 100.0f);
        glm::mat4 model(1);

        GLint modelLoc = glGetUniformLocation(lampShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lampShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lampShader.Program, "projection");

        // Vincular textura a la unidad 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(lampShader.Program, "ourTexture"), 0);

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // Dibujar el cubo (36 indices = 12 triangulos = 6 caras)
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();

    return 0;
}

void DoMovement()
{
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)   keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}
