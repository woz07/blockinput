#pragma once

#ifndef BLOCKINPUT_H
#define BLOCKINPUT_H

#ifdef __cplusplus
extern "C"
{
#endif

	// Keyboard
	void BlockKeyboardInput();
	void UnblockKeyboardInput();

	// Mouse
	void BlockMouseMovement(int sleep);
	void UnblockMouseMovement();
	void BlockMouseButtons();
	void UnblockMouseButtons();

#ifdef __cplusplus
}
#endif

#endif // BLOCKINPUT_H
