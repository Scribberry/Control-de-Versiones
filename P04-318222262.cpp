/*Práctica 4: Modelado Jerárquico.
Se implementa el uso de matrices adicionales para almacenar información de transformaciones geométricas que se quiere 
heredar entre diversas instancias para que estén unidas
Teclas de la F a la K para rotaciones de articulaciones
*/
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
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
Sphere sp = Sphere(0.80, 20, 20); //recibe radio, slices, stacks

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
/*
Crear cilindro y cono con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
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

}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	glEnable(GL_DEPTH_TEST);
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(15, 1.0f);//índice 2 en MeshList
	CrearCono(25, 2.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
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
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.2f, 0.2f);
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);
	
	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Matriz Principal
	glm::mat4 modelaux(1.0);//Para Brazos y Articulaciones
	glm::mat4 modelaux1(1.0);  //Para Base y Llantas
	glm::mat4 modelaux2(1.0); //Inicializar para el Tercer Brazo
	glm::mat4 modelaux3(1.0); //Inicializar para la Cuarta Articulación
	//Inicializar para la Mascota
	glm::mat4 modelaux4(1.0);
	glm::mat4 modelaux5(1.0);

	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f); //inicializar Color para enviar a variable Uniform;

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
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		/* ¡COMENTADO PARA NO OBSTACULIZAR EL CÓDIGO DE LA MASCOTA!
		
		// ========================== EMPIEZA GRUA ==================================

		//**************** CABINA ****************
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, -4.0f));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		modelaux = model; //Matriz auxiliar para Brazos
		modelaux1 = model; //Matriz auxiliar para Base y Llantas

		model = glm::scale(model, glm::vec3(5.0f, 3.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f); //Rojo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		meshList[0]->RenderMesh(); //Cubo

		//**************** ARTICULACIÓN 1 Y BRAZO 1 ****************
		model = modelaux;
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f); //Azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Esfera como Articulación

		//**************** BRAZO 1 ****************
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));

		modelaux = model;
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 1.0f));
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));				
		color = glm::vec3(1.0f, 0.0f, 1.0f); //Rosa
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //Cubo
		
		//**************** ARTICULACIÓN 2 ****************
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); //Esfera

		//**************** BRAZO 2 ****************
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 1.0f, 0.0f); //Verde
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh(); //Cubo

		//**************** ARTICULACIÓN 3 (Extremo Derecho del Segundo Brazo) ****************
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); //Esfera

		//**************** BRAZO 3 (Ejercicio) ****************
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));

		modelaux = model; //Guardamos para la siguiente Articulación
		model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));

		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 1.0f, 1.0f); // Cyan
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh(); //Cubo

		//**************** ARTICULACIÓN 4 (Ejercicio) ****************
		//Nos movemos al extremo del tercer brazo
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(1.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		//**************** CANASTA ****************
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		
		modelaux = model;
		//Escalamos para dar forma de canasta
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//Dibujamos la canasta (usamos un cubo)
		color = glm::vec3(0.0f, 0.5f, 0.5f); //Verde Azulado
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh(); //Cubo

		//**************** BASE ****************
		model = modelaux1;
		model = glm::translate(model, glm::vec3(0.0f, -2.25f, 0.0f));

		modelaux1 = model;
		model = glm::scale(model, glm::vec3(5.0f, 3.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//Dibujamos la base con una pirámide
		color = glm::vec3(1.0f, 1.0f, 0.0f); //Amarillo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[4]->RenderMesh(); //Pirámide Cuadrangular

		//**************** LLANTA DELANTERA IZQUIERDA ****************
		model = modelaux1;
		model = glm::translate(model, glm::vec3(-2.5f, -1.0f, 1.5f));

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.75f, 1.0f, 1.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//Dibujamos la llanta con un cilindro
		color = glm::vec3(0.5f, 0.5f, 0.5f); //Gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry(); //Cilindro

		//**************** LLANTA DELANTERA DERECHA ****************
		model = modelaux1;
		model = glm::translate(model, glm::vec3(-2.5f, -1.0f, -1.5f));
		
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.75f, 1.0f, 1.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

	    //**************** LLANTA TRASERA IZQUIERDA ****************
		model = modelaux1;
		model = glm::translate(model, glm::vec3(2.5f, -1.0f, 1.5f));
		
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.75f, 1.0f, 1.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		//**************** LLANTA TRASERA DERECHA ****************
		model = modelaux1;
		model = glm::translate(model, glm::vec3(2.5f, -1.0f, -1.5f));
		
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.75f, 1.0f, 1.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		// ========================== TERMINA GRUA ==================================

		*/

		// ========================== EMPIEZA MASCOTA ==================================
		//Una criatura de 4 patas con cabeza y cola articuladas

		//**************** CUERPO (Nodo Raíz) ****************
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 4.0f)); //Posición diferente a la grúa
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		
		//Guardamos la posición del cuerpo para todas las partes
		modelaux = model;   //Para cabeza y cuello
		modelaux1 = model;  //Para pata frontal derecha
		modelaux2 = model;  //Para pata frontal izquierda
		modelaux3 = model;  //Para pata trasera derecha
		modelaux4 = model;  //Para pata trasera izquierda
		modelaux5 = model;  //Para cola
		
		//Dibujamos el cuerpo (cubo alargado | color naranja)
		model = glm::scale(model, glm::vec3(5.0f, 2.5f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.9f, 0.5f, 0.2f); // Naranja
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh(); //Cubo
		
		// ==================== PATA FRONTAL DERECHA ====================
		//Usando articulaciones 1 y 2
		
		//************* ARTICULACIÓN 1 (Cadera Frontal Derecha) ************* 
		model = modelaux1;
		model = glm::translate(model, glm::vec3(-1.8f, -1.2f, -1.2f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.8f, 0.0f); //Dorado
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();
		
		//************* MUSLO FRONTAL DERECHO *************
		model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.0f));
		modelaux1 = model;
		model = glm::scale(model, glm::vec3(0.8f, 2.4f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.8f, 0.4f, 0.1f); //Marrón Claro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry(); //Cilindro
		
		//************* ARTICULACIÓN 2 (Rodilla Frontal Derecha) *************
		model = modelaux1;
		model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		
		//************* PIERNA FRONTAL DERECHA *************
		model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.0f));
		modelaux1 = model;
		model = glm::scale(model, glm::vec3(0.8f, 2.4f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.8f, 0.4f, 0.1f); // Marrón claro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();
		
		//************* PIE FRONTAL DERECHO *************
		model = modelaux1;
		model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.6f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.8f, 0.0f); // Dorado
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();
		
		// ==================== PATA FRONTAL IZQUIERDA ====================
		//Usando articulaciones 3 y 4
		
		//************* ARTICULACIÓN 3 (Cadera Frontal Izquierda) *************
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.8f, -1.2f, 1.2f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.8f, 0.0f); //Dorado
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();
		
		//************* MUSLO FRONTAL IZQUIERDO *************
		model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.8f, 2.4f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.8f, 0.4f, 0.1f); //Marrón Claro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();
		
		//************* ARTICULACIÓN 4 (Rodilla Frontal Izquierda) *************
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		
		//************* PIERNA FRONTAL IZQUIERDA *************
		model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.8f, 2.4f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.8f, 0.4f, 0.1f); //Marrón Claro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();
		
		//************* PIE FRONTAL IZQUIERDO *************
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.6f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.8f, 0.0f); //Dorado
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();
		
		// ==================== PATA TRASERA DERECHA ====================
		//Usando articulaciones 5 y 6
		
		//************* ARTICULACIÓN 5 (Cadera Trasera Derecha) *************
		model = modelaux3;
		model = glm::translate(model, glm::vec3(1.8f, -1.2f, -1.2f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.8f, 0.0f); //Dorado
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();
		
		//************* MUSLO TRASERO DERECHO *************
		model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.0f));
		modelaux3 = model;
		model = glm::scale(model, glm::vec3(0.8f, 2.4f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.8f, 0.4f, 0.1f); //Marrón Claro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();
		
		//************* ARTICULACIÓN 6 (Rodilla Trasera Derecha) *************
		model = modelaux3;
		model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		
		//************* PIERNA TRASERA DERECHA *************
		model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.0f));
		modelaux3 = model;
		model = glm::scale(model, glm::vec3(0.8f, 2.4f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.8f, 0.4f, 0.1f); //Marrón Claro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		//************* PIE TRASERO DERECHO *************
		model = modelaux3;
		model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.6f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.8f, 0.0f); //Dorado
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();
		
		// ==================== PATA TRASERA IZQUIERDA ====================
		// Usando articulaciones 7 y 8
		
		//************* ARTICULACIÓN 7 (Cadera Trasera Izquierda) *************
		model = modelaux4;
		model = glm::translate(model, glm::vec3(1.8f, -1.2f, 1.2f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.8f, 0.0f); //Dorado
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();
		
		//************* MUSLO TRASERO IZQUIERDO *************
		model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.0f));
		modelaux4 = model;
		model = glm::scale(model, glm::vec3(0.8f, 2.4f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.8f, 0.4f, 0.1f); // Marrón claro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();
		
		//************* ARTICULACIÓN 8 (Rodilla Trasera Izquierda) *************
		model = modelaux4;
		model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		
		//************* PIERNA TRASERA IZQUIERDA *************
		model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.0f));
		modelaux4 = model;
		model = glm::scale(model, glm::vec3(0.8f, 2.4f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.8f, 0.4f, 0.1f); //Marrón Claro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();
		
		//************* PIE TRASERO IZQUIERDO *************
		model = modelaux4;
		model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.6f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.8f, 0.0f); // Dorado
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();
		
		// ==================== CUELLO Y CABEZA ====================
		//Usando articulaciones 9 y 10
		 
		//************* ARTICULACIÓN 9 (Base del Cuello) *************
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.5f, 0.5f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.8f, 0.0f); //Dorado
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();
		
		//************* CUELLO (Cilindro Alargado) *************
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.8f, 4.0f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.8f, 0.4f, 0.1f); //Marrón Claro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();
		
		//************* ARTICULACIÓN 10 (Unión Cuello-Cabeza) *************
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion10()), glm::vec3(1.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		
		//************* CABEZA (Base) *************
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(3.0f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.9f, 0.5f, 0.2f); //Naranja (igual que el Cuerpo)
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		//************* NARIZ *************
		model = modelaux;  // Usamos la posición guardada de la cabeza
		// Ajustamos para que quede en el centro de la cara (sin escala)
		model = glm::translate(model, glm::vec3(-2.5f, 0.2f, 0.0f)); // Ligeramente adelante y arriba
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); // Negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();
		
		//************* CRÁNEO (Parte Superior de la Cabeza) *************
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.5f, 1.2f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(2.0f, 1.5f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.8f, 0.0f); //Dorado
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();
		
		//************* OJO DERECHO *************
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.2f, 0.5f, -0.8f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); // Negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

        //************* OJO IZQUIERDO *************
        model = modelaux;
        model = glm::translate(model, glm::vec3(-1.2f, 0.5f, 0.8f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(0.0f, 0.0f, 0.0f); //Negro
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        sp.render();
		
		//************* OREJA DERECHA *************
        model = modelaux;
        model = glm::translate(model, glm::vec3(1.2f, 1.2f, -1.0f));
        model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.5f, 2.5f, 0.8f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(0.8f, 0.4f, 0.1f); //Marrón Claro
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[0]->RenderMesh(); //Cubo

        //************* OREJA IZQUIERDA *************
        model = modelaux;
        model = glm::translate(model, glm::vec3(1.2f, 1.2f, 1.0f));
        model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.5f, 2.5f, 0.8f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(0.8f, 0.4f, 0.1f); //Marrón Claro
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[0]->RenderMesh(); //Cubo

        // ==================== COLA ====================
        //Usando articulación 11

        //************* ARTICULACIÓN 11 (Base de la Cola) *************
        model = modelaux5;
        model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(mainWindow.getarticulacion11()), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(1.0f, 0.8f, 0.0f); //Dorado
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        sp.render();

        //************* COLA (Cono) *************
        modelaux5 = model;
        model = glm::translate(model, glm::vec3(1.5f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.6f, 4.0f, 0.6f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        color = glm::vec3(0.8f, 0.4f, 0.1f); //Marrón Claro
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[3]->RenderMeshGeometry(); //Cono

        // ========================== TERMINA MASCOTA ==================================

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

	
		