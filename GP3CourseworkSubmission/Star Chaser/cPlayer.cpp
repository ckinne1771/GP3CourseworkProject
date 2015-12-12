#include "cPlayer.h"

cPlayer::cPlayer() : cModel()
{

}

void cPlayer::attachInputMgr(cInputMgr* inputMgr)
{
	m_InputMgr = inputMgr;
}

void cPlayer::update(float elapsedTime)
{
	if (m_InputMgr->isKeyDown(VK_LEFT))
	{
		hasMovedLeft = true;
		
	}
	if (m_InputMgr->isKeyDown(VK_RIGHT))
	{
		hasMovedRight = true;
	}

	if (m_InputMgr->isKeyDown(VK_UP))
	{
		hasMovedUp = true;
	}

	if (m_InputMgr->isKeyDown(VK_DOWN))
	{
		hasMovedDown = true;
	}
	if (m_InputMgr->isKeyDown(87))
	{
		//translationZ += 5.0f;
		//m_mdlPosition.z += 5.0f;
		hasMovedForward = true;
	

	}
	if (m_InputMgr->isKeyDown(83))
	{
		//translationZ -= 5.0f;
		//m_mdlPosition.z -= 5.0f;
		hasMovedBackward = true;
	}

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