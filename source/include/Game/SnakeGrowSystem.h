#pragma once
#include "SnakeGame.h"

class SnakeGrowSystem : public System {
public:
        
    SGWorld* world;
	SGWorld::Group<SG::Apple,Transform> apples;
	SGWorld::Group<SG::SnakeHead,Transform> snakeHeads;
    SnakeGrowSystem(SGWorld* world):
		apples(world),
		snakeHeads(world)
    {
        this->world = world;
    }

    virtual void Update() override {
		for (auto& snakeHead : snakeHeads) {
			for (auto& apple : apples) {
				float distance = glm::distance(snakeHead.Get<Transform>()->position, apple.Get<Transform>()->position);
				if (distance < 0.5f) {
					apple.Destroy();
					world->Get<Score>()->score += 20;
					world->Get<Difficulty>()->level = world->Get<Score>()->score / 60;
					SGGameUtil::CreatePlayerBody(world, world->GetEntityHandle(snakeHead.Get <SG::SnakeHead>()->currentTail));
					break;
				}
			}
		}
    }
};