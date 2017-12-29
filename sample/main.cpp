
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <memory>
#include <type_traits>
#include <stack>
#include <functional>

#include "RenderAPI/Shader.h"
#include "RenderAPI/Camera.h"
#include "RenderAPI/Graphics.h"
#include "RenderAPI/Model.h"
#include "Game/SnakeGame.h"
#include "Game/SnakeGameSystems.h"

#include <ResEngine\OpenGLApplication.h>
/*
加入多重光源。
加入Configurable Rendering pipeline。

*/
using namespace std;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

// timing

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

//game world
SGWorld tWorld;


#if 0
void TestFunc1(int t) {
	cout << "Func1" << endl;
}
void TestFunc2(int t,string sth) {
	cout << "Func2" << endl;
}

int main(){


	int a = 1, b = 2, c = 3;
	Serialization::StringWriter writer;
	Serialization::Serialize(2, writer);

	Serialization::StringReader reader(writer.DebugSTring());

	reader.Read(&a, 4);
	cout << a << endl;
    system("pause");
}
#else
int main()
{
	OpenGLApplication app;
	app.Init();
	while (!app.Run()) {

	}
	app.End();
	return 0;
}
#endif

