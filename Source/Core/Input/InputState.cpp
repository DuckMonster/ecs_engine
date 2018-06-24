#include "CorePCH.h"
#include "InputState.h"

/**	Constructor
*******************************************************************************/
InputState::InputState()
{
	memset(keys, 0, sizeof(keys));
}

/**	Handle Action
*******************************************************************************/
void InputState::HandleAction( Key key, KeyAction action )
{
	// We don't care about repeats
	if (action == KeyAction::Repeat)
		return;

	KeyData& data = keys[(uint16)key];
	data.pressed = (action == KeyAction::Pressed ? true : false);
	data.frame = currentFrame;
}

/**	Get Key
*******************************************************************************/
bool InputState::GetKey( Key key ) const
{
	return keys[(uint16)key].pressed;
}

/**	Get Key Pressed
*******************************************************************************/
bool InputState::GetKeyPressed( Key key ) const
{
	const KeyData& data = keys[(uint16)key];
	return data.pressed && data.frame == currentFrame;
}

/**	Get Key Released
*******************************************************************************/
bool InputState::GetKeyReleased( Key key ) const
{
	const KeyData& data = keys[(uint16)key];
	return !data.pressed && data.frame == currentFrame;
}

