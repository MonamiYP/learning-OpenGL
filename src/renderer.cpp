#include "Renderer.hpp"

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL_Error] (" << error << "): " << function
        << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Draw(const VertexArray& VAO, const IndexBuffer& IBO, const Shader& shader) const {
    shader.Bind();
    VAO.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, IBO.GetCount(), GL_UNSIGNED_INT, 0));
}

void Renderer::Clear() const {
    GLCall(glClearColor(0.2f, 0.4f, 0.7f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}