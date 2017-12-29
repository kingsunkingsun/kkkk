#pragma once
#include <ResEngine\Texture.h>
#include <glad\glad.h>

ResEngine::Texture::~Texture() {
	glDeleteBuffers(1, &this->id);
}
