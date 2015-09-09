#include "DXUT.h"
#include <stdio.h>
#include "resource.h"
#include "game.h"
#include "audio.h"


INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
    HRESULT hr = S_OK;

    
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	DXUTSetCallbackD3D9DeviceAcceptable( IsDeviceAcceptable );
    DXUTSetCallbackD3D9DeviceCreated( OnCreateDevice );
    DXUTSetCallbackD3D9FrameRender( OnFrameRender );
    DXUTSetCallbackD3D9DeviceReset( OnResetDevice );
    DXUTSetCallbackD3D9DeviceLost( OnLostDevice );
    DXUTSetCallbackD3D9DeviceDestroyed( OnDestroyDevice );
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackFrameMove( OnFrameMove );
    DXUTSetCallbackKeyboard( KeyboardProc );
    DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );

    
	
	DXUTSetCursorSettings( false, true );
    DXUTInit( true, true ); 
    DXUTSetHotkeyHandling( false, false, false );

    
	
	
	InitApp();


    hr = PrepareAudio();
    if( FAILED( hr ) )
    {
        OutputDebugString( L"PrepareAudio() failed.  Disabling audio support\n" );
    }

    
	DXUTCreateWindow( L"Buser Stima" );
    DXUTCreateDevice( false, 0, 0 );


    
	DXUTMainLoop();

    CleanupAudio();

    return DXUTGetExitCode();
}

