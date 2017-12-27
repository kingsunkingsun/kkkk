#pragma once
#include <ResEngine\IApplication.h>

class OpenGLApplication : public IApplication {
public:
	virtual void Init() override;
	virtual bool Run() override;
	virtual void End() override;
private:
	GLFWwindow* window;
};