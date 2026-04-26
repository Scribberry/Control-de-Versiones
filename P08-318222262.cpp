/*
Práctica 8: Iluminación 2 
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture pinturaTexture;
Texture dadoTexture;

Model Coche_M;
Model Llanta_M;
Model Nave_M;
Model Lampara_M;

//Variables para la pecera
Mesh* peceraVidrio = nullptr;  //Paredes y fondo (vidrio)
Mesh* peceraAgua = nullptr;    //Superficie superior (agua)
Texture texturaAgua;
Texture texturaVidrio;
Model pezModel;
glm::vec3 posicionPez = glm::vec3(0.0f, 2.0f, -5.0f);
float velocidadPez = 3.0f;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
unsigned int pointLightCount = 0;

//Control de la luz de la lámpara
bool lamparaEncendida = true;
bool lKeyPressed = false;  //Para evitar múltiples cambios por frame

//Control de color del faro del coche
int faroColorIndex = 0;
bool cKeyPressed = false;

//Control de la luz del pez
bool luzPezEncendida = true;
bool lPezKeyPressed = false;  //Para tecla P (Pez)

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	//Octaedro (Dado de 8 Caras Triangulares)

	unsigned int octaedro_indices[] = {

		//Caras Superiores (Fila 1 de la Textura)
		0,1,2, 3,4,5, 6,7,8, 9,10,11,

		//Caras Inferiores (Fila 2 de la Textura)
		12,13,14, 15,16,17, 18,19,20, 21,22,23
	};

	GLfloat octaedro_vertices[] = {

		//************ CARAS SUPERIORES ************ 

		//Cara 1: Frente

		//x     y      z      S      T       NX     NY      NZ
	   -0.5f, 0.0f,  0.5f,   0.00f, 1.0f,   0.0f,  0.0f,  -1.0f,
		0.5f, 0.0f,  0.5f,   0.50f, 1.0f,   0.0f,  0.0f,  -1.0f,
		0.0f, 0.5f,  0.0f,   0.25f, 0.5f,   0.0f,  0.0f,  -1.0f,

		//Cara 2: Derecha

		//x     y      z      S      T       NX     NY      NZ
		0.5f, 0.0f,  0.5f,   0.50f, 1.0f,  -1.0f,  0.0f,  0.0f,
		0.5f, 0.0f, -0.5f,   1.00f, 1.0f,  -1.0f,  0.0f,  0.0f,
		0.0f, 0.5f,  0.0f,   0.75f, 0.5f,  -1.0f,  0.0f,  0.0f,

		//Cara 3: Atrás

		//x     y      z      S      T       NX     NY      NZ
		0.5f, 0.0f, -0.5f,   0.00f, 0.5f,   0.0f,  0.0f,  1.0f,
	   -0.5f, 0.0f, -0.5f,   0.50f, 0.5f,   0.0f,  0.0f,  1.0f,
		0.0f, 0.5f,  0.0f,   0.25f, 0.0f,   0.0f,  0.0f,  1.0f,

		//Cara 4: Izquierda
		
		//x     y      z      S      T       NX     NY      NZ
	   -0.5f, 0.0f, -0.5f,   0.50f, 0.5f,   1.0f,  0.0f,  0.0f,
	   -0.5f, 0.0f,  0.5f,   1.00f, 0.5f,   1.0f,  0.0f,  0.0f,
	    0.0f, 0.5f,  0.0f,   0.75f, 0.0f,   1.0f,  0.0f,  0.0f,

	    //************ CARAS INFERIORES ************ 

		//Cara 5: Frente-Abajo

	    //x     y      z      S      T       NX     NY      NZ
	    0.5f, 0.0f,  0.5f,   0.00f, 0.5f,   0.0f,  0.0f,  -1.0f,
	   -0.5f, 0.0f,  0.5f,   0.50f, 0.5f,   0.0f,  0.0f,  -1.0f,
		0.0f,-0.5f,  0.0f,   0.25f, 0.0f,   0.0f,  0.0f,  -1.0f,

	    //Cara 6: Derecha-Abajo
		
		//x     y      z      S      T       NX     NY      NZ
	    0.5f, 0.0f, -0.5f,   0.50f, 0.5f,  -1.0f,  0.0f,  0.0f,
		0.5f, 0.0f,  0.5f,   1.00f, 0.5f,  -1.0f,  0.0f,  0.0f,
	    0.0f,-0.5f,  0.0f,   0.75f, 0.0f,  -1.0f,  0.0f,  0.0f,

		//Cara 7: Atrás-Abajo

		//x     y      z      S      T       NX     NY      NZ
	   -0.5f, 0.0f, -0.5f,   0.00f, 0.0f,   0.0f,  0.0f,  1.0f,
		0.5f, 0.0f, -0.5f,   0.50f, 0.0f,   0.0f,  0.0f,  1.0f,
		0.0f,-0.5f,  0.0f,   0.25f,-0.5f,   0.0f,  0.0f,  1.0f,

		//Cara 8: Izquierda-Abajo

		//x     y      z      S      T       NX     NY      NZ
	   -0.5f, 0.0f,  0.5f,   0.50f, 0.0f,   1.0f,  0.0f,  0.0f,
	   -0.5f, 0.0f, -0.5f,   1.00f, 0.0f,   1.0f,  0.0f,  0.0f,
	    0.0f,-0.5f,  0.0f,   0.75f,-0.5f,   1.0f,  0.0f,  0.0f,
	};

	Mesh* dado8 = new Mesh();
	//24 vertices * 8 floats = 192 floats
    //8 caras * 3 índices = 24 indices
	dado8->CreateMesh(octaedro_vertices, octaedro_indices, 192, 24);
	meshList.push_back(dado8);
}

void CrearPecera()
{
	//Vértices de las paredes y fondo (vidrio)
	//Normales hacia adentro

	GLfloat verticesVidrio[] = {
		//Cara frontal (normal hacia adentro: -Z)
		-2.0f, -2.0f,  2.0f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
		 2.0f, -2.0f,  2.0f,  1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
		 2.0f,  3.0f,  2.0f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
		-2.0f,  3.0f,  2.0f,  0.0f, 1.0f,  0.0f, 0.0f, -1.0f,

		//Cara trasera (normal hacia adentro: +Z)
		-2.0f, -2.0f, -2.0f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
		-2.0f,  3.0f, -2.0f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
		 2.0f,  3.0f, -2.0f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
		 2.0f, -2.0f, -2.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,

		 //Cara izquierda (normal hacia adentro: -X)
		 -2.0f, -2.0f, -2.0f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		 -2.0f, -2.0f,  2.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		 -2.0f,  3.0f,  2.0f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		 -2.0f,  3.0f, -2.0f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,

		 //Cara derecha (normal hacia adentro: +X)
		  2.0f, -2.0f,  2.0f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		  2.0f, -2.0f, -2.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		  2.0f,  3.0f, -2.0f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
		  2.0f,  3.0f,  2.0f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,

		  //Cara inferior (fondo) (normal hacia adentro: -Y)
		  -2.0f, -2.0f, -2.0f,  0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
		   2.0f, -2.0f, -2.0f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
		   2.0f, -2.0f,  2.0f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,
		  -2.0f, -2.0f,  2.0f,  0.0f, 1.0f,  0.0f, -1.0f, 0.0f,
	};

	//Vértices de la superficie superior (agua)
	//Normales hacia adentro (+Y)

	GLfloat verticesAgua[] = {
		//Cara superior (normal hacia adentro: +Y)
		-2.0f,  3.0f,  2.0f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		 2.0f,  3.0f,  2.0f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		 2.0f,  3.0f, -2.0f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
		-2.0f,  3.0f, -2.0f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
	};

	unsigned int indicesVidrio[] = {
		0,1,2, 0,2,3,       //frontal
		4,5,6, 4,6,7,       //trasera
		8,9,10, 8,10,11,    //izquierda
		12,13,14, 12,14,15, //derecha
		16,17,18, 16,18,19  //fondo
	};

	unsigned int indicesAgua[] = {
		0,1,2, 0,2,3   //superficie superior
	};

	//Mesh de vidrio (5 caras: frontal, trasera, izquierda, derecha, fondo)
	peceraVidrio = new Mesh();
	peceraVidrio->CreateMesh(verticesVidrio, indicesVidrio, 20 * 8, 30); //20 vértices * 8 floats, 30 índices

	//Mesh de agua (1 cara: superior)
	peceraAgua = new Mesh();
	peceraAgua->CreateMesh(verticesAgua, indicesAgua, 4 * 8, 6); //4 vértices * 8 floats, 6 índices

	//Cargar texturas
	texturaAgua = Texture("Textures/water.tga");
	texturaAgua.LoadTextureA();

	texturaVidrio = Texture("Textures/glass.tga");
	texturaVidrio.LoadTextureA();

	//Cargar modelo del pez
	pezModel.LoadModel("Models/cuerpo_pez.obj");
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CrearDado();
	CrearPecera();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	pinturaTexture = Texture("Textures/pintura.tga");
	pinturaTexture.LoadTextureA();
	dadoTexture = Texture("Textures/octaedro.tga");
	dadoTexture.LoadTextureA();

	Coche_M = Model();
	Coche_M.LoadModel("Models/coche.obj"); 
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta.obj");
	Nave_M = Model();
	Nave_M.LoadModel("Models/nave.obj");
	Lampara_M = Model();
	Lampara_M.LoadModel("Models/lampara.obj");
	
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.1f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	//unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//Faro azul del coche
	spotLights[2] = SpotLight(0.0f, 0.0f, 1.0f,  //Color azul en RGB
		1.0f, 5.0f,                              //Intensidades ambiental y difusa
		0.0f, 1.0f, 0.0f,                        //Posición (se actualizará)
		1.0f, 0.0f, 0.0f,                        //Dirección hacia adelante
		1.0f, 0.01f, 0.001f,                     //Atenuación de la luz
		20.0f);                                  //Ángulo del cono
	spotLightCount++;

	//Luz DELANTERA del helicópter (blanca) - índice 3
	spotLights[3] = SpotLight(0.0f, 0.0f, 1.0f,  //Color azul en RGB
		1.0f, 3.0f,                              //Intensidades ambiental y difusa
		0.0f, 5.0f, 6.0f,                        //Posición (se actualizará)
		0.0f, -1.0f, 0.0f,                       //Dirección hacia abajo
		1.0f, 0.01f, 0.001f,                     //Atenuación de la luz
		45.0f);                                  //Ángulo del cono

	//Luz trasera del helicóptero (roja)
	spotLights[4] = SpotLight(1.0f, 0.0f, 0.0f,  //Color rojo en RGB
		1.0f, 3.0f,                              //Intensidades ambiental y difusa
		0.0f, 5.0f, 6.0f,                        //Posición (se actualizará)
		0.0f, -1.0f, 0.0f,                       //Dirección hacia abajo
		1.0f, 0.01f, 0.001f,                     //Atenuación de la luz
		45.0f);                                  //Ángulo del cono

	//Spotlight del pez
	spotLights[5] = SpotLight(1.0f, 0.5f, 0.0f,  //Color naranja en RGB
		1.0f, 5.0f,                              //Intensidades ambiental y difusa
		0.0f, 1.5f, -8.0f,                       //Posición (se actualizará)
		1.0f, 0.0f, 0.0f,                        //Dirección inicial (X positiva)
		1.0f, 0.1f, 0.05f,                       //Atenuación de la luz
		30.0f);                                  //Ángulo del cono

	//Variables para control de dirección del spotlight
	glm::vec3 direccionSpotPez = glm::vec3(1.0f, 0.0f, 0.0f);
	bool spotPezEncendido = true;
	bool sKeyPressed = false;

	//Luz puntual celeste para la lámpara
	pointLights[pointLightCount] = PointLight(0.0f, 1.0f, 1.0f,    //Color celeste en RGB
		5.0f, 10.0f,                                               //Intensidades ambiental y difusa
		0.0f, -1.0f, 15.0f,                                        //Posición
		1.0f, 1.0f, 0.35f);                                        //Atenuación de la luz
	pointLightCount++;

	// Luz puntual AZUL para el pez (índice 2)
	pointLights[2] = PointLight(0.0f, 0.0f, 1.0f,   //Color azul en RGB
		2.0f, 5.0f,                                 //Intensidades ambiental y difusa
		0.0f, 1.5f, -8.0f,                          //Posición (se actualizará)
		0.5f, 0.3f, 0.1f);                          //Atenuación de la luz
	pointLightCount++;

	//Guardar valor original
	unsigned int basePointLightCount = pointLightCount - 1; //Línea para el Ejercicio 8

	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	//******** MATRICES *********
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::mat4 modelCoche;
	glm::mat4 faroMatrix;
	glm::mat4 modelHeliBase;
	glm::mat4 luzMatrix;
	glm::mat4 modelHeli;
	glm::mat4 modelLampara;
	glm::mat4 modelPecera;
	glm::mat4 modelPez;

	SpotLight finalSpotLights[5];  //Arreglo temporal para reordenar luces
	
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Apagar/encender lámpara con tecla L
		if (mainWindow.getsKeys()[GLFW_KEY_L] && !lKeyPressed) {
			lamparaEncendida = !lamparaEncendida;
			lKeyPressed = true;

			if (lamparaEncendida) {
				if (luzPezEncendida) {
					pointLightCount = basePointLightCount + 1;  //Lámpara + luz pez
				}
				else {
					pointLightCount = basePointLightCount;      //Solo lámpara
				}
				printf("LAMPARA ENCENDIDA!\n");
			}
			else {
				if (luzPezEncendida) {
					pointLightCount = 2;  //Luz roja (0) + luz pez (2)
				}
				else {
					pointLightCount = 1;  //Solo luz roja
				}
				printf("LAMPARA APAGADA!\n");
			}
		}
		if (!mainWindow.getsKeys()[GLFW_KEY_L]) {
			lKeyPressed = false;
		}

		// Apagar/encender luz del pez con tecla P
		if (mainWindow.getsKeys()[GLFW_KEY_P] && !lPezKeyPressed) {
			luzPezEncendida = !luzPezEncendida;
			lPezKeyPressed = true;

			if (luzPezEncendida) {
				if (lamparaEncendida) {
					pointLightCount = basePointLightCount + 1;
				}
				else {
					pointLightCount = 2;  //Luz roja + luz pez
				}
				printf("LUZ DEL PEZ ENCENDIDA\n");
			}
			else {
				if (lamparaEncendida) {
					pointLightCount = basePointLightCount;
				}
				else {
					pointLightCount = 1;  //Solo luz roja
				}
				printf("LUZ DEL PEZ APAGADA\n");
			}
		}
		if (!mainWindow.getsKeys()[GLFW_KEY_P]) {
			lPezKeyPressed = false;
		}

		//Control de spotlight del pez con tecla S
		if (mainWindow.getsKeys()[GLFW_KEY_S] && !sKeyPressed) {
			spotPezEncendido = !spotPezEncendido;
			sKeyPressed = true;
			printf("SPOTLIGHT DEL PEZ %s\n", spotPezEncendido ? "ENCENDIDO" : "APAGADO");
		}
		if (!mainWindow.getsKeys()[GLFW_KEY_S]) {
			sKeyPressed = false;
		}

		//Control de dirección del spotlight
		//Teclas: Q/E para X, R/F para Y, T/G para Z
		if (mainWindow.getsKeys()[GLFW_KEY_Q]) {
			direccionSpotPez.x += deltaTime * 2.0f;
			if (direccionSpotPez.x > 1.0f) direccionSpotPez.x = 1.0f;
			printf("Direccion X: %.2f\n", direccionSpotPez.x);
		}
		if (mainWindow.getsKeys()[GLFW_KEY_E]) {
			direccionSpotPez.x -= deltaTime * 2.0f;
			if (direccionSpotPez.x < -1.0f) direccionSpotPez.x = -1.0f;
		}
		if (mainWindow.getsKeys()[GLFW_KEY_R]) {
			direccionSpotPez.y += deltaTime * 2.0f;
			if (direccionSpotPez.y > 1.0f) direccionSpotPez.y = 1.0f;
		}
		if (mainWindow.getsKeys()[GLFW_KEY_F]) {
			direccionSpotPez.y -= deltaTime * 2.0f;
			if (direccionSpotPez.y < -1.0f) direccionSpotPez.y = -1.0f;
		}
		if (mainWindow.getsKeys()[GLFW_KEY_T]) {
			direccionSpotPez.z += deltaTime * 2.0f;
			if (direccionSpotPez.z > 1.0f) direccionSpotPez.z = 1.0f;
		}
		if (mainWindow.getsKeys()[GLFW_KEY_G]) {
			direccionSpotPez.z -= deltaTime * 2.0f;
			if (direccionSpotPez.z < -1.0f) direccionSpotPez.z = -1.0f;
		}

		//Normalizar dirección
		direccionSpotPez = glm::normalize(direccionSpotPez);

		//Cambiar color del faro del coche con tecla C
		if (mainWindow.getsKeys()[GLFW_KEY_C] && !cKeyPressed) {  //Si se presiona C y no estaba presionada antes
			cKeyPressed = true;                                   //Marca que ya se presionó la tecla
			faroColorIndex = (faroColorIndex + 1) % 6;            //Avanza al siguiente color
			
			switch (faroColorIndex) {                             //Según el índice del color seleccionado
			case 0:                                               //Índice 0
				spotLights[2].SetColour(1.0f, 0.0f, 0.0f);        //Cambia el faro (tercer foco) a Rojo
				printf("FARO DE COLOR ROJO\n");                   //Mensaje en consola para corroborar
				break;
			case 1:                                               //Índice 1
				spotLights[2].SetColour(1.0f, 1.0f, 0.0f);        //Cambia a Amarillo
				printf("FARO DE COLOR AMARILLO\n");
				break;
			case 2:                                               //Índice 2
				spotLights[2].SetColour(0.0f, 1.0f, 0.0f);        //Cambia a Verde
				printf("FARO DE COLOR VERDE\n");
				break;
			case 3:                                               //Índice 3
				spotLights[2].SetColour(0.0f, 1.0f, 1.0f);        //Cambia a Cian
				printf("FARO DE COLOR CIAN\n");
				break;
			case 4:                                               //Índice 4
				spotLights[2].SetColour(0.0f, 0.0f, 1.0f);        //Cambia a Azul
				printf("FARO DE COLOR AZUL\n");
				break;
			case 5:                                               //Índice 5
				spotLights[2].SetColour(1.0f, 0.0f, 1.0f);        //Cambia a Magenta
				printf("FARO DE COLOR MAGENTA\n");
				break;
			}
		}
		if (!mainWindow.getsKeys()[GLFW_KEY_C]) {                 //Cuando se suelta la tecla C...
			cKeyPressed = false;                                  //Resetea
		}

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();

		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
			glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		//spotLights[1].SetPos(poscoche + glm::vec(x, y, cofre));

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		//shaderList[0].SetSpotLights(spotLights, mainWindow.getSpotLightCount());

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//********** DADO DE OCHO CARAS ********** 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.5f, 4.5f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshList[4]->RenderMesh();

		//********** COCHE CON FARO AZUL Y LLANTAS ********** 

		//Matriz base del coche
		modelCoche = glm::mat4(1.0);
		modelCoche = glm::translate(modelCoche, glm::vec3(0.0f + mainWindow.getmuevex(), -1.0f, -10.0f));
		modelCoche = glm::scale(modelCoche, glm::vec3(0.5f, 0.5f, 0.5f));
		modelCoche = glm::rotate(modelCoche, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCoche));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
		Coche_M.RenderModel();

		//********** FARO AZUL **********
		faroMatrix = modelCoche;
		faroMatrix = glm::translate(faroMatrix, glm::vec3(-4.0f, 0.5f, -7.0f));

		glm::vec3 faroPos = glm::vec3(faroMatrix[3]);
		glm::vec3 faroDir = glm::vec3(faroMatrix * glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
		spotLights[2].SetFlash(faroPos, glm::normalize(faroDir));

		//********** HELICÓPTERO (NAVE) **********

		//Matriz base del helicóptero
		modelHeliBase = glm::mat4(1.0);
		modelHeliBase = glm::translate(modelHeliBase, glm::vec3(mainWindow.getHeliX(), 5.0f, 20.0f));
		modelHeliBase = glm::scale(modelHeliBase, glm::vec3(1.5f, 1.5f, 1.5f));

		//Matriz para la luz del helicóptero (sin rotaciones)
		luzMatrix = modelHeliBase;
		luzMatrix = glm::translate(luzMatrix, glm::vec3(0.0f, -1.5f, 3.0f));

		//Rotaciones del helicóptero
		modelHeli = modelHeliBase;
		modelHeli = glm::rotate(modelHeli, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelHeli = glm::rotate(modelHeli, 90.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		//Renderizar helicóptero
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelHeli));
		Nave_M.RenderModel();

		//********** LUZ DEL HELICÓPTERO **********
		//Posición base del helicóptero para las luces
		glm::vec3 heliPad = glm::vec3(luzMatrix[3]);
		glm::vec3 heliDir = glm::vec3(luzMatrix * glm::vec4(0.0f, -1.0f, 0.0f, 0.0f));
		heliDir = glm::normalize(heliDir);

		// Actualizar ambas luces (aunque solo una se usará)
		glm::vec3 forwardLightPos = heliPad;
		forwardLightPos.x -= 2.0f;  // Adelante
		spotLights[3].SetFlash(forwardLightPos, heliDir);

		glm::vec3 backwardLightPos = heliPad;
		backwardLightPos.x += 2.0f;  // Atrás
		spotLights[4].SetFlash(backwardLightPos, heliDir);

		//********** REORDENAR LUCES PARA EL SHADER **********
        //Copiar las luces fijas (0,1,2)
		finalSpotLights[0] = spotLights[0];  //Linterna
		finalSpotLights[1] = spotLights[1];  //Luz fija verde
		finalSpotLights[2] = spotLights[2];  //Faro del coche

		//********** AGREGAR LA LUZ DEL HELICÓPTERO SEGÚN LA DIRECCIÓN **********
		//Sólo si el helicóptero se está moviendo
		if (mainWindow.getHeliMovingForward()) {
			finalSpotLights[3] = spotLights[3];  //Luz delantera (azul)
			printf("LUZ AZUL DELANTERA ENCENDIDA\n");
		}
		else if (mainWindow.getHeliMovingBackward()) {
			finalSpotLights[3] = spotLights[4];  //Luz trasera (roja)
			printf("LUZ ROJA TRASERA ENCENDIDA\n");
		}
		else {
			finalSpotLights[3] = SpotLight(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 5.0f, 6.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.01f, 0.001f, 45.0f);
		}

		//Agregar spotlight del pez (índice 4)
		if (spotPezEncendido) {
			finalSpotLights[4] = spotLights[5];
		}
		else {
			finalSpotLights[4] = SpotLight(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.5f, -8.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.1f, 0.05f, 30.0f);
		}

		//Enviar siempre 5 spotlights al shader
		shaderList[0].SetSpotLights(finalSpotLights, 5);

		//********** PECERA Y PEZ ABISAL **********
	    //Posición inicial del pez (solo la primera vez)

		static bool primeraVez = true;
		if (primeraVez) {
			posicionPez = glm::vec3(8.0f, 1.5f, -8.0f);
			primeraVez = false;
		}

		//Mover pez con teclas I y K (movimiento diagonal)
		float movimiento = velocidadPez * deltaTime;
		if (mainWindow.getPezSubir()) {
			posicionPez.y += movimiento;
			posicionPez.z += movimiento * 0.5f;  //Diagonal hacia arriba
		}
		if (mainWindow.getPezBajar()) {
			posicionPez.y -= movimiento;
			posicionPez.z -= movimiento * 0.5f;  //Diagonal hacia abajo
		}

		//Limitar movimiento dentro de la pecera
		if (posicionPez.y > 2.0f) posicionPez.y = 2.0f;     //Más abajo del agua (Y=3)
		if (posicionPez.y < -0.5f) posicionPez.y = -0.5f;   //Más arriba del fondo (Y=-2)
		if (posicionPez.z > -6.9f) posicionPez.z = -6.9f;   //Más atrás de pared frontal (Z=-6)
		if (posicionPez.z < -9.1f) posicionPez.z = -9.1f;   //Más adelante de pared trasera (Z=-10)
		if (posicionPez.x > 9.1f) posicionPez.x = 9.1f;     //Más adentro pared derecha (X=10)
		if (posicionPez.x < 6.9f) posicionPez.x = 6.9f;     //Más adentro pared izquierda (X=6)

		//Actualizar posición de la luz puntual del pez (sigue al pez)
		pointLights[2].SetPos(glm::vec3(posicionPez.x, posicionPez.y + 0.3f, posicionPez.z));

		//************** RENDERIZAR PECERA *****************
		modelPecera = glm::mat4(1.0f);
		modelPecera = glm::translate(modelPecera, glm::vec3(8.0f, 0.0f, -8.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPecera));

		//Blending para transparencia del vidrio
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);  //Mejora la transparencia

		//Paredes de vidrio
		texturaVidrio.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		peceraVidrio->RenderMesh();

		//Superficie del agua
		texturaAgua.UseTexture();
		peceraAgua->RenderMesh();

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

		//************ RENDERIZAR PEZ ***********
		// Cambiar el color del pez (azul abisal)
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.0f, 0.5f, 0.8f)));  //Azul abisal

		modelPez = glm::mat4(1.0f);
		modelPez = glm::translate(modelPez, posicionPez);
		modelPez = glm::scale(modelPez, glm::vec3(0.08f, 0.08f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPez));
		pezModel.RenderModel();

		//Actualizar spotlight del pez (posición y dirección)
		if (spotPezEncendido) {
			glm::vec3 posSpotPez = glm::vec3(posicionPez.x, posicionPez.y + 0.3f, posicionPez.z);
			spotLights[5].SetFlash(posSpotPez, direccionSpotPez);
		}

		// Restaurar color blanco para otros objetos
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

		//********** LAMPARA **********
		modelLampara = glm::mat4(1.0);
		modelLampara = glm::translate(modelLampara, glm::vec3(0.0f, -1.0f, 10.0f));
		modelLampara = glm::scale(modelLampara, glm::vec3(0.5f, 0.5f, 0.5f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLampara));
		Lampara_M.RenderModel();

		//Actualizar la posición de la luz puntual
		glm::vec3 lamparaPos = glm::vec3(modelLampara[3]);
		pointLights[1].SetPos(lamparaPos);

		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
