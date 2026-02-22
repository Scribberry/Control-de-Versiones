#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>
#include <stdlib.h> //Nueva librería
#include <time.h> // Nueva librería

//Variables globales para el color de fondo RGB
float r = 0.0f;
float g = 0.0f;
float b = 0.0f;

//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 800;
GLuint VAO, VBO, shader;

//Se añaden los identificadores para cada letra

GLuint VAO_P, VBO_P;
GLuint VAO_A, VBO_A;
GLuint VAO_O, VBO_O;

//LENGUAJE DE SHADER (SOMBRAS) GLSL
//Vertex Shader
//recibir color, salida Vcolor
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";

//Fragment Shader
// Fragment shader incluye 'uColor' que nos permite dibujar todas las letras con el mismo color
static const char* fShader = "						\n\
#version 330										\n\
uniform vec3 uColor;								\n\
out vec4 color;                                     \n\
void main()											\n\
{													\n\
	color = vec4(uColor, 1.0f);         	 	    \n\
}";

//Funciones para las Letras

// Cada función define los triángulos que forman la letra correspondiente

//Se establecen los triángulos para la letra P
void CrearP() {
	GLfloat vertices[] = {

		//Barra vertical izquierda
		-0.9f, -0.9f, 0.0f,
		-0.8f, -0.9f, 0.0f,
		-0.8f, -0.3f, 0.0f,

		-0.9f, -0.9f, 0.0f,
		-0.8f, -0.3f, 0.0f,
		-0.9f, -0.3f, 0.0f,

		//Barra superior horizontal
		-0.8f, -0.3f, 0.0f,
		-0.6f, -0.3f, 0.0f,
		-0.6f, -0.4f, 0.0f,

		-0.8f, -0.3f, 0.0f,
		-0.6f, -0.4f, 0.0f,
		-0.8f, -0.4f, 0.0f,

		//Lado derecho del semicírculo
		-0.6f, -0.3f, 0.0f,
		-0.5f, -0.3f, 0.0f,
		-0.5f, -0.6f, 0.0f,

		-0.6f, -0.3f, 0.0f,
		-0.5f, -0.6f, 0.0f,
		-0.6f, -0.6f, 0.0f,

		//Barra media horizontal
		-0.8f, -0.6f, 0.0f,
		-0.6f, -0.6f, 0.0f,
		-0.6f, -0.7f, 0.0f,

		-0.8f, -0.6f, 0.0f,
		-0.6f, -0.7f, 0.0f,
		-0.8f, -0.7f, 0.0f
	};

	glGenVertexArrays(1, &VAO_P);
	glBindVertexArray(VAO_P);

	glGenBuffers(1, &VBO_P);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_P);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

//Se establecen los triángulos para la letra A
void CrearA() {
	GLfloat vertices[] = {

		//Barra izquierda
		-0.15f, -0.4f, 0.0f,
		-0.05f, -0.4f, 0.0f,
		-0.05f,  0.2f, 0.0f,

		-0.15f, -0.4f, 0.0f,
		-0.05f,  0.2f, 0.0f,
		-0.15f,  0.2f, 0.0f,

		//Barra derecha
		 0.15f, -0.4f, 0.0f,
		 0.25f, -0.4f, 0.0f,
		 0.25f,  0.2f, 0.0f,

		 0.15f, -0.4f, 0.0f,
		 0.25f,  0.2f, 0.0f,
		 0.15f,  0.2f, 0.0f,

		 //Barra superior
		 -0.15f, 0.1f, 0.0f,
		  0.25f, 0.1f, 0.0f,
		  0.25f, 0.2f, 0.0f,

		 -0.15f, 0.1f, 0.0f,
		  0.25f, 0.2f, 0.0f,
		 -0.15f, 0.2f, 0.0f,

		 //Barra central
	     -0.15f, -0.1f, 0.0f,
	      0.25f, -0.1f, 0.0f,
	      0.25f,  0.0f, 0.0f,

	     -0.15f, -0.1f, 0.0f,
	      0.25f,  0.0f, 0.0f,
	     -0.15f,  0.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO_A);
	glBindVertexArray(VAO_A);

	glGenBuffers(1, &VBO_A);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_A);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

//Se establecen los triángulos para la letra O
void CrearO() {
	GLfloat vertices[] = {

		//Izquierda
		0.6f, 0.0f, 0.0f,
		0.7f, 0.0f, 0.0f,
		0.7f, 0.6f, 0.0f,

		0.6f, 0.0f, 0.0f,
		0.7f, 0.6f, 0.0f,
		0.6f, 0.6f, 0.0f,

		//Derecha
		0.9f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.6f, 0.0f,

		0.9f, 0.0f, 0.0f,
		1.0f, 0.6f, 0.0f,
		0.9f, 0.6f, 0.0f,

		//Arriba
		0.6f, 0.5f, 0.0f,
		1.0f, 0.5f, 0.0f,
		1.0f, 0.6f, 0.0f,

		0.6f, 0.5f, 0.0f,
		1.0f, 0.6f, 0.0f,
		0.6f, 0.6f, 0.0f,

		//Abajo
		0.6f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.1f, 0.0f,

		0.6f, 0.0f, 0.0f,
		1.0f, 0.1f, 0.0f,
		0.6f, 0.1f, 0.0f
	};

	glGenVertexArrays(1, &VAO_O);
	glBindVertexArray(VAO_O);

	glGenBuffers(1, &VBO_O);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_O);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) //Función para agregar los shaders a la tarjeta gráfica

//the Program recibe los datos de theShader

{
	GLuint theShader = glCreateShader(shaderType);//theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
	const GLchar* theCode[1];
	theCode[0] = shaderCode;//shaderCode es el texto que se le pasa a theCode
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);//longitud del texto
	glShaderSource(theShader, 1, theCode, codeLength);//Se le asigna al shader el código
	glCompileShader(theShader);//Se comila el shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	//verificaciones y prevención de errores
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n", shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);//Si no hubo problemas se asigna el shader a theProgram el cual asigna el código a la tarjeta gráfica
}

void CompileShaders() {
	shader = glCreateProgram(); //se crea un programa
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader
	//Para terminar de linkear el programa y ver que no tengamos errores
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);//se linkean los shaders a la tarjeta gráfica
	//verificaciones y prevención de errores
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}



}
int main()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}

	//****  LAS SIGUIENTES 4 LÍNEAS SE COMENTAN EN DADO CASO DE QUE AL USUARIO NO LE FUNCIONE LA VENTANA Y PUEDA CONOCER LA VERSIÓN DE OPENGL QUE TIENE ****/

	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Primer ventana", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	srand(time(NULL)); //Inicialización que los colores del fondo sean aleatorios

	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);

	//Crear las letras en memoria GPU
	CrearP();
	CrearA();
	CrearO();

	CompileShaders();

	//Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow))
	{
		//Recibir eventos del usuario
		glfwPollEvents();

		//Limpiar la ventana
		// glClearColor(0.0f,0.0f,0.0f,1.0f); Reemplacé esta línea, para que no se quede un color sólido nada más

		//Función para cambiar el color del fondo cada 2 segundos
		static float ultimoCambio = 0.0f;
		float tiempo = glfwGetTime();

		if (tiempo - ultimoCambio >= 2.0f)
		{
			float r = (float)rand() / RAND_MAX; //Color rojo aleatorio
			float g = (float)rand() / RAND_MAX; //Color verde aleatorio
			float b = (float)rand() / RAND_MAX; //Color azul aleatorio

			glClearColor(r, g, b, 1.0f);

			ultimoCambio = tiempo;
		}

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		//Para que todas las letras tengan el mismo color
		GLint colorLocation = glGetUniformLocation(shader, "uColor");
		glUniform3f(colorLocation, 0.0f, 0.0f, 0.0f); //Color negro

		//Para dibujar la P
		glBindVertexArray(VAO_P);
		glDrawArrays(GL_TRIANGLES, 0, 24);

		//Para dibujar la A
		glBindVertexArray(VAO_A);
		glDrawArrays(GL_TRIANGLES, 0, 24);

		//Para dibujar la O
		glBindVertexArray(VAO_O);
		glDrawArrays(GL_TRIANGLES, 0, 24);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);

		//NO ESCRIBIR NINGUNA LÍNEA DESPUÉS DE glfwSwapBuffers(mainWindow); 
	}


	return 0;
}