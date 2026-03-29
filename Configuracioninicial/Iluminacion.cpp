//319057399
//Atzin Ugalde Santos
//29 de marzo del 2026
//Practica 8
//Materiales e iluminación


// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

//agregando fuente de luz
glm::vec3 lightPos(0.0f, 3.0f, 0.0f);   
bool isDayMode = true;                      // true = dia, false = noche



int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Materiales e iluminacion-Atzin", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    //shader de iluminacion para los modelos
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");   
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");               

    // Load models
    Model dog((char*)"Models/RedDog.obj");
    Model cake((char*)"Models/cake.obj");
    Model table((char*)"Models/Table.obj");
    Model curtain((char*)"Models/uploads_files_4180038_pinch+pleat+curtain.obj");
    Model hat((char*)"Models/Party Hat Package.obj");
    Model fox((char*)"Models/Fox.obj");

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);


    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

 //configuracion de la lampara VBO, VAO
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
  
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();


        if (isDayMode)
            glClearColor(0.53f, 0.70f, 0.90f, 1.0f); // cielo azul claro (dia)
        else
            glClearColor(0.02f, 0.02f, 0.10f, 1.0f); // azul muy oscuro (noche)

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.GetViewMatrix();

        lightingShader.Use();

        // Posicion de la camara (para calculo especular)
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Matrices de transformacion
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Posicion de la luz: siempre fija en lightPos (0, 3, 0)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].position"),
            lightPos.x, lightPos.y, lightPos.z);

        if (isDayMode)
        {
            // --- DIA: colores calidos naranja/amarillo ---
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].ambient"), 0.40f, 0.30f, 0.10f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].diffuse"), 0.95f, 0.70f, 0.20f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].specular"), 1.00f, 0.85f, 0.30f);
        }
        else
        {
            // --- NOCHE: colores frios azul oscuro ---
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].ambient"), 0.00f, 0.05f, 0.15f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].diffuse"), 0.05f, 0.20f, 0.70f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].specular"), 0.10f, 0.30f, 1.00f);
        }

        // Propiedades del material de los modelos
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
        // =========================================================


        // Draw the loaded model
   
        //---- Perro ----
        glm::mat4 model(1);
        model = glm::translate(model, glm::vec3(0.0f, -0.8f, -2.5f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(lightingShader);

        // ---- Pastel ----
        glm::mat4 modelCake(1);
        modelCake = glm::translate(modelCake, glm::vec3(0.6f, -0.9f, -1.2f));
        modelCake = glm::scale(modelCake, glm::vec3(0.004f, 0.004f, 0.004f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCake));
        cake.Draw(lightingShader);

        // -----Mesa ----
        glm::mat4 modeltable(1);
        modeltable = glm::translate(modeltable, glm::vec3(0.0f, -2.0f, 0.0f));
        modeltable = glm::scale(modeltable, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modeltable));
        table.Draw(lightingShader);

        // ---- cortina -----
        glm::mat4 modelCurtain(1);
        modelCurtain = glm::translate(modelCurtain, glm::vec3(1.4f, -2.6f, -3.0f));
        modelCurtain = glm::scale(modelCurtain, glm::vec3(3.3f, 3.3f, 3.3f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCurtain));
        curtain.Draw(lightingShader);

        //---- sombrerito de fiesta 1 ----
        glm::mat4 modelHat(1);
        modelHat = glm::translate(modelHat, glm::vec3(1.6f, -1.5f, -1.4f));
        modelHat = glm::scale(modelHat, glm::vec3(0.04f, 0.04f, 0.04f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelHat));
        hat.Draw(lightingShader);

        //---- sombrerito de fiesta 2 ----
        glm::mat4 modelHat2(1);
        modelHat2 = glm::translate(modelHat2, glm::vec3(0.0f, -1.1f, -2.0f));
        modelHat2 = glm::scale(modelHat2, glm::vec3(0.03f, 0.03f, 0.03f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelHat2));
        hat.Draw(lightingShader);

        // ---- Zorro ----
        glm::mat4 modelFox(1);
        modelFox = glm::translate(modelFox, glm::vec3(1.9f, -1.4f, -1.4f));
        modelFox = glm::scale(modelFox, glm::vec3(1.2f, 1.2f, 1.2f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelFox));
        fox.Draw(lightingShader);

        // =========================================================
        // === AGREGADO: Cubo visual que representa la unica luz  ===
        // === estatica encima del escenario                      ===
        // =========================================================
        lampShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.3f));
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        // =========================================================

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    // AGREGADO: Limpiar VAO y VBO al terminar
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}


// Moves/alters the camera positions based on user input
// === SIN CAMBIOS ===
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }

    // === AGREGADO: Tecla N para alternar entre dia y noche  ===

    if (key == GLFW_KEY_N && action == GLFW_PRESS)
    {
        isDayMode = !isDayMode;
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
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}
