#include <iostream>
#include "GL\glew.h"
#include "GL\wglew.h"
#include "GL\glext.h"
#include "glm\glm.hpp"
#include"GL\freeglut.h"
using glm::mat4;
using glm::vec4;
using glm::vec3;

#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include <gl\GLU.h>
#include <gl\glew.h>
#include "glew-master\include\GL\glew.h"
#include <Windows.h>
#include "glx\glext.h"

//#include "Vertex.h"
//#include "Shader.h"
//#include "Texture.h"
#include "GameObject.h"
#include "Transform.h"
//#include "Material.h"
#include "Camera.h"
#include "Light.h"
#include "CameraController.h"
#include "Input.h"
#include "Timer.h"
//nclude  "WNDManager.h"
#include "SDL2-devel-2.0.3-VC\SDL2-2.0.3\include\SDL.h"
#include "SDL2_image-devel-2.0.0-VC\SDL2_image-2.0.0\include\SDL_image.h"
#include "SDL2_ttf-devel-2.0.12-VC\SDL2_ttf-2.0.12\include\SDL_ttf.h"
#include "SDL2-devel-2.0.3-VC\SDL2-2.0.3\include\SDL_opengl.h"
//nclude "WindowOGL.h"


//SDL Window
SDL_Window * window = NULL;
//SDL GL Context
SDL_GLContext glcontext = NULL;

//Window Width
const int WINDOW_WIDTH = 1280;
//Window Height
const int WINDOW_HEIGHT = 960;

bool running = true;




vec4 ambientLightColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
std::vector<GameObject*> displayList;
GameObject * mainCamera;
GameObject * mainLight;
CameraController * controller;
GameObject * skyBox = NULL;
	//Attach our example to our window


	// Attach the keyboard manager


	//Attempt to create the window

void InitWindow(int width, int height, bool fullscreen)
{
	//Create a window
	window = SDL_CreateWindow(
		"Star Chaser",     // window title
		SDL_WINDOWPOS_CENTERED,     // x position, centered
		SDL_WINDOWPOS_CENTERED,     // y position, centered
		width,                        // width, in pixels
		height,                        // height, in pixels
		SDL_WINDOW_OPENGL           // flags
		);
}

void CleanUp()
{

	if (skyBox)
	{
		skyBox->destroy();
		delete skyBox;
		skyBox = NULL;
	}

	auto iter = displayList.begin();
	while (iter != displayList.end())
	{
		(*iter)->destroy();
		if ((*iter))
		{
			delete (*iter);
			(*iter) = NULL;
			iter = displayList.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	displayList.clear();

	Input::getInput().destroy();

	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}
	void initOpenGL()
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		glcontext = SDL_GL_CreateContext(window);

		glewExperimental = GL_TRUE;
		

		//Smooth shading
		glShadeModel(GL_SMOOTH);

		//clear the background to black
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		//Clear the depth buffer
		glClearDepth(1.0f);

		//Enable depth testing
		glEnable(GL_DEPTH_TEST);

		//The depth test to go
		glDepthFunc(GL_LEQUAL);

		//Turn on best perspective correction
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	}
	void setViewport(int width, int height)
	{

		//height must always be 1 or above.
		if (height == 0) {
			height = 1;
		}


		//Creation of the viewport
		glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	}

void CheckForErrors()
	{
		GLenum error;
		do{
			error = glGetError();
		} 
		while (error != GL_NO_ERROR);
	}

void initInput()
{
	Input::getInput().init();
}



void Initialise()
{
	mainCamera = new GameObject();
	mainCamera->setName("MainCamera");

	Transform *t = new Transform();
	t->setPosition(0.0f, 0.0f, 8.0f);
	t->setRotation(0.0f, -glm::radians(180.0f), 0.0f);
	mainCamera->setTransform(t);

	Camera * c = new Camera();
	c->setAspectRatio((float)(WINDOW_WIDTH/ WINDOW_HEIGHT));
	c->setFOV(45.0f);
	c->setNearClip(0.1f);
	c->setFarClip(1000.0f);

	mainCamera->setCamera(c);
	controller = new CameraController();
	controller->setCamera(c);

	mainCamera->addComponent(controller);

	displayList.push_back(mainCamera);
	mainLight = new GameObject();
	mainLight->setName("MainLight");

	t = new Transform();
	t->setPosition(0.0f, 0.0f, 0.0f);
	mainLight->setTransform(t);

	Light * light = new Light();
	mainLight->setLight(light);
	displayList.push_back(mainLight);

	for (auto iter = displayList.begin(); iter != displayList.end(); iter++)
	{
		(*iter)->init();
	}

	Timer::getTimer().start();
}

void update()
{
	

	for (auto iter = displayList.begin(); iter != displayList.end(); iter++)
	{
		(*iter)->update();
	}
	Input::getInput().update();
}


void renderGameObject(GameObject * pObject)
{
	if (!pObject)
		return;

	pObject->render();

	//Mesh * currentMesh = pObject->getMesh();
	Transform * currentTransform = pObject->getTransform();
	//Material * currentMaterial = (Material*)pObject->getMaterial();

/*	if (currentMesh && currentMaterial && currentTransform)
	{
		currentMaterial->bind();
		currentMesh->bind();

		GLint MVPLocation = currentMaterial->getUniformLocation("MVP");
		GLint ModelLocation = currentMaterial->getUniformLocation("Model");
		GLint ambientMatLocation = currentMaterial->getUniformLocation("ambientMaterialColour");
		GLint ambientLightLocation = currentMaterial->getUniformLocation("ambientLightColour");
		GLint diffuseMatLocation = currentMaterial->getUniformLocation("diffuseMaterialColour");
		GLint diffuseLightLocation = currentMaterial->getUniformLocation("diffuseLightColour");
		GLint lightDirectionLocation = currentMaterial->getUniformLocation("lightDirection");
		GLint specularMatLocation = currentMaterial->getUniformLocation("specularMaterialColour");
		GLint specularLightLocation = currentMaterial->getUniformLocation("specularLightColour");
		GLint specularpowerLocation = currentMaterial->getUniformLocation("specularPower");
		GLint cameraPositionLocation = currentMaterial->getUniformLocation("cameraPosition");
		GLint diffuseTextureLocation = currentMaterial->getUniformLocation("diffuseMap");
		GLint specularTextureLocation = currentMaterial->getUniformLocation("specularMap");
		GLint bumpTextureLocation = currentMaterial->getUniformLocation("bumpMap");
		Camera * cam = mainCamera->getCamera();
		Light* light = mainLight->getLight();


		mat4 MVP = cam->getProjection()*cam->getView()*currentTransform->getModel();
		mat4 Model = currentTransform->getModel();

		vec4 ambientMaterialColour = currentMaterial->getAmbientColour();
		vec4 diffuseMaterialColour = currentMaterial->getDiffuseColour();
		vec4 specularMaterialColour = currentMaterial->getSpecularColour();
		float specularPower = currentMaterial->getSpecularPower();

		vec4 diffuseLightColour = light->getDiffuseColour();
		vec4 specularLightColour = light->getSpecularColour();
		vec3 lightDirection = light->getDirection();

		vec3 cameraPosition = mainCamera->getTransform()->getPosition();

		glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, glm::value_ptr(Model));
		glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(MVP));
		glUniform4fv(ambientMatLocation, 1, glm::value_ptr(ambientMaterialColour));
		glUniform4fv(ambientLightLocation, 1, glm::value_ptr(ambientLightColour));

		glUniform4fv(diffuseMatLocation, 1, glm::value_ptr(diffuseMaterialColour));
		glUniform4fv(diffuseLightLocation, 1, glm::value_ptr(diffuseLightColour));
		glUniform3fv(lightDirectionLocation, 1, glm::value_ptr(lightDirection));

		glUniform4fv(specularMatLocation, 1, glm::value_ptr(specularMaterialColour));
		glUniform4fv(specularLightLocation, 1, glm::value_ptr(specularLightColour));

		glUniform3fv(cameraPositionLocation, 1, glm::value_ptr(cameraPosition));
		glUniform1f(specularpowerLocation, specularPower);

		glUniform1i(diffuseTextureLocation, 0);
		glUniform1i(specularTextureLocation, 1);
		glUniform1i(bumpTextureLocation, 2);

		glDrawElements(GL_TRIANGLES, currentMesh->getIndexCount(), GL_UNSIGNED_INT, 0);


	}*/

	for (int i = 0; i < pObject->getChildCount(); i++)
	{
		renderGameObject(pObject->getChild(i));
	}
}

void render()
{

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	

	for (auto iter = displayList.begin(); iter != displayList.end(); iter++)
	{
		renderGameObject((*iter));
	}

	SDL_GL_SwapWindow(window);

}

 int main(int nArgs, char **args)
{

	 // Initilalising everything.
	 if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	 {
		 std::cout << "ERROR SDL_Init " << SDL_GetError() << std::endl;

		 return -1;
	 }

	 int imageInitFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	 int returnInitFlags = IMG_Init(imageInitFlags);
	 if (((returnInitFlags)& (imageInitFlags)) != imageInitFlags) {
		 std::cout << "ERROR SDL_Image Init " << IMG_GetError() << std::endl;
	 }

	 if (TTF_Init() == -1) {
		 std::cout << "TTF_Init: " << TTF_GetError();
	 }


	 InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false);
	 SDL_WarpMouseInWindow(window, (1280 / 2), (960 / 2));

	 initOpenGL();
	 CheckForErrors();

	 setViewport(WINDOW_WIDTH, WINDOW_HEIGHT);
	 initInput();
	 Initialise();

	 SDL_Event event;
	 //The Game Loop
	 while (running)

	 {
		 Timer::getTimer().update();
		 //While we still have events in the queue
		 while (SDL_PollEvent(&event)) {
			 switch (event.type)
			 {
			 case SDL_QUIT:
			 case SDL_WINDOWEVENT_CLOSE:
			 {
				 running = false;
				 break;
			 }
			 case SDL_KEYDOWN:

			 {
				 if (event.key.keysym.sym == (SDLK_ESCAPE))
				 {
					 running = false;
					 break;
				 }
				 else{
					 Input::getInput().getKeyboard()->setKeyDown(event.key.keysym.sym);
				 }
				 break;
			 }

			 case SDL_KEYUP:
			 {
				 Input::getInput().getKeyboard()->setKeyUp(event.key.keysym.sym);
				 break;
			 }

			 case SDL_MOUSEMOTION:
			 {
				 int xID = 0;
				 int xRelID = 0;
				 int yID = 0;
				 int yRelID = 0;

				 if (event.motion.x > Mouse::DeadzoneNeg && event.motion.x < Mouse::DeadzonePos)
				 {
					 Input::getInput().getMouse()->setMousePosition(xID, yID, xRelID, yRelID);
				 }
				 else{
					 Input::getInput().getMouse()->setMousePosition(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
				 }
				 if (event.motion.y > Mouse::DeadzoneNeg && event.motion.y < Mouse::DeadzonePos)
				 {
					 Input::getInput().getMouse()->setMousePosition(xID, yID, xRelID, yRelID);
				 }
				 else{
					 Input::getInput().getMouse()->setMousePosition(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
				 }

				 break;
			 }
			 }
		 }
		 update();
		 render();
	 }
	 CleanUp();

	 return 0;
 }