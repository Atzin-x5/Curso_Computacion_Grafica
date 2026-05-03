//Atzin Ugalde Santos
//319057399
//Skybox
//06/05/2026



#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cmath>
#include <cstdio>
#include <vector>   

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"


// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();

GLuint loadCubemap(std::vector<const GLchar*> faces);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};


GLfloat skyboxVertices[] = {
	// Positions
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

GLuint indices[] =
{  // Note that we start from 0!
	0,1,2,3,
	4,5,6,7,
	8,9,10,11,
	12,13,14,15,
	16,17,18,19,
	20,21,22,23,
	24,25,26,27,
	28,29,30,31,
	32,33,34,35
};


glm::vec3 Light1 = glm::vec3(0);
//Anim
float rotBall = 0.0f;
float rotDog = 0.0f;
int dogAnim = 0;
float FLegs = 0.0f;
float RLegs = 0.0f;
float head = 0.0f;
float tail = 0.0f;


//Variables independientes para cada pata

float FLLeg = 0.0f;  //Pata Frontal Izquierda

float FRLeg = 0.0f;  //Pata Frontal Derecha

float BLLeg = 0.0f;  //Pata Trasera Izquierda

float BRLeg = 0.0f;  //Pata Trasera Derecha


//Inclinacion del tronco (rotacion sobre eje X) - solo afecta cuerpo, cabeza y cola
float bodyTilt = 0.0f;

//===============
//Volteo del tronco (rotacion sobre eje Z) - para hacerse el muertito
float bodyRoll = 0.0f;

//===============
//Slot de animacion actual (1, 2 o 3) para guardar/cargar archivos
int currentAnimSlot = 1;


//KeyFrames
float dogPosX, dogPosY, dogPosZ;

#define MAX_FRAMES 15
int i_max_steps = 80;
int i_curr_steps = 0;
typedef struct _frame {

	float rotDog;
	float rotDogInc;
	float dogPosX;
	float dogPosY;
	float dogPosZ;
	float incX;
	float incY;
	float incZ;
	float head;
	float headInc;

	//===============
	//Cola
	float tail;
	//===============
	float tailInc;
	//===============
	//Pata Frontal Izquierda
	float FLLeg;
	//===============
	float FLLegInc;
	//===============
	//Pata Frontal Derecha
	float FRLeg;
	//===============
	float FRLegInc;
	//===============
	//Pata Trasera Izquierda
	float BLLeg;
	//===============
	float BLLegInc;
	//===============
	//Pata Trasera Derecha
	float BRLeg;
	//===============
	float BRLegInc;
	//===============
	//Inclinacion del tronco
	float bodyTilt;
	//===============
	float bodyTiltInc;
	//===============
	//Volteo del tronco (para hacerse el muertito)
	float bodyRoll;
	//===============
	float bodyRollInc;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].dogPosX = dogPosX;
	KeyFrame[FrameIndex].dogPosY = dogPosY;
	KeyFrame[FrameIndex].dogPosZ = dogPosZ;

	KeyFrame[FrameIndex].rotDog = rotDog;
	KeyFrame[FrameIndex].head = head;

	//===============
	KeyFrame[FrameIndex].tail = tail;
	//===============
	KeyFrame[FrameIndex].FLLeg = FLLeg;
	//===============
	KeyFrame[FrameIndex].FRLeg = FRLeg;
	//===============
	KeyFrame[FrameIndex].BLLeg = BLLeg;
	//===============
	KeyFrame[FrameIndex].BRLeg = BRLeg;
	//===============
	KeyFrame[FrameIndex].bodyTilt = bodyTilt;
	//===============
	KeyFrame[FrameIndex].bodyRoll = bodyRoll;

	FrameIndex++;
}

void resetElements(void)
{
	dogPosX = KeyFrame[0].dogPosX;
	dogPosY = KeyFrame[0].dogPosY;
	dogPosZ = KeyFrame[0].dogPosZ;
	head = KeyFrame[0].head;

	rotDog = KeyFrame[0].rotDog;

	//===============
	tail = KeyFrame[0].tail;
	//===============
	FLLeg = KeyFrame[0].FLLeg;
	//===============
	FRLeg = KeyFrame[0].FRLeg;
	//===============
	BLLeg = KeyFrame[0].BLLeg;
	//===============
	BRLeg = KeyFrame[0].BRLeg;
	//===============
	bodyTilt = KeyFrame[0].bodyTilt;
	//===============
	bodyRoll = KeyFrame[0].bodyRoll;

}
void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].dogPosX - KeyFrame[playIndex].dogPosX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].dogPosY - KeyFrame[playIndex].dogPosY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].dogPosZ - KeyFrame[playIndex].dogPosZ) / i_max_steps;
	KeyFrame[playIndex].headInc = (KeyFrame[playIndex + 1].head - KeyFrame[playIndex].head) / i_max_steps;

	KeyFrame[playIndex].rotDogInc = (KeyFrame[playIndex + 1].rotDog - KeyFrame[playIndex].rotDog) / i_max_steps;

	//===============
	KeyFrame[playIndex].tailInc = (KeyFrame[playIndex + 1].tail - KeyFrame[playIndex].tail) / i_max_steps;
	//===============
	KeyFrame[playIndex].FLLegInc = (KeyFrame[playIndex + 1].FLLeg - KeyFrame[playIndex].FLLeg) / i_max_steps;
	//===============
	KeyFrame[playIndex].FRLegInc = (KeyFrame[playIndex + 1].FRLeg - KeyFrame[playIndex].FRLeg) / i_max_steps;
	//===============
	KeyFrame[playIndex].BLLegInc = (KeyFrame[playIndex + 1].BLLeg - KeyFrame[playIndex].BLLeg) / i_max_steps;
	//===============
	KeyFrame[playIndex].BRLegInc = (KeyFrame[playIndex + 1].BRLeg - KeyFrame[playIndex].BRLeg) / i_max_steps;
	//===============
	KeyFrame[playIndex].bodyTiltInc = (KeyFrame[playIndex + 1].bodyTilt - KeyFrame[playIndex].bodyTilt) / i_max_steps;
	//===============
	KeyFrame[playIndex].bodyRollInc = (KeyFrame[playIndex + 1].bodyRoll - KeyFrame[playIndex].bodyRoll) / i_max_steps;

}


void saveAnimationToFile(void)
{
	char filename[64];
	sprintf(filename, "animacion%d.txt", currentAnimSlot);

	FILE* f = fopen(filename, "w");
	if (f == NULL)
	{
		printf("ERROR: No se pudo crear el archivo %s\n", filename);
		return;
	}

	//Primera linea: numero total de frames
	fprintf(f, "%d\n", FrameIndex);

	//Una linea por cada frame con todos los valores separados por espacio
	for (int i = 0; i < FrameIndex; i++)
	{
		fprintf(f, "%f %f %f %f %f %f %f %f %f %f %f %f\n",
			KeyFrame[i].dogPosX, KeyFrame[i].dogPosY, KeyFrame[i].dogPosZ,
			KeyFrame[i].rotDog, KeyFrame[i].head, KeyFrame[i].tail,
			KeyFrame[i].FLLeg, KeyFrame[i].FRLeg,
			KeyFrame[i].BLLeg, KeyFrame[i].BRLeg,
			KeyFrame[i].bodyTilt, KeyFrame[i].bodyRoll);
	}

	fclose(f);
	printf("Animacion guardada en %s (%d frames)\n", filename, FrameIndex);
}

//===============
//Carga una animacion desde el archivo .txt del slot seleccionado
//Retorna true si la cargo, false si el archivo no existe
bool loadAnimationFromFile(void)
{
	char filename[64];
	sprintf(filename, "animacion%d.txt", currentAnimSlot);

	FILE* f = fopen(filename, "r");
	if (f == NULL)
	{
		//Archivo no existe: no es error, solo significa que el slot esta vacio
		return false;
	}

	//Lee el numero total de frames
	fscanf(f, "%d", &FrameIndex);
	if (FrameIndex > MAX_FRAMES) FrameIndex = MAX_FRAMES;

	//Lee cada frame
	for (int i = 0; i < FrameIndex; i++)
	{
		fscanf(f, "%f %f %f %f %f %f %f %f %f %f %f %f",
			&KeyFrame[i].dogPosX, &KeyFrame[i].dogPosY, &KeyFrame[i].dogPosZ,
			&KeyFrame[i].rotDog, &KeyFrame[i].head, &KeyFrame[i].tail,
			&KeyFrame[i].FLLeg, &KeyFrame[i].FRLeg,
			&KeyFrame[i].BLLeg, &KeyFrame[i].BRLeg,
			&KeyFrame[i].bodyTilt, &KeyFrame[i].bodyRoll);
	}

	fclose(f);
	printf("Animacion cargada desde %s (%d frames)\n", filename, FrameIndex);
	return true;
}

//===============
//Limpia todos los frames para empezar a grabar una animacion nueva
void clearFrames(void)
{
	FrameIndex = 0;
	printf("Frames limpiados. Listo para grabar nueva animacion.\n");
}


GLuint loadCubemap(std::vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		if (image == NULL)
		{
			printf("ERROR: No se pudo cargar la cara del cubemap: %s\n", faces[i]);
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}



// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Animacion maquina de estados - Atzin", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	Shader skyboxshader("Shader/SkyBox.vs", "Shader/SkyBox.frag");





	//models
	Model DogBody((char*)"Models/DogBody.obj");
	Model HeadDog((char*)"Models/HeadDog.obj");
	Model DogTail((char*)"Models/TailDog.obj");
	Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
	Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
	Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
	Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
	Model Piso((char*)"Models/piso.obj");
	Model Ball((char*)"Models/ball.obj");


	//KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].dogPosX = 0;
		KeyFrame[i].dogPosY = 0;
		KeyFrame[i].dogPosZ = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotDog = 0;
		KeyFrame[i].rotDogInc = 0;
		KeyFrame[i].head = 0;
		KeyFrame[i].headInc = 0;

		//===============
		KeyFrame[i].tail = 0;
		//===============
		KeyFrame[i].tailInc = 0;
		//===============
		KeyFrame[i].FLLeg = 0;
		//===============
		KeyFrame[i].FLLegInc = 0;
		//===============
		KeyFrame[i].FRLeg = 0;
		//===============
		KeyFrame[i].FRLegInc = 0;
		//===============
		KeyFrame[i].BLLeg = 0;
		//===============
		KeyFrame[i].BLLegInc = 0;
		//===============
		KeyFrame[i].BRLeg = 0;
		//===============
		KeyFrame[i].BRLegInc = 0;
		//===============
		KeyFrame[i].bodyTilt = 0;
		//===============
		KeyFrame[i].bodyTiltInc = 0;
		//===============
		KeyFrame[i].bodyRoll = 0;
		//===============
		KeyFrame[i].bodyRollInc = 0;
	}


	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//VAO/VBO/EBO del skybox
	GLuint skyboxVAO, skyboxVBO, skyboxEBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);

	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	std::vector<const GLchar*> faces;
	faces.push_back("SkyBox/right.jpg");
	faces.push_back("SkyBox/left.jpg");
	faces.push_back("SkyBox/top.jpg");
	faces.push_back("SkyBox/bottom.jpg");
	faces.push_back("SkyBox/back.jpg");
	faces.push_back("SkyBox/front.jpg");
	GLuint cubemapTexture = loadCubemap(faces);

	
	skyboxshader.Use();
	glUniform1i(glGetUniformLocation(skyboxshader.Program, "skybox"), 0);

	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);


	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		DoMovement();
		Animation();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);


		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp

		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));


		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		glm::mat4 view;
		view = camera.GetViewMatrix();

		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);



		//Carga de modelo 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		//Body
		modelTemp = model = glm::translate(model, glm::vec3(dogPosX, dogPosY, dogPosZ));
		modelTemp = model = glm::rotate(model, glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f));
		//===============
		//Pivote de la inclinacion: movemos el origen a las caderas (parte trasera)
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.22f));
		//===============
		//Aplicamos la inclinacion del tronco SOLO al cuerpo, cabeza y cola
		model = glm::rotate(model, glm::radians(bodyTilt), glm::vec3(1.0f, 0.0f, 0.0f));
		//===============
		//Regresamos al origen para que cabeza y cola sigan posicionandose correctamente
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.22f));
		//===============
		//Aplicamos el volteo del tronco (eje Z) - para hacerse el muertito
		model = glm::rotate(model, glm::radians(bodyRoll), glm::vec3(0.0f, 0.0f, 1.0f));
		//===============
		//Guardamos la matriz con inclinacion para que cabeza y cola la hereden
		glm::mat4 modelTempTronco = model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogBody.Draw(lightingShader);
		//Head
		//===============
		model = modelTempTronco;
		model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
		model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HeadDog.Draw(lightingShader);
		//Tail 
		//===============
		model = modelTempTronco;
		model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
		model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogTail.Draw(lightingShader);
		//Front Left Leg
		//===============
		//Las patas delanteras heredan modelTempTronco para seguir al cuerpo cuando se inclina
		model = modelTempTronco;
		model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
		//===============
		//Ahora usa FLLeg en lugar de FLegs
		model = glm::rotate(model, glm::radians(FLLeg), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_LeftLeg.Draw(lightingShader);
		//Front Right Leg
		//===============
		//Las patas delanteras heredan modelTempTronco para seguir al cuerpo cuando se inclina
		model = modelTempTronco;
		model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
		//===============
		//Ahora usa FRLeg en lugar de FLegs
		model = glm::rotate(model, glm::radians(FRLeg), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_RightLeg.Draw(lightingShader);
		//Back Left Leg
		//===============
		//Las patas traseras tambien heredan modelTempTronco para seguir al cuerpo
		model = modelTempTronco;
		model = glm::translate(model, glm::vec3(0.082f, -0.046, -0.218));
		//===============
		//Ahora usa BLLeg en lugar de RLegs
		model = glm::rotate(model, glm::radians(BLLeg), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_LeftLeg.Draw(lightingShader);
		//Back Right Leg
		//===============
		//Las patas traseras tambien heredan modelTempTronco para seguir al cuerpo
		model = modelTempTronco;
		model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
		//===============
		//Ahora usa BRLeg en lugar de RLegs
		model = glm::rotate(model, glm::radians(BRLeg), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_RightLeg.Draw(lightingShader);


		model = glm::mat4(1);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ball.Draw(lightingShader);
		glDisable(GL_BLEND);
		glBindVertexArray(0);


		lampShader.Use();
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		
		glDepthFunc(GL_LEQUAL);
		skyboxshader.Use();
		
		glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(skyboxView));
		glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		
		glDepthFunc(GL_LESS);
		

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	//Dog Controls

	//===============
	//Cabeza arriba
	if (keys[GLFW_KEY_1])
	{
		head += 1.0f;
	}
	//===============
	//Cabeza abajo
	if (keys[GLFW_KEY_2])
	{
		head -= 1.0f;
	}

	//===============
	//Rotacion del perro hacia un lado
	if (keys[GLFW_KEY_3])
	{
		rotDog += 1.0f;
	}
	//===============
	//Rotacion del perro hacia el otro lado
	if (keys[GLFW_KEY_4])
	{
		rotDog -= 1.0f;
	}

	//===============
	//Inclinar tronco hacia atras (para sentarse)
	if (keys[GLFW_KEY_5])
	{
		bodyTilt += 2.0f;
	}
	//===============
	//Inclinar tronco hacia adelante
	if (keys[GLFW_KEY_6])
	{
		bodyTilt -= 2.0f;
	}

	//===============
	//Voltear tronco hacia un lado (para hacerse el muertito)
	if (keys[GLFW_KEY_7])
	{
		bodyRoll += 2.0f;
	}
	//===============
	//Voltear tronco hacia el otro lado
	if (keys[GLFW_KEY_8])
	{
		bodyRoll -= 2.0f;
	}

	if (keys[GLFW_KEY_H])
	{
		dogPosZ += 0.01;
	}

	if (keys[GLFW_KEY_Y])
	{
		dogPosZ -= 0.01;
	}

	if (keys[GLFW_KEY_G])
	{
		dogPosX -= 0.01;
	}

	if (keys[GLFW_KEY_J])
	{
		dogPosX += 0.01;
	}

	//===============
	//Mover el tronco (cuerpo) hacia arriba en Y
	if (keys[GLFW_KEY_I])
	{
		dogPosY += 0.01f;
	}
	//===============
	//Mover el tronco (cuerpo) hacia abajo en Y
	if (keys[GLFW_KEY_O])
	{
		dogPosY -= 0.01f;
	}

	//===============
	//Controles de la cola
	if (keys[GLFW_KEY_Q])
	{
		tail += 1.0f;
	}
	//===============
	if (keys[GLFW_KEY_E])
	{
		tail -= 1.0f;
	}

	//===============
	//Controles Pata Frontal Izquierda (FL)
	if (keys[GLFW_KEY_Z])
	{
		FLLeg += 1.0f;
	}
	//===============
	if (keys[GLFW_KEY_X])
	{
		FLLeg -= 1.0f;
	}

	//===============
	//Controles Pata Frontal Derecha (FR)
	if (keys[GLFW_KEY_R])
	{
		FRLeg += 1.0f;
	}
	//===============
	if (keys[GLFW_KEY_F])
	{
		FRLeg -= 1.0f;
	}

	//===============
	//Controles Pata Trasera Izquierda (BL)
	if (keys[GLFW_KEY_C])
	{
		BLLeg += 1.0f;
	}
	//===============
	if (keys[GLFW_KEY_V])
	{
		BLLeg -= 1.0f;
	}

	//===============
	//Controles Pata Trasera Derecha (BR)
	if (keys[GLFW_KEY_B])
	{
		BRLeg += 1.0f;
	}
	//===============
	if (keys[GLFW_KEY_N])
	{
		BRLeg -= 1.0f;
	}

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{

	if (keys[GLFW_KEY_L])
	{
		if (play == false && (FrameIndex > 1))
		{

			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}

	}

	if (keys[GLFW_KEY_K])
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
			//===============
			printf("Keyframe %d/%d guardado en slot %d\n", FrameIndex, MAX_FRAMES, currentAnimSlot);
		}
		//===============
		else
		{
			printf("LIMITE ALCANZADO: ya tienes %d frames (max). Guarda con P y limpia con 0.\n", MAX_FRAMES);
		}

	}

	//===============
	//Cambiar al slot N: limpia los frames actuales y trata de cargar el archivo del slot
	//Si el archivo existe: deja la animacion lista para reproducir con L
	//Si no existe: deja los frames vacios listos para grabar con K
	if (keys[GLFW_KEY_F1])
	{
		currentAnimSlot = 1;
		FrameIndex = 0;
		play = false;
		printf("--- Slot %d seleccionado ---\n", currentAnimSlot);
		if (loadAnimationFromFile() == false)
		{
			printf("Slot vacio. Listo para grabar con K.\n");
		}
		else
		{
			printf("Animacion lista. Presiona L para reproducir.\n");
		}
	}
	//===============
	if (keys[GLFW_KEY_F2])
	{
		currentAnimSlot = 2;
		FrameIndex = 0;
		play = false;
		printf("--- Slot %d seleccionado ---\n", currentAnimSlot);
		if (loadAnimationFromFile() == false)
		{
			printf("Slot vacio. Listo para grabar con K.\n");
		}
		else
		{
			printf("Animacion lista. Presiona L para reproducir.\n");
		}
	}
	//===============
	if (keys[GLFW_KEY_F3])
	{
		currentAnimSlot = 3;
		FrameIndex = 0;
		play = false;
		printf("--- Slot %d seleccionado ---\n", currentAnimSlot);
		if (loadAnimationFromFile() == false)
		{
			printf("Slot vacio. Listo para grabar con K.\n");
		}
		else
		{
			printf("Animacion lista. Presiona L para reproducir.\n");
		}
	}
	//===============
	//Guardar animacion actual al archivo del slot
	if (keys[GLFW_KEY_P])
	{
		saveAnimationToFile();
	}
	//===============
	//Cargar animacion del archivo del slot manualmente
	if (keys[GLFW_KEY_M])
	{
		if (loadAnimationFromFile() == false)
		{
			printf("No hay archivo guardado en el slot %d\n", currentAnimSlot);
		}
	}
	//===============
	//Limpiar todos los frames para empezar nueva animacion
	if (keys[GLFW_KEY_0])
	{
		clearFrames();
	}
	//===============
	//Velocidad de animacion: aumentar i_max_steps = mas lento
	if (keys[GLFW_KEY_COMMA])
	{
		i_max_steps += 10;
		if (i_max_steps > 300) i_max_steps = 300;
		printf("Velocidad: %d pasos por segmento (mas lento)\n", i_max_steps);
	}
	//===============
	//Velocidad de animacion: disminuir i_max_steps = mas rapido
	if (keys[GLFW_KEY_PERIOD])
	{
		i_max_steps -= 10;
		if (i_max_steps < 20) i_max_steps = 20;
		printf("Velocidad: %d pasos por segmento (mas rapido)\n", i_max_steps);
	}



	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);

		}
		else
		{
			Light1 = glm::vec3(0);
		}
	}


}
void Animation() {

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			dogPosX += KeyFrame[playIndex].incX;
			dogPosY += KeyFrame[playIndex].incY;
			dogPosZ += KeyFrame[playIndex].incZ;
			head += KeyFrame[playIndex].headInc;

			rotDog += KeyFrame[playIndex].rotDogInc;

			//===============
			tail += KeyFrame[playIndex].tailInc;
			//===============
			FLLeg += KeyFrame[playIndex].FLLegInc;
			//===============
			FRLeg += KeyFrame[playIndex].FRLegInc;
			//===============
			BLLeg += KeyFrame[playIndex].BLLegInc;
			//===============
			BRLeg += KeyFrame[playIndex].BRLegInc;
			//===============
			bodyTilt += KeyFrame[playIndex].bodyTiltInc;
			//===============
			bodyRoll += KeyFrame[playIndex].bodyRollInc;

			i_curr_steps++;
		}

	}

}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}