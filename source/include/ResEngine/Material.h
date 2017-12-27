#pragma once
#include <ResEngine\ResourceManagement.h>
#include <ResEngine\Serialization.h>

namespace ResEngine {
	using namespace ResourceManagement;
	class Material : public Resource<Material>{
	public:
		Material(std::shared_ptr<Shader> shader = nullptr);
		std::shared_ptr<Shader> shader;
		void SetTexture(std::string id, std::shared_ptr<Texture> texture);
		void SetMat(std::string id, glm::mat4 texture);
		void SetVec(std::string id, glm::vec4 texture);
		void SetFloat(std::string id, float val);
		void SetActive() const;
		void SetBlendFunc(GLenum src, GLenum dest);

	private:
		std::vector<std::pair<std::string, std::shared_ptr<Texture>>> textures;
		std::vector<std::pair<std::string, glm::vec4>> vecs;
		std::vector<std::pair<std::string, float>> floats;
		std::vector<std::pair<std::string, glm::mat4>> mats;
		bool blendEnabled;
		GLenum blendSrc;
		GLenum blendDst;
	};

	class MaterialImporter : public AbstractImporter<Material> {
	public:

		virtual BaseResource* Import(std::string path) override {

		}
	};
}