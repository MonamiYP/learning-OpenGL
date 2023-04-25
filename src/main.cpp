#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>
#include <fstream>
#include <cmath>

#include "Shader.hpp"
#include "Renderer.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Texture.hpp"
#include "Camera.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xScroll, double yScroll);

Camera camera(glm::vec3(0.0f, 0.0f, 8.0f));

const float WINDOW_WIDTH = 1200.0f;
const float WINDOW_HEIGHT = 800.0f;

float lastX =  WINDOW_WIDTH / 2.0;
float lastY =  WINDOW_HEIGHT / 2.0;

float deltaTime;
float lastTime;

bool guiEnable = false;

int main() {
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window;
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGLLLLLL", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    float cube_vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 pointLightPositions[] = {
	    glm::vec3( 0.7f,  0.2f,  2.0f),
	    glm::vec3( 2.3f, -3.3f, -4.0f),
	    glm::vec3(-4.0f,  2.0f, -12.0f),
	    glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    VertexArray cube_VAO;
    VertexBuffer cube_VBO(cube_vertices, sizeof(cube_vertices));
    VertexBufferLayout layout;
    layout.AddAttribute(3);
    layout.AddAttribute(3);
    layout.AddAttribute(2);
    cube_VAO.AddBuffer(cube_VBO, layout);

    VertexArray lightVAO;
    VertexBufferLayout light_layout;
    light_layout.AddAttribute(3);
    light_layout.IgnoreAttribute(3);
    light_layout.IgnoreAttribute(2);
    lightVAO.AddBuffer(cube_VBO, light_layout);

    Shader shader;
    std::string vertex_source = shader.ParseShader("res/cube.vs");
    std::string fragment_source = shader.ParseShader("res/cube.fs");
    shader.CreateShaderProgram(vertex_source, fragment_source);

    Shader lightShader;
    std::string light_fragment_source = lightShader.ParseShader("res/light.fs");
    lightShader.CreateShaderProgram(vertex_source, light_fragment_source);

    shader.Bind();

    Texture texture("res/assets/container.png");
    Texture specular_texture("res/assets/container_specular.png");
    texture.Bind();
    specular_texture.Bind(1);
    shader.SetInt("material.diffuse", 0);
    shader.SetInt("material.specular", 1);

    Renderer renderer;

    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        processInput(window);
        renderer.Clear();

        // Apply shaders
        shader.Bind();
        glm::vec3 viewPos = camera.GetPosition();
        glm::vec3 viewForwardDir = camera.GetForwards();
        shader.SetVector3("u_viewPos", viewPos);
        shader.SetFloat("material.shininess", 32.0f);

        // directional light
        shader.SetVector3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        shader.SetVector3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        shader.SetVector3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        shader.SetVector3("dirLight.specular", 0.5f, 0.5f, 0.5f);
         	    
        // point light 1
        shader.SetVector3("pointLights[0].position", pointLightPositions[0]);
        shader.SetVector3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        shader.SetVector3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        shader.SetVector3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        shader.SetFloat("pointLights[0].constant", 1.0f);
        shader.SetFloat("pointLights[0].linear", 0.09f);
        shader.SetFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        shader.SetVector3("pointLights[1].position", pointLightPositions[1]);
        shader.SetVector3("pointLights[1].ambient", 0.05f, 0.05f, 0.6f);
        shader.SetVector3("pointLights[1].diffuse", 1.0f, 0.8f, 0.8f);
        shader.SetVector3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        shader.SetFloat("pointLights[1].constant", 1.0f);
        shader.SetFloat("pointLights[1].linear", 0.09f);
        shader.SetFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        shader.SetVector3("pointLights[2].position", pointLightPositions[2]);
        shader.SetVector3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        shader.SetVector3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        shader.SetVector3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        shader.SetFloat("pointLights[2].constant", 1.0f);
        shader.SetFloat("pointLights[2].linear", 0.09f);
        shader.SetFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        shader.SetVector3("pointLights[3].position", pointLightPositions[3]);
        shader.SetVector3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        shader.SetVector3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        shader.SetVector3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        shader.SetFloat("pointLights[3].constant", 1.0f);
        shader.SetFloat("pointLights[3].linear", 0.09f);
        shader.SetFloat("pointLights[3].quadratic", 0.032f);
        // spotLight
        shader.SetVector3("spotLight.position", viewPos);
        shader.SetVector3("spotLight.direction", viewForwardDir);
        shader.SetVector3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        shader.SetVector3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        shader.SetVector3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        shader.SetFloat("spotLight.constant", 1.0f);
        shader.SetFloat("spotLight.linear", 0.09f);
        shader.SetFloat("spotLight.quadratic", 0.032f);
        shader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(10.5f)));
        shader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(12.5f)));

        // View and projection transformations
        glm::mat4 view = camera.GetCameraView();
        glm::mat4 projection = glm::perspective(camera.GetFOV(), WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, 100.0f);
        shader.SetMatrix4("u_view", view);
        shader.SetMatrix4("u_projection", projection);

        // Render cubes
        cube_VAO.Bind();
        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(20.0f*i), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.SetMatrix4("u_model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Render light
        lightVAO.Bind();
        lightShader.Bind();
        lightShader.SetMatrix4("u_view", view);
        lightShader.SetMatrix4("u_projection", projection);
        for (unsigned int i = 0; i < 4; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));

            lightShader.SetMatrix4("u_model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        // ImGui stuff
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (guiEnable) {
            static float f = 0.0f;
            ImGui::Begin("A window");
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

void scroll_callback(GLFWwindow* window, double xScroll, double yScroll) {
    camera.ProcessScrollInput(yScroll);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseInput(xOffset, yOffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (guiEnable == false) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboardInput(FORWARDS, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboardInput(BACKWARDS, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboardInput(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboardInput(RIGHT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera.ProcessKeyboardInput(UP, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camera.ProcessKeyboardInput(DOWN, deltaTime);
        
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
            guiEnable = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursorPosCallback(window, NULL);
            glfwSetScrollCallback(window, NULL);
        }
    } else {
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
            guiEnable = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPosCallback(window, mouse_callback);
            glfwSetScrollCallback(window, scroll_callback);
        }
    }
}