#pragma once
#include "SnakeGame.h"
#include "RenderAPI/Graphics.h"
class UISystem : public System {
public:

	SGWorld* world;
	SGWorld::Group<SG::SGCamera> cameras;
	Material fontMat;
	UISystem(SGWorld* world) :cameras(world){
		this->world = world;
		auto textShader = std::make_shared<Shader>("shaders/fontv.glsl", "shaders/fontf.glsl");
		fontMat = Material(textShader);
		fontMat.SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	virtual void Update() override {
		char buf[50];
		sprintf_s(buf, "Score : %d", world->Get<SG::Score>()->score);
		char dbuf[50];
		sprintf_s(dbuf, "Difficulty Level : %d", world->Get<SG::Difficulty>()->level + 1);
		for (auto& ent : cameras) {
			Graphics::DrawTextMesh(buf, *ent.Get<SG::SGCamera>()->pCam, fontMat, glm::translate(glm::mat4(), glm::vec3(0.0f, SCR_HEIGHT - 50.0f, 0)));
			Graphics::DrawTextMesh(dbuf, *ent.Get<SG::SGCamera>()->pCam, fontMat, glm::translate(glm::mat4(), glm::vec3(0.0f, SCR_HEIGHT - 100.0f, 0)));
			if (world->Has<Gameover>()) {
				Graphics::DrawTextMesh("Game Over!", *ent.Get<SG::SGCamera>()->pCam, fontMat, glm::translate(glm::mat4(), glm::vec3(SCR_WIDTH / 2 - 100.0f , SCR_HEIGHT/2, 0)));
				Graphics::DrawTextMesh("Press Space to start again.", *ent.Get<SG::SGCamera>()->pCam, fontMat, glm::translate(glm::mat4(), glm::vec3(SCR_WIDTH / 2 - 230.0f, SCR_HEIGHT / 2 - 50.0f, 0)));
			}
		}
	}
};