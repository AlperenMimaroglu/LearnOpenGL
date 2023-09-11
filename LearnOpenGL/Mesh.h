#pragma once
#include <vector>

#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"

class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    void Draw(Shader &shader);

private:
    unsigned int VAO, VBO, EBO;

    void SetupMesh();
};
