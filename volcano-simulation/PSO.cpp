#include "PSO.h"

PSO::PSO(glm::vec3 targetPosition,int swarmSize, float inertia, float correctionFactorLocal,float correctionFactorGlobal,float corGen,glm::vec3 forces)
{
	this->swarmSize = swarmSize;
	this->inertia = inertia;
	this->correctionFactorLocal = correctionFactorLocal;
	this->correctionFactorGlobal = correctionFactorGlobal;
	globalBest = glm::vec3(-1);
	evaluatedGlobalBest = 1000000;
	this->targetPosition = targetPosition;
	this->correctionGen = corGen;
	this->forces = forces;
	rand_1 = 0;
	rand_2 = 0;
}

void PSO::init(std::vector<Model*>* models,Material* material, Texture* texture, glm::vec3 pinicial )
{
	int begin = models->size();

	for (int i = 0;i<swarmSize;i++) {
		models->push_back(new Model(
			pinicial,
			material,
			texture,
			texture,
			"OBJFiles/triangle.obj",
			false
		)
		);
	}

	for (int i = begin; i < models->size(); i++)
	{
		particles.push_back(new Particle((*models)[i],targetPosition));
	}
	for (int i = 0; i < particles.size(); i++)
	{
		if (particles[i]->getEvaluatedLocalBest() < evaluatedGlobalBest)
			evaluatedGlobalBest = particles[i]->getEvaluatedLocalBest();
	}
}

void PSO::iterate()
{

	for (int i = 0; i < particles.size(); i++)
	{
		rand_1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		rand_2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

		std::default_random_engine generator;
		std::uniform_real_distribution<double> distribution(-1,1);
		
		double random_x = distribution(generator);

		double random_z = distribution(generator);

		//Actualizando velocidad 
		particles[i]->setSpeed(
			((inertia * particles[i]->getSpeed())
				+ correctionFactorLocal * rand_1 * (particles[i]->getLocalBest() - particles[i]->getPosition())
				+ correctionFactorGlobal * rand_2 * (globalBest - particles[i]->getPosition())
				+ glm::normalize(targetPosition - particles[i]->getPosition()) * correctionGen
				)
			);
		glm::vec3 temp = particles[i]->getSpeed();

		//Actualizando posicion
		particles[i]->setPosition(glm::vec3(temp.x,0, temp.z)+forces);

		//Actualizando costo
		particles[i]->setEvaluatedCost(euclidianDistance(particles[i]));

		//Actualizando local best
		if (particles[i]->getEvaluatedCost() < particles[i]->getEvaluatedLocalBest()) {
			particles[i]->setLocalBest(particles[i]->getPosition());
			particles[i]->setEvaluatedLocalBest(particles[i]->getEvaluatedCost());
			if (particles[i]->getEvaluatedLocalBest() < evaluatedGlobalBest) {
				globalBest = particles[i]->getLocalBest();
			}
		}
	}

}



float PSO::euclidianDistance(Particle* particle)
{
	return glm::distance(targetPosition,particle->getPosition());
}

PSO::~PSO()
{
	for (int i = 0; i < particles.size(); i++)
	{
		delete particles[i];
	}
}
