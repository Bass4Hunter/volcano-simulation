#pragma once
#include<vector>
#include"Model.h"
#include "Particle.h"
#include "Texture.h"
#include "Material.h"
#include<random>
class PSO
{
private:
	std::vector<Particle*> particles;
	int swarmSize;
	float inertia;
	float correctionFactorLocal;	
	float correctionFactorGlobal;
	float correctionGen;
	glm::vec3 globalBest;
	int evaluatedGlobalBest;
	glm::vec3 targetPosition;
	float rand_1, rand_2;
	glm::vec3 forces;
public:

	

	PSO(glm::vec3 targetPosition,int swarmSize,float inertia,float correctionFactorLocal,float correctionFactorGlobal,float corGen, glm::vec3 forces = glm::vec3(0));
	
	void init(std::vector<Model*>* models, Material* material,Texture* texture,glm::vec3 pinit);
	
	void iterate();
	
	float euclidianDistance(Particle* particle);

	std::vector<Particle*>* getParticles() { return &particles; }

	~PSO();
};

