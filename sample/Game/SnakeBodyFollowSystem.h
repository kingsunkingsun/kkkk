#pragma once
#include "SnakeGame.h"

class SnakeBodyFollowSystem : public System {
public:
        
    SGWorld* world;
    SnakeBodyFollowSystem(SGWorld* world)
    {
        this->world = world;
    }

    virtual void Update() override {
        world->Each<Transform,SG::SnakeBody>(
            [=](SGEntity ent, Transform* pTrans, SG::SnakeBody* pSnakeBody){
				Vector3 currentFolPosition;
                //保存被跟随者的位置。
				if (world->CheckEntityAlive(pSnakeBody->following)){
					auto folEnt = world->GetEntityHandle(pSnakeBody->following);
                    if (!folEnt.Has<Transform>()){
                        return;
                    }
					currentFolPosition = folEnt.Get<Transform>()->position;
                    pSnakeBody->keyPoints.push(SG::SnakeBody::FollowerKeypoint(world->Get<Time>()->time, currentFolPosition));
                }
                //更新当前的位置
                glm::vec3 result = pTrans->position;
                while (!pSnakeBody->keyPoints.empty()){
                    auto qTop = pSnakeBody->keyPoints.front();
					auto manhattanDistance =
						abs(qTop.position.x - currentFolPosition.x) +
						abs(qTop.position.y - currentFolPosition.y) +
						abs(qTop.position.z - currentFolPosition.z);
					if (manhattanDistance < pSnakeBody->delayDistance){
                        result = qTop.position;
                        break;
                    }else{
                        pSnakeBody->keyPoints.pop();
                    }
                }
                pTrans->position = result;
            }
        );
    }
};