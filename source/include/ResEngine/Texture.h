#pragma once
#include <ResEngine\ResourceManagement.h>
#include <ResEngine\Serialization.h>
#include <ResEngine\TextAsset.h>

namespace ResEngine {
	using namespace ResourceManagement;
	class Texture : public Resource<Texture> {
		class TextureImporter;
		friend TextureImporter;
	public:
		Texture(unsigned int id) : id(id) {

		}
		unsigned int GetID() {
			return id;
		}
		~Texture();
	private:
		unsigned int id;
	};

	
}