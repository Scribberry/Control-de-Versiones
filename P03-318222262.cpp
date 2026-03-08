//Práctica 3: Modelado Geométrico y Cámara Sintética.
//Paola Celina de Jesús Pérez Avin | Grupo 02
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z


using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<MeshColor*> meshColorList; //Se añade para tener objetos con color por vértice
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks
float angulo = 0.0f;  //Se agrega la variable angulo para una rotación automática

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}

//Funcion para crear la Pirámide Triángular de Colores

void CrearPiramideTriangularColores()
{
	GLfloat vertices_piramide_colores[] = {

		//Para la Cara Rosa Mexicano
		-0.5f, -0.5f,  0.0f,   1.0f, 0.0f, 0.5f,
		 0.5f, -0.5f,  0.0f,   1.0f, 0.0f, 0.5f,
		 0.0f,  0.5f, -0.25f,  1.0f, 0.0f, 0.5f,

		//Para la Cara Rosa Pastel
		 0.5f, -0.5f,  0.0f,   1.0f, 0.6f, 0.8f,
		 0.0f, -0.5f, -0.5f,   1.0f, 0.6f, 0.8f,
		 0.0f,  0.5f, -0.25f,  1.0f, 0.6f, 0.8f,

		//Para la Cara Lavanda
		 0.0f, -0.5f, -0.5f,   0.9f, 0.5f, 1.0f,
		-0.5f, -0.5f,  0.0f,   0.9f, 0.5f, 1.0f,
	     0.0f,  0.5f, -0.25f,  0.9f, 0.5f, 1.0f,

		//Para la Cara Naranja
		-0.5f, -0.5f,  0.0f,   1.0f, 0.4f, 0.4f,
		 0.5f, -0.5f,  0.0f,   1.0f, 0.4f, 0.4f,
	     0.0f, -0.5f, -0.5f,   1.0f, 0.4f, 0.4f
	};

	MeshColor* obj1 = new MeshColor(); //Para crear un objeto MeshColor
	//Inicializa la malla con 12 vértices (6 floats para cada uno)
	obj1->CreateMeshColor(vertices_piramide_colores, 72); //12 vértices*6floats=72
	meshColorList.push_back(obj1);
}

void CrearPiramideTriangularBordes()
{
	GLfloat vertices_piramide_bordes[] = {

		//Para la Cara Rosa Mexicano
	   -0.5f, -0.5f,  0.0f,   0.0f, 0.0f, 0.0f,
	    0.5f, -0.5f,  0.0f,   0.0f, 0.0f, 0.0f,
	    0.0f,  0.5f, -0.25f,  0.0f, 0.0f, 0.0f,

		//Para la Cara Rosa Pastel
		0.5f, -0.5f,  0.0f,   0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,
		0.0f,  0.5f, -0.25f,  0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,
	   -0.5f, -0.5f,  0.0f,   0.0f, 0.0f, 0.0f,
		0.0f,  0.5f, -0.25f,  0.0f, 0.0f, 0.0f,

	   -0.5f, -0.5f,  0.0f,   0.0f, 0.0f, 0.0f,
		0.5f, -0.5f,  0.0f,   0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f
	};

	MeshColor* obj2 = new MeshColor(); //Para crear un objeto MeshColor
	//Inicializa la malla con 12 vértices (6 floats para cada uno)
	obj2->CreateMeshColor(vertices_piramide_bordes, 72); //12 vértices*6floats=72
	meshColorList.push_back(obj2);
}

/*
Crear cilindro, cono y esferas con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0)*dt);
			z = R * sin((0)*dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los vértices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh *cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res,float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);
	
	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res+2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh *cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh *piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(5, 1.0f);//índice 2 en MeshList
	CrearCono(25, 2.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
	//Se añaden las pirámide de colores
	CrearPiramideTriangularColores(); //índice 0 en meshColorList
	CrearPiramideTriangularBordes(); //índice 1 en meshColorList
	CreateShaders();
	
	

	/*Cámara se usa el comando: glm::lookAt(vector de posición, vector de orientación, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posición,
	glm::vec3 vector up,
	GlFloat yaw rotación para girar hacia la derecha e izquierda
	GlFloat pitch rotación para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posición inicial está en 0,0,1 y ve hacia 0,0,-1.
	*/

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);

	
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	//GLuint uniformColor = 0; Porque no se usa
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);
	
	//Loop mientras no se cierra la ventana
    // sp.init(); // Comentada porque no se usa la esfera
    // sp.load(); // Comentada porque no se usa la esfera

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4

	//No se usa para esta ocasión
	//glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{
		
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //Para que el fondo sea de color Blanco
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
		//Usar el shader de color (índice 1)
		shaderList[1].useShader(); //Se cambia de shaderList[0] a shaderList[1]
		GLint uniformModel = shaderList[1].getModelLocation();
		GLint uniformView = shaderList[1].getViewLocation();
		GLint uniformProjection = shaderList[1].getProjectLocation();

		angulo += 0.05f;  //Incrementar ángulo para rotación automática

		//¡Ahora sí! Hora de hacer pirámide por pirámide

		//********** PIRÁMIDE 1 (Central Superior) **********
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f)); //Alejar en Z
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));  //Tecla E: rotación en X
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));  //Tecla R: rotación en Y
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));  //Tecla T: rotación en Z
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 0.0f)); //Rotación automática
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f)); //Subir en Y
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.732050806f)); //Estirar en Z para forma tetraédrica

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); //Matriz proyección perspectiva
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix())); //Vista de cámara
		meshColorList[0]->RenderMeshColor(); //Dibuja la pirámide con colores por cara
		meshColorList[1]->RenderMeshColorLines(); //Dibuja bordes negros

		//********** PIRÁMIDE 2 (Lateral Derecha Superior) **********
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.5f, 0.5f, 0.4330127017f));  //Posición en vértice del tetraedro
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.732050806f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();
		meshColorList[1]->RenderMeshColorLines();

		//********** PIRÁMIDE 3 (Lateral Izquierda Superior) **********
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.5f, 0.5f, 0.4330127017f)); //Posición simétrica en -X
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.732050806f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();
		meshColorList[1]->RenderMeshColorLines();

		//********** PIRÁMIDE 4 (Central Frente) **********
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.8660254036f)); //Posición inferior frontal
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.732050806f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();
		meshColorList[1]->RenderMeshColorLines();

		//********** PIRÁMIDE 5 (Derecha Frente) **********
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.0f, -0.5f, 0.8660254036f)); //Derecha y frontal
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.732050806f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();
		meshColorList[1]->RenderMeshColorLines();

		//********** PIRÁMIDE 6 (Izquierda Frente) **********
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-1.0f, -0.5f, 0.8660254036f)); //Izquierda y frontal
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.732050806f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();
		meshColorList[1]->RenderMeshColorLines();

		//********** PIRÁMIDE 7 (Central Superior Trasera) **********
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -0.4330127017f)); //Posición superior trasera
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.732050806f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();
		meshColorList[1]->RenderMeshColorLines();

		//********** PIRÁMIDE 8 (Central Inferior Trasera) **********
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -0.8660254036f)); //Posición inferior trasera
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.732050806f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();
		meshColorList[1]->RenderMeshColorLines();

		//**********  PIRÁMIDE 9 (Derecha Media) **********
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.5f, -0.5f, 0.0f));          //Posición lateral derecha media
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.732050806f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();
		meshColorList[1]->RenderMeshColorLines();

		//********** PIRÁMIDE 10 (Izquierda Media) **********
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.5f, -0.5f, 0.0f));         //Posición lateral izquierda media
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.732050806f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();
		meshColorList[1]->RenderMeshColorLines();

		//********** PIRÁMIDE 11 (Abarca Todo el Conjunto) **********
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.8660254036f));  //Posición central superior
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 5.19615242f));      //Escala 3x más grande

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshColorList[0]->RenderMeshColor();
		meshColorList[1]->RenderMeshColorLines();   //Dibuja bordes de la pirámide grande

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

	
		