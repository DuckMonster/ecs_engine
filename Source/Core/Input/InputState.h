#pragma once
#include "Key.h"

class InputState
{
	struct KeyData
	{
		int pressed : 1;
		int frame : 31;
	};

public:
	InputState();

	void HandleAction(Key key, KeyAction action);
	void EndFrame() { currentFrame++; }

	bool GetKey(Key key) const;
	bool GetKeyPressed(Key key) const;
	bool GetKeyReleased(Key key) const;

private:
	int currentFrame = 0;
	KeyData keys[(uint16)Key::MAX];
};