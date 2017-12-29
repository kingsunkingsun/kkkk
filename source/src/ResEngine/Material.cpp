#include <ResEngine\Material.h>
#include <ResEngine\TextAsset.h>
#include <utility>

using namespace ResEngine;
using namespace std;

Material::Material() {
	dirty = true;
}

Material::~Material() {
	if (this->programID != -1)
		glDeleteProgram(this->programID);
}

void Material::SetTexture(string id, ResourceHandle<Texture> texture) {
	this->textures.push_back(pair<string, ResourceHandle<Texture>>(id, texture));
}

void Material::SetVec(string id, glm::vec4 texture) {
	this->vecs.push_back(pair<string, glm::vec4>(id, texture));
}

void Material::SetFloat(string id, float val) {
	this->floats.push_back(pair<string, float>(id, val));
}

void Material::SetActive() const {
	if (this->programID == -1) {
		return;
	}

	glUseProgram(this->programID);
	for (int i = 0; i<textures.size(); i++) {
		auto t = textures[i];
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, t.second->GetID());
		glUniform1i(GetLocationID(t.first), i);
	}
	for (int i = 0; i<vecs.size(); i++) {
		auto t = vecs[i];
		glUniform4fv(GetLocationID(t.first), 1, &t.second[0]);
	}
	for (int i = 0; i<floats.size(); i++) {
		auto t = floats[i];
		glUniform1f(GetLocationID(t.first), std::get<float>(t));
	}
	if (blendEnabled) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}

GLint Material::GetLocationID(const std::string &name) const {
	return glGetUniformLocation(this->programID, name.c_str());
}

void Material::SetBlendFunc(GLenum src, GLenum dest) {
	this->blendEnabled = true;
	this->blendSrc = src;
	this->blendDst = dest;
}

void ResEngine::Material::UpdateProgram() {
	if (this->programID != -1) {
		glDeleteProgram(this->programID);
	}

	//compile shaders.
	auto vcode = vertexShader->Text().c_str();
	auto fcode = fragmentShader->Text().c_str();

	unsigned int vertex, fragment;

	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vcode, NULL);
	glCompileShader(vertex);
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fcode, NULL);
	glCompileShader(fragment);

	// shader Program
	this->programID = glCreateProgram();
	glAttachShader(this->programID, vertex);
	glAttachShader(this->programID, fragment);
	glLinkProgram(this->programID);
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	dirty = false;
}
