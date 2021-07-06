#pragma once
#include "Model.h"
#include <vec3.hpp>
class Particle
{
private:
	Model* particle;
	glm::vec3 position;
	glm::vec3 localBest;
	glm::vec3 speed;
	float evaluatedLocalBest;	
	float evaluatedCost;
public:
	Particle(Model* particula,glm::vec3 target) {
		this->particle = particula;
		position = particula->getPosition();
		speed = glm::vec3(0.f);
		localBest = particula->getPosition();
		evaluatedCost = glm::distance(target,position);
		evaluatedLocalBest = evaluatedCost;
	};
	void setEvaluatedCost(float cost) {
		this->evaluatedCost = cost;
	}
	float getEvaluatedCost() {
		return evaluatedCost;
	}
	Model* getParticle() {
		return particle;
	}
	void setLocalBest(glm::vec3 localBest) {
		this->localBest = localBest;
	}
	glm::vec3 getLocalBest() {
		return localBest;
	}
	void setPosition(glm::vec3 position) {
		particle->darFuerzas(position);
		this->position = particle->getPosition();
	}
	glm::vec3 getPosition() {
		return position;
	}
	void  setSpeed(glm::vec3 speed) {
		this->speed = speed;
	}
	glm::vec3 getSpeed() {
		return speed;
	}
	void setEvaluatedLocalBest(float evaluate) {
		this->evaluatedLocalBest = evaluate;
	}
	float getEvaluatedLocalBest() {
		return evaluatedLocalBest;
	}
	Model* getModel() { return particle; }

};

