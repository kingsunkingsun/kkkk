#pragma once
#include "SnakeGame.h"
#include "RenderAPI\Mesh.h"

#include <cmath>

class SGGameUtil {
public:
	static SGEntity CreatePlayer(SGWorld* world) {
		auto playerEntity = world->CreateEntity();
		playerEntity.Set<SG::SnakeHead>();
		auto pRend = playerEntity.Set<MeshRenderer>();
		playerEntity.Set<Transform>(Vector3(0), Vector3(0.3f, 0.3f, 0.3f));
		pRend->mesh = GetCircleMesh();
		pRend->material = GetMaterial();
		return playerEntity;
	}

	static SGEntity CreatePlayerBody(SGWorld* world, SGEntity toFollow) {
		auto playerEntity = world->CreateEntity();
		playerEntity.Set<SG::SnakeBody>(toFollow.GetID(), 0.5f);
		auto pRend = playerEntity.Set<MeshRenderer>();
		playerEntity.Set<Transform>(Vector3(0), Vector3(0.2f, 0.2f, 0.2f));
		pRend->mesh = GetCircleMesh();
		pRend->material = GetMaterial();

		while (toFollow.isAlive()) {
			if (toFollow.Has<SnakeHead>()) {
				toFollow.Get<SnakeHead>()->currentTail = playerEntity.GetID();
				break;
			}
			else {
				if (toFollow.Has<SnakeBody>() && world->CheckEntityAlive(toFollow.Get<SnakeBody>()->following)) {
					toFollow = world->GetEntityHandle(toFollow.Get<SnakeBody>()->following);
				}
				else {
					break;
				}
			}
		}
		return playerEntity;
	}

	static SGEntity CreateApple(SGWorld* world) {
		static std::shared_ptr<Material> appleMaterial;
		if (appleMaterial == nullptr) {
			appleMaterial = std::make_shared<Material>();
			appleMaterial->shader = std::shared_ptr<Shader>(new Shader("./shaders/colorv.glsl", "./shaders/colorf.glsl"));
			appleMaterial->SetVec("_Color", glm::vec4(0.8f, 0.1f, 0.1f,1.0f));
		}

		auto playerEntity = world->CreateEntity();
		playerEntity.Set<SG::Apple>();
		auto pRend = playerEntity.Set<MeshRenderer>();
		playerEntity.Set<Transform>(Vector3(0), Vector3(0.2f, 0.2f, 0.2f));
		pRend->mesh = GetCircleMesh();
		pRend->material = appleMaterial;

		return playerEntity;
	}

private:
	static std::shared_ptr<Mesh> m_circleMesh;
	static std::shared_ptr<Material> lightingMat;
	static std::shared_ptr<Mesh> GetCircleMesh() {
		if (m_circleMesh == nullptr) {
			//generate one.
			std::vector<Vertex> verticies;
			std::vector<unsigned int> indicies;
			for (int i = 0; i < 16; i++) {
				Vertex center, t1, t2;
				center.Position = glm::vec3(0);
				t1.Position = glm::rotate(glm::mat4(1), 2 * glm::pi<float>() / 16.0f * i, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
				t2.Position = glm::rotate(glm::mat4(1), 2 * glm::pi<float>() / 16.0f * (i + 1), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
				verticies.push_back(center);
				verticies.push_back(t1);
				verticies.push_back(t2);
				indicies.push_back(i * 3);
				indicies.push_back(i * 3 + 1);
				indicies.push_back(i * 3 + 2);
			}
			m_circleMesh = std::make_shared<Mesh>(verticies, indicies);
		}
		return m_circleMesh;
	}
	static std::shared_ptr<Material> GetMaterial() {
		if (lightingMat == nullptr) {
			//generate one.
			auto t = std::shared_ptr<Shader>(new Shader("./shaders/colorv.glsl", "./shaders/colorf.glsl"));
			lightingMat = std::make_shared<Material>(t);
			lightingMat->SetVec("_Color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}
		return lightingMat;
	}
};

std::shared_ptr<Mesh> SGGameUtil::m_circleMesh = nullptr;
std::shared_ptr<Material> SGGameUtil::lightingMat = nullptr;