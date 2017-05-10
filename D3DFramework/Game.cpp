#include "Game.h"
#include "Direct3D.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Mouse.h"


Game::Game( Graphics & Gfx, Keyboard & Kbd, Mouse & Ms, Direct3D & D3d )
	:
	m_graphics( Gfx ),
	m_mouse( Ms ),
	m_keyboard( Kbd ),
	m_direct3d( D3d )
{
	
}

void Game::Go()
{
	m_graphics.BeginFrame( Color( 0, 0, 64 ) );
	UpdateFrame();
	ComposeFrame();
	m_graphics.EndFrame();
}

void Game::UpdateFrame()
{
}

void Game::ComposeFrame()
{
}
