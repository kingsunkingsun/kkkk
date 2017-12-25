#pragma once
#include "SnakeGame.h"
#include <glm/glm.hpp>

class MoveSystem : public System {
public:
    
    SGWorld* world;
    MoveSystem(SGWorld* world)
    {
        this->world = world;
    }

    virtual void Update() override {
        world->Each<Transform,SG::Velocity>([=](SGEntity entity,Transform* pTrans,SG::Velocity* pVel)
        {
			pTrans->position += glm::vec3(pVel->hor, pVel->vert, 0) * world->Get<Time>()->dt;
        });
    }
};