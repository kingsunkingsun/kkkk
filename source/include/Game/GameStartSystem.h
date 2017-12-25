#pragma once
#include "SnakeGame.h"
#include "SGGameUtil.h"
#include "RenderAPI/Mesh.h"

#include <cmath>

class GameStartSystem : public System {
public:
        
    SGWorld* world;
	SGWorld::Group<SG::Apple> apples;
	SGWorld::Group<SG::SnakeHead> heads;
	SGWorld::Group<SG::SnakeBody> bodies;

	GameStartSystem(SGWorld* world):
		apples(world),
		heads(world),
		bodies(world)
    {
        this->world = world;
    }

	virtual void Update() override {
		if (world->Has<StartGame>()) {
			auto player = SGGameUtil::CreatePlayer(this->world);
			SGGameUtil::CreatePlayerBody(this->world, player);
			world->Remove<StartGame>();
			world->Remove<Gameover>();
			world->Set<Score>()->score = 0;
			world->Set<Difficulty>()->level = 0;
		}
		if (apples.Count() == 0 && !world->Has<Gameover>()) {
			//random spawn an apple.
			auto apple = SGGameUtil::CreateApple(world);
			glm::vec3 randomPosition = glm::vec3(((double)rand() / RAND_MAX - 0.5f) * 5.0f, ((double)rand() / RAND_MAX - 0.5f) * 5.0f, 0.0f);
			apple.Get<Transform>()->position = randomPosition;
		}

		for (auto& head : heads) {
			for (auto& body : bodies) {
				auto distance =
					abs(head.Get<Transform>()->position.x - body.Get<Transform>()->position.x) +
					abs(head.Get<Transform>()->position.y - body.Get<Transform>()->position.y) +
					abs(head.Get<Transform>()->position.z - body.Get<Transform>()->position.z);

				if (distance < 0.4f) {
					if (!body.Get<SG::SnakeBody>()->collisionValidTrigger)
						continue;
					//game over.
					world->Set<Gameover>();
				}
				else {
					body.Get<SG::SnakeBody>()->collisionValidTrigger = true;
				}
			}
		}
		if (world->Has<Gameover>()) {
			for (auto& head : heads.GetVectorClone()) {
				head.Destroy();
			}
			for (auto& body: bodies.GetVectorClone()) {
				body.Destroy();
			}
			for (auto& apple : apples.GetVectorClone()) {
				apple.Destroy();
			}
		}
	}
};