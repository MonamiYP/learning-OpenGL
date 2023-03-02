#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.hpp"
#include "Renderer.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window;
    window = glfwCreateWindow(640, 480, "This is a window", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    float vertex[] = {
        // positions        // colors
        0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // top right 0
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // bottom right 1
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // bottom left 2
        -0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f // top left 3
        
    };
    unsigned int indices[] = {
        0, 1, 2, // triangle 1
        1, 2, 3 // triangle 2
    };

    float rect_vertex[] = {
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,// bottom left
        -1.0f, -0.6f, 0.0f,  0.0f, 0.0f, 1.0f,// top left
        -0.6f, -0.6f, 0.0f,  0.0f, 0.0f, 1.0f,// top right
        -0.6f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f// bottom right
    };
    unsigned int rect_indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    VertexArray VAO;
    VertexBuffer VBO(vertex, sizeof(vertex));
    VertexBufferLayout layout;
    layout.AddAttribute(3);
    layout.AddAttribute(3);
    VAO.AddBuffer(VBO, layout);

    IndexBuffer IBO(indices, 6);

    VertexArray VAO2;
    VertexBuffer VBO2(rect_vertex, sizeof(rect_vertex));
    VertexBufferLayout layout2;
    IndexBuffer IBO2(rect_indices, 6);
    layout2.AddAttribute(3);
    layout2.AddAttribute(3);
    VAO2.AddBuffer(VBO2, layout2);

    Shader shader;

    std::string vertex_source = shader.ParseShader("res/Vertex.shader");
    std::string fragment_source = shader.ParseShader("res/Fragment.shader");
    shader.CreateShaderProgram(vertex_source, fragment_source);
    shader.Bind();

    // unsigned int texture;
    // glGenTextures(1, &texture);
    // glBindTexture(GL_TEXTURE_2D, texture);
    // // Set tecture wrapping/filtering options
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // int width, height, nrChannels;
    // unsigned char* image_data = stbi_load("res/assets/container.jpeg", &width, &height, &nrChannels, 0);

    // if (image_data) {
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    //     glGenerateMipmap(GL_TEXTURE_2D);
    // } else {
    //     std::cout << "Failed to load texture" << std::endl;
    // }
    // stbi_image_free(image_data);

    Renderer renderer;
    // Render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        renderer.Clear();

        float timeValue = glfwGetTime();
        float multipleValue = sin(timeValue) / 2.0f + 0.5f;

        shader.SetUniform1f("multiple", multipleValue);

        renderer.Draw(VAO, IBO, shader);
        renderer.Draw(VAO2, IBO2, shader);
        
        glfwSwapBuffers(window); // Double buffer for rendering, front buffer contains final output while rendering happens on the back. Then it is swapped
        glfwPollEvents(); // Checks if any events are triggered
    }

    glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}