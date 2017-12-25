#include "RenderAPI/Material.h"

using namespace std;

Material::Material(shared_ptr<Shader> shader)
{
    this->shader = shader;
}

void Material::SetTexture(string id, std::shared_ptr<Texture> texture)
{
    this->textures.push_back(pair<string, std::shared_ptr<Texture>>(id,texture));
}
void Material::SetMat(string id, glm::mat4 texture)
{
    this->mats.push_back(pair<string, glm::mat4>(id,texture));
}
void Material::SetVec(string id, glm::vec4 texture)
{
    this->vecs.push_back(pair<string, glm::vec4>(id,texture));
}
void Material::SetFloat(string id, float val)
{
    this->floats.push_back(pair<string, float>(id,val));
}
void Material::SetActive () const
{
    if (shader == nullptr)
    {
        return;
    }
	glUseProgram(shader->ID);
    for(int i=0;i<textures.size();i++){
        auto t = textures[i];
        glActiveTexture(GL_TEXTURE0+i);
        glBindTexture(GL_TEXTURE_2D,t.second->id);
        glUniform1i(shader->GetLocationID(t.first),i);
    }
    for(int i=0;i<vecs.size();i++){
        auto t = vecs[i];
        glUniform4fv(shader->GetLocationID(t.first),1,&t.second[0]);
    }
    for(int i=0;i<mats.size();i++){
        auto t = mats[i];
        glUniformMatrix4fv(shader->GetLocationID(t.first),1,GL_FALSE,&t.second[0][0]);
    }
    for(int i=0;i<floats.size();i++){
        auto t = floats[i];
        glUniform1f(shader->GetLocationID(t.first),std::get<float>(t));
    }
	if (blendEnabled) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}

void Material::SetBlendFunc(GLenum src, GLenum dest) {
	this->blendEnabled = true;
	this->blendSrc = src;
	this->blendDst = dest;
}
