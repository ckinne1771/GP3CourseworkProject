#ifndef Input_h
#define Input_h


#include "Keyboard.h"
#include "Mouse.h"


#include <string>
#include <vector>

class Input
{
public:
	Input();
	~Input();

	bool init();
	void destroy();

	void update();

	Keyboard* getKeyboard()
	{
		return m_Keyboard;
	};

	Mouse* getMouse()
	{
		return m_Mouse;
	};

	static Input& getInput()
	{
		static Input inputSystem;
		return inputSystem;
	}
private:
	Keyboard * m_Keyboard;
	Mouse *  m_Mouse;
};

#endif