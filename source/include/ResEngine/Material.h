#pragma once
#include <ResEngine\ResourceManagement.h>
#include <ResEngine\Serialization.h>
#include <ResEngine\TextAsset.h>
#include <ResEngine\Texture.h>
#include <glm\glm.hpp>
#include <glad\glad.h>

namespace ResEngine {
	using namespace ResourceManagement;
	class Material : public Resource<Material>{
	public:
		Material();
		~Material();
		ResourceHandle<TextResource> vertexShader;
		ResourceHandle<TextResource> fragmentShader;
		void SetTexture(std::string id, ResourceHandle<Texture> texture);
		void SetMat(std::string id, glm::mat4 texture);
		void SetVec(std::string id, glm::vec4 texture);
		void SetFloat(std::string id, float val);
		void SetActive() const;
		void SetBlendFunc(GLenum src, GLenum dest);

	private:
		void UpdateProgram();
		bool dirty = true;
		unsigned int programID = -1;

		GLint GetLocationID(const std::string & name) const;
		std::vector<std::pair<std::string, ResourceHandle<Texture>>> textures;
		std::vector<std::pair<std::string, glm::vec4>> vecs;
		std::vector<std::pair<std::string, float>> floats;
		bool blendEnabled;
		GLenum blendSrc;
		GLenum blendDst;
	
	public:
		
		template <typename Archive>
		void SerializeFunc(Archive& arc){
			std::unordered_map<std::string, ResourceHandle<Texture>> serializedTextures;
			std::unordered_map<std::string, glm::vec4> serializedvecs;
			std::unordered_map<std::string, float> serializedfloats;
			for (auto& t : textures) {
				serializedTextures[t.first] = t.second;
			}
			for (auto& t : vecs) {
				serializedvecs[t.first] = t.second;
			}
			for (auto& t : floats) {
				serializedfloats[t.first] = t.second;
			}
			arc("vertexShader", vertexShader);
			arc("fragmentShader", fragmentShader);
			arc("textures", serializedTextures);
			arc("vecs", serializedvecs);
			arc("floats", serializedfloats);

			textures.clear();
			for (auto& t : serializedTextures) {
				SetTexture(t.first, t.second);
			}

			vecs.clear();
			for (auto& t : serializedvecs) {
				SetVec(t.first, t.second);
			}

			floats.clear();
			for (auto& t : serializedfloats) {
				SetFloat(t.first, t.second);
			}

		}
	};

	class MaterialImporter : public AbstractImporter<Material> {
	public:

		virtual BaseResource* Import(std::string path) override {
			
		}
	};
}