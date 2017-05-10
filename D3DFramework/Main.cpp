#include "System.h"
#include "Game.h"

INT WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, INT )
{
	System sys;
	Game game( sys.m_graphics, sys.m_keyboard, sys.m_mouse, sys.m_direct3D );
	sys.m_win.SetText( L"D3DFramework" );
	return sys.Run<Game, &Game::Go>( game );
	
}