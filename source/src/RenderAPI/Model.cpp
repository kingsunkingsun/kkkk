#include "RenderAPI/Model.h"

#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "RenderAPI/Mesh.h"
#include "RenderAPI/Shader.h"
#include "ResEngine\Material.h"
#include "ResEngine\Texture.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;
using namespace ResEngine;

using pMesh = shared_ptr<Mesh>;
using pTex = shared_ptr<Texture>;
using pMat = shared_ptr<Material>;

Model::Model(string const &path, bool gamma) : gammaCorrection(gamma)
{ 
}

void Model::AssignShader(shared_ptr<Shader> pShader){
    for(int i=0; i < materials.size();i++){
   //    materials[i]->shader = pShader;
    }
}

