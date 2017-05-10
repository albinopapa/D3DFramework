#include "System.h"
#include <algorithm>
#include <vector>

System::System( )
	:
	m_win( *this, MsgProc_Init ),
	m_direct3D( m_win ),
	m_graphics( m_win, m_direct3D )
{
	SetMessageFunc( []( HWND WinHandle, UINT Message, WPARAM wParam, LPARAM lParam )
	{
		System *pSys = System::GetUserData<System>( WinHandle );
		return pSys->MsgProc( Message, wParam, lParam );
	} );
}

LRESULT System::MsgProc_Init( HWND WinHandle, UINT Message, WPARAM wParam, LPARAM lParam )
{
	if( Message == WM_NCCREATE )
	{
		Window::CreateStruct params( lParam );
		auto *pSys = params.GetUserData<System>();
		System::SetUserData( pSys, WinHandle );
	}

	return DefWindowProc( WinHandle, Message, wParam, lParam );
}

LRESULT System::MsgProc( UINT Message, WPARAM wParam, LPARAM lParam )
{
	switch( Message )
	{
		case WM_CLOSE:
			m_win.Destroy();
			break;
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;
		case WM_KEYDOWN:
			if( wParam == VK_ESCAPE )
				SendMessage( m_win.GetHandle(), WM_CLOSE, 0, 0 );
			else
				m_keyboard.OnKeyPress( static_cast< unsigned char >( wParam ) );
			break;
		case WM_KEYUP:
			m_keyboard.OnKeyRelease( static_cast< unsigned char >( wParam ) );
			break;
		case WM_MOUSEMOVE:
		{
			POINTS pt = MAKEPOINTS( lParam );
			const auto scrWidth = g_winrect.GetWidth();
			const auto scrHeight = g_winrect.GetHeight();
			if( pt.x > 0 && pt.x < scrWidth && pt.y > 0 && pt.y < scrHeight )
			{				
				m_mouse.OnMouseMove( pt.x, pt.y );
				if( !m_mouse.IsInWindow() )
				{
					SetCapture( m_win.GetHandle() );
					m_mouse.OnMouseEnter();
				}
			}
			else
			{
				if( wParam & ( MK_LBUTTON | MK_RBUTTON ) )
				{
					pt.x = std::max( short( 0 ), pt.x );
					pt.x = std::min( short( scrWidth - 1 ), pt.x );
					pt.y = std::max( short( 0 ), pt.y );
					pt.y = std::min( short( scrHeight - 1 ), pt.y );
					m_mouse.OnMouseMove( pt.x, pt.y );
				}
				else
				{
					ReleaseCapture();
					m_mouse.OnMouseLeave();
					m_mouse.OnLeftReleased( pt.x, pt.y );
					m_mouse.OnRightReleased( pt.x, pt.y );
				}
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS( lParam );
			m_mouse.OnLeftPressed( pt.x, pt.y );
			break;
		}
		case WM_RBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS( lParam );
			m_mouse.OnRightPressed( pt.x, pt.y );
			break;
		}
		case WM_LBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS( lParam );
			m_mouse.OnLeftReleased( pt.x, pt.y );
			break;
		}
		case WM_RBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS( lParam );
			m_mouse.OnRightReleased( pt.x, pt.y );
			break;
		}
		case WM_MOUSEWHEEL:
		{
			const POINTS pt = MAKEPOINTS( lParam );
			if( GET_WHEEL_DELTA_WPARAM( wParam ) > 0 )
			{
				m_mouse.OnWheelUp( pt.x, pt.y );
			}
			else if( GET_WHEEL_DELTA_WPARAM( wParam ) < 0 )
			{
				m_mouse.OnWheelDown( pt.x, pt.y );
			}
			break;
		}
	}
	
	return DefWindowProc( m_win.GetHandle(), Message, wParam, lParam );
}
