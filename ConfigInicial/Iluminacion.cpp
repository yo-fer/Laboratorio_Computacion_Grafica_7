/*
* Práctica 8
* Fecha de entrega: 12 de octubre de 2025
* Fernando Pérez Cruz
* 422022569
*/


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
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;


// ==== Sun attributes ====
glm::vec3 sunPos(0.0f, 0.0f, 0.0f);
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
bool activanim = false;

float sun_radius = 12.0f;
float sun_height_radius = 5.0f; 
float moveSunPos = 0.0f;


// ==== Moon attributes ====
glm::vec3 moonPos(0.0f, 0.0f, 0.0f);
float moon_radius = 12.0f;
float moon_height_radius = 5.0f; 
float moveMoonPos = 0.0f;

// ==== Control de día/noche ====
bool isDay = true;

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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 8 Fernando Perez", nullptr, nullptr);

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
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");

    // Load models
    Model dog((char*)"Models/RedDog.obj");
    Model forest((char*)"Models/forest.obj");
    Model arlo((char*)"Models/arlo.obj");
    Model trex((char*)"Models/dino2.obj");
    Model mesaPicnic((char*)"Models/mesapicnic.obj");
    Model fence((char*)"Models/fence.obj");
    Model agave((char*)"Models/agave.obj");

    // === Sun and Moon ===
    Model sun((char*)"Models/sun.obj");
    Model moon((char*)"Models/moon.obj");

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    float vertices[] = {
                              // hacia donde apunta el vector normal
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

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Load textures

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    image = stbi_load("Models/Texture_albedo.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
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
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // ==== Calculando la posición del sol y la luna ====

        float angle_degrees = glm::degrees(moveSunPos);
        float clamped_angle = glm::clamp(angle_degrees, 0.0f, 180.0f);
        float clamped_radians = glm::radians(clamped_angle);

        sunPos.x = sun_radius * glm::cos(clamped_radians);
        sunPos.y = sun_height_radius * glm::sin(clamped_radians); 
        sunPos.z = -10.0f;

        float moon_angle_degrees = glm::degrees(moveMoonPos);
        float moon_clamped_angle = glm::clamp(moon_angle_degrees, 0.0f, 180.0f);
        float moon_clamped_radians = glm::radians(moon_clamped_angle);

        moonPos.x = -5.0f; 
        moonPos.y = moon_height_radius * glm::sin(moon_clamped_radians);
        moonPos.z = moon_radius * glm::cos(moon_clamped_radians);

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // ==== Propiedades para las luces y color de fondo ====
        glm::vec3 sun_ambient, sun_diffuse, sun_specular;
        glm::vec3 moon_ambient, moon_diffuse, moon_specular;
        glm::vec4 clearColor;

        // Para el día
        if (isDay) {
            clearColor = glm::vec4(0.53f, 0.81f, 0.98f, 1.0f);

            sun_ambient = glm::vec3(0.5f, 0.5f, 0.4f);  
            sun_diffuse = glm::vec3(1.0f, 1.0f, 0.9f);  
            sun_specular = glm::vec3(1.0f, 1.0f, 1.0f);

            moon_ambient = glm::vec3(0.0f, 0.0f, 0.0f);
            moon_diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
            moon_specular = glm::vec3(0.0f, 0.0f, 0.0f);
        }
        // Para la noche
        else {
            clearColor = glm::vec4(0.05f, 0.05f, 0.15f, 1.0f); 

            sun_ambient = glm::vec3(0.0f, 0.0f, 0.0f);
            sun_diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
            sun_specular = glm::vec3(0.0f, 0.0f, 0.0f);

            moon_ambient = glm::vec3(0.1f, 0.1f, 0.2f);
            moon_diffuse = glm::vec3(0.3f, 0.3f, 0.4f);
            moon_specular = glm::vec3(0.4f, 0.4f, 0.5f);
        }


        // Clear the colorbuffer
        glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        lightingShader.Use();

        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Luces usadas
        glUniform1i(glGetUniformLocation(lightingShader.Program, "numLights"), 2);

        //glm::vec3 L1 = sunPos + glm::vec3(moveSunPos);
        //glUniform3fv(glGetUniformLocation(lightingShader.Program, "lights[0].position"), 1, glm::value_ptr(L1));
        glm::vec3 L1 = sunPos;
        glUniform3fv(glGetUniformLocation(lightingShader.Program, "lights[0].position"), 1, glm::value_ptr(L1));
        glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].ambient"), sun_ambient.x, sun_ambient.y, sun_ambient.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].diffuse"), sun_diffuse.x, sun_diffuse.y, sun_diffuse.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].specular"), sun_specular.x, sun_specular.y, sun_specular.z);

        //glm::vec3 L2 = moonPos + glm::vec3(moveMoonPos);
        //glUniform3fv(glGetUniformLocation(lightingShader.Program, "lights[1].position"), 1, glm::value_ptr(L2));
        glm::vec3 L2 = moonPos; // Simplemente usa la nueva posición
        glUniform3fv(glGetUniformLocation(lightingShader.Program, "lights[1].position"), 1, glm::value_ptr(L2));
        glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[1].ambient"), moon_ambient.x, moon_ambient.y, moon_ambient.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[1].diffuse"), moon_diffuse.x, moon_diffuse.y, moon_diffuse.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[1].specular"), moon_specular.x, moon_specular.y, moon_specular.z);

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Set material properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.7f, 0.2f, 0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.6f, 0.6f, 0.6f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);


        // ==== Cargando escenario ====
        
        // Drawing forest
        glm::mat4 model(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        forest.Draw(lightingShader);

        // Arlo the dinosaur
        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(12.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arlo.Draw(lightingShader);

        // Another dino
        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        trex.Draw(lightingShader);

        // Our dog
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 0.6f, 2.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(lightingShader);

        // Picnic table
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        mesaPicnic.Draw(lightingShader);

        // Some fences
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        fence.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(4.5f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        fence.Draw(lightingShader);

        // Some plants
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        agave.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        agave.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        agave.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.0f, 0.0f, -1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        agave.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-5.0f, 0.0f, -1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        agave.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-5.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        agave.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-4.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        agave.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-4.0f, 0.0f, -1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        agave.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-4.0f, 0.0f, -2.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        agave.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-3.0f, 0.0f, -2.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        agave.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-2.0f, 0.0f, -2.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        agave.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -2.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        agave.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-0.0f, 0.0f, -2.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        agave.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.0f, 0.0f, -2.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        agave.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-5.0f, 0.0f, -2.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        agave.Draw(lightingShader);

        // ==== Termina carga de escenario

        /*
        // Draw the loaded model
        glm::mat4 model(1);
        model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
       
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        // cargando modelo
        red_dog.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        dino.Draw(lightingShader);

        */

        glBindVertexArray(0);


        lampshader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // ==== Dibujo del Sol y la Luna ====
        if ( isDay ) {
            // Lámpara de la luz 1 (Sol)
            glm::mat4 lampM = glm::mat4(1.0f);
            lampM = glm::translate(lampM, L1);
            lampM = glm::scale(lampM, glm::vec3(0.5f));
            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(lampM));
            
            sun.Draw(lampshader);
        }
        else {
            // Lámpara de la luz 2 (Luna)
            glm::mat4 lampM = glm::mat4(1.0f);
            lampM = glm::translate(lampM, L2);
            lampM = glm::scale(lampM, glm::vec3(0.5f));
            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(lampM));
            

            moon.Draw(lampshader); 
        }

        /* ==== Luces en cubos ====
        // Lámpara de la luz 1
        glm::mat4 lampM = glm::mat4(1.0f);
        lampM = glm::translate(lampM, L1);
        lampM = glm::scale(lampM, glm::vec3(0.2f));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(lampM));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Lámpara de la luz 2
        lampM = glm::mat4(1.0f);
        lampM = glm::translate(lampM, L2);
        lampM = glm::scale(lampM, glm::vec3(0.2f));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(lampM));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        */

        glBindVertexArray(0);

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}


// Moves/alters the camera positions based on user input
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

    if (activanim)
    {
        if (rot > -90.0f)
            rot -= 0.1f;
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

    if (keys[GLFW_KEY_O])
    {  
        //moveSunPos += 0.1f;
        if (isDay)
            moveSunPos += glm::radians(1.0f);
        else
            moveMoonPos += glm::radians(1.0f);

    }

    if (keys[GLFW_KEY_L])
    {
        
        //moveSunPos -= 0.1f;
        if (isDay)
            moveSunPos -= glm::radians(1.0f);
        else
            moveMoonPos -= glm::radians(1.0f);

    }

    // ==== Alternamos entre día y noche con la tecla P ===
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        isDay = !isDay;
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


