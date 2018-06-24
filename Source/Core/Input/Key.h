#pragma once
#include "Core/Tools/Types.h"

/* All codes are scancodes */
enum class Key : uint16
{
	Escape = 0x1,
	Num1,
	Num2,
	Num3,
	Num4,
	Num5,
	Num6,
	Num7,
	Num8,
	Num9,
	Num0,
	Minus,		// + ? backslash
	Plus,		// `
	Backspace,
	Tab,
	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,
	LeftBracket,		// Å
	RightBracket,		// Ä
	Enter,
	LeftControl,
	A,
	S,
	D,
	F,
	G,
	H,
	J,
	K,
	L,
	SemiColon,			// Ö
	SingleQuote,		// Ä
	Tilde,
	LeftShift,
	BackSlash,			// ' *
	Z,
	X,
	C,
	V,
	B,
	N,
	M,
	LessThan,			// , ;
	GreaterThan,		// . :
	ForwardSlash,		// - _
	RightShift,
	KeypadStar,
	LeftAlt,
	Spacebar,
	CapsLock,
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	NumLock, // Pause break
	ScrollLock,
	Keypad7,
	Keypad8,
	Keypad9,
	KeypadMinus,
	Keypad4,
	Keypad5,
	Keypad6,
	KeypadPlus,
	Keypad1,
	Keypad2,
	Keypad3,
	Keypad0,
	KeypadPeriod,
	F11 = 0x57,
	F12,
	KeypadEnter = 0x11c,
	RightControl = 0x11d,
	KeypadSlash = 0x135,
	PrintScreen = 0x137,
	AltGr = 0x138,
	Home = 0x147,
	End = 0x14f,
	PgUp = 0x149,
	PgDn = 0x151,
	Insert = 0x152,
	Delete = 0x153,
	LeftWin = 0x15b,
	RightWin = 0x15c,
	MAX // Always leave last
};

enum class KeyAction : uint8
{
	Released,
	Pressed,
	Repeat
};