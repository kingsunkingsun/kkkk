#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <memory>

#include "Mesh.h"
#include "Shader.h"
#include "Material.h"

class Model 
{
public:
    std::vector<std::shared_ptr<Texture>> textures_loaded;
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::vector<std::shared_ptr<Material>> materials;
	std::string directory;
    bool gammaCorrection;
    Model(std::string const &path, bool gamma = false);
    void AssignShader(std::shared_ptr<Shader> pShader);
    
private:
    using pTex = std::shared_ptr<Texture>;
};

#endif