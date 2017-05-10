#pragma once

#include "Direct3D.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Timer.h"
#include "Window.h"

class System
{
public:
	System();

	// Must provide a class object and member function pointer 
	// Run calls the App::Func each frame
	template<class Type, void(Type::*Func)()>
	int Run( Type &App)
	{
		m_win.Show();
		Window::Message msg;

		while( !msg.IsQuit() )
		{
			msg.Process();
			(App.*Func)();
		}

		return 0;

	}

	static LRESULT CALLBACK MsgProc_Init( HWND WinHandle, UINT Message, WPARAM wParam, LPARAM lParam );
	LRESULT MsgProc( UINT Message, WPARAM wParam, LPARAM lParam );

	template<class UserData> void SetUserData( UserData *pUserData )const
	{
		SetUserData( pUserData, hWnd );
	}
	template<class UserData> static void SetUserData( UserData *pUserData, HWND WinHandle )
	{
		SetWindowLongPtr( WinHandle, GWLP_USERDATA,
						  reinterpret_cast< LONG_PTR >( pUserData ) );
	}

	void SetMessageFunc( MsgFunc MsgHandlerFunc )const
	{
		SetMessageFunc( MsgHandlerFunc, m_win.GetHandle() );
	}
	static void SetMessageFunc( MsgFunc MsgHandlerFunc, HWND WinHandle )
	{
		SetWindowLongPtr( WinHandle, GWLP_WNDPROC, reinterpret_cast< LONG_PTR >( MsgHandlerFunc ) );
	}

	template<class UserData> UserData* GetUserData()const
	{
		return GetUserData( hWnd );
	}
	template<class UserData> static UserData* GetUserData( HWND WinHandle )
	{
		return reinterpret_cast< UserData* >(
			GetWindowLongPtr( WinHandle, GWLP_USERDATA ) );
	}

	MsgFunc GetMessageFunc()const
	{
		return reinterpret_cast< MsgFunc >(
			GetWindowLongPtr( m_win.GetHandle(), GWLP_WNDPROC ) );
	}
	static MsgFunc GetMessageFunc( HWND WinHandle )
	{
		return reinterpret_cast< MsgFunc >( 
			GetWindowLongPtr( WinHandle, GWLP_WNDPROC ) );
	}

public:
	Keyboard m_keyboard;
	Mouse m_mouse;
	Timer m_timer;
	Window m_win;
	Direct3D m_direct3D;
	Graphics m_graphics;
};

