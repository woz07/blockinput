// blockinput.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include "blockinput.h"
#include <windows.h>
#include <iostream>
#include <stdexcept>

// Keyboard

/*
 * BlockKeyboardInput()
 * 
 * This function blocks keyboard input
 */
void BlockKeyboardInput()
{
	if (!BlockInput(TRUE))
	{
		throw std::exception("blockinput.cpp::BlockKeyboardInput() failed to block keyboard input.");
	}
	return;
}

/*
 * UnblockKeyboardInput()
 * 
 * This function unblocks keyboard input
 */
void UnblockKeyboardInput()
{
	if (!BlockInput(FALSE))
	{
		throw std::exception("blockinput.cpp::UnblockKeyboardInput() failed to unblock keyboard input.");
	}
	return;
}

// Mouse
bool bmm = false;  // Block Mouse Movement
HHOOK hook = NULL; // Hook for mouse

/*
 * BlockMouseMovement()
 * int stime ~ How long to sleep (Musn't be less than or equal to 0)
 * This function blocks mouse movement
 */
void BlockMouseMovement(int stime)
{
	// Ensuring sleep cannot be less than or equal to 0
	if (stime <= 0)
	{
		throw std::exception("blockinput.cpp::BlockMouseMovement(int stime) the value of stime must  be less than or equal to 0.");
	}

	bmm = true;
	
	// Get mouse position and store it
	POINT point;
	GetCursorPos(&point);

	// Get current position of mouse and if not equal to `point` then change it
	POINT current;
	while (bmm)
	{
		GetCursorPos(&current);
		if (current.x != point.x || current.y != point.y)
		{
			SetCursorPos(point.x, point.y);
		}
		Sleep(stime);
	}

	return;
}

/*
 * UnblockMouseMovement()
 * 
 * This function unblocks mouse movement
 */
void UnblockMouseMovement()
{
	bmm = false;
	return;
}

/*
 *  LowLevelMouseProc()
 * 
 * This is a private function, hence there isn't anyway you could directly use it
 * 
 * This function just blocks the mouse button inputs
 */
LRESULT CALLBACK LowLevelMouseProc(int ncode, WPARAM wparam, LPARAM lparam)
{
	if (ncode == HC_ACTION)
	{
		// Block mouse button pressed
		if (wparam == WM_LBUTTONDOWN || wparam == WM_LBUTTONUP ||
			wparam == WM_RBUTTONDOWN || wparam == WM_RBUTTONUP ||
			wparam == WM_MBUTTONDOWN || wparam == WM_MBUTTONUP ||
			wparam == WM_XBUTTONDOWN || wparam == WM_XBUTTONUP)
		{
			return 1;
		}
	}
	return CallNextHookEx(hook, ncode, wparam, lparam);
}

/*
 * BlockMouseButtons()
 * 
 * This function blocks mouse input
 */
void BlockMouseButtons()
{
	try 
	{
		hook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, 0);
	}
	catch (const std::exception e) 
	{
		throw std::exception("blockinput.cpp::BlockMouseButtons() unable to set hook.");
	}
	return;
}

/*
 * UnblockMouseButtons()
 * 
 * This function unblocks mouse input
 */
void UnblockMouseButtons()
{
	if (hook != NULL)
	{
		if (!UnhookWindowsHookEx(hook))
		{
			throw std::exception("blockinput.cpp::UnblockMouseButtons() unable to unhook hook.");
		}
		hook = NULL;
	}
	return;
}