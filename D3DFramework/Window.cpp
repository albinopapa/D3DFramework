#include "Window.h"
#include "Graphics.h"

// A function to return a Rect instead of having to construct one and pass it in.
Rectl GetRectFromHandle( HWND WinHandle )
{
	Rectl r{};
	GetClientRect( WinHandle, reinterpret_cast< RECT * >( &r ) );
	return r;
}

// Might not need this but it's here, global Rect for desktop client rect size
const Rectl g_winrect = GetRectFromHandle( GetDesktopWindow() );


HINSTANCE Window::GetInstance()
{
	return GetModuleHandle( nullptr );
}

HWND Window::GetHandle() const
{
	return m_hWnd;
}

LONG Window::GetWidth() const
{
	return m_width;
}

LONG Window::GetHeight() const
{
	return m_height;
}

BOOL Window::IsValid() const
{
	return IsWindow( m_hWnd );
}

void Window::Show()const
{
	ShowWindow( m_hWnd, SW_SHOWDEFAULT );
}

void Window::Destroy() const
{
	DestroyWindow( m_hWnd );
}

void Window::SetText( const std::wstring &Text )
{
	SetWindowText( m_hWnd, Text.c_str() );
}