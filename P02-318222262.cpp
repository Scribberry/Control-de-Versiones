//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
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
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
//std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";

//Declarar la existencia de los distintos shaders
static const char* vShaderRojo = "shaders/shaderrojo.vert";
static const char* fShaderRojo = "shaders/shaderrojo.frag";
//static const char* vShaderColor = "shaders/shadercolor.vert";
//static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
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
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

/* void CrearLetrasyFiguras()
{

	GLfloat vertices_trianguloazul[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,

	};

	MeshColor* trianguloazul = new MeshColor();
	trianguloazul->CreateMeshColor(vertices_trianguloazul, 18);
	meshColorList.push_back(trianguloazul);

	GLfloat vertices_trianguloverde[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.5f,	0.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.5f,	0.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.5f,	0.0f,

	};

	MeshColor* trianguloverde = new MeshColor();
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
	meshColorList.push_back(trianguloverde);

	GLfloat vertices_cuadradorojo[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,

	};

	MeshColor* cuadradorojo = new MeshColor();
	cuadradorojo->CreateMeshColor(vertices_cuadradorojo, 36);
	meshColorList.push_back(cuadradorojo);

	GLfloat vertices_cuadradoverde[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,

	};

	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);

	GLfloat vertices_cuadradocafe[] = {
	-0.5f, -0.5f, 0.5f,   0.478f, 0.255f, 0.067f,
	 0.5f, -0.5f, 0.5f,   0.478f, 0.255f, 0.067f,
	 0.5f,  0.5f, 0.5f,   0.478f, 0.255f, 0.067f,
	-0.5f, -0.5f, 0.5f,   0.478f, 0.255f, 0.067f,
	 0.5f,  0.5f, 0.5f,   0.478f, 0.255f, 0.067f,
	-0.5f,  0.5f, 0.5f,   0.478f, 0.255f, 0.067f,
	};

	MeshColor* cuadradocafe = new MeshColor();
	cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);
	meshColorList.push_back(cuadradocafe);

	GLfloat vertices_letras[] = {	
			//X			Y			Z			R		G		B
			-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			
	};
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,18);
	meshColorList.push_back(letras);
} */


void CreateShaders()
{

	//Se añaden los shaders recién creados

	Shader shaderrojo;
	shaderrojo.CreateFromFiles("shaders/shaderrojo.vert", "shaders/shaderrojo.frag");
	shaderList.push_back(shaderrojo);

	Shader shaderverde;
	shaderverde.CreateFromFiles("shaders/shaderverde.vert", "shaders/shaderverde.frag");
	shaderList.push_back(shaderverde);

	Shader shaderazul;
	shaderazul.CreateFromFiles("shaders/shaderazul.vert", "shaders/shaderazul.frag");
	shaderList.push_back(shaderazul);

	Shader shadercafe;
	shadercafe.CreateFromFiles("shaders/shadercafe.vert", "shaders/shadercafe.frag");
	shaderList.push_back(shadercafe);

	Shader shaderverdeoscuro;
	shaderverdeoscuro.CreateFromFiles("shaders/shaderverdeoscuro.vert", "shaders/shaderverdeoscuro.frag");
	shaderList.push_back(shaderverdeoscuro);

	/* Shader* shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1); */

	/* Shader* shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2); */
}


int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	glEnable(GL_DEPTH_TEST);
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	//CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(1.0f,1.0f,1.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad

		//Techo Azul (Pirámide)

		shaderList[2].useShader(); //Para usar el shader Azul

		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.65f, -3.0f));
		model = glm::scale(model, glm::vec3(0.625f, 0.35f, 0.2f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[0]->RenderMesh(); //Pirámide

		//Cuerpo Rojo (Cubo)

		shaderList[0].useShader(); //Para usar el shader Rojo

		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.295f, -3.0f));
		model = glm::scale(model, glm::vec3(1.05f, 1.4f, 0.5f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[1]->RenderMesh(); //Cubo

		//Pirámide Verde Izquierda

		shaderList[1].useShader(); //Para usar el shader Verde

		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.05f, -0.295f, -3.0f));
		model = glm::scale(model, glm::vec3(0.625f, 1.4f, 0.2f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[0]->RenderMesh(); //Pirámide

		//Píramide Verde Derecha

		shaderList[1].useShader(); //Para usar el shader Verde

		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.05f, -0.295f, -3.0f));
		model = glm::scale(model, glm::vec3(0.625f, 1.4f, 0.2f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[0]->RenderMesh(); //Pirámide

		//Ventana Izquierda (Verde Oscuro)

		shaderList[4].useShader(); //Para el shader Verde Oscuro

		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.24f, -0.05f, -2.8f));
		model = glm::scale(model, glm::vec3(0.34f, 0.45f, 0.3f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[1]->RenderMesh(); //Cubo

		//Ventana Derecha (Verde Oscuro)

		shaderList[4].useShader();

		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.24f, -0.05f, -2.8f));
		model = glm::scale(model, glm::vec3(0.34f, 0.45f, 0.3f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[1]->RenderMesh();

		//Ventana Inferior (Verde Oscuro)

		shaderList[4].useShader();

		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.7684f, -2.8f));
		model = glm::scale(model, glm::vec3(0.34f, 0.45f, 0.3f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[1]->RenderMesh();

		//Tronco Café Izquierdo

		shaderList[3].useShader(); //Para usar el shader Café

		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.79f, -0.84825f, -2.8f));
		model = glm::scale(model, glm::vec3(0.225f, 0.295f, 0.3f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[1]->RenderMesh();

		//Tronco Café Derecho

		shaderList[3].useShader();

		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.79f, -0.84825f, -2.8f));
		model = glm::scale(model, glm::vec3(0.225f, 0.295f, 0.3f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[1]->RenderMesh();
													
		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		/* shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation(); */
		
		/* Comentado para tenerlo presente sin usarlo
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas del Triángulo Azul
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.65f, -1.0f));
		model = glm::scale(model, glm::vec3(0.625f, 0.35f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor(); //Para indicar la posición del Triángulo Azul en el orden

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas del Triángulo Verde Derecho
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.79f, -0.35f, -1.0f));
		model = glm::scale(model, glm::vec3(0.21f, 0.35f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshColorList[1]->RenderMeshColor(); //Para indicar la posición del Triángulo Verde en el orden

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas del Triángulo Verde Izquierdo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.79f, -0.35f, -1.0f));
		model = glm::scale(model, glm::vec3(0.21f, 0.35f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshColorList[1]->RenderMeshColor(); //Para indicar la posición del Triángulo Verde en el orden

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas del Cuadrado Rojo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.295f, -1.0f));
		model = glm::scale(model, glm::vec3(1.05f, 1.4f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshColorList[2]->RenderMeshColor(); //Para indicar la posición del Cuadrado Rojo en el orden

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas del Cuadrado Verde Izquierdo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.24f, -0.05f, -0.9f));
		model = glm::scale(model, glm::vec3(0.34f, 0.4532f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshColorList[3]->RenderMeshColor(); //Para indicar la posición del Cuadrado Verde en el orden

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas del Cuadrado Verde Derecho
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.24f, -0.05f, -0.9f));
		model = glm::scale(model, glm::vec3(0.34f, 0.4532f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshColorList[3]->RenderMeshColor(); //Para indicar la posición del Cuadrado Verde en el orden

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas del Cuadrado Verde de Abajo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.7684f, -0.9f));
		model = glm::scale(model, glm::vec3(0.34f, 0.4532f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshColorList[3]->RenderMeshColor(); //Para indicar la posición del Cuadrado Verde en el orden

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas del Cuadrado Café Izquierdo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.79f, -0.84825f, -0.8f));
		model = glm::scale(model, glm::vec3(0.2250f, 0.2950f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshColorList[4]->RenderMeshColor(); //Para indicar la posición del Cuadrado Café en el orden

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas del Cuadrado Café Izquierdo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.79f, -0.84825f, -0.8f));
		model = glm::scale(model, glm::vec3(0.2250f, 0.2950f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshColorList[4]->RenderMeshColor(); //Para indicar la posición del Cuadrado Café en el orden 
		*/

		////Para el cubo y la pirámide se usa el primer set de shaders con índice 0 en ShaderList
		//shaderList[0].useShader(); 
		//uniformModel = shaderList[0].getModelLocation();
		//uniformProjection = shaderList[0].getProjectLocation();
		//angulo += 0.01;
		////Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshList[1]->RenderMesh();


		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 1.2f, -3.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//meshList[0]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/