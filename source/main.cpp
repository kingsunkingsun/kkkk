#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
#include "RenderAPI/Material.h"
#include "Game/SnakeGame.h"
#include "Game/SnakeGameSystems.h"

/*
加入多重光源。
加入Configurable Rendering pipeline。

*/
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
	
	using TestWorldType = World<Transform, Renderer, SG::SnakeHead, SG::SnakeBody>;

    TestWorldType tWorld;

	TestWorldType::Group<Transform> group(&tWorld);
	TestWorldType::Group<Renderer> group2(&tWorld);

	for (auto& test : group) {
		cout << test.ToString() << endl;
	}
	cout << "Create Entity 0" << endl;
    auto t = tWorld.CreateEntity();
    t.Set<Renderer>();
    t.Set<Transform>();

	cout << "Create Entity 1" << endl;
    auto t2 = tWorld.CreateEntity();
    t2.Set<Renderer>();
    t2.Set<Transform>();

	cout << "Create Entity 2" << endl;
    auto t3 = tWorld.CreateEntity();
    t3.Set<Renderer>();
    t3.Set<Transform>();

	for (auto& test : group) {
		cout << test.ToString() << endl;
	}

	cout << "Remove Entity 1's Transform" << endl;
    t2.Remove<Transform>();

	cout << "Group1" << endl;
	for (auto& test : group) {
		cout << test.ToString() << endl;
	}

	cout << "Group2" << endl;
	for (auto& test : group2) {
		cout << test.ToString() << endl;
	}

	cout << "Destroy Entity 0" << endl;
	t.Destroy();

	cout << "Group1" << endl;
	for (auto& test : group) {
		cout << test.ToString() << endl;
	}

	cout << "Group2" << endl;
	for (auto& test : group2) {
		cout << test.ToString() << endl;
	}

    tWorld.Each<Transform,Renderer>([](auto ent,Transform* trans,Renderer* rend){
        cout << trans->position.x << trans->position.y << trans->position.z << endl;
    });
    
	using TestDelegate = Signal<int>;	
	auto testEvent = new TestDelegate();
	if (1) {
		if (1) {
			auto testCon1 = testEvent->Connect(&TestFunc1);
			auto testCon2 = testEvent->Connect(std::bind(TestFunc2 ,std::placeholders::_1, "arg"));
			auto testCon3 = testEvent->Connect(&TestFunc1);
			delete testEvent;
		}
	}

    system("pause");
}
#else
int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Recs-Sample", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
   // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    auto lightingShader = std::shared_ptr<Shader>(new Shader("./shaders/colorv.glsl", "./shaders/colorf.glsl"));

    auto pLight = make_shared<Light>(glm::vec3());
    Graphics::AddLight(pLight);

    auto camEntity = tWorld.CreateEntity();
    camEntity.Set<SGCamera>();
    camEntity.Set<Transform>();
    camEntity.Get<SGCamera>()->pCam = &camera;

    Feature allSys;
	allSys.systems = {
		make_shared<RenderSystem>(&tWorld),
		make_shared<MoveSystem>(&tWorld),
		make_shared<SnakeBodyFollowSystem>(&tWorld),
		make_shared<PlayerControlSystem>(&tWorld),
		make_shared<GameStartSystem>(&tWorld),
		make_shared<SnakeGrowSystem>(&tWorld),
		make_shared<UISystem>(&tWorld)
    }; 
    allSys.Start();

	tWorld.Set<StartGame>();
	tWorld.Set<Time>();
	tWorld.Get<Time>()->time = 0;
	tWorld.Get<Time>()->dt = 0;

	// render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
		tWorld.Get<Time>()->dt = min(deltaTime,0.333f);
		tWorld.Get<Time>()->time += deltaTime;
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        lightPos = glm::vec3(glm::sin(currentFrame),1.0f,glm::cos(currentFrame));
        pLight->position = lightPos;

        allSys.Update();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
#endif

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float hor = 0;
    float vert = 0;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        vert ++;
//        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        vert --;
 //       camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        hor --;
 //       camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        hor ++;
  //      camera.ProcessKeyboard(RIGHT, deltaTime);
    } 

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && tWorld.Has<Gameover>()) {
		tWorld.Set<StartGame>();
	}
	tWorld.Set<SG::Input>(hor, vert);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
    }

    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos; // reversed since y-coordinates go from bottom to top

    lastX = (float)xpos;
    lastY = (float)ypos;

  //  camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll((float)yoffset);
}