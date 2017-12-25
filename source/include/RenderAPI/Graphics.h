#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "Model.h"
#include "TextRenderUtil.h"

class Light
{
public:
    Light(glm::vec3 direction,float strength = 1.0f);
    enum LightType{
        Directional,
        Point,
        Spot
    };
    LightType type;
    glm::vec3 position; //dierction for directional light.
    float range = 1.0f;    //only valid for point/spot light
    float strength = 1.0f;
    glm::vec3 color = {1.0f,1.0f,1.0f};
};

class Graphics
{
public:
    static void AddLight(std::shared_ptr<Light> light);
    static void DrawModelImmediate(const Model& model,const Camera& cam,glm::mat4 trs);
    static void DrawMeshImmediate(const Mesh& mesh,const Camera& cam,const Material& mat,glm::mat4 trs);
	static void DrawTextMesh(std::string s, const Camera& cam, const Material& mat, glm::mat4 trs);
private:
    static std::vector<std::shared_ptr<Light>> lights;
};
#endif