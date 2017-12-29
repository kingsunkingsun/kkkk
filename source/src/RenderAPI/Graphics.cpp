#include "RenderAPI/Graphics.h"

using namespace std;
using namespace ResEngine;

std::vector<shared_ptr<Light>> Graphics::lights = std::vector<shared_ptr<Light>>();
Light::Light(glm::vec3 direction,float strength)
{
    this->position = direction;
    this->strength = strength;
}

void Graphics::AddLight(shared_ptr<Light> light)
{
    lights.push_back(light);
}

void Graphics::DrawModelImmediate(const Model& model,const Camera& cam,glm::mat4 trs)
{
    for(int i=0; i < model.meshes.size();i++){
        DrawMeshImmediate(*model.meshes[i],cam,*model.materials[i],trs);
    }
}

void Graphics::DrawMeshImmediate(const Mesh& mesh,const Camera& cam,Material& mat,glm::mat4 trs)
{
	mat.SetActive();
    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = cam.GetViewMatrix();
    mat.SetMat("projection", projection);
    mat.SetMat("view", view);
    // world transformation
    mat.SetMat("model", trs);

    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES,mesh.indices.size(),GL_UNSIGNED_INT,0);
    glBindVertexArray(0);
}

void Graphics::DrawTextMesh(std::string s, const Camera & cam, Material & mat, glm::mat4 trs) {

	mat.SetActive();
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(TextRenderUtil::GetInstance().GetVAO());

	//glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 projection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT);
	glm::mat4 view = cam.GetViewMatrix();
	mat.SetMat("projection", projection);
	mat.SetMat("view", view);
	//mat.SetInt("text", 0);
	for (auto c = s.begin(); c != s.end(); c++) {
		mat.SetMat("model", trs);
		float advance;
		auto tid = TextRenderUtil::GetInstance().GetTextureIDForCharacter(*c, &advance,1.0f);
		glBindTexture(GL_TEXTURE_2D, tid);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		trs = glm::translate(trs, glm::vec3(advance, 0, 0));
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

