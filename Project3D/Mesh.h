#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace aie { class ShaderProgram; }
//namespace aie { class Texture; }
#include <vector>
#include "Texture.h"

class Mesh
{
public:

    Mesh();
    virtual ~Mesh();

    struct Vertex {
        glm::vec4 position;
        glm::vec4 normal;
        glm::vec2 texCoord;
        glm::vec4 tangent;
    };

    void initialiseQuad();
    void initialise(unsigned int vertexCount, const Vertex* vertices,
                    unsigned int indexCount = 0,
                    unsigned int* indices = nullptr);
    void initialiseFromFile(const char* filename);

    virtual void draw();

    void applyMaterial(aie::ShaderProgram* shader);
    void loadMaterial(const char* fileName);

    void loadTexture(const char* fileName);

    void setMaterial(glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, float ns);
    glm::vec3 getAmbient() { return Ka; }
    glm::vec3 getDiffuse() { return Kd; }
    glm::vec3 getSpecular() { return Ks; }
    float* getSpecularPower() { return &specularPower; }

    void calculateTangents(Vertex* vertices, unsigned int vertexCount, const std::vector<unsigned int>& indices);
    bool getHasNormalMap() { return hasNormalMap; }

protected:

    unsigned int triCount;
    unsigned int vao, vbo, ibo;

    aie::Texture mapKd; // diffuse texture map
    aie::Texture mapKs; // specular texture map
    aie::Texture mapBump; // normal map
    bool hasNormalMap;

    glm::vec3 Ka; // ambient colour of the surface (ambient light reflected - mainly used for parts in shadow)
    glm::vec3 Kd; // diffuse colour of the surface (light reflected)
    glm::vec3 Ks; // specular colour of the surface (light reflected as highlights)
    float specularPower; // tightness (sharpness) of specular highlights

};

