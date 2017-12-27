#pragma once

#include "Resecs/Resecs.h"
#include <glm/glm.hpp>
#include <memory>
#include <queue>
#include "RenderAPI/Mesh.h"
#include "RenderAPI/Material.h"

namespace SG {
	using Vector3 = glm::vec3;
	using namespace Resecs;

	class Transform : public Component {
	public:
		Transform(Vector3 position = Vector3(0), Vector3 scale = Vector3(1)) {
			this->position = position;
			this->scale = scale;
		}
		Vector3 position;
		Vector3 scale;
	};    
	
	class SGCamera : public Component {
	public:
		Camera* pCam;
	};
	
	class Renderer : public Component {
	public:
		std::shared_ptr<Model> model;
	};

	class MeshRenderer : public Component {
	public:
		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<Material> material;
	};

	struct Velocity : public Component {
	public:
		Velocity() = default;
		Velocity(float hor, float vert) {
			this->hor = hor;
			this->vert = vert;
		}
		float hor;
		float vert;
	};

	struct SnakeHead : public Component {
		EntityID currentTail;
	};

	struct SnakeBody : public Component {
		SnakeBody() = default;
		SnakeBody(EntityID following, float delayTime) {
			this->following = following;
			this->delayDistance = delayTime;
			this->collisionValidTrigger = false;
		}
		EntityID following;
		float delayDistance;
		struct FollowerKeypoint {
			FollowerKeypoint(float time, glm::vec3 position) {
				this->time = time;
				this->position = position;
			}
			float time;
			glm::vec3 position;
		};
		std::queue<FollowerKeypoint> keyPoints;
		bool collisionValidTrigger;	//避免一生成就产生碰撞。
	};

	struct Apple : public Component {

	};

	struct Input : public Component ,public ISingletonComponent{
		Input() = default;
		Input(float hor,float vert) {
			this->horizontal = hor;
			this->vertical = vert;
		}
		float horizontal;
		float vertical;
	};

	struct Time : public Component, public ISingletonComponent{
		float time;
		float dt;
	};

	struct StartGame : public Component, public ISingletonComponent {};
	struct Gameover : public Component, public ISingletonComponent {};

	struct Score : public Component, public ISingletonComponent {
		int score = 0;
	};

	struct Difficulty : public Component, public ISingletonComponent {
		int level = 0;
	};
};