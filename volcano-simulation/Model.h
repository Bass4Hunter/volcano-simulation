#pragma once
#include"Mesh.h"
#include"Texture.h"
#include"Shader.h"
#include"Material.h"
#include"OBJLoader.h"

///////////////////////
// convertir en clase////
////////////////////////
struct TriangleMesh {
	glm::vec3 vertice1;
	glm::vec3 vertice2;
	glm::vec3 vertice3;
	glm::vec3 normal; // vector direccion
	glm::vec3 puntoMedio; // punto medio baricentro
	glm::vec3 ray; // normal en posicion real punto

	void setPuntoMedio() {
		puntoMedio = (vertice1 + vertice2 + vertice3) / 3.f;
	}

	void calculateNormalPosReal() {
		ray = puntoMedio + normal;
	}

	void update(glm::vec3& vec) {
		vertice1 += vec;
		vertice2 += vec;
		vertice3 += vec;
		setPuntoMedio();
		calculateNormalPosReal();
	}

	//addthis in other place 
	bool solveLinearEquation(GLfloat a, GLfloat b, GLfloat c, GLfloat d,
		GLfloat l, GLfloat m, GLfloat n, GLfloat k,
		GLfloat p, GLfloat q, GLfloat r, GLfloat s) {
		GLfloat D, x, y, z, finalA;
		D = (a * m * r + b * p * n + c * l * q) - (a * n * q + b * l * r + c * m * p);
		x = ((b * r * k + c * m * s + d * n * q) - (b * n * s + c * q * k + d * m * r)) / D;
		if (x < 0)
			return false;
		y = ((a * n * s + c * p * k + d * l * r) - (a * r * k + c * l * s + d * n * p)) / D;
		if (y < 0)
			return false;
		z = ((a * q * k + b * l * s + d * m * p) - (a * m * s + b * p * k + d * l * q)) / D;
		if (z < 0)
			return false;

		finalA = x + y + z;
		if (finalA < 1.2) {
			printf(" final A: %f\n", finalA);
			printf("  x = %f,y = % 5.2f, z = % 5.2f\n", x, y, z);
		}
		return true;

	}

	bool insideTriangle(glm::vec3& punto) {
		bool answer =
			solveLinearEquation(vertice1.x, vertice2.x, vertice3.x, punto.x * (-1.f),
				vertice1.y, vertice2.y, vertice3.y, punto.y * (-1.f),
				vertice1.z, vertice2.z, vertice3.z, punto.z * (-1.f)
			);
		return answer;
	}
	bool intersectWithRay(glm::vec3& rayLine, glm::vec3& rayPointOrigin) {
		//puntoMedio;
		//normal;
		GLfloat d = glm::dot(normal, puntoMedio);
		if (glm::dot(normal, rayLine) == 0) {
			return false;
		}
		GLfloat x = (d - glm::dot(normal, rayPointOrigin)) / glm::dot(normal, rayLine);

		glm::vec3 answer = rayPointOrigin + rayLine * x;
		float distancia = glm::length(answer - rayPointOrigin);
		if (distancia <= 0.05f) {

			std::cout << "intersecta en: x =" << answer.x << " ,y= " << answer.y << " ,z= " << answer.z << std::endl;
			std::cout << "colision detectada\n";

			return true;
		}

		return false;

	}
	//glm::vec3 vertexT0, glm::vec3 vertexT1, glm::vec3 vertexT2,
	bool _rayIntersectsTriangle(glm::vec3 rayOrigin,
		glm::vec3 rayVector,
		glm::vec3& outIntersectionPoint)
	{
		glm::vec3 vertexT0 = vertice1;
		glm::vec3 vertexT1 = vertice2;
		glm::vec3 vertexT2 = vertice3;

		const float EPSILON = 0.01f;
		glm::vec3 edge1, edge2, h, s, q;
		float a, f, u, v;
		edge1 = vertexT1 - vertexT0;
		edge2 = vertexT2 - vertexT0;
		h = glm::cross(edge2, rayVector);
		//std::cout << "ray x= " << h.x << " y= " << h.y << " z= " << h.z << std::endl;
		a = dot(h, edge1);
		if (a > -EPSILON && a < EPSILON)
			return false;    // This ray is parallel to this triangle.
		f = 1.0f / a;
		s = rayOrigin - vertexT0;
		u = dot(h, f * s);
		if (u < 0.0f || u > 1.0f)
			return false;
		q = cross(edge1, s);
		v = glm::dot(q, f * rayVector);
		if (v < 0.0f || u + v > 1.0f)
			return false;
		// At this stage we can compute t to find out where the intersection point is on the line.
		float t = glm::dot(q, f * edge2);
		if (t > EPSILON) // ray intersection
		{
			outIntersectionPoint = rayOrigin + rayVector * t;

			float distancia = glm::length(outIntersectionPoint - rayOrigin);
			outIntersectionPoint = normal;
			if (distancia <= 0.05f) {
				return true;
			}
			return false;
		}
		else // This means that there is a line intersection but not a ray intersection.
			return false;
	}
	void print() {
		std::cout << "\n Triangle " << "\nvertice1: " << vertice1.x << " " << vertice1.y << " " << vertice1.z <<
			" \nvertice2: " << vertice2.x << " " << vertice2.y << " " << vertice2.z <<
			" \nvertice3: " << vertice3.x << " " << vertice3.y << " " << vertice3.z <<
			" \nnormal: " << normal.x << " " << normal.y << " " << normal.z <<
			" \nray: " << ray.x << " " << ray.y << " " << ray.z <<
			" \npunto medio: " << puntoMedio.x << " " << puntoMedio.y << " " << puntoMedio.z <<
			std::endl;
	}
	void printRay() {
		std::cout << " \nray: " << ray.x << " " << ray.y << " " << ray.z << std::endl;
	}
	TriangleMesh(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 nor) :vertice1(v1), vertice2(v2), vertice3(v3), normal(nor) {
		setPuntoMedio();
		calculateNormalPosReal();
	}



};

class Model
{
private:
	Material* material;
	Texture* overrideTextureDiffuse;
	Texture* overrideTextureSpecular;
	std::vector<Mesh*> meshes;
	glm::vec3 position;
	bool isStatic;
	std::vector<TriangleMesh> bufferTriangles; //informacion repetida para facil uso pero ordenar

	void updateUniforms()
	{
		
	}

public:
	Model(
		glm::vec3 position,
		Material* material,
		Texture* orTexDif,
		Texture* orTexSpec,
		std::vector<Mesh*>& meshes,
		bool isStatic
	)
	{
		this->position = position;
		this->material = material;
		this->overrideTextureDiffuse = orTexDif;
		this->overrideTextureSpecular = orTexSpec;
		this->isStatic = isStatic;

		for (auto* i : meshes)
		{
			this->meshes.push_back(new Mesh(*i));
		}

		for (auto& i : this->meshes)
		{
			i->move(this->position);
			i->setOrigin(this->position);
		}
	}

	//OBJ file loaded model
	Model(
		glm::vec3 position,
		Material* material,
		Texture* orTexDif,
		Texture* orTexSpec,
		const char* objFile,
		bool isStatic
	)
	{
		this->position = position;
		this->material = material;
		this->overrideTextureDiffuse = orTexDif;
		this->overrideTextureSpecular = orTexSpec;
		this->isStatic = isStatic;

		std::vector<Vertex> mesh = loadOBJ(objFile);
		this->meshes.push_back(new Mesh(mesh.data(), mesh.size(), NULL, 0, glm::vec3(1.f, 0.f, 0.f),
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(1.f)));

		for (auto& i : this->meshes)
		{
			i->move(this->position);
			i->setOrigin(this->position);
		}

		getMeshTriangles();
	}

	~Model()
	{
		for (auto*& i : this->meshes)
			delete i;
	}

	//Functions
	glm::vec3 getPosition() {
		for (auto& i : this->meshes)
			return i->getPosition();
	}
	void rotate(const glm::vec3 rotation)
	{
		for (auto& i : this->meshes)
			i->rotate(rotation);
	}
	void move(const glm::vec3 move)
	{
		for (auto& i : this->meshes)
			i->setPosition(i->getPosition()+move);
	}
	void setPosition(const glm::vec3 position)
	{
		for (auto& i : this->meshes)
			i->setPosition(position);
	}
	
	void update()
	{

	}
	bool getIsStatic() {
		return isStatic;
	}

	// dar fuerzar poner en mejor lugar o pensarla mejor (hacer algo)

	void darFuerzas(glm::vec3 fuerza) {
		this->meshes[0]->move(fuerza);
		//update triangles pos
		for (auto& i : bufferTriangles) {
			i.update(fuerza);

		}
	}

	// ordernar bonito (hacer algo) informacion repetida tratar de chapr de lmismo mesh
	int getMeshTriangles() {
		bufferTriangles.clear();
		Vertex* vertexes = meshes[0]->getVertexArray();
		//TriangleMesh* tempTriangle;
		int totalTriangles = 0;
		glm::vec3 vertice1, vertice2, vertice3, normal;

		for (int i = 0; i < meshes[0]->getTotalVertices(); i += 3) {
			vertice1 = glm::vec3(vertexes[i].position.x, vertexes[i].position.y, vertexes[i].position.z) + position;
			vertice2 = glm::vec3(vertexes[i + 1].position.x, vertexes[i + 1].position.y, vertexes[i + 1].position.z) + position;
			vertice3 = glm::vec3(vertexes[i + 2].position.x, vertexes[i + 2].position.y, vertexes[i + 2].position.z) + position;
			normal = glm::vec3(vertexes[i].normal.x, vertexes[i].normal.y, vertexes[i].normal.z);
			totalTriangles++;
			/*std::cout << "normal vertice1: " << vertexes[i].normal.x << " " << vertexes[i].normal.y << " " << vertexes[i].normal.z << " "
				<< "normal vertice2: " << vertexes[i + 1].normal.x << " " << vertexes[i + 1].normal.y << " " << vertexes[i + 1].normal.z << " "
				<< "normal vertice3: " << vertexes[i + 2].normal.x << " " << vertexes[i + 2].normal.y << " " << vertexes[i + 2].normal.z << " " << std::endl;*/
			bufferTriangles.push_back(TriangleMesh(vertice1, vertice2, vertice3, normal));
			//std::cout << "triangle" << i << i + 1 << i + 2;
			//std::cout << i << ": " << tempTriangle->vertice1.x << " " << tempTriangle->vertice1.y << " " << tempTriangle->vertice1.z <<
				//i + 1 << ": " << tempTriangle->vertice2.x << " " << tempTriangle->vertice2.y << " " << tempTriangle->vertice2.z <<
				//i + 2 << ": " << tempTriangle->vertice3.x << " " << tempTriangle->vertice3.y << " " << tempTriangle->vertice3.z << std::endl;
		}

		return totalTriangles;
	}


	void render(Shader* shader)
	{
		//Update the uniforms
		this->updateUniforms();

		//Update uniforms
		this->material->sendToShader(*shader);

		//Use a program (BECAUSE SHADER CLASS LAST UNIFORM UPDATE UNUSES IT)
		shader->use();

		//Draw
		for (auto& i : this->meshes)
		{
			//Activate texture for each mesh
			this->overrideTextureDiffuse->bind(0);
			this->overrideTextureSpecular->bind(1);

			i->render(shader); //Activates shader also
		}
	}
	std::vector<Mesh*>& getMeshes() {
		return meshes;
	}

	std::vector<TriangleMesh>* getBufferTriangles() {
		return &bufferTriangles;
	}

	//pertenece a  triangels convert class pass to that class
	void deleteMeshTriangles() {
		bufferTriangles.clear();
	}

};