#pragma once

#include "SimpleModel.h"

class Direct3D;
class Graphics;
class Keyboard;
class Mouse;

class Game
{
public:
	Game( Graphics &Gfx, Keyboard &Kbd, Mouse &_Ms, Direct3D &D3d );
	Game( const Game & ) = delete;
	Game( Game&& ) = delete;
	Game &operator=( const Game & ) = delete;
	Game &operator=( Game&& ) = delete;

	void Go();
private:
	void UpdateFrame();
	void ComposeFrame();
private:
	Direct3D &m_direct3d;
	Graphics &m_graphics;
	Keyboard &m_keyboard;
	Mouse &m_mouse;
	SimpleModel m_model;
};