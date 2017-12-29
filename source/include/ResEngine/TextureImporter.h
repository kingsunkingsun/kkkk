#pragma once
#include <ResEngine\ResourceManagement.h>
#include <ResEngine\Texture.h>
#include <glad\glad.h>

namespace ResEngine {
	namespace Importers {

	class TextureImporter : public AbstractImporter<Texture> {
	public:
		virtual BaseResource * Import(std::string path) override;
	private:
	};
	}
}