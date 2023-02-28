#pragma once

#include <glad/glad.h>
#include <iostream>
#include <fstream>

class Shader {
    private:
        unsigned int shader_id;
    public:
        Shader();

        void Use();
        unsigned int GetID();

        unsigned int CompileShader(unsigned int type, const std::string& source);
        std::string ParseShader(const std::string& filepath);
        void CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
};