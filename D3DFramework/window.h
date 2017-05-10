#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <Windows.h>
#include <Objbase.h>
#include <string>
#include "Rect.h"

extern const Rectl g_winrect;
// Set the style of window to WS_POPUP to take up the entire screen, set extended style to 0
constexpr auto g_winstyle = WS_POPUP;
constexpr auto g_exwinstyle = 0;
constexpr auto g_wintitle = L"Cubism";

using MsgFunc = LRESULT( *)( HWND, UINT, WPARAM, LPARAM );

class Window
{
	// Registers the WNDCLASSEX upon construction and unregisters it upon destruction
	class WinClassManager
	{
	public:
		~WinClassManager()
		{
			UnregisterClass( MAKEINTATOM( m_atom ), GetModuleHandle( nullptr ) );
		}
		void Register( const WNDCLASSEX &Wc ) { m_atom = RegisterClassEx( &Wc ); }
	private:
		ATOM m_atom = 0;
	};
public:
	class Message
	{
	public:
		void Process()
		{
			while( Pull( 0u, 0u, PM_REMOVE ) )
			{
				if( m_message.message == WM_QUIT ) break;
				Translate();
				Dispatch();
			}
		}
		BOOL IsQuit()const
		{
			return m_message.message == WM_QUIT;
		}
	private:
		BOOL Translate()
		{
			return TranslateMessage( &m_message );
		}
		LRESULT Dispatch()
		{
			return DispatchMessage( &m_message );
		}
		BOOL Pull( UINT MinFilter = 0u, UINT MaxFilter = 0u, UINT RemoveMessage = PM_REMOVE )
		{
			return PeekMessageW( &m_message, nullptr, MinFilter, MaxFilter, RemoveMessage );
		}
		BOOL Pull( UINT MinFilter = 0u, UINT MaxFilter = 0u )
		{
			return GetMessage( &m_message, nullptr, MinFilter, MaxFilter );
		}
	private:
		MSG m_message;
	};
	class CreateStruct
	{
	public:
		CreateStruct( LPARAM lParam )
			:
			params( *( reinterpret_cast< CREATESTRUCT* >( lParam ) ) )
		{}

		template<class UserDataType>
		UserDataType *GetUserData()const 
		{ 
			return reinterpret_cast< UserDataType* >( params.lpCreateParams ); 
		}

	private:
		CREATESTRUCT params;
	};
	class COMManager
	{
	public:
		COMManager()
		{
			CoInitialize( nullptr );
		}
		~COMManager()
		{
			CoUninitialize();
		}
	};
public:
	template<class UserDataType>
	Window( UserDataType &UserData, WNDPROC WndProc )
		:
		m_width( g_winrect.GetWidth() ),
		m_height( g_winrect.GetHeight() )
	{
		CoInitialize( nullptr );

		WNDCLASSEX wc{};
		wc.cbSize = sizeof( WNDCLASSEX );
		wc.hInstance = GetInstance();
		wc.lpszClassName = L"MyClass";
		wc.lpfnWndProc = WndProc;

		m_winmanager.Register( wc );

		m_hWnd = CreateWindowEx( g_exwinstyle, wc.lpszClassName, g_wintitle,
								 g_winstyle, 0, 0, m_width, m_height, nullptr,
								 nullptr, GetInstance(), &UserData );
	}

	HINSTANCE GetInstance();
	HWND GetHandle()const;
	LONG GetWidth()const;
	LONG GetHeight()const;
	BOOL IsValid()const;
	void Show()const;
	void Destroy()const;
	void SetText( const std::wstring &Text );
private:
	COMManager m_commanager;
	WinClassManager m_winmanager;
	LONG m_width = 0l, m_height = 0l;
	HWND m_hWnd = nullptr;
};

