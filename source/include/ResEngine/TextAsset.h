#pragma once
#include <ResEngine\ResourceManagement.h>
#include <sstream>
#include <fstream>
#include <string>
namespace ResEngine {
	using namespace ResourceManagement;
	class TextResource : public Resource<TextResource> {
	public:
		TextResource(std::string text) {
			this->text = text;
		}
		std::string Text() {
			return text;
		}
	private:
		std::string text;
	}; 
	class TextImporter : public AbstractImporter<TextResource> {
		virtual BaseResource* Import(std::string path) override {
			std::ifstream t(path);
			std::stringstream sstring;
			sstring << t.rdbuf();
			return new TextResource(sstring.str());
		}
	};
}