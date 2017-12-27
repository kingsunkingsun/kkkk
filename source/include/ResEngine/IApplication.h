#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class IApplication {
public:
	virtual void Init() {};
	virtual bool Run() { return false; };
	virtual void End() {};
};
