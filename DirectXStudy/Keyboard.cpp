#include "Keyboard.h"

void Keyboard::Update()
{
	for (int i = 0; i < currentState.size(); i++)
	{
		if (previousState[i] == currentState[i] && currentState[i] == true)
		{
			if(OnKeyHold)
				OnKeyHold(i);
		}
		if (previousState[i] != currentState[i])
		{
			if(OnKeyPressed)
				OnKeyPressed(i,currentState[i]);
			previousState[i]= currentState[i];
		}
	}
}

void Keyboard::SetKeyState(int idx, bool value)
{
	currentState[idx] = value;
}
