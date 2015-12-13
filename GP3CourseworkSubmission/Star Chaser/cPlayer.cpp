#include "cPlayer.h"
//Constructor
cPlayer::cPlayer() : cModel()
{

}

//attaches the input Manager
void cPlayer::attachInputMgr(cInputMgr* inputMgr)
{
	m_InputMgr = inputMgr;
}

//Where most of the input logic takes place.Different boolean values are changed
// depnding on the key pressed. These values are then used in other methods to assist with logic in the main.
void cPlayer::update(float elapsedTime)
{
	//if left arrow if pressed move left
	if (m_InputMgr->isKeyDown(VK_LEFT))
	{
		hasMovedLeft = true;
		
	}
	//if right arrow is pressed move right
	if (m_InputMgr->isKeyDown(VK_RIGHT))
	{
		hasMovedRight = true;
	}
	//if up arrow is pressed move up
	if (m_InputMgr->isKeyDown(VK_UP))
	{
		hasMovedUp = true;
	}
	// if down arrow is pressed move down
	if (m_InputMgr->isKeyDown(VK_DOWN))
	{
		hasMovedDown = true;
	}
	//if W is pressed move forward
	if (m_InputMgr->isKeyDown(87))
	{
		//translationZ += 5.0f;
		//m_mdlPosition.z += 5.0f;
		hasMovedForward = true;
	

	}
	// If S is pressed move backward
	if (m_InputMgr->isKeyDown(83))
	{
		//translationZ -= 5.0f;
		//m_mdlPosition.z -= 5.0f;
		hasMovedBackward = true;
	}

	//If R is pressed change the camera
	if (m_InputMgr->isKeyDown(82))
	{
		if (changeCam == false)
		{
			changeCam = true;
		}

		else
		{
			changeCam == false;
		}
	}
	//If space is pressed togge sound
	if (m_InputMgr->isKeyDown(VK_SPACE))
	{
		if (hasToggledSound == true)
		{

			hasToggledSound = false;
			SoundToggle = false;
		}
		else
		{
			hasToggledSound = true;
			SoundToggle = true;
		}

	}



	

}

cPlayer::~cPlayer()
{

}

//The following mothods are used in main to determine if it sound carry out certain functions like movement, changing camera or toggling sound. 

bool cPlayer::MoveForward()
{
	if (hasMovedForward == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool cPlayer::MoveBackward()
{
	if (hasMovedBackward == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool cPlayer::MoveUp()
{
	if (hasMovedUp == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool cPlayer::MoveDown()
{
	if (hasMovedDown == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool cPlayer::MoveLeft()
{
	if (hasMovedLeft == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool cPlayer::MoveRight()
{
	if (hasMovedRight == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool cPlayer::ChangingCam()
{
	if (changeCam == true)
	{
		return true;
	}
	else
	{
		return false;
	}

}
//used for debug camera as it requires a few more parameters.
bool cPlayer::enteredDebugState()
{

		if (debugState == false)
		{
			debugState = true;
			return true;
			
		}

		else
		{
			debugState = false;
			return false;
			
		}
	
}

bool cPlayer::returnDebugState()
{
	if (debugState == false)
	{
		return false;
	}

	if (debugState == true)
	{
		return true;
	}
}

//Reset boolean values. Helpful with logic management
void cPlayer::Reset()
{
	hasMovedBackward = false;
	hasMovedForward = false;
	hasMovedUp = false;
	hasMovedDown = false;
	hasMovedLeft = false;
	hasMovedRight = false;
	changeCam = false;
	
}