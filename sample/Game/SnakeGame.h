#pragma once

#include "Resecs/Resecs.h"
#include "Components.h"

using namespace Resecs;

struct Input {
    float horizontal;
    float vertical;
};

using SGWorld = World
<
	SG::Transform,
	SG::Renderer,	//deprected
	SG::MeshRenderer,
	SG::SGCamera,
	SG::Velocity,
	SG::Apple,
	SG::SnakeBody,
	SG::SnakeHead,
	//Singletons
	SG::Input,
	SG::Time,
	SG::StartGame,
	SG::Score,
	SG::Difficulty,
	SG::Gameover
>;

    
using SGEntity = SGWorld::Entity;