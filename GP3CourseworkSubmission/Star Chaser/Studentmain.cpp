#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one


//#include <windows.h>
#include "GameConstants.h"
#include "windowOGL.h"
#include "cWNDManager.h"
#include "cColours.h"
#include "cShapes.h"

#include "cSphere.h"
#include "cMaterial.h"
#include "cLight.h"
#include "cStarfield.h"
#include "cFontMgr.h"
#include "cCamera.h"
#include "cInputMgr.h"
#include "cSoundMgr.h"
#include "cModelLoader.h"
#include "cModel.h"
#include "cPlayer.h"
#include "cSound.h"

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR cmdLine,
                   int cmdShow)
{

    //Set our window settings
    const int windowWidth = 1024;
    const int windowHeight = 768;
    const int windowBPP = 16;

	float lookAt = 5.0f;

    //This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

	// This is the input manager
	static cInputMgr* theInputMgr = cInputMgr::getInstance();

	// This is the Font manager
	static cFontMgr* theFontMgr = cFontMgr::getInstance();

	// This is the sound manager
	static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
	
	//The example OpenGL code
    windowOGL theOGLWnd;

    //Attach our example to our window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);

	// Attach the keyboard manager
	pgmWNDMgr->attachInputMgr(theInputMgr);

	//Creating the sun and earth spheres
	cSphere theSun(4, 40, 40);
	cSphere theEarth(1, 40, 40);

    //Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
    {
        //If it fails

        MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	if (!theOGLWnd.initOGL(windowWidth, windowHeight)) //Initialize our example
    {
        MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	// Creates Texture maps. These textures will be used for many of the obkects rendered in the scene.
	cTexture tardisTexture;
	tardisTexture.createTexture("Models/tardis.png");
	cTexture spaceShipTexture;
	spaceShipTexture.createTexture("Models/SpaceShip/sh3.jpg");
	cTexture laserTexture;
	laserTexture.createTexture("Models/laser.tga");
	cTexture starTexture;
	starTexture.createTexture("Images/star.png");
	cTexture sunTexture;
	sunTexture.createTexture("Images/sun.png");
	cTexture earthTexture;
	earthTexture.createTexture("Images/EarthFlip.png");


	//Initialising the sun and earth
	theSun.initialise(sunTexture.getTexture(), glm::vec3(10, 10, 0), glm::vec3(0,0,0));
	theEarth.initialise(earthTexture.getTexture(), glm::vec3(5, 10, 10), glm::vec3(0, 0, 0));
	// the starfield
	cStarfield theStarField(starTexture.getTexture(), glm::vec3(50.0f, 50.0f, 50.0f));

	// Create Materials for lights
	cMaterial sunMaterial(lightColour4(0.0f, 0.0f, 0.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(0, 0, 0, 1.0f), 5.0f);
	cMaterial earthMaterial(lightColour4(0.2f, 0.2f, 0.2f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(0, 0, 0, 1.0f), 50.0f);
	// Create Lights
	cLight sunLight(GL_LIGHT0, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(0, 0, 20, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	cLight lfLight(GL_LIGHT1, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(30, 0, 100, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	cLight rfLight(GL_LIGHT2, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(-30, 0, 100, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	cLight cbLight(GL_LIGHT3, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(0, 0, -100, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	//Define Ambient light for scene
	GLfloat g_Ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, g_Ambient);

	//Creates a list of fonts and loads them
	LPCSTR gameFonts[3] = { "Fonts/digital-7.ttf", "Fonts/space age.ttf", "Fonts/doctor_who.ttf" };
	
	theFontMgr->addFont("SevenSeg", gameFonts[0], 24);
	theFontMgr->addFont("Space", gameFonts[1], 24);
	theFontMgr->addFont("DrWho", gameFonts[2], 48);

	//Creates a list of sounds and loads them
	LPCSTR gameSounds[3] = { "Audio/who10Edit.wav", "Audio/shot007.wav", "Audio/explosion2.wav" };


	theSoundMgr->add("Theme", gameSounds[0]);
	theSoundMgr->add("Shot", gameSounds[1]);
	theSoundMgr->add("Explosion", gameSounds[2]);

	// Create a camera
	cCamera theCamera;
	theCamera.setTheCameraPos(glm::vec3(0.0f, 0.0f, 30.0f));
	theCamera.setTheCameraLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	theCamera.setTheCameraUpVector(glm::vec3(0.0f, 1.0f, 0.0f)); // pointing upwards in world space
	theCamera.setTheCameraAspectRatio(windowWidth, windowHeight);
	theCamera.setTheProjectionMatrix(45.0f, theCamera.getTheCameraAspectRatio(), 0.1f, 5000.0f);
	theCamera.update();

	//Clear key buffers
	theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);

	// Loading the models
	cModelLoader tardisMdl;
	tardisMdl.loadModel("Models/tardis1314.obj", tardisTexture); // Player

	cModelLoader spaceShipMdl;
	spaceShipMdl.loadModel("Models/SpaceShip/Sample_Ship.obj", spaceShipTexture); // Enemy

	//Initalising the player objects. These are used for the controllable player and the enemies

	//the player
	cPlayer thePlayer;
	thePlayer.initialise(glm::vec3(0, 0, -15), 0.0f, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 5.0f, true);

	thePlayer.setMdlDimensions(tardisMdl.getModelDimensions());
	thePlayer.attachInputMgr(theInputMgr);
	thePlayer.attachSoundMgr(theSoundMgr);


	//the enemies
	cPlayer theEnemyShip;
	theEnemyShip.initialise(glm::vec3(10, 3, 0), 270.0f, glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), 5.0f, true);
	theEnemyShip.setPosition(glm::vec3(10, 3, 0));
	theEnemyShip.setMdlDimensions(spaceShipMdl.getModelDimensions());
	theEnemyShip.attachInputMgr(theInputMgr);
	theEnemyShip.attachSoundMgr(theSoundMgr);

	cPlayer theEnemyShip4;
	theEnemyShip4.initialise(glm::vec3(30, -4, 10), 270.0f, glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), 5.0f, true);
	theEnemyShip4.setPosition(glm::vec3(30, -4, 10));
	theEnemyShip4.setMdlDimensions(spaceShipMdl.getModelDimensions());
	theEnemyShip4.attachInputMgr(theInputMgr);
	theEnemyShip4.attachSoundMgr(theSoundMgr);

	cPlayer theEnemyShip2;
	theEnemyShip2.initialise(glm::vec3(25, 5, 20), 270.0f, glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), 5.0f, true);
	theEnemyShip2.setMdlDimensions(spaceShipMdl.getModelDimensions());
	theEnemyShip2.setPosition(glm::vec3(25, 5, 0));
	theEnemyShip2.attachInputMgr(theInputMgr);
	theEnemyShip2.attachSoundMgr(theSoundMgr);

	cPlayer theEnemyShip3;
	theEnemyShip3.initialise(glm::vec3(20, 1, 0), 270.0f, glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), 5.0f, true);
	theEnemyShip3.setMdlDimensions(spaceShipMdl.getModelDimensions());
	theEnemyShip3.setPosition(glm::vec3(20, 1, 0));
	theEnemyShip3.attachInputMgr(theInputMgr);
	theEnemyShip3.attachSoundMgr(theSoundMgr);
	


	cPlayer theEnemyShip1;
	theEnemyShip1.initialise(glm::vec3(15, 1, -10), 270.0f, glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), 5.0f, true);
	theEnemyShip1.setMdlDimensions(spaceShipMdl.getModelDimensions());
	theEnemyShip1.setPosition(glm::vec3(15, 1, -10));
	theEnemyShip1.attachInputMgr(theInputMgr);
	theEnemyShip1.attachSoundMgr(theSoundMgr);
	float tCount = 0.0f;
	string outputMsg;
	

	


   //This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
    {
		pgmWNDMgr->processWNDEvents(); //Process any window events

        //We get the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();
		
		//Clear the buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Initialise OpenGL
		theOGLWnd.initOGL(windowWidth,windowHeight);

		//Set matrix Mode to Model View. Ensures the models load properly.
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glLoadMatrixf((GLfloat*)&theCamera.getTheViewMatrix());

		//Render the star field
		theStarField.render(0.0f);
		//render the sun, set its materials and turn on its assinged light.
		theSun.prepare(0.0f);
		sunMaterial.useMaterial();
		sunLight.lightOn();
		theSun.render(0.0f);
		
		//Turn on ambient lights
		lfLight.lightOn();
		rfLight.lightOn();
		cbLight.lightOn();

		glPushMatrix();
		//render the earth.
		theEarth.prepare(0.0f);
		earthMaterial.useMaterial();
		theEarth.render(0.0f);

		
		//Render the player
		tardisMdl.renderMdl(thePlayer.getPosition(), thePlayer.getRotation(), thePlayer.getScale());

		//render the enemies
		if (theEnemyShip.isActive() == true)
		{
			spaceShipMdl.renderMdl(theEnemyShip.getPosition(), theEnemyShip.getRotation(), theEnemyShip.getScale());
		}

		if (theEnemyShip1.isActive() == true)
		{
			spaceShipMdl.renderMdl(theEnemyShip1.getPosition(), theEnemyShip1.getRotation(), theEnemyShip1.getScale());
		}
		if (theEnemyShip2.isActive() == true)
		{
			spaceShipMdl.renderMdl(theEnemyShip2.getPosition(), theEnemyShip2.getRotation(), theEnemyShip2.getScale());
		}

		if (theEnemyShip3.isActive() == true)
		{
			spaceShipMdl.renderMdl(theEnemyShip3.getPosition(), theEnemyShip3.getRotation(), theEnemyShip3.getScale());
		}

		if (theEnemyShip4.isActive() == true)
		{
			spaceShipMdl.renderMdl(theEnemyShip4.getPosition(), theEnemyShip4.getRotation(), theEnemyShip4.getScale());
		}

		

		spaceShipMdl.renderMdl(theEnemyShip2.getPosition(), theEnemyShip2.getRotation(), theEnemyShip2.getScale());
		thePlayer.update(elapsedTime);

		//Update Camera
		theCamera.update();

		//This section of the code inteacts with the input logic. It calls methods in the player class in order to return boolean values edited by the input.
		//With htis, switching the camera, movement of models and toggling of sound is enabled.
		//moving forward
		if (thePlayer.MoveForward() == true)
		{
						
			if (thePlayer.returnDebugState() == false)
			{
				theEnemyShip.setPosition(glm::vec3(theEnemyShip.getPosition().x, theEnemyShip.getPosition().y, theEnemyShip.getPosition().z - 5.0f));
				theEnemyShip1.setPosition(glm::vec3(theEnemyShip1.getPosition().x, theEnemyShip1.getPosition().y, theEnemyShip1.getPosition().z - 5.0f));
				theEnemyShip2.setPosition(glm::vec3(theEnemyShip2.getPosition().x, theEnemyShip2.getPosition().y, theEnemyShip2.getPosition().z - 5.0f));
				theEnemyShip3.setPosition(glm::vec3(theEnemyShip3.getPosition().x, theEnemyShip3.getPosition().y, theEnemyShip3.getPosition().z - 5.0f));
				theEnemyShip4.setPosition(glm::vec3(theEnemyShip4.getPosition().x, theEnemyShip4.getPosition().y, theEnemyShip4.getPosition().z - 5.0f));

				thePlayer.Reset();

				if (theEnemyShip.SphereSphereCollision(thePlayer.getPosition(), 10 ) == true)
				{
					//theEnemyShip.~cModel();
					theEnemyShip.setIsActive(false);

				}
				if (theEnemyShip1.SphereSphereCollision(thePlayer.getPosition(), thePlayer.getMdlRadius()) == true)
				{
					theEnemyShip1.setIsActive(false);

				}
			}
			else if (thePlayer.returnDebugState() == true)
			{
				theEnemyShip1.setPosition(glm::vec3(theEnemyShip1.getPosition().x - 5.0f, theEnemyShip1.getPosition().y, theEnemyShip1.getPosition().z));
				theEnemyShip2.setPosition(glm::vec3(theEnemyShip2.getPosition().x - 5.0f, theEnemyShip2.getPosition().y, theEnemyShip2.getPosition().z));
				theEnemyShip3.setPosition(glm::vec3(theEnemyShip3.getPosition().x - 5.0f, theEnemyShip3.getPosition().y, theEnemyShip3.getPosition().z));
				theEnemyShip4.setPosition(glm::vec3(theEnemyShip4.getPosition().x - 5.0f, theEnemyShip4.getPosition().y, theEnemyShip4.getPosition().z));
				thePlayer.setPosition(glm::vec3(thePlayer.getPosition().x - 5.0f, thePlayer.getPosition().y, thePlayer.getPosition().z));
				thePlayer.Reset();
			}
			
			
			
		}
		//toggle sound
		if (thePlayer.hasToggledSound == true)
		{

			theSoundMgr->getSnd("Theme")->stopAudio();
			thePlayer.SoundToggle = false;
		}

		if (thePlayer.hasToggledSound == false)
		{
			if (thePlayer.SoundToggle == false)
			{
				theSoundMgr->getSnd("Theme")->playAudio(AL_LOOPING);
				thePlayer.SoundToggle = true;
			}
		}

		//moving backward
		if (thePlayer.MoveBackward() == true)
		{


			if (thePlayer.returnDebugState() == false)
			{
				theEnemyShip.setPosition(glm::vec3(theEnemyShip.getPosition().x, theEnemyShip.getPosition().y, theEnemyShip.getPosition().z + 5.0f));
				theEnemyShip1.setPosition(glm::vec3(theEnemyShip1.getPosition().x, theEnemyShip1.getPosition().y, theEnemyShip1.getPosition().z + 5.0f));
				theEnemyShip2.setPosition(glm::vec3(theEnemyShip2.getPosition().x, theEnemyShip2.getPosition().y, theEnemyShip2.getPosition().z + 5.0f));
				theEnemyShip3.setPosition(glm::vec3(theEnemyShip3.getPosition().x, theEnemyShip3.getPosition().y, theEnemyShip3.getPosition().z + 5.0f));
				theEnemyShip4.setPosition(glm::vec3(theEnemyShip4.getPosition().x, theEnemyShip4.getPosition().y, theEnemyShip4.getPosition().z + 5.0f));
				thePlayer.Reset();
			}
			else
			{
				theEnemyShip.setPosition(glm::vec3(theEnemyShip.getPosition().x + 5.0f, theEnemyShip.getPosition().y, theEnemyShip.getPosition().z));
				theEnemyShip1.setPosition(glm::vec3(theEnemyShip1.getPosition().x + 5.0f, theEnemyShip1.getPosition().y, theEnemyShip1.getPosition().z));
				theEnemyShip2.setPosition(glm::vec3(theEnemyShip2.getPosition().x + 5.0f, theEnemyShip2.getPosition().y, theEnemyShip2.getPosition().z));
				theEnemyShip3.setPosition(glm::vec3(theEnemyShip3.getPosition().x + 5.0f, theEnemyShip3.getPosition().y, theEnemyShip3.getPosition().z));
				theEnemyShip4.setPosition(glm::vec3(theEnemyShip4.getPosition().x + 5.0f, theEnemyShip4.getPosition().y, theEnemyShip4.getPosition().z));
				thePlayer.setPosition(glm::vec3(thePlayer.getPosition().x + 5.0f, thePlayer.getPosition().y, thePlayer.getPosition().z));
				thePlayer.Reset();
			}
		}
		//moving left

		if (thePlayer.MoveLeft() == true)
		{
			if (thePlayer.returnDebugState() == false)
			{
				thePlayer.setPosition(glm::vec3(thePlayer.getPosition().x - 1.0f, thePlayer.getPosition().y, thePlayer.getPosition().z));
				thePlayer.Reset();
			}

		}
		//moving right
		if (thePlayer.MoveRight() == true)
		{
			if (thePlayer.returnDebugState() == false)
			{
				thePlayer.setPosition(glm::vec3(thePlayer.getPosition().x + 1.0f, thePlayer.getPosition().y, thePlayer.getPosition().z));
				thePlayer.Reset();
			}

		}
		//moving up
		if (thePlayer.MoveUp() == true)
		{
			if (thePlayer.returnDebugState() == false)
			{
				thePlayer.setPosition(glm::vec3(thePlayer.getPosition().x, thePlayer.getPosition().y + 1.0f, thePlayer.getPosition().z));
				thePlayer.Reset();
			}



		}
		//moving down
		if (thePlayer.MoveDown() == true)
		{
			if (thePlayer.returnDebugState()==false)
			{

				thePlayer.setPosition(glm::vec3(thePlayer.getPosition().x, thePlayer.getPosition().y - 1.0f, thePlayer.getPosition().z));
				thePlayer.Reset();
			}



		}
		//changing the camera
		if (thePlayer.ChangingCam() == true)
		{
			if (thePlayer.returnDebugState() == false)
			{
				thePlayer.enteredDebugState();
				theCamera.setTheCameraPos(glm::vec3(-50.0f, 0.0f, 0.0f));
				theCamera.setTheCameraLookAt(glm::vec3(90.0f, 0, 0));
				thePlayer.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
				theEnemyShip.setPosition(glm::vec3(10, 3, 0));
				theEnemyShip1.setPosition(glm::vec3(15.0f, 1.0f, 0.0f));
				theEnemyShip3.setPosition(glm::vec3(20, 1, 0));
				theEnemyShip4.setPosition(glm::vec3(30, -4, 10));
				theEnemyShip2.setPosition(glm::vec3(25, 5, 0));
				thePlayer.Reset();
			}
			else
			{
				thePlayer.enteredDebugState();
				theCamera.setTheCameraPos(glm::vec3(0.0f, 0.0f, 30.0f));
				theCamera.setTheCameraLookAt(glm::vec3(0, 0, 0));
				thePlayer.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
				theEnemyShip.setPosition(glm::vec3(10, 3, 0));
				theEnemyShip1.setPosition(glm::vec3(15.0f, 1.0f, 0.0f));
				theEnemyShip3.setPosition(glm::vec3(20, 1, 0));
				theEnemyShip4.setPosition(glm::vec3(30, -4, 10));
				theEnemyShip2.setPosition(glm::vec3(25, 5, 0));
				thePlayer.Reset();

			}
			
		}

		
		
		
		glPushMatrix();

		//changes the camera to orthographic to draw text
		theOGLWnd.setOrtho2D(windowWidth, windowHeight);
		theFontMgr->getFont("Space")->printText("Star Chaser", FTPoint(10, 35, 0.0f), colour3f(255.0f,0.0f,0.0f));
		
		glPopMatrix();

		//swap buffers
		pgmWNDMgr->swapBuffers();
		
		//increment the elapsed time.
		tCount += elapsedTime;

		//Clear key buffers
		theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);
		
	}

	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window

    return 0; //Return success
}
