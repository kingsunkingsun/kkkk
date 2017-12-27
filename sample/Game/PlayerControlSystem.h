#pragma once
#include "SnakeGame.h"

#include <cmath>

class PlayerControlSystem : public System {
public:
        
    SGWorld* world;
    PlayerControlSystem(SGWorld* world)
    {
        this->world = world;
    }

    virtual void Update() override {
        world->Each<Transform,SG::SnakeHead>([=](SGEntity entity,Transform* pTrans,SG::SnakeHead* head)
        {
            if (!entity.Has<SG::Velocity>())
                entity.Set<SG::Velocity>();
            auto t = entity.Get<Velocity>();
            auto currentVel = glm::vec2(t->hor,t->vert);
            currentVel += glm::vec2(0.00001f,0.0f);
            auto inputVel = glm::vec2(world->Get<SG::Input>()->horizontal,world->Get<SG::Input>()->vertical);
            
            auto result = glm::normalize(currentVel + inputVel * 1.2f);

			result *= 1 + world->Get<Difficulty>()->level / 4.0f;
            entity.Set<Velocity>(result.x,result.y);
        });
    }
};