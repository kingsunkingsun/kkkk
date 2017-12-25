#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h> // holds all OpenGL type declarations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <iostream>
#include <memory>

class Texture {
public:
    unsigned int id;
	std::string path;
    Texture(const char *path, bool gamma);
    ~Texture();
    static std::shared_ptr<Texture> Create(const char* path,bool gamma);
};

#endif