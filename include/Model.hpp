#pragma once

#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"
#include "Shader.hpp"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model {
    public:
        Model(std::string const& path);
        void Draw(Shader& shader);
        
    private:
        std::vector<Mesh> meshes;
        std::string directory;
        std::vector<Texture> loaded_textures;

        void LoadModel(std::string const& path);
        void ProcessNode(aiNode* node, const aiScene* scene);
        Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma) {
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, BPP;
    unsigned char* textureData = stbi_load(filename.c_str(), &width, &height, &BPP, 0);
    if (textureData) {
            GLenum format;
            if (BPP == 1) {format = GL_RED;}
            else if (BPP == 3) {format = GL_RGB;}
            else {format = GL_RGBA;}
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load texture" << std::endl;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(textureData);

    return textureID;
}