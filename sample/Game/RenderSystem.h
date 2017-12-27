#pragma once
#include "SnakeGame.h"
#include "RenderAPI/Graphics.h"
#include <glm/glm.hpp>

using namespace SG;

class RenderSystem : public System {
public:
    SGWorld* world;
	SGWorld::Group<Transform, SGCamera> cameras;
	SGWorld::Group<Transform, MeshRenderer> renderers;
    RenderSystem(SGWorld* world):
		cameras(world),
		renderers(world)
    {
        this->world = world;
    }

    virtual void Update() override {
		for(auto& camEnt : cameras) {
			for (auto& rendEnt : renderers) {
				auto translate = glm::translate(glm::mat4(), rendEnt.Get<Transform>()->position);
				auto translateScale = glm::scale(translate, rendEnt.Get<Transform>()->scale);
				Graphics::DrawMeshImmediate(*rendEnt.Get<MeshRenderer>()->mesh, *camEnt.Get<SGCamera>()->pCam, *rendEnt.Get<MeshRenderer>()->material, translateScale);
			}
		}
    }
};