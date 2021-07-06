#pragma once
#pragma once
#include "libs.h"
#define GRAVEDAD 0
#define VELOCIDAD 1
#define REPULSION 2
#define FRICCION 3

struct Fuerzas {
	glm::vec3 gravedad;
	glm::vec3 velocidad;
	glm::vec3 repulsion;
	glm::vec3 friccion;
};

struct Collision {
	Model* modelo1;
	Model* modelo2;
	glm::vec3 contacto;
	// ANGULO DE INCIDENCIA
	// FUERZA DE CHOQUE
	// MAZA
};

class Physics {
public:
	Physics() {}
	~Physics() {}

	void setCollisiones(Model* modelo1, Model* modelo2, glm::vec3 contacto) {
		Collision* colision = new Collision();
		colision->modelo1 = modelo1;
		colision->modelo2 = modelo2;
		colision->contacto = contacto;
		collisiones.push_back(colision);
	}

	void setFuerza(glm::vec3 fuerza, int posFuerza) {
		switch (posFuerza) {
		case GRAVEDAD:
			fuerzas.gravedad = fuerza;
			break;
		case VELOCIDAD:
			fuerzas.velocidad = fuerza;
			break;
		case REPULSION:
			fuerzas.repulsion = fuerza;
			break;
		case FRICCION:
			fuerzas.friccion = fuerza;
			break;
		default:
			break;
		}
	}
	void checkColision( bool& aF, Model* Volcan, PSO* pso) {
		//std::cout << "*\nCEHCKINGCOLLISION\n*\n";
		std::vector<TriangleMesh>* actualBufferTriangles, * secondBufferTriangle;
		auto& k = Volcan;
		secondBufferTriangle = k->getBufferTriangles();
		glm::vec3 out;
		
		for (auto& i : *(pso->getParticles())) {
			actualBufferTriangles = i->getModel()->getBufferTriangles();
			for (auto& j : *actualBufferTriangles) {
				glm::vec3 rayLine = j.ray - j.puntoMedio;
				for (auto& l : *secondBufferTriangle) {
					float distancia = glm::length(l.puntoMedio - j.puntoMedio);
					if (l._rayIntersectsTriangle(j.puntoMedio, rayLine, out)) {
						glm::vec3 newout = glm::normalize(out);
						//newout.y = 0.2f;	

						for (int x = 0;x<pso->getParticles()->size();x++) {
							pso->getParticles()->at(x)->getModel()->darFuerzas(l.normal * glm::vec3(0.1));
						}

					}
				}
			}
		}
	
	}
	void aplicarFuerza(std::vector<PSO*>& pso) {
		for (int i = 0;i<pso.size();i++) {
			pso[i]->iterate();
			for (int x = 0; x < pso[i]->getParticles()->size(); x++) {

			
				pso[i]->getParticles()->at(x)->setPosition(fuerzas.gravedad);
			}
		}

	}
	/*
	void aplicarFuerza(Model* objeto) {
			objeto->darFuerzas(fuerzas.gravedad);
			objeto->darFuerzas(fuerzas.velocidad);
			objeto->darFuerzas(fuerzas.friccion);
			objeto->darFuerzas(fuerzas.repulsion);
	}*/

private:
	std::vector<Collision*> collisiones;
	Fuerzas fuerzas;
};

