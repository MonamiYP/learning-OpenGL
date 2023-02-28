#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

static std::string ParseShader(const std::string& filepath) {
    std::ifstream file(filepath);
    std::string str;
    std::string content;

    while (std::getline(file, str)) {
        content.append(str + "\n");
    }

    return content;
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // Returns pointer to data in std::string
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    // Compiler error handling
    int success;
    char infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER:"<<
        (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT")
        <<":COMPILATION_FAILED\n" << infoLog << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vertex_shader = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fragment_shader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

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

    float vertex_positions[] = {
        // positions        //colors
        0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // top right 0
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // bottom right 1
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // bottom left 2
        -0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f // top left 3
    };
    unsigned int indices[] = {
        0, 1, 2, // triangle 1
        1, 2, 3 // triangle 2
    };
    // Vertex buffer (VBO) is a blob of memory (in GPU), in this case it stores vertex data
    unsigned int VBO, VAO, EBO; // Vertex buffer object, vertex attribute object, entity buffer object
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Select buffer (and bind it to array buffer)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW); // Specify buffer data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Need to tell OpenGL how the memory is laid out
    // Which buffer it takes data from is determined by buffer currently bound to GL_ARRAY_BUFFER
    // Location, size, type, normalised?, space between vertices, offset
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    std::string vertex_source = ParseShader("resources/Vertex.shader");
    std::string fragment_source = ParseShader("resources/Fragment.shader");
    unsigned int shader = CreateShaderProgram(vertex_source, fragment_source);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.4f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        float timeValue = glfwGetTime();
        float multipleValue = sin(timeValue) / 2.0f + 0.5f;
        int vertexMultipleLocation = glGetUniformLocation(shader, "multiple");
        glUniform1f(vertexMultipleLocation, multipleValue);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window); // Double buffer for rendering, front buffer contains final output while rendering happens on the back. Then it is swapped
        glfwPollEvents(); // Checks if any events are triggered
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader);

    glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}