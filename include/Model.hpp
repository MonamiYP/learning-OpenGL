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

class Model {
    public:
        Model(std::string const& path);
        void Draw(Shader& shader);
        
    private:
        std::vector<Mesh> meshes;
        std::string directory;
        std::vector<MeshTexture> loaded_textures;

        void LoadModel(std::string const& path);
        void ProcessNode(aiNode* node, const aiScene* scene);
        Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<MeshTexture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
        unsigned int TextureFromFile(const char* path, const std::string& directory);
};