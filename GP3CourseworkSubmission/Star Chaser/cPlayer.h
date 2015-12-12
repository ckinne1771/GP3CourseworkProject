#ifndef _CPLAYER_H
#define _CPLAYER_H


#include "cModel.h"
#include "cInputMgr.h"

class cPlayer : public cModel
{
private:
	cInputMgr* m_InputMgr;
public:
	cPlayer();
	void attachInputMgr(cInputMgr* inputMgr);
	virtual void update(float elapsedTime);
	bool hasMovedForward;
	bool hasMovedBackward;
	bool hasMovedUp;
	bool hasMovedDown;
	bool hasMovedLeft;
	bool hasMovedRight;
	bool changeCam;
	bool debugState;
	bool SoundToggle = false;
	bool hasToggledSound = false;

	bool enteredDebugState();
	bool returnDebugState();
	bool ChangingCam();
	bool MoveForward();
	bool MoveBackward();
	bool MoveUp();
	bool MoveDown();
	bool MoveLeft();
	bool MoveRight();
	void Reset();

	~cPlayer();
};
#endif