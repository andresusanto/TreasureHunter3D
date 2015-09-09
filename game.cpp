#include "DXUT.h"
#include "SDKmisc.h"
#include <stdio.h>
#include "game.h"
#include "audio.h"
#include "resource.h"



// UI control
#define IDC_STATIC              1
#define IDC_AUDIO               2
#define IDC_VIDEO               3
#define IDC_RESUME              4
#define IDC_QUIT                5
#define IDC_BACK                8
#define IDC_SOUNDFX_SCALE       6
#define IDC_MUSIC_SCALE         7
#define IDC_RESOLUTION          9
#define IDC_ANTI_ALIASING       10
#define IDC_MAX_DROIDS          11
#define IDC_HIGH_MODEL_RES      12
#define IDC_MAX_DROIDS_TEXT     13
#define IDC_APPLY               14
#define IDC_FULLSCREEN          15
#define IDC_ASPECT              16
#define IDC_AI_MODE				17
#define IDC_LEVEL1				18
#define IDC_LEVEL2				19
#define IDC_LEVEL3				20
#define IDC_VIDEO2				21
#define IDC_HELPS				22
#define IDC_CLOSE_HELP			23




void BacaFile(char* NamaFile, int *Modal,int *tipeInput,int *NPetiMerah, int *NPetiKuning, int *NPetiHijau,int *NBot,int *&isiSimpul,int *targetSkor,int *batasWaktu,int *NSimpul, float *&LokasiXSimpul, float *&LokasiYSimpul, float *&LokasiZSimpul,bool *validitas);
void TulisFile(int* IDRobot, int* JenisPeti, int* Simpul,int N);

void ComputeMeshScaling( CDXUTXFileMesh& Mesh, D3DXMATRIX* pmScalingCenter, float fNewRadius );
void ComputeMeshScalingBox( CDXUTXFileMesh& Mesh, D3DXMATRIX* pmScalingCenter, D3DXVECTOR3 vNewMin,
                            D3DXVECTOR3 vNewMax );
void CreateAmmo( int nIndex, D3DXVECTOR4 Pos, D3DXVECTOR4 Vel );
void CreateDroid();
void CreateChest();
void SetEffectTechnique();
void RenderText();
void LoadLevel(int iLevel);
void ToggleMenu();
void RenderShop();
void RenderTool(int T);
void UpdateAspectRatioList( DXUTDeviceSettings* pDS );
void UpdateResolutionList( DXUTDeviceSettings* pDS );
void RenderLogo();

// Global variables
CFirstPersonCamera  g_Camera;
RENDER_STATE        g_Render;
GAME_STATE          g_GameState;
SHOP_STATE			g_Shop;
CH_TASKING			g_Task[MAX_DROID+1];
OUTFILE				g_OutFile;



//--------------------------------------------------------------------------------------
void InitApp()
{
    srand( 0 );

    g_Render.pEffect = NULL;
    g_Render.pDefaultTex = NULL;
    g_Render.UseFixedFunction = 0.0f;
    g_Render.ForceShader = 0;
    g_Render.MaximumResolution = 4096.0f;
    g_Render.DisableSpecular = 0.0f;
    g_Render.bDetectOptimalSettings = true;

    
	
	g_Render.MainMenuDlg.Init( &g_Render.DialogResourceManager );
    g_Render.MainMenuDlg.SetCallback( OnGUIEvent ); int iY = ( ( 300 - 30 * 6 ) / 2 );
	
	g_Render.MainMenuDlg.AddButton( IDC_AUDIO, L"Kontrol Suara", ( 250 - 125 ) / 2, iY += 30, 125, 22 );
    g_Render.MainMenuDlg.AddButton( IDC_VIDEO, L"Video", ( 250 - 125 ) / 2, iY += 30, 125, 22 );
    g_Render.MainMenuDlg.AddButton( IDC_RESUME, L"Lanjutkan", ( 250 - 125 ) / 2, iY += 30, 125, 22 );
    g_Render.MainMenuDlg.AddButton( IDC_QUIT, L"Keluar", ( 250 - 125 ) / 2, iY += 60, 125, 22, 'Q' );

    g_Render.AudioMenuDlg.Init( &g_Render.DialogResourceManager );
    g_Render.AudioMenuDlg.SetCallback( OnGUIEvent ); iY = 60;
    g_Render.AudioMenuDlg.AddStatic( IDC_STATIC, L"Volume Musik", ( 250 - 125 ) / 2, iY += 24, 125, 22 );
    g_Render.AudioMenuDlg.AddSlider( IDC_MUSIC_SCALE, ( 250 - 100 ) / 2, iY += 24, 100, 22, 0, 100, 100 );
    g_Render.AudioMenuDlg.AddStatic( IDC_STATIC, L"Volume SFX", ( 250 - 125 ) / 2, iY += 35, 125, 22 );
    g_Render.AudioMenuDlg.AddSlider( IDC_SOUNDFX_SCALE, ( 250 - 100 ) / 2, iY += 24, 100, 22, 0, 100, 100 );
    g_Render.AudioMenuDlg.AddButton( IDC_BACK, L"Kembali", ( 250 - 125 ) / 2, iY += 40, 125, 22 );

    g_Render.VideoMenuDlg.Init( &g_Render.DialogResourceManager );
    g_Render.VideoMenuDlg.SetCallback( OnGUIEvent ); iY = 35;
    g_Render.VideoMenuDlg.AddCheckBox( IDC_FULLSCREEN, L"Full screen", ( 250 - 200 ) / 2, iY += 30, 200, 22, true );
    g_Render.VideoMenuDlg.AddStatic( IDC_STATIC, L"Rasio:", 50, iY += 22, 50, 22 );
    g_Render.VideoMenuDlg.AddComboBox( IDC_ASPECT, 100, iY, 100, 22 );
    g_Render.VideoMenuDlg.AddStatic( IDC_STATIC, L"Resolusi:", 30, iY += 22, 75, 22 );
    g_Render.VideoMenuDlg.AddComboBox( IDC_RESOLUTION, 100, iY, 125, 22 );
    g_Render.VideoMenuDlg.AddCheckBox( IDC_ANTI_ALIASING, L"Anti-Aliasing", ( 250 - 200 ) / 2, iY += 26, 200, 22,
                                       false );
    g_Render.VideoMenuDlg.AddCheckBox( IDC_HIGH_MODEL_RES, L"High res models", ( 250 - 200 ) / 2, iY += 26, 200, 22,
                                       true );
    g_Render.VideoMenuDlg.AddButton( IDC_APPLY, L"OK", ( 250 - 125 ) / 2, iY += 35, 125, 22 );
    g_Render.VideoMenuDlg.AddButton( IDC_BACK, L"Kembali", ( 250 - 125 ) / 2, iY += 30, 125, 22 );


    g_Render.IntroDlg.Init( &g_Render.DialogResourceManager );
    g_Render.IntroDlg.SetCallback( OnGUIEvent ); iY = 35;
	g_Render.IntroDlg.AddButton( IDC_AI_MODE, L"AI WAR", 0 , iY += 40, 260, 32 );
    g_Render.IntroDlg.AddButton( IDC_LEVEL1, L"Level 1", 0, iY += 40, 260, 32 );
	g_Render.IntroDlg.AddButton( IDC_LEVEL2, L"Level 2", 0, iY += 40, 260, 32 );
	g_Render.IntroDlg.AddButton( IDC_LEVEL3, L"Level 3", 0, iY += 40, 260, 32 );
	g_Render.IntroDlg.AddButton( IDC_VIDEO2, L"Video Setting", 0, iY += 60, 260, 32 );
	g_Render.IntroDlg.AddButton( IDC_HELPS, L"Pemula?", 0, iY += 40, 260, 32 );
	g_Render.IntroDlg.AddButton( IDC_QUIT, L"Keluar", 0, iY += 40, 260, 32 );

	g_Render.HelpDlg.Init( &g_Render.DialogResourceManager );
    g_Render.HelpDlg.SetCallback( OnGUIEvent ); iY = 5;
	g_Render.HelpDlg.AddStatic( IDC_MAX_DROIDS_TEXT, L"Hallo Stimers!!\n\nTombol ASDW digunakan untuk bergerak\nTombol Mouse Kiri untuk menembakan senjata\nTombol Mouse Kanan untuk Membuka ToolBOX\n\nGame ini dibuat untuk Tugas STIMA:\n13512018 Tony\n13512028 Andre Susanto\n13512092 Reinaldo MH", 0, iY += 26, 250, 200 );
    
	g_Render.HelpDlg.AddButton( IDC_CLOSE_HELP, L"Tutup", ( 250 - 125 ) / 2, iY += 225, 125, 22 );

	for(int J=0; J<=MAX_DROID; J++)
		g_Task[J].bEnabled = false;

	g_Shop.bWeapon = false;
	g_Shop.iAlphaKey = 0;
	g_Shop.bOpen = true;
	g_Shop.iMoney = 0;
	g_Shop.iTimer = 10;
	g_Shop.iTool1 = 0;
	g_Shop.iTool2 = 0;
	g_Shop.iTool3 = 0;
	g_Shop.bT1 = false;
	g_Shop.bT2 = false;
	g_Shop.bT3 = false;
	g_Shop.bW = false;


	g_Render.fNotif1 = 0;
	g_Render.fNotif2 = 0;
	g_Render.fNotif3 = 0;
	g_Render.fNotif4 = 0;


	g_GameState.rHelper = false;
	g_GameState.fSub = 0.0f;
	
	D3DXVECTOR3 MinBound( g_MinBound.x + CAMERA_SIZE, g_MinBound.y + CAMERA_SIZE, g_MinBound.z + CAMERA_SIZE );
    D3DXVECTOR3 MaxBound( g_MaxBound.x - CAMERA_SIZE, g_MaxBound.y - CAMERA_SIZE, g_MaxBound.z - CAMERA_SIZE );
    g_Camera.SetClipToBoundary( true, &MinBound, &MaxBound );
    g_Camera.SetRotateButtons( false, false, true );
    g_Camera.SetScalers( 0.001f, 2.0f );

    D3DXVECTOR3 vecEye( 0.0f, -GROUND_Y + 0.7f, 0.0f );
    D3DXVECTOR3 vecAt ( 0.0f, -GROUND_Y + 0.7f, 1.0f );
    g_Camera.SetViewParams( &vecEye, &vecAt );

    ZeroMemory( &g_GameState, sizeof( GAME_STATE ) );
    g_GameState.gameMode = GAME_INTRO; //GAME_INTRO;
    g_GameState.nAmmoCount = 0;
    g_GameState.fAmmoColorLerp = 1000.0f;
    g_GameState.BlendFromColor = D3DXCOLOR( 0.6f, 0, 0, 1 );
    g_GameState.bAutoAddDroids = false;
    g_GameState.bDroidMove = true;

    
	g_GameState.iCAlphaV = 0;
	g_GameState.iGT = 20;
	g_GameState.fScnd = 0;

	g_GameState.iAmmo = 100;
	g_GameState.iLimit = 100;
	g_GameState.iMoney = 10;
	g_GameState.iGetChest = 0;



    IDirect3D9* pD3D = DXUTGetD3D9Object();
    UINT numAdapters = pD3D->GetAdapterCount();
    for( UINT adapterOrdinal = 0; adapterOrdinal < numAdapters && adapterOrdinal < 10; adapterOrdinal++ )
    {
        MONITORINFO miAdapter;
        miAdapter.cbSize = sizeof( MONITORINFO );
        DXUTGetMonitorInfo( pD3D->GetAdapterMonitor( adapterOrdinal ), &miAdapter );
        g_Render.rcAdapterWork[adapterOrdinal] = miAdapter.rcWork;
    }


   
	
	D3DDISPLAYMODE dm = {0, 0, 0, D3DFMT_UNKNOWN};
    dm.Width = 640; dm.Height = 480; g_Render.aWindowedDMList.Add( dm ); // 4:3
    dm.Width = 800; dm.Height = 600; g_Render.aWindowedDMList.Add( dm ); // 4:3
    dm.Width = 1024; dm.Height = 768; g_Render.aWindowedDMList.Add( dm ); // 4:3
    dm.Width = 1280; dm.Height = 960; g_Render.aWindowedDMList.Add( dm ); // 4:3
    dm.Width = 1600; dm.Height = 1200; g_Render.aWindowedDMList.Add( dm ); // 4:3

    dm.Width = 852; dm.Height = 480; g_Render.aWindowedDMList.Add( dm ); // 16:9
    dm.Width = 1067; dm.Height = 600; g_Render.aWindowedDMList.Add( dm ); // 16:9
    dm.Width = 1280; dm.Height = 720; g_Render.aWindowedDMList.Add( dm ); // 16:9
    dm.Width = 1920; dm.Height = 1080; g_Render.aWindowedDMList.Add( dm ); // 16:9
}


//--------------------------------------------------------------------------------------
bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat,
                                  D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
    // Skip backbuffer formats that don't support alpha blending
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                                         AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
                                         D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

    // No fallback defined by this app, so reject any device that 
    // doesn't support at least ps2.0
    if( pCaps->PixelShaderVersion < D3DPS_VERSION( 2, 0 ) )
        return false;

    return true;
}


//--------------------------------------------------------------------------------------
static int __cdecl SortAspectRatios( const void* arg1, const void* arg2 )
{
    float* pArg1 = ( float* )arg1;
    float* pArg2 = ( float* )arg2;

    if( *pArg1 > *pArg2 )
        return 1;
    if( *pArg1 < *pArg2 )
        return -1;
    return 0;
}


//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    D3DCAPS9 caps;
    HRESULT hr;

    V( pD3D->GetDeviceCaps( pDeviceSettings->d3d9.AdapterOrdinal, pDeviceSettings->d3d9.DeviceType, &caps ) );

    // If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW 
    // then switch to SWVP.
    if( ( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ) == 0 ||
        caps.VertexShaderVersion < D3DVS_VERSION( 1, 1 ) )
    {
        pDeviceSettings->d3d9.BehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    }

    // Look at the caps, and choose reasonable defaults.  Only do this
    // upon changing devices (not when the user changes the settings)
    if( g_Render.bDetectOptimalSettings )
    {
        if( caps.VertexShaderVersion < D3DVS_VERSION( 1, 1 ) )
        {
            g_GameState.nMaxDroids = 5;
            g_Render.UseHighResModels = false;
            g_Render.UseAnisotropicFilter = 0;
        }
        else if( caps.VertexShaderVersion < D3DVS_VERSION( 2, 0 ) )
        {
            g_GameState.nMaxDroids = 10;
            g_Render.UseHighResModels = false;
            g_Render.UseAnisotropicFilter = 1;
        }
        else
        {
            g_GameState.nMaxDroids = 10;
            g_Render.UseHighResModels = true;
            g_Render.UseAnisotropicFilter = 1;
        }
    }

    while( g_GameState.nDroidCount < g_GameState.nMaxDroids )
        CreateDroid();

    // Wipe aspect/resolution lists if switching adapters so currently selected choice doesn't remain
    static UINT s_nAdapterOrdinal = 0xFFFF;
    if( pDeviceSettings->d3d9.AdapterOrdinal != s_nAdapterOrdinal )
    {
        s_nAdapterOrdinal = pDeviceSettings->d3d9.AdapterOrdinal;
        g_Render.VideoMenuDlg.GetComboBox( IDC_ASPECT )->RemoveAllItems();
        g_Render.VideoMenuDlg.GetComboBox( IDC_RESOLUTION )->RemoveAllItems();
        UpdateAspectRatioList( pDeviceSettings );
    }

    // For the first device created if its a REF device, optionally display a warning dialog box
    static bool s_bFirstTime = true;
    if( s_bFirstTime )
    {
        s_bFirstTime = false;
        if( pDeviceSettings->d3d9.DeviceType == D3DDEVTYPE_REF )
            DXUTDisplaySwitchingToREFWarning( pDeviceSettings->ver );
    }

    pDeviceSettings->d3d9.pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    return true;
}


//--------------------------------------------------------------------------------------
// Rendering pemecah segitiga untuk mesh dan pengaplikasian tekstur

HRESULT SplitIntoSeperateTriangles( IDirect3DDevice9* pd3dDevice, ID3DXMesh* pInMesh, CDXUTXFileMesh* pOutMesh )
{
    HRESULT hr;
    DWORD dwNumFaces = pInMesh->GetNumFaces();
    DWORD dwOutNumVerts = dwNumFaces * 3;
    D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];
    pInMesh->GetDeclaration( decl );

    ID3DXMesh* pNewMesh = NULL;
    hr = D3DXCreateMesh( dwNumFaces, dwOutNumVerts, D3DXMESH_32BIT | D3DXMESH_MANAGED, decl, pd3dDevice, &pNewMesh );
    if( FAILED( hr ) )
        return hr;

    WORD* pInIndexData;
    DWORD* pNewIndexData;
    DROID_VERTEX* pInVertexData;
    DROID_VERTEX* pNewVertexData;
    DWORD* pInAttribData;
    DWORD* pNewAttribData;
    pInMesh->LockAttributeBuffer( D3DLOCK_READONLY, ( DWORD** )&pInAttribData );
    pInMesh->LockVertexBuffer( D3DLOCK_READONLY, ( VOID** )&pInVertexData );
    pInMesh->LockIndexBuffer( D3DLOCK_READONLY, ( VOID** )&pInIndexData );
    {
        pNewMesh->LockAttributeBuffer( 0, ( DWORD** )&pNewAttribData );
        pNewMesh->LockIndexBuffer( 0, ( VOID** )&pNewIndexData );
        pNewMesh->LockVertexBuffer( 0, ( VOID** )&pNewVertexData );
        for( DWORD iFace = 0; iFace < dwNumFaces; iFace++ )
        {
            DWORD iV1 = pInIndexData[ iFace * 3 + 0 ];
            DWORD iV2 = pInIndexData[ iFace * 3 + 1 ];
            DWORD iV3 = pInIndexData[ iFace * 3 + 2 ];

            pNewAttribData[ iFace ] = 0;

            D3DXVECTOR3 vA = pInVertexData[ iV1 ].pos - pInVertexData[ iV2 ].pos;
            D3DXVECTOR3 vB = pInVertexData[ iV1 ].pos - pInVertexData[ iV3 ].pos;
            D3DXVECTOR3 vFaceNormal;
            D3DXVec3Cross( &vFaceNormal, &vA, &vB );
            D3DXVec3Normalize( &vFaceNormal, &vFaceNormal );

            pNewIndexData[ iFace * 3 + 0 ] = iFace * 3 + 0;
            pNewVertexData[ iFace * 3 + 0 ].pos = pInVertexData[ iV1 ].pos;
            pNewVertexData[ iFace * 3 + 0 ].normal = vFaceNormal; // pInVertexData[ iV1 ].normal;
            pNewVertexData[ iFace * 3 + 0 ].uv = pInVertexData[ iV1 ].uv;

            pNewIndexData[ iFace * 3 + 1 ] = iFace * 3 + 1;
            pNewVertexData[ iFace * 3 + 1 ].pos = pInVertexData[ iV2 ].pos;
            pNewVertexData[ iFace * 3 + 1 ].normal = vFaceNormal; // pInVertexData[ iV2 ].normal;
            pNewVertexData[ iFace * 3 + 1 ].uv = pInVertexData[ iV2 ].uv;

            pNewIndexData[ iFace * 3 + 2 ] = iFace * 3 + 2;
            pNewVertexData[ iFace * 3 + 2 ].pos = pInVertexData[ iV3 ].pos;
            pNewVertexData[ iFace * 3 + 2 ].normal = vFaceNormal; // pInVertexData[ iV3 ].normal;
            pNewVertexData[ iFace * 3 + 2 ].uv = pInVertexData[ iV3 ].uv;
        }
        pNewMesh->UnlockIndexBuffer();
        pNewMesh->UnlockVertexBuffer();
        pNewMesh->UnlockAttributeBuffer();
    }
    pInMesh->UnlockAttributeBuffer();
    pInMesh->UnlockVertexBuffer();
    pInMesh->UnlockIndexBuffer();

    DWORD dwMaterials = 1;
    D3DXMATERIAL d3dxMaterials[1];
    d3dxMaterials[0].MatD3D = g_Render.meshDroid.m_pMaterials[0];
    d3dxMaterials[0].pTextureFilename = "media\\droid\\wasphunter_texture.jpg";

    hr = pOutMesh->Create( pd3dDevice, pNewMesh, d3dxMaterials, dwMaterials );
    SAFE_RELEASE( pNewMesh );

    return hr;
}

//--------------------------------------------------------------------------------------
//	Fungsi ketika D3DDevice berhasil dibuat oleh sistem
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                 void* pUserContext )
{
    HRESULT hr;
    V_RETURN( g_Render.DialogResourceManager.OnD3D9CreateDevice( pd3dDevice ) );

    // inisialisasi font objek
    V_RETURN( D3DXCreateFont( pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
                              OUT_DEFAULT_PRECIS, 0, DEFAULT_PITCH | FF_DONTCARE,
                              L"Arial", &g_Render.pFont ) );

	V_RETURN( D3DXCreateFont( pd3dDevice, 14, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
                              OUT_DEFAULT_PRECIS, 4, DEFAULT_PITCH | FF_DONTCARE,
                              L"Arial", &g_Render.pFont1 ) );

	V_RETURN( D3DXCreateFont( pd3dDevice, 70, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
                              OUT_DEFAULT_PRECIS, 4, DEFAULT_PITCH | FF_DONTCARE,
                              L"Arial", &g_Render.pFont2 ) );
	
	V_RETURN( D3DXCreateFont( pd3dDevice, 17, 0, FW_SEMIBOLD, 1, FALSE, DEFAULT_CHARSET,
                              OUT_DEFAULT_PRECIS, 4, DEFAULT_PITCH | FF_DONTCARE,
                              L"Arial", &g_Render.pFont3 ) );


	
	V_RETURN ( D3DXCreateSprite ( pd3dDevice, &g_Render.pShop ));
	
	V_RETURN ( D3DXCreateTextureFromFile( pd3dDevice , L"media\\shop\\shop.tex", &g_Render.pShopRes ) );

	V_RETURN ( D3DXCreateTextureFromFile( pd3dDevice , L"media\\shop\\shop_1.tex", &g_Render.pShopRes1 ) );

	V_RETURN ( D3DXCreateTextureFromFile( pd3dDevice , L"media\\shop\\shop_2.tex", &g_Render.pShopRes2 ) );

	V_RETURN ( D3DXCreateTextureFromFile( pd3dDevice , L"media\\shop\\shop_3.tex", &g_Render.pShopRes3 ) );

	V_RETURN ( D3DXCreateTextureFromFile( pd3dDevice , L"media\\shop\\shop_4.tex", &g_Render.pShopRes4 ) );

	V_RETURN ( D3DXCreateTextureFromFile( pd3dDevice , L"media\\shop\\shop_c.tex", &g_Render.pShopRes5 ) );


	V_RETURN ( D3DXCreateTextureFromFile( pd3dDevice , L"media\\x.tex", &g_Render.pWindowX ) );
	V_RETURN ( D3DXCreateTextureFromFile( pd3dDevice , L"media\\sembah.tex", &g_Render.pFinishRes ) );


	V_RETURN ( D3DXCreateTextureFromFile( pd3dDevice , L"media\\chest\\notif1.tex", &g_Render.pNotif1 ) );
	V_RETURN ( D3DXCreateTextureFromFile( pd3dDevice , L"media\\chest\\notif2.tex", &g_Render.pNotif2 ) );
	V_RETURN ( D3DXCreateTextureFromFile( pd3dDevice , L"media\\chest\\notif3.tex", &g_Render.pNotif3 ) );
	V_RETURN ( D3DXCreateTextureFromFile( pd3dDevice , L"media\\chest\\notif4.tex", &g_Render.pNotif4 ) );

	// loader untuk tool box
	
	V_RETURN ( D3DXCreateSprite ( pd3dDevice, &g_Render.pTool ));

	V_RETURN ( D3DXCreateTextureFromFile( pd3dDevice , L"media\\tool\\tool1.tex", &g_Render.pToolRes1 ) );
	V_RETURN ( D3DXCreateTextureFromFile( pd3dDevice , L"media\\tool\\tool2.tex", &g_Render.pToolRes2 ) );
	V_RETURN ( D3DXCreateTextureFromFile( pd3dDevice , L"media\\tool\\tool3.tex", &g_Render.pToolRes3 ) );
	V_RETURN ( D3DXCreateTextureFromFile( pd3dDevice , L"media\\tool\\tool10.tex", &g_Render.pToolRes10 ) );
	V_RETURN ( D3DXCreateTextureFromFile( pd3dDevice , L"media\\tool\\tool20.tex", &g_Render.pToolRes20 ) );
	V_RETURN ( D3DXCreateTextureFromFile( pd3dDevice , L"media\\tool\\tool30.tex", &g_Render.pToolRes30 ) );
	
	// loader helper
	V_RETURN ( D3DXCreateSprite ( pd3dDevice, &g_Render.pHelper ));
	V_RETURN ( D3DXCreateTextureFromFile( pd3dDevice , L"media\\small.png", &g_Render.pHelperRes ) );
	V_RETURN ( D3DXCreateTextureFromFile( pd3dDevice , L"media\\itb.png", &g_Render.pITB ) );

	// status loader

	V_RETURN ( D3DXCreateSprite ( pd3dDevice, &g_Render.pStatus ));
	
	V_RETURN ( D3DXCreateTextureFromFile( pd3dDevice , L"media\\status\\status.tex", &g_Render.pStatusRes ) );
	V_RETURN ( D3DXCreateTextureFromFile( pd3dDevice , L"media\\status\\cstatus.tex", &g_Render.pStatusCRes ) );


    // Membuka path effek
    WCHAR str[MAX_PATH];
    V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"Main.fx" ) );


    // gagal
	V_RETURN( D3DXCreateEffectFromFile( pd3dDevice, str, NULL, NULL, D3DXFX_NOT_CLONEABLE,
                                        NULL, &g_Render.pEffect, NULL ) );

    // Objek handler
    g_Render.hMatW = g_Render.pEffect->GetParameterByName( NULL, "g_mWorld" );
    g_Render.hMatV = g_Render.pEffect->GetParameterByName( NULL, "g_mView" );
    g_Render.hMatP = g_Render.pEffect->GetParameterByName( NULL, "g_mProj" );
    g_Render.hMatWV = g_Render.pEffect->GetParameterByName( NULL, "g_mWorldView" );
    g_Render.hMatWVP = g_Render.pEffect->GetParameterByName( NULL, "g_mWorldViewProj" );
    g_Render.hDiffuse = g_Render.pEffect->GetParameterByName( NULL, "g_matDiffuse" );
    g_Render.hSpecular = g_Render.pEffect->GetParameterByName( NULL, "g_matSpecular" );
    g_Render.hDiffuseTexture = g_Render.pEffect->GetParameterByName( NULL, "g_DiffuseTexture" );
    g_Render.hNormalMap = g_Render.pEffect->GetParameterByName( NULL, "g_NormalMapTexture" );
    g_Render.hPower = g_Render.pEffect->GetParameterByName( NULL, "g_matPower" );
    g_Render.hAnimation = g_Render.pEffect->GetParameterByName( NULL, "g_fAnimation" );

    // Load Mesh yang ada
    WCHAR wsz[MAX_PATH];
    V_RETURN( DXUTFindDXSDKMediaFileCch( wsz, MAX_PATH, L"room\\droidroom.x" ) );
    g_Render.meshCell.Create( pd3dDevice, wsz );

    V_RETURN( DXUTFindDXSDKMediaFileCch( wsz, MAX_PATH, L"ammo.x" ) );
    g_Render.meshAmmo.Create( pd3dDevice, wsz );

	V_RETURN( DXUTFindDXSDKMediaFileCch( wsz, MAX_PATH, L"droid\\wasphunter.x" ) );
    g_Render.meshDroid.Create( pd3dDevice, wsz );
    g_Render.meshDroidLow.Create( pd3dDevice, wsz );

	V_RETURN( DXUTFindDXSDKMediaFileCch( wsz, MAX_PATH, L"chest\\green.x" ) );
    g_Render.meshChestGreen.Create( pd3dDevice, wsz );

	V_RETURN( DXUTFindDXSDKMediaFileCch( wsz, MAX_PATH, L"chest\\yellow.x" ) );
    g_Render.meshChestYellow.Create( pd3dDevice, wsz );

	V_RETURN( DXUTFindDXSDKMediaFileCch( wsz, MAX_PATH, L"chest\\red.x" ) );
    g_Render.meshChestRed.Create( pd3dDevice, wsz );

	
	g_Render.iPixelH = pBackBufferSurfaceDesc->Height;
	g_Render.iPixelW = pBackBufferSurfaceDesc->Width;

    // Buat deklarasi vertex baru
    const D3DVERTEXELEMENT9 VertexDecl[] =
    {
        { 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
        { 0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0 },
        { 0, 36, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
        D3DDECL_END()
    };

    // Apply pemecah segitiga dan tekstur
    SplitIntoSeperateTriangles( pd3dDevice, g_Render.meshDroid.GetMesh(), &g_Render.meshDroidExplosion );
	SplitIntoSeperateTriangles( pd3dDevice, g_Render.meshDroid.GetMesh(), &g_Render.meshDroid );
    SplitIntoSeperateTriangles( pd3dDevice, g_Render.meshDroidLow.GetMesh(), &g_Render.meshDroidExplosionLow );
	

    g_Render.meshCell.SetVertexDecl( pd3dDevice, VertexDecl );
    g_Render.meshAmmo.SetVertexDecl( pd3dDevice, VertexDecl );
    g_Render.meshDroid.SetVertexDecl( pd3dDevice, VertexDecl );
    g_Render.meshDroidExplosion.SetVertexDecl( pd3dDevice, VertexDecl );
    g_Render.meshDroidLow.SetVertexDecl( pd3dDevice, VertexDecl );
    g_Render.meshDroidExplosionLow.SetVertexDecl( pd3dDevice, VertexDecl );
	g_Render.meshChestGreen.SetVertexDecl( pd3dDevice, VertexDecl );
	g_Render.meshChestYellow.SetVertexDecl( pd3dDevice, VertexDecl );
	g_Render.meshChestRed.SetVertexDecl( pd3dDevice, VertexDecl );



    V_RETURN( pd3dDevice->CreateTexture( 1, 1, 1, 0, D3DFMT_A8R8G8B8,
                                         D3DPOOL_MANAGED, &g_Render.pDefaultTex, NULL ) );
    D3DLOCKED_RECT lr;
    V_RETURN( g_Render.pDefaultTex->LockRect( 0, &lr, NULL, 0 ) );
    *( LPDWORD )lr.pBits = D3DCOLOR_RGBA( 255, 255, 255, 255 );
    V_RETURN( g_Render.pDefaultTex->UnlockRect( 0 ) );

    
	V_RETURN( pd3dDevice->CreateTexture( 1, 1, 1, 0, D3DFMT_A8R8G8B8,
                                         D3DPOOL_MANAGED, &g_Render.pDefaultNormalMap, NULL ) );
    V_RETURN( g_Render.pDefaultNormalMap->LockRect( 0, &lr, NULL, 0 ) );
    *( LPDWORD )lr.pBits = D3DCOLOR_RGBA( 127, 127, 255, 0 );
    V_RETURN( g_Render.pDefaultNormalMap->UnlockRect( 0 ) );


    // Apply ukuran sesuai konfirgurasi
    ComputeMeshScalingBox( g_Render.meshCell, &g_Render.mCellWorld, g_MinBound + D3DXVECTOR3( -0.2f, -0.2f, -0.2f ),
                           g_MaxBound + D3DXVECTOR3( 0.2f, 0.2f, 0.2f ) );
    ComputeMeshScaling( g_Render.meshAmmo, &g_Render.mAmmoWorld, AMMO_SIZE * 0.5f );
    ComputeMeshScaling( g_Render.meshDroid, &g_Render.mDroidWorld, DROID_SIZE * 0.5f );
	ComputeMeshScaling( g_Render.meshChestGreen, &g_Render.mChestWorld, CHEST_SIZE * 0.5f );
	ComputeMeshScaling( g_Render.meshChestYellow, &g_Render.mChestWorld, CHEST_SIZE * 0.5f );
	ComputeMeshScaling( g_Render.meshChestRed, &g_Render.mChestWorld, CHEST_SIZE * 0.5f );


    // Rendering technique (FF atau shader)
    SetEffectTechnique();
    g_Render.pEffect->SetBool( "g_bUseSpecular", !g_Render.DisableSpecular );
    g_Render.pEffect->SetBool( "g_bUseAnisotropic", g_Render.UseAnisotropicFilter );
    g_Render.pEffect->SetInt( "g_MinFilter", g_Render.UseAnisotropicFilter ? D3DTEXF_ANISOTROPIC : D3DTEXF_LINEAR );
    g_Render.pEffect->SetInt( "g_MaxAnisotropy", g_Render.UseAnisotropicFilter ? 8 : 1 );
    g_Render.pEffect->SetTexture( g_Render.hNormalMap, g_Render.pDefaultNormalMap );

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Fungsi perhitungan scaling dari mesh
//--------------------------------------------------------------------------------------
void ComputeMeshScaling( CDXUTXFileMesh& Mesh, D3DXMATRIX* pmScalingCenter, float fNewRadius )
{
    LPVOID pVerts = NULL;
    D3DXMatrixIdentity( pmScalingCenter );
    if( SUCCEEDED( Mesh.GetMesh()->LockVertexBuffer( 0, &pVerts ) ) )
    {
        D3DXVECTOR3 vCtr;
        float fRadius;
        if( SUCCEEDED( D3DXComputeBoundingSphere( ( const D3DXVECTOR3* )pVerts,
                                                  Mesh.GetMesh()->GetNumVertices(),
                                                  Mesh.GetMesh()->GetNumBytesPerVertex(),
                                                  &vCtr, &fRadius ) ) )
        {
            D3DXMatrixTranslation( pmScalingCenter, -vCtr.x, -vCtr.y, -vCtr.z );
            D3DXMATRIXA16 m;
            D3DXMatrixScaling( &m, fNewRadius / fRadius,
                               fNewRadius / fRadius,
                               fNewRadius / fRadius );
            D3DXMatrixMultiply( pmScalingCenter, pmScalingCenter, &m );
        }
        Mesh.GetMesh()->UnlockVertexBuffer();
    }
}


//--------------------------------------------------------------------------------------
void ComputeMeshScalingBox( CDXUTXFileMesh& Mesh, D3DXMATRIX* pmScalingCenter, D3DXVECTOR3 vNewMin,
                            D3DXVECTOR3 vNewMax )
{
    LPVOID pVerts = NULL;
    D3DXMatrixIdentity( pmScalingCenter );
    if( SUCCEEDED( Mesh.GetMesh()->LockVertexBuffer( 0, &pVerts ) ) )
    {
        D3DXVECTOR3 vCurMin;
        D3DXVECTOR3 vCurMax;
        D3DXVECTOR3 vCurCtr;
        D3DXVECTOR3 vNewCtr;

        if( SUCCEEDED( D3DXComputeBoundingBox( ( const D3DXVECTOR3* )pVerts,
                                               Mesh.GetMesh()->GetNumVertices(),
                                               Mesh.GetMesh()->GetNumBytesPerVertex(),
                                               &vCurMin, &vCurMax ) ) )
        {
            D3DXMATRIXA16 m;
            D3DXMatrixScaling( &m, ( vNewMax.x - vNewMin.x ) / ( vCurMax.x - vCurMin.x ),
                               ( vNewMax.y - vNewMin.y ) / ( vCurMax.y - vCurMin.y ),
                               ( vNewMax.z - vNewMin.z ) / ( vCurMax.z - vCurMin.z ) );

            vCurCtr = ( vCurMax + vCurMin ) / 2.0f;
            vNewCtr = ( vNewMax + vNewMin ) / 2.0f;

            D3DXMATRIX mShift;
            D3DXMatrixTranslation( &mShift, vNewCtr.x - vCurCtr.x,
                                   vNewCtr.y - vCurCtr.y,
                                   vNewCtr.z - vCurCtr.z );

            D3DXMatrixMultiply( pmScalingCenter, pmScalingCenter, &m );
        }
        Mesh.GetMesh()->UnlockVertexBuffer();
    }
}


//--------------------------------------------------------------------------------------
// Effek dan Technique
//--------------------------------------------------------------------------------------
void SetEffectTechnique()
{
    if( g_Render.UseFixedFunction )
    {
        g_Render.pEffect->SetTechnique( "FFRenderScene" );
    }
    else
    {
        switch( g_Render.ForceShader )
        {
            case 9999:
                // "do not use shader"
                g_Render.hShaderTech = g_Render.pEffect->GetTechniqueByName( "FFRenderScene" );
                break;

            case 9998:
                // shader 2_a
                g_Render.hShaderTech = g_Render.pEffect->GetTechniqueByName( "RenderScenePS20A" );
                break;

            case 9997:
                // shader 2_b
                g_Render.hShaderTech = g_Render.pEffect->GetTechniqueByName( "RenderScenePS20B" );
                break;

            case 0:
                // highest valid technique.
                g_Render.pEffect->FindNextValidTechnique( NULL, &g_Render.hShaderTech );
                break;
        }

        g_Render.pEffect->SetTechnique( g_Render.hShaderTech );
    }
}


//--------------------------------------------------------------------------------------
// Kode yang dipanggil ketika D3D dalam kondisi reset (misal pada saat lost device)
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice,
                                const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;

    V_RETURN( g_Render.DialogResourceManager.OnD3D9ResetDevice() );

    if( g_Render.pFont )
        V_RETURN( g_Render.pFont->OnResetDevice() );
	if( g_Render.pFont1 )
        V_RETURN( g_Render.pFont1->OnResetDevice() );
	if( g_Render.pFont2 )
        V_RETURN( g_Render.pFont2->OnResetDevice() );
	if( g_Render.pFont3 )
        V_RETURN( g_Render.pFont3->OnResetDevice() );
    if( g_Render.pEffect )
        V_RETURN( g_Render.pEffect->OnResetDevice() );
	if( g_Render.pShop )
		V_RETURN ( g_Render.pShop->OnResetDevice() );
	if ( g_Render.pTool )
		V_RETURN ( g_Render.pTool->OnResetDevice() );
	if ( g_Render.pHelper )
		V_RETURN ( g_Render.pHelper->OnResetDevice() );
	if ( g_Render.pStatus )
		V_RETURN ( g_Render.pStatus->OnResetDevice() );
    g_Render.meshCell.RestoreDeviceObjects( pd3dDevice );
    g_Render.meshAmmo.RestoreDeviceObjects( pd3dDevice );
    g_Render.meshDroid.RestoreDeviceObjects( pd3dDevice );
    g_Render.meshDroidExplosion.RestoreDeviceObjects( pd3dDevice );
    g_Render.meshDroidLow.RestoreDeviceObjects( pd3dDevice );
    g_Render.meshDroidExplosionLow.RestoreDeviceObjects( pd3dDevice );
	g_Render.meshChestGreen.RestoreDeviceObjects ( pd3dDevice );
	g_Render.meshChestYellow.RestoreDeviceObjects ( pd3dDevice );
	g_Render.meshChestRed.RestoreDeviceObjects ( pd3dDevice );

    pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

    
	V_RETURN( D3DXCreateSprite( pd3dDevice, &g_Render.pTextSprite ) );
	
    
	float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
    g_Camera.SetProjParams( D3DX_PI / 4, fAspectRatio, 0.1f, 1000.0f );
	
    if( DXUTIsWindowed() )
    {

		g_Camera.SetRotateButtons( 0, 0, false, true );
        g_Camera.SetResetCursorAfterMove( true );
    }
    else
    {
        g_Camera.SetRotateButtons( 0, 0, false, true );
        g_Camera.SetResetCursorAfterMove( true );
    }

	g_Render.iPixelH = pBackBufferSurfaceDesc->Height;
	g_Render.iPixelW = pBackBufferSurfaceDesc->Width;

    g_Render.MainMenuDlg.SetBackgroundColors(	D3DCOLOR_ARGB( 131, 131, 131, 123 ),
												D3DCOLOR_ARGB( 141, 131, 131, 123 ),
												D3DCOLOR_ARGB( 151, 131, 131, 123 ),
												D3DCOLOR_ARGB( 71, 71, 73, 80 ) );
    g_Render.MainMenuDlg.SetLocation( ( pBackBufferSurfaceDesc->Width - 250 ) / 2,
                                      ( pBackBufferSurfaceDesc->Height - 300 ) / 2 );
    g_Render.MainMenuDlg.SetSize( 250, 300 );

    g_Render.AudioMenuDlg.SetBackgroundColors( D3DCOLOR_ARGB( 131, 131, 131, 123 ),
                                               D3DCOLOR_ARGB( 141, 131, 131, 123 ),
                                               D3DCOLOR_ARGB( 151, 131, 131, 123 ),
                                               D3DCOLOR_ARGB( 71, 71, 73, 80 ) );
    g_Render.AudioMenuDlg.SetLocation( ( pBackBufferSurfaceDesc->Width - 250 ) / 2,
                                       ( pBackBufferSurfaceDesc->Height - 300 ) / 2 );
    g_Render.AudioMenuDlg.SetSize( 250, 300 );

    g_Render.VideoMenuDlg.SetBackgroundColors( D3DCOLOR_ARGB( 131, 131, 131, 123 ),
                                               D3DCOLOR_ARGB( 141, 131, 131, 123 ),
                                               D3DCOLOR_ARGB( 151, 131, 131, 123 ),
                                               D3DCOLOR_ARGB( 71, 71, 73, 80 ) );
    g_Render.VideoMenuDlg.SetLocation( ( pBackBufferSurfaceDesc->Width - 250 ) / 2,
                                       ( pBackBufferSurfaceDesc->Height - 300 ) / 2 );
    g_Render.VideoMenuDlg.SetSize( 250, 300 );

    g_Render.IntroDlg.SetLocation( ( pBackBufferSurfaceDesc->Width - 300 ), 
                                       ( pBackBufferSurfaceDesc->Height - 400 ));
    g_Render.IntroDlg.SetSize( 300, 400 );

	g_Render.HelpDlg.SetBackgroundColors( D3DCOLOR_ARGB( 131, 131, 131, 123 ),
                                               D3DCOLOR_ARGB( 141, 131, 131, 123 ),
                                               D3DCOLOR_ARGB( 151, 131, 131, 123 ),
                                               D3DCOLOR_ARGB( 71, 71, 73, 80 ) );
    g_Render.HelpDlg.SetLocation( ( pBackBufferSurfaceDesc->Width - 250 ) / 2,
                                       ( pBackBufferSurfaceDesc->Height - 300 ) / 2 );
    g_Render.HelpDlg.SetSize( 250, 300 );

    PlayBGMusic();

    DXUTSetCursorSettings( ( g_GameState.gameMode != GAME_RUNNING ), true );


    return S_OK;
}


//--------------------------------------------------------------------------------------
void FireAmmo()
{
    
	// cek apakah masih ada ammo

	if (g_GameState.iAmmo > 0) {
		g_GameState.iAmmo--;
		double fOldest = g_GameState.AmmoQ[0].fTimeCreated;
		int nOldestIndex = 0;
		int nInactiveIndex = -1;
		for( int iAmmo = 0; iAmmo < MAX_AMMO; iAmmo++ )
		{
			if( !g_GameState.AmmoQ[iAmmo].bActive )
			{
				nInactiveIndex = iAmmo;
				break;
			}
			if( g_GameState.AmmoQ[iAmmo].fTimeCreated < fOldest )
			{
				fOldest = g_GameState.AmmoQ[iAmmo].fTimeCreated;
				nOldestIndex = iAmmo;
			}
		}

		if( nInactiveIndex < 0 )
		{
			g_GameState.AmmoQ[nOldestIndex].bActive = false;
			g_GameState.nAmmoCount--;
			nInactiveIndex = nOldestIndex;
		}

		int nNewAmmoIndex = nInactiveIndex;

		// Dapatkan inverse view matrix
		D3DXMATRIXA16 mInvView;
		D3DXMatrixInverse( &mInvView, NULL, g_Camera.GetViewMatrix() );

		// hitung kecepatan dari camera space
		D3DXVECTOR4 InitialVelocity( 0.0f, 0.0f, 6.0f, 0.0f );
		D3DXVec4Transform( &InitialVelocity, &InitialVelocity, &mInvView );
		D3DXVECTOR4 InitialPosition( 0.0f, -0.15f, 0.0f, 1.0f );
		D3DXVec4Transform( &InitialPosition, &InitialPosition, &mInvView );

		PlayAudioCue( g_audioState.iAmmoFire );
		CreateAmmo( nNewAmmoIndex, InitialPosition, InitialVelocity );
	}
}


//--------------------------------------------------------------------------------------
float GetDistFromWall( D3DXVECTOR3 P1, D3DXVECTOR3 P2, D3DXVECTOR3 P3, D3DXVECTOR3 N )
{
    
	D3DXVECTOR3 P3minusP1;
    D3DXVECTOR3 P2minusP1;
    D3DXVec3Subtract( &P3minusP1, &P3, &P1 );
    D3DXVec3Subtract( &P2minusP1, &P2, &P1 );
    float a = D3DXVec3Dot( &N, &P3minusP1 );
    float b = D3DXVec3Dot( &N, &P2minusP1 );
    if( b < 0.01f )
    {
        float u = a / b;
        D3DXVECTOR3 PonLineInterestingPlane = P1 + u * P2minusP1;
        D3DXVECTOR3 P1ToP = P1 - PonLineInterestingPlane;
        float fDist = D3DXVec3Length( &P1ToP );

        return fDist;
    }
    else
    {
        return 9999999.0f;
    }
}

// -------------------------------------------------------------------------------------
void DroidGreedy (int A)
{
	int i = MAX_CHEST;
	int min = -1;
	float fmin = 99;
	float tmp = 99;
	float fDistFromWalls = 0.0f;
    int nAngle;
    float fRotAngle = 0.0f;
	D3DXVECTOR3 vCount;

	D3DXMATRIXA16 mRot;
    D3DXQUATERNION q;

	
	float fX; // = g_Camera.GetEyePt()->x - g_GameState.DroidQ[A].vPosition.x;
	float fY; // = g_Camera.GetEyePt()->y - g_GameState.DroidQ[A].vPosition.y;
	float fZ; // = g_Camera.GetEyePt()->z - g_GameState.DroidQ[A].vPosition.z;
	
	// greedy berdasarkan masa jenis (ro)
	// peti merah memiliki masa jenis paling tinggi, sehingga BOT akan fokus
	// mengejar peti merah (jika punya tool 1)

	if (g_GameState.DroidQ[A].iTool1 > 0) {
		for (i=0; i<MAX_CHEST;i++) {
			vCount = g_GameState.ChestQ[i].vPosition - g_GameState.DroidQ[A].vPosition;
			if (min == -1 && g_GameState.ChestQ[i].bTargeted == false && g_GameState.ChestQ[i].iChestType == 3 && g_GameState.ChestQ[i].bActive == true){
				min = i;
				fmin = D3DXVec3Length(&vCount);
			}else if(g_GameState.ChestQ[i].bTargeted == false && g_GameState.ChestQ[i].iChestType == 3 && g_GameState.ChestQ[i].bActive == true){
				tmp = D3DXVec3Length(&vCount);
				if (tmp < fmin) {
					min = i;
					fmin = tmp;
				}
			}
		}
	}

	//// Greedy by Jarak (peti kuning)
	//// dapatkan peti dengan jarak terdekat (peti hijau diabaikan)

	if ((g_GameState.DroidQ[A].iTool2 > 0 || g_GameState.DroidQ[A].iTool1 > 0) && min == -1) { 
		for (i=0; i<MAX_CHEST;i++) {
			vCount = g_GameState.ChestQ[i].vPosition - g_GameState.DroidQ[A].vPosition;
			if (min == -1 && g_GameState.ChestQ[i].bTargeted == false && g_GameState.ChestQ[i].iChestType == 2 && g_GameState.ChestQ[i].bActive == true){
				min = i;
				fmin = D3DXVec3Length(&vCount);
			}else if(g_GameState.ChestQ[i].bTargeted == false && g_GameState.ChestQ[i].iChestType == 2 && g_GameState.ChestQ[i].bActive == true){
				tmp = D3DXVec3Length(&vCount);
				if (tmp < fmin) {
					min = i;
					fmin = tmp;
				}
			}
		}
	}

	if ((g_GameState.DroidQ[A].iTool2 > 0 || g_GameState.DroidQ[A].iTool1 > 0) && min == -1) { 
		for (i=0; i<MAX_CHEST;i++) {
			vCount = g_GameState.ChestQ[i].vPosition - g_GameState.DroidQ[A].vPosition;
			if (min == -1 &&  g_GameState.ChestQ[i].iChestType == 2 && g_GameState.ChestQ[i].bActive == true){
				min = i;
				fmin = D3DXVec3Length(&vCount);
			}else if(g_GameState.ChestQ[i].iChestType == 2 && g_GameState.ChestQ[i].bActive == true){
				tmp = D3DXVec3Length(&vCount);
				if (tmp < fmin) {
					min = i;
					fmin = tmp;
				}
			}
		}
	}

	g_GameState.DroidQ[A].min = min;
	// greedy by value : lebih baik tidak membuka daripada membuka peti hijau (membuang-buang waktu)
	// muter-muter sejenak, lalu mencari peti yang masih tersedia
	if (min == -1) {
		Play3DAudioCue( g_audioState.iDroidScan, &g_GameState.DroidQ[A].vPosition );
		g_GameState.DroidQ[A].bBusy = false;
        
		nAngle = ( rand() % 360 );
		float fRotAngle = 0.0f;
		while( fDistFromWalls < 1.0f )
		{
			nAngle += 62; nAngle %= 360;
			fRotAngle = ( nAngle / 360.0f ) * D3DX_PI * 2.0f;
			D3DXQuaternionRotationYawPitchRoll( &q, fRotAngle, 0.0f, 0.0f );
			D3DXMatrixRotationQuaternion( &mRot, &q );
			D3DXVECTOR3 vVel = D3DXVECTOR3( mRot._31, mRot._32, mRot._33 );
			D3DXVECTOR3 vPos = g_GameState.DroidQ[A].vPosition;
			D3DXVECTOR3 vPosPlus = vPos + vVel;
		
			D3DXVECTOR3 vWallPtA = D3DXVECTOR3( g_MinBound.x, vPos.y, g_MinBound.z );
			D3DXVECTOR3 vWallPtB = D3DXVECTOR3( g_MaxBound.x, vPos.y, g_MinBound.z );
			D3DXVECTOR3 vWallPtC = D3DXVECTOR3( g_MinBound.x, vPos.y, g_MaxBound.z );
			D3DXVECTOR3 vWallPtD = D3DXVECTOR3( g_MaxBound.x, vPos.y, g_MaxBound.z );
			D3DXVECTOR3 vWallABNormal = D3DXVECTOR3( 0, 0, 1 );
			D3DXVECTOR3 vWallACNormal = D3DXVECTOR3( 1, 0, 0 );
			D3DXVECTOR3 vWallBDNormal = D3DXVECTOR3( 0, 0, -1 );
			D3DXVECTOR3 vWallCDNormal = D3DXVECTOR3( -1, 0, 0 );

			float fDistFromWallAB = GetDistFromWall( vPos, vPosPlus, vWallPtA, vWallABNormal );
			float fDistFromWallAC = GetDistFromWall( vPos, vPosPlus, vWallPtA, vWallACNormal );
			float fDistFromWallBD = GetDistFromWall( vPos, vPosPlus, vWallPtD, vWallBDNormal );
			float fDistFromWallCD = GetDistFromWall( vPos, vPosPlus, vWallPtD, vWallCDNormal );
			float fMin1 = min( fDistFromWallAB, fDistFromWallAC );
			float fMin2 = min( fDistFromWallBD, fDistFromWallCD );
			fDistFromWalls = min( fMin1, fMin2 );
		}

		g_GameState.DroidQ[A].fTargetRotation = fRotAngle;
		D3DXQuaternionRotationYawPitchRoll( &g_GameState.DroidQ[A].qTarget, g_GameState.DroidQ[A].fTargetRotation, 0.0f, 0.0f );
		g_GameState.DroidQ[A].qStart = g_GameState.DroidQ[A].qCurrent;
		g_GameState.DroidQ[A].fRotInterp = 0.0f;
		g_GameState.DroidQ[A].aiState = AI_TURNING;
		g_GameState.DroidQ[A].vVelocity = D3DXVECTOR3( 0, 0, 0 );
		g_GameState.DroidQ[A].vNudgeVelocity = D3DXVECTOR3( 0, 0, 0 );
		return;
	}

	g_GameState.ChestQ [min].bTargeted = true;
	g_GameState.DroidQ[A].bBusy = true;
	// greedy berdasarkan jarak

	
	fX = g_GameState.ChestQ[min].vPosition.x - g_GameState.DroidQ[A].vPosition.x;
	fY = g_GameState.ChestQ[min].vPosition.y - g_GameState.DroidQ[A].vPosition.y;
	fZ = g_GameState.ChestQ[min].vPosition.z - g_GameState.DroidQ[A].vPosition.z;
	fRotAngle = (D3DX_PI/2) - atan(fZ/fX);

	vCount.x = fX;
	vCount.y = fY;
	vCount.z = fZ;
	fmin = D3DXVec3Length(&vCount);

	float fRotAngleY =  -asin(fY / fmin);

	g_GameState.DroidQ[A].fTargetRotation = fRotAngle;
    D3DXQuaternionRotationYawPitchRoll( &g_GameState.DroidQ[A].qTarget, g_GameState.DroidQ[A].fTargetRotation, fRotAngleY, 0.0f );
    D3DXMatrixRotationQuaternion( &mRot, &g_GameState.DroidQ[A].qTarget );
	D3DXVECTOR3 vVel = D3DXVECTOR3( mRot._31, mRot._32, mRot._33 );

	if ((vVel.x > 0 && fX < 0) || (vVel.x < 0 && fX > 0) || (vVel.z > 0 && fZ < 0) || (vVel.z < 0 && fZ > 0)){
		fRotAngle += D3DX_PI;
		if (fRotAngle > 6.28) fRotAngle -= 2*D3DX_PI;
		g_GameState.DroidQ[A].fTargetRotation = fRotAngle;
		D3DXQuaternionRotationYawPitchRoll( &g_GameState.DroidQ[A].qTarget, g_GameState.DroidQ[A].fTargetRotation, fRotAngleY, 0.0f );
		D3DXMatrixRotationQuaternion( &mRot, &g_GameState.DroidQ[A].qTarget );
	}
    
    g_GameState.DroidQ[A].qStart = g_GameState.DroidQ[A].qCurrent;
    g_GameState.DroidQ[A].fRotInterp = 0.0f;
    g_GameState.DroidQ[A].aiState = AI_TURNING;
    g_GameState.DroidQ[A].vVelocity = D3DXVECTOR3( 0, 0, 0 );
    g_GameState.DroidQ[A].vNudgeVelocity = D3DXVECTOR3( 0, 0, 0 );
	
}

//--------------------------------------------------------------------------------------
void DroidPickNewDirection( int A )
{
	DroidGreedy(A);
	
}

//--------------------------------------------------------------------------------------
void DroidChooseNewTask( int A )
{
	if (!g_GameState.bDroidMove || g_GameState.DroidQ[A].bBusy){
		if( g_GameState.DroidQ[A].aiState == AI_STOPPED )
		{
			g_GameState.DroidQ[A].aiState = AI_MOVING;
			D3DXMATRIXA16 mRot;
			D3DXMatrixRotationQuaternion( &mRot, &g_GameState.DroidQ[A].qTarget );
			g_GameState.DroidQ[A].vVelocity = D3DXVECTOR3( mRot._31, mRot._32, mRot._33 ) * DROID_VELOCITY;
			g_GameState.DroidQ[A].fTaskTimer = ( ( rand() % DROID_MOVE_TIME_FLUX ) + 2 * DROID_MOVE_TIME_MIN ) / 1000.0f;
		}
		else
		{
			Play3DAudioCue( g_audioState.iDroidScan, &g_GameState.DroidQ[A].vPosition );
			g_GameState.DroidQ[A].aiState = AI_STOPPED;
			g_GameState.DroidQ[A].fTaskTimer = 1.5f;
			g_GameState.DroidQ[A].fRotInterp = 0.0f;
			g_GameState.DroidQ[A].qStart = g_GameState.DroidQ[A].qCurrent;
			g_GameState.DroidQ[A].vVelocity = D3DXVECTOR3( 0, 0, 0 );
		}
	}else{
		int nRand = rand() % 100;
		if( nRand < DROID_TURN_AI_PERCENT && g_GameState.DroidQ[A].aiState != AI_STOPPED )
		{
			DroidPickNewDirection( A );
		}
		else if( nRand < ( DROID_TURN_AI_PERCENT + DROID_TURN_AI_PERCENT ) || g_GameState.DroidQ[A].aiState == AI_STOPPED )
		{
			g_GameState.DroidQ[A].aiState = AI_MOVING;
			D3DXMATRIXA16 mRot;
			D3DXMatrixRotationQuaternion( &mRot, &g_GameState.DroidQ[A].qTarget );
			g_GameState.DroidQ[A].vVelocity = D3DXVECTOR3( mRot._31, mRot._32, mRot._33 ) * DROID_VELOCITY;
			g_GameState.DroidQ[A].fTaskTimer = ( ( rand() % DROID_MOVE_TIME_FLUX ) + DROID_MOVE_TIME_MIN ) / 1000.0f;
		}
		else
		{
			Play3DAudioCue( g_audioState.iDroidScan, &g_GameState.DroidQ[A].vPosition );
			g_GameState.DroidQ[A].aiState = AI_STOPPED;
			g_GameState.DroidQ[A].fTaskTimer = 1.5f;
			g_GameState.DroidQ[A].fRotInterp = 0.0f;
			g_GameState.DroidQ[A].qStart = g_GameState.DroidQ[A].qCurrent;
			g_GameState.DroidQ[A].vVelocity = D3DXVECTOR3( 0, 0, 0 );
		}
	}
}


//--------------------------------------------------------------------------------------
void HandleDroidAI( float fElapsedTime )
{
	D3DXQUATERNION q; int i;
    for( int A = 0; A < MAX_DROID; A++ )
    {
        if( !g_GameState.DroidQ[A].bActive )
            continue;

        if( g_GameState.DroidQ[A].nHitPoints > 0 )
        {
            if( g_GameState.DroidQ[A].fAlpha < 1.0f )
            {
                g_GameState.DroidQ[A].fAlpha += fElapsedTime;
                g_GameState.DroidQ[A].fAlpha = min( g_GameState.DroidQ[A].fAlpha, 1.0f );
            }

			if (g_Task[A].bEnabled) {
				g_GameState.DroidQ[A].fRotInterp += fElapsedTime * 10.0f;
				if( g_GameState.DroidQ[A].fRotInterp > 4.0f * D3DX_PI )
					g_GameState.DroidQ[A].fRotInterp = 4.0f * D3DX_PI;
				float fRot = sinf( g_GameState.DroidQ[A].fRotInterp ) * 0.3f;
				D3DXQuaternionRotationYawPitchRoll( &q, 0.0f, 0.0f , fRot );
				D3DXQuaternionMultiply( &g_GameState.DroidQ[A].qCurrent, &q, &g_GameState.DroidQ[A].qStart );
			}else{
				if (g_GameState.DroidQ[A].iTool1 > 0 || g_GameState.DroidQ[A].iTool2 > 0) {
					for (i=0; i<MAX_CHEST; i++) {
						if (g_GameState.ChestQ[i].bActive == true &&  pow((float)(pow(g_GameState.ChestQ[i].vPosition.x - g_GameState.DroidQ[A].vPosition.x, 2) + pow(g_GameState.ChestQ[i].vPosition.y - g_GameState.DroidQ[A].vPosition.y, 2) + pow(g_GameState.ChestQ[i].vPosition.z - g_GameState.DroidQ[A].vPosition.z, 2))  ,0.5f) <= 0.35){
								g_Task[A].bEnabled = true;
								g_Task[A].IDC = i;
								if (g_GameState.DroidQ[A].iTool1 > 0) {
									g_GameState.DroidQ[A].iTool1--;
									g_Task[A].iTimer = 13;
								}else{
									g_GameState.DroidQ[A].iTool2--;
									g_Task[A].iTimer = 23;
								}
								break;
						} 
					}
				}

				switch( g_GameState.DroidQ[A].aiState )
				{
					case AI_TURNING:
					{
						g_GameState.DroidQ[A].fRotInterp += fElapsedTime;
						if( g_GameState.DroidQ[A].fRotInterp > 1.0f )
							g_GameState.DroidQ[A].fRotInterp = 1.0f;
						D3DXQuaternionSlerp( &g_GameState.DroidQ[A].qCurrent, &g_GameState.DroidQ[A].qStart,
											 &g_GameState.DroidQ[A].qTarget, g_GameState.DroidQ[A].fRotInterp );

						if( g_GameState.DroidQ[A].fRotInterp == 1.0f )
						{
							D3DXMATRIXA16 mRot;
							D3DXMatrixRotationQuaternion( &mRot, &g_GameState.DroidQ[A].qCurrent );
							g_GameState.DroidQ[A].vVelocity = D3DXVECTOR3( mRot._31, mRot._32, mRot._33 ) * DROID_VELOCITY;
							g_GameState.DroidQ[A].aiState = AI_MOVING;
						}
						break;
					}

					case AI_MOVING:
					{
						if( g_GameState.bDroidMove )
							g_GameState.DroidQ[A].vPosition += g_GameState.DroidQ[A].vVelocity * fElapsedTime;

						// Pengecekan tembok depan dan belakang
						if( g_GameState.DroidQ[A].vPosition.z < g_MinBound.z + ( DROID_SIZE * 0.6f ) )
						{
							g_GameState.DroidQ[A].vPosition.z = g_MinBound.z + ( DROID_SIZE * 0.6f );
							DroidPickNewDirection( A );
						}
						if( g_GameState.DroidQ[A].vPosition.z > g_MaxBound.z - ( DROID_SIZE * 0.6f ) )
						{
							g_GameState.DroidQ[A].vPosition.z = g_MaxBound.z - ( DROID_SIZE * 0.6f );
							DroidPickNewDirection( A );
						}

						// Pengecekan tembk atas bawah
						if( g_GameState.DroidQ[A].vPosition.y < g_MinBound.y + ( DROID_SIZE * 0.6f ) )
						{
							g_GameState.DroidQ[A].vPosition.y = g_MinBound.y + ( DROID_SIZE * 0.6f );
							DroidPickNewDirection( A );
						}
						if( g_GameState.DroidQ[A].vPosition.y > g_MaxBound.y - ( DROID_SIZE * 0.6f ) )
						{
							g_GameState.DroidQ[A].vPosition.y = g_MaxBound.y - ( DROID_SIZE * 0.6f );
							DroidPickNewDirection( A );
						}

						// Pengecekan tembok kanan dan kiri
						if( g_GameState.DroidQ[A].vPosition.x < g_MinBound.x + ( DROID_SIZE * 0.6f ) )
						{
							g_GameState.DroidQ[A].vPosition.x = g_MinBound.x + ( DROID_SIZE * 0.6f );
							DroidPickNewDirection( A );
						}
						if( g_GameState.DroidQ[A].vPosition.x > g_MaxBound.x - ( DROID_SIZE * 0.6f ) )
						{
							g_GameState.DroidQ[A].vPosition.x = g_MaxBound.x - ( DROID_SIZE * 0.6f );
							DroidPickNewDirection( A );
						}

						g_GameState.DroidQ[A].fTaskTimer -= fElapsedTime;

						if( g_GameState.DroidQ[A].fTaskTimer < 0.0f )
							DroidChooseNewTask( A );
							//DroidGreedy(A);
						break;
					}

					case AI_STOPPED:
					{
						
						g_GameState.DroidQ[A].fRotInterp += fElapsedTime * 10.0f;
						if( g_GameState.DroidQ[A].fRotInterp > 4.0f * D3DX_PI )
							g_GameState.DroidQ[A].fRotInterp = 4.0f * D3DX_PI;
						float fRot = sinf( g_GameState.DroidQ[A].fRotInterp ) * 0.3f;
						D3DXQuaternionRotationYawPitchRoll( &q, 0.0f, 0.0f , fRot );
						D3DXQuaternionMultiply( &g_GameState.DroidQ[A].qCurrent, &q, &g_GameState.DroidQ[A].qStart );

						g_GameState.DroidQ[A].fTaskTimer -= fElapsedTime;
						if( g_GameState.DroidQ[A].fTaskTimer < 0.0f )
							DroidChooseNewTask( A );
						break;
					}
				}
				
			}

            // Menjaga agar bot tetap dalam boundaries
            g_GameState.DroidQ[A].vPosition += g_GameState.DroidQ[A].vNudgeVelocity * fElapsedTime;
            if( g_GameState.DroidQ[A].vPosition.z < g_MinBound.z + ( DROID_SIZE * 0.6f ) )
                g_GameState.DroidQ[A].vPosition.z = g_MinBound.z + ( DROID_SIZE * 0.6f );
            if( g_GameState.DroidQ[A].vPosition.z > g_MaxBound.z - ( DROID_SIZE * 0.6f ) )
                g_GameState.DroidQ[A].vPosition.z = g_MaxBound.z - ( DROID_SIZE * 0.6f );
            if( g_GameState.DroidQ[A].vPosition.x < g_MinBound.x + ( DROID_SIZE * 0.6f ) )
                g_GameState.DroidQ[A].vPosition.x = g_MinBound.x + ( DROID_SIZE * 0.6f );
            if( g_GameState.DroidQ[A].vPosition.x > g_MaxBound.x - ( DROID_SIZE * 0.6f ) )
                g_GameState.DroidQ[A].vPosition.x = g_MaxBound.x - ( DROID_SIZE * 0.6f );
            if( g_GameState.DroidQ[A].vPosition.y < -GROUND_Y + ( DROID_SIZE * 0.6f ) )
                g_GameState.DroidQ[A].vPosition.y = -GROUND_Y + ( DROID_SIZE * 0.6f );
            if( g_GameState.DroidQ[A].vPosition.y > GROUND_Y - ( DROID_SIZE * 0.6f ) )
                g_GameState.DroidQ[A].vPosition.y = GROUND_Y - ( DROID_SIZE * 0.6f );

            
            g_GameState.DroidQ[A].vNudgeVelocity -= g_GameState.DroidQ[A].vNudgeVelocity * ( fElapsedTime * 20.0f );
        }
        else
        {
            // Ledakan bot
            g_GameState.DroidQ[A].fDeathAnimation += fElapsedTime * DROID_DEATH_SPEED;
            if( g_GameState.DroidQ[A].fDeathAnimation > 1.0f )
            {
                if( g_GameState.DroidQ[A].fAlpha > 0.0f )
                {
                    g_GameState.DroidQ[A].fAlpha -= fElapsedTime * DROID_DEATH_SPEED;
                    g_GameState.DroidQ[A].fAlpha = max( g_GameState.DroidQ[A].fAlpha, 0.0f );
                }
                else
                {
                    g_GameState.DroidQ[A].bActive = false;
                    g_GameState.nDroidCount--;
                    SetNumDroidsForAudio( g_GameState.nDroidCount );
                }
            }
        }
    }
}


//--------------------------------------------------------------------------------------
void CheckForAmmoToDroidCollision( int A )
{
    for( int B = 0; B < MAX_DROID; B++ )
    {
        if( !g_GameState.DroidQ[B].bActive || g_GameState.DroidQ[B].nHitPoints <= 0 )
            continue;

		// Handle kasus tabrakan ammo
        D3DXVECTOR3 vAToB = - g_GameState.AmmoQ[A].vPosition + g_GameState.DroidQ[B].vPosition;
        float DistSq = D3DXVec3LengthSq( &vAToB );
        if( DistSq < ( ( AMMO_SIZE + DROID_SIZE ) / 2.0f ) * ( ( AMMO_SIZE + DROID_SIZE ) / 2.0f ) )
        {
            D3DXVec3Normalize( &vAToB, &vAToB );

            // Cek apakah bot sudah jauh dari jangkauan ketika peluru sampai target.
            float fImpact = D3DXVec3Dot( &vAToB, &g_GameState.AmmoQ[A].vVelocity ) -
                D3DXVec3Dot( &vAToB, &g_GameState.DroidQ[B].vVelocity );
            if( fImpact > 0.0f )
            {
                g_GameState.AmmoQ[A].bActive = false;
                //g_GameState.DroidQ[B].vNudgeVelocity += g_GameState.AmmoQ[A].vVelocity * 0.3f;

                g_GameState.DroidQ[B].nHitPoints--;
                if( g_GameState.DroidQ[B].nHitPoints <= 0 )
                {
                    g_GameState.DroidQ[B].fDeathAnimation = 0.0f;
                    Play3DAudioCue( g_audioState.iDroidDestroyed, &g_GameState.DroidQ[B].vPosition );
                }
            }
        }
    }
}


//--------------------------------------------------------------------------------------
void CheckForInterAmmoCollision( float fElapsedTime )
{
    if( g_GameState.nAmmoCount <= 1 )
        return;

    for( int A = 0; A < MAX_AMMO - 1; A++ )
    {
        if( !g_GameState.AmmoQ[A].bActive )
            continue;

        for( int B = A + 1; B < MAX_AMMO; B++ )
        {
            if( !g_GameState.AmmoQ[B].bActive )
                continue;

            // Cek untuk tabrakan antar ammo
            D3DXVECTOR3 vAToB = g_GameState.AmmoQ[B].vPosition - g_GameState.AmmoQ[A].vPosition;
            float DistSq = D3DXVec3LengthSq( &vAToB );
            if( DistSq < AMMO_SIZE * AMMO_SIZE )
            {
                D3DXVec3Normalize( &vAToB, &vAToB );

                // Cek apakah ammo saling sudah jauh pada saat ammo mencapai target
                float fImpact = D3DXVec3Dot( &vAToB, &g_GameState.AmmoQ[A].vVelocity ) -
                    D3DXVec3Dot( &vAToB, &g_GameState.AmmoQ[B].vVelocity );
                if( fImpact > 0.0f )
                {
                    
					D3DXVECTOR3 vVelocityAN = ( 1 - BOUNCE_LOST ) *
                        D3DXVec3Dot( &vAToB, &g_GameState.AmmoQ[A].vVelocity ) * vAToB;
                    D3DXVECTOR3 vVelocityAT = ( 1 - BOUNCE_LOST ) * g_GameState.AmmoQ[A].vVelocity - vVelocityAN;

                    
					D3DXVECTOR3 vVelocityBN = ( 1 - BOUNCE_LOST ) *
                        D3DXVec3Dot( &vAToB, &g_GameState.AmmoQ[B].vVelocity ) * vAToB;
                    D3DXVECTOR3 vVelocityBT = ( 1 - BOUNCE_LOST ) * g_GameState.AmmoQ[B].vVelocity - vVelocityBN;

                    
					g_GameState.AmmoQ[A].vVelocity = vVelocityAT - vVelocityAN * ( 1 - BOUNCE_TRANSFER ) +
                        vVelocityBN * BOUNCE_TRANSFER;
                    g_GameState.AmmoQ[B].vVelocity = vVelocityBT - vVelocityBN * ( 1 - BOUNCE_TRANSFER ) +
                        vVelocityAN * BOUNCE_TRANSFER;

                    
					float fDistanceToMove = ( AMMO_SIZE - sqrtf( DistSq ) ) * 0.5f;
                    g_GameState.AmmoQ[A].vPosition -= vAToB * fDistanceToMove;
                    g_GameState.AmmoQ[B].vPosition += vAToB * fDistanceToMove;

                    
					g_GameState.AmmoQ[A].bGround = g_GameState.AmmoQ[B].bGround = false;

                }
            }
        }
    }
}


//--------------------------------------------------------------------------------------
void CheckForAmmoToWallCollision( int A )
{
    // Pengecekan untuk bounce ke tanah
    if( !g_GameState.AmmoQ[A].bGround )
    {
        if( g_GameState.AmmoQ[A].vPosition.y < -GROUND_Y + ( AMMO_SIZE * 0.5f ) )
        {
            g_GameState.AmmoQ[A].vPosition.y = -GROUND_Y + ( AMMO_SIZE * 0.5f );
            g_GameState.AmmoQ[A].vVelocity.y = -g_GameState.AmmoQ[A].vVelocity.y * ( 1 - GROUND_ABSORBANCE );
            
            // Kurangi kecepatan X dan Z.
            g_GameState.AmmoQ[A].vVelocity.x *= 0.9f;
            g_GameState.AmmoQ[A].vVelocity.z *= 0.9f;
            PlayAudioCue( g_audioState.iAmmoBounce );
        }
    }
    else
    {
        // Bola sudah dekat dengan tanah (berputar).
        g_GameState.AmmoQ[A].vVelocity.x *= 0.9f;
        g_GameState.AmmoQ[A].vVelocity.z *= 0.9f;
    }

    // Pengecekan untuk tabrakan dengan atap
    if( g_GameState.AmmoQ[A].vPosition.y > GROUND_Y - ( AMMO_SIZE * 0.5f ) )
    {
        g_GameState.AmmoQ[A].vPosition.y = GROUND_Y - ( AMMO_SIZE * 0.5f );
        g_GameState.AmmoQ[A].vVelocity.y = -g_GameState.AmmoQ[A].vVelocity.y * ( 1 - GROUND_ABSORBANCE );
        

        // Kurangi kecepatan X dan Z.
        g_GameState.AmmoQ[A].vVelocity.x *= 0.9f;
        g_GameState.AmmoQ[A].vVelocity.z *= 0.9f;
        PlayAudioCue( g_audioState.iAmmoBounce );
    }

    // Jika si bola sudah berada lebih kecil dari batas ambang, maka anggap bola sudah berhenti
    if( GRAVITY * ( g_GameState.AmmoQ[A].vPosition.y + GROUND_Y - ( AMMO_SIZE * 0.5f ) ) + 0.5f *
        g_GameState.AmmoQ[A].vVelocity.y * g_GameState.AmmoQ[A].vVelocity.y < REST_THRESHOLD )
    {
        g_GameState.AmmoQ[A].vPosition.y = -GROUND_Y + ( AMMO_SIZE * 0.5f );
        g_GameState.AmmoQ[A].vVelocity.y = 0.0f;
        g_GameState.AmmoQ[A].bGround = true;
    }

    // Pengecekan tabrakan dengan tembok depan dan belakang
    if( g_GameState.AmmoQ[A].vPosition.z < g_MinBound.z + ( AMMO_SIZE * 0.5f ) )
    {
        g_GameState.AmmoQ[A].vPosition.z = g_MinBound.z + ( AMMO_SIZE * 0.5f );
        g_GameState.AmmoQ[A].vVelocity.z = -g_GameState.AmmoQ[A].vVelocity.z * ( 1 - GROUND_ABSORBANCE );
        PlayAudioCue( g_audioState.iAmmoBounce );
    }
    if( g_GameState.AmmoQ[A].vPosition.z > g_MaxBound.z - ( AMMO_SIZE * 0.5f ) )
    {
        g_GameState.AmmoQ[A].vPosition.z = g_MaxBound.z - ( AMMO_SIZE * 0.5f );
        g_GameState.AmmoQ[A].vVelocity.z = -g_GameState.AmmoQ[A].vVelocity.z * ( 1 - GROUND_ABSORBANCE );
        PlayAudioCue( g_audioState.iAmmoBounce );
    }

    // Pengecekan untuk tabrakan dengan tembok kiri dan kanan
    if( g_GameState.AmmoQ[A].vPosition.x < g_MinBound.x + ( AMMO_SIZE * 0.5f ) )
    {
        g_GameState.AmmoQ[A].vPosition.x = g_MinBound.x + ( AMMO_SIZE * 0.5f );
        g_GameState.AmmoQ[A].vVelocity.x = -g_GameState.AmmoQ[A].vVelocity.x * ( 1 - GROUND_ABSORBANCE );
        PlayAudioCue( g_audioState.iAmmoBounce );
    }
    if( g_GameState.AmmoQ[A].vPosition.x > g_MaxBound.x - ( AMMO_SIZE * 0.5f ) )
    {
        g_GameState.AmmoQ[A].vPosition.x = g_MaxBound.x - ( AMMO_SIZE * 0.5f );
        g_GameState.AmmoQ[A].vVelocity.x = -g_GameState.AmmoQ[A].vVelocity.x * ( 1 - GROUND_ABSORBANCE );
        PlayAudioCue( g_audioState.iAmmoBounce );
    }
}


//--------------------------------------------------------------------------------------
void HandleAmmoAI( float fElapsedTime )
{
    if( g_GameState.nAmmoCount == 0 )
        return;

    CheckForInterAmmoCollision( fElapsedTime );

    // Terapkan gravitasi dan tumbukan dengan objek lain
    for( int A = 0; A < MAX_AMMO; A++ )
    {
        if( !g_GameState.AmmoQ[A].bActive )
            continue;

        g_GameState.AmmoQ[A].fLifeCountdown -= fElapsedTime;
        if( g_GameState.AmmoQ[A].fLifeCountdown < 1.0f / 0.5f )
        {
            g_GameState.AmmoQ[A].fAlpha = g_GameState.AmmoQ[A].fLifeCountdown * 0.5f;
        }

        if( g_GameState.AmmoQ[A].fLifeCountdown < 0.0f )
        {
            g_GameState.AmmoQ[A].bActive = false;
            g_GameState.nAmmoCount--;
            continue;
        }

        
		float fTimeLeft = fElapsedTime;
        float fElapsedFrameTime;

        while( fTimeLeft > 0.0f )
        {
            fElapsedFrameTime = min( fTimeLeft, PHYSICS_FRAMELENGTH );
            fTimeLeft -= fElapsedFrameTime;

            g_GameState.AmmoQ[A].vPosition += g_GameState.AmmoQ[A].vVelocity * fElapsedFrameTime;

            g_GameState.AmmoQ[A].vVelocity.x -= g_GameState.AmmoQ[A].vVelocity.x * 0.1f * fElapsedFrameTime;
            g_GameState.AmmoQ[A].vVelocity.z -= g_GameState.AmmoQ[A].vVelocity.z * 0.1f * fElapsedFrameTime;

            
			if( !g_GameState.AmmoQ[A].bGround )
                g_GameState.AmmoQ[A].vVelocity.y -= GRAVITY * fElapsedFrameTime;

            CheckForAmmoToWallCollision( A );
            CheckForAmmoToDroidCollision( A );
        }
    }
}


//--------------------------------------------------------------------------------------
// Fungsi yang dieksekusi pada setiap awal frame. Untuk update event, bukan render 
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{	
	int i,j;

	if ((g_GameState.gameMode != GAME_INTRO && g_GameState.gameMode != GAME_FINISH && g_GameState.gameMode != GAME_HELP && g_GameState.gameMode != GAME_VIDEO_INTRO) ) {
		g_GameState.fScnd += fElapsedTime;
		g_GameState.fSub += fElapsedTime;
		

		if (g_GameState.fSub > 0.1f){
			g_GameState.fSub = 0;
			for(i=0;i<=MAX_DROID;i++){
				if (g_Task[i].bEnabled == true) {
					g_Task[i].iTimer -= 1;
					if (g_Task[i].iTimer < 0) {
						g_Task[i].bEnabled = false;
						if (!g_GameState.ChestQ[g_Task[i].IDC].bActive) {
							if (i== MAX_DROID) {
								g_Render.fNotif1 = 400; 
								g_Camera.SetEnablePositionMovement (true);
								
								g_GameState.gameMode = GAME_RUNNING;
							}else{
								if (g_GameState.bAiMode){
									g_OutFile.iJenis[g_OutFile.N] = 0;
									g_OutFile.iRobot[g_OutFile.N] = i;
									g_OutFile.iSimpul[g_OutFile.N] = g_Task[i].IDC;
									g_OutFile.N++;
								}
								DroidGreedy(i);
							}
						}else{
							if (i == MAX_DROID){
								g_GameState.iGetChest++;
								switch(g_GameState.ChestQ[ g_Task[i].IDC ].iChestType ){
									case 1: g_GameState.iMoney += 100; g_Render.fNotif2 = 400;break;
									case 2: g_GameState.iMoney += 300; g_Render.fNotif3 = 400;break;
									case 3: g_GameState.iMoney += 500; g_Render.fNotif4 = 400;break;
								}
								g_GameState.ChestQ[ g_Task[i].IDC ].bActive = false;
								g_GameState.ChestQ[ g_Task[i].IDC ].chState = CH_EXPLODE;
								g_Camera.SetEnablePositionMovement (true);
								g_GameState.gameMode = GAME_RUNNING;
							}else{
								switch(g_GameState.ChestQ[ g_Task[i].IDC ].iChestType ){
									case 1: g_GameState.DroidQ [i].iScore  += 100; break;
									case 2: g_GameState.DroidQ [i].iScore  += 300; break;
									case 3: g_GameState.DroidQ [i].iScore  += 500; break;
								}

								if (g_GameState.bAiMode){
									g_OutFile.iJenis[g_OutFile.N] = g_GameState.ChestQ[g_Task[i].IDC].iChestType;
									g_OutFile.iRobot[g_OutFile.N] = i;
									g_OutFile.iSimpul[g_OutFile.N] = g_Task[i].IDC;
									g_OutFile.N++;
								}
								g_GameState.ChestQ[ g_Task[i].IDC ].bActive = false;
								g_GameState.ChestQ[ g_Task[i].IDC ].chState = CH_EXPLODE;
								DroidGreedy(i);
							}
							for(j=0;j<=MAX_DROID;j++) {
								if (g_Task[j].bEnabled == true && g_Task[j].IDC == g_Task[i].IDC) {
									g_Task[j].bEnabled = false;
									if (j == MAX_DROID) {
										g_Render.fNotif1 = 400; 
										g_Camera.SetEnablePositionMovement (true);
										g_GameState.gameMode = GAME_RUNNING;
									}else{
										if (g_GameState.bAiMode){
											g_OutFile.iJenis[g_OutFile.N] = 0;
											g_OutFile.iRobot[g_OutFile.N] = i;
											g_OutFile.iSimpul[g_OutFile.N] = g_Task[i].IDC;
											g_OutFile.N++;
										}
										DroidGreedy(j);
									}
								}
							}
						}
					}
				}
			}

			if (g_GameState.nChestCount == 0) {
				for(i=0; i<=9; i++){
					int idm = -999;
					int maks = -999;

					for(j=0;j<MAX_DROID;j++){
						if (g_GameState.DroidQ[j].iScore > maks){
							maks = g_GameState.DroidQ[j].iScore;
							idm = j;
						}
					}

					if (g_GameState.iMoney > maks && g_GameState.bAiMode == false) {
						maks = g_GameState.iMoney;
						idm = 999;
						g_GameState.iMoney = -999;
					}

					if (maks == -999)
						break;

					if (idm == 999) {
						if (i==0) {
							g_GameState.HS[i].iColor = 1;
							g_GameState.HS[i].iSkor = maks;
							g_GameState.HS[i].iID = 999;
						}else if(i > 0 && i < 3){
							g_GameState.HS[i].iColor = 1;
							g_GameState.HS[i].iSkor = maks;
							g_GameState.HS[i].iID = 888;
						}else{
							g_GameState.HS[i].iColor = 2;
							g_GameState.HS[i].iSkor = maks;
							g_GameState.HS[i].iID = 777;
						}
					}else{
						if (g_GameState.DroidQ[idm].bActive) {
							g_GameState.HS[i].iColor = 3;
							g_GameState.HS[i].iSkor = maks;
							g_GameState.HS[i].iID = idm;
						}else{
							g_GameState.HS[i].iColor = 4;
							g_GameState.HS[i].iSkor = maks;
							g_GameState.HS[i].iID = idm;
						}
						g_GameState.DroidQ[idm].iScore = -999;
					}
					
					
				}
				g_GameState.iHS = i++;
				if (g_GameState.bAiMode)
						TulisFile(g_OutFile.iRobot, g_OutFile.iJenis, g_OutFile.iSimpul, g_OutFile.N);
				DXUTSetCursorSettings(true, false);
				g_GameState.gameMode = GAME_FINISH;
			}
		}

		if (g_GameState.fScnd > 1.0f){

			g_GameState.fScnd = 0;
			g_GameState.iGT--;
		}
		if (g_GameState.iGT == 1 && g_GameState.gameMode == GAME_SHOP)
			g_Shop.bOpen = false;
		if (g_GameState.iGT == 0) {
			switch (g_GameState.gameMode){
				case GAME_PRESHOP	:	g_GameState.iGT = 15; DXUTSetCursorSettings(true, false); g_GameState.gameMode = GAME_SHOP; break;
				case GAME_TOOL		:
				case GAME_WAITING	:
				case GAME_MAIN_MENU	:
				case GAME_VIDEO_MENU:
				case GAME_AUDIO_MENU:
				case GAME_RUNNING	:	
					for(i=0; i<=9; i++){
						int idm = -999;
						int maks = -999;

						for(j=0;j<MAX_DROID;j++){
							if (g_GameState.DroidQ[j].iScore > maks){
								maks = g_GameState.DroidQ[j].iScore;
								idm = j;
							}
						}

						if (g_GameState.iMoney > maks && g_GameState.bAiMode == false) {
							maks = g_GameState.iMoney;
							idm = 999;
							g_GameState.iMoney = -999;
						}

						if (maks == -999)
							break;

						if (idm == 999) {
							if (i==0) {
								g_GameState.HS[i].iColor = 1;
								g_GameState.HS[i].iSkor = maks;
								g_GameState.HS[i].iID = 999;
							}else if(i > 0 && i < 3){
								g_GameState.HS[i].iColor = 1;
								g_GameState.HS[i].iSkor = maks;
								g_GameState.HS[i].iID = 888;
							}else{
								g_GameState.HS[i].iColor = 2;
								g_GameState.HS[i].iSkor = maks;
								g_GameState.HS[i].iID = 777;
							}
						}else{
							if (g_GameState.DroidQ[idm].bActive) {
								g_GameState.HS[i].iColor = 3;
								g_GameState.HS[i].iSkor = maks;
								g_GameState.HS[i].iID = idm;
							}else{
								g_GameState.HS[i].iColor = 4;
								g_GameState.HS[i].iSkor = maks;
								g_GameState.HS[i].iID = idm;
							}
							g_GameState.DroidQ[idm].iScore = -999;
						}
					
					
					}
					g_GameState.iHS = i++;
					DXUTSetCursorSettings(true, false);
					if (g_GameState.bAiMode)
						TulisFile(g_OutFile.iRobot, g_OutFile.iJenis, g_OutFile.iSimpul, g_OutFile.N);
					g_GameState.gameMode = GAME_FINISH; break;
			}
		}
	}
    for( DWORD iPort = 0; iPort < DXUT_MAX_CONTROLLERS; iPort++ )
        DXUTGetGamepadState( iPort, &g_GameState.gamePad[iPort], true, true );

    for( DWORD iPort = 0; iPort < DXUT_MAX_CONTROLLERS; iPort++ )
    {
        if( g_GameState.gamePad[iPort].bConnected )
        {
            if( g_GameState.gamePad[iPort].wPressedButtons & XINPUT_GAMEPAD_START )
            {
                ToggleMenu();
                break;
            }
        }
    }

	for( int A = 0; A < MAX_CHEST; A++ )
	{
		

		if (g_GameState.ChestQ[A].chState == CH_EXPLODE) {
			g_GameState.ChestQ[A].fExplodeAnimation += fElapsedTime * 3;
			if( g_GameState.ChestQ[A].fExplodeAnimation > 1.0f )
			{
				if( g_GameState.ChestQ[A].fAlpha > 0.0f )
				{
					g_GameState.ChestQ[A].fAlpha -= fElapsedTime * 3;
					g_GameState.ChestQ[A].fAlpha = max( g_GameState.ChestQ[A].fAlpha, 0.0f );
				}
				else
				{
					g_GameState.ChestQ[A].chState = CH_OPEN;
					g_GameState.nChestCount--;
				}
			}
		}

		if( g_GameState.ChestQ[A].bActive ){
			
				g_GameState.ChestQ[A].fRotator = (float)(g_GameState.ChestQ[A].fRotator + fElapsedTime * 100);
				if (g_GameState.ChestQ[A].fRotator > 360.0f) g_GameState.ChestQ[A].fRotator = 0;
				float fStartRotation = g_GameState.ChestQ[A].fRotator/360.0f * D3DX_PI * 2.0f;
				g_GameState.ChestQ[A].fTargetRotation = fStartRotation;
				D3DXQuaternionRotationYawPitchRoll( &g_GameState.ChestQ[A].qTarget, g_GameState.ChestQ[A].fTargetRotation, 0.0f, 0.0f );
				g_GameState.ChestQ[A].qStart = g_GameState.ChestQ[A].qTarget;
				g_GameState.ChestQ[A].qCurrent = g_GameState.ChestQ[A].qTarget;
				g_GameState.ChestQ[A].fRotInterp = 0.0f;
			
		}


		g_GameState.ChestQ[A].bAvail = false;
		if (g_GameState.ChestQ[A].bActive == true &&  pow((long)(pow(g_GameState.ChestQ[A].vPosition.x - g_Camera.GetEyePt()->x, 2) + pow(g_GameState.ChestQ[A].vPosition.y - g_Camera.GetEyePt()->y, 2) + pow(g_GameState.ChestQ[A].vPosition.z - g_Camera.GetEyePt()->z, 2))  ,0.5) <= 0.3){
			if (abs(g_Camera.GetLookAtPt()->x - g_GameState.ChestQ[A].vPosition.x) < 0.7f && abs(g_Camera.GetLookAtPt()->y - g_GameState.ChestQ[A].vPosition.y) < 0.7f && abs(g_Camera.GetLookAtPt()->z - g_GameState.ChestQ[A].vPosition.z) < 0.7f) {
				g_GameState.ChestQ[A].bAvail = true;

				if ( DXUTIsMouseButtonDown( VK_RBUTTON ) && g_GameState.gameMode == GAME_RUNNING && !g_GameState.bAiMode) {
					g_GameState.rHelper = true;
				}else{
					if(g_GameState.rHelper){		
						g_Shop.bOpen = true;
						g_Shop.bClose = false;
						g_Shop.iAlphaKey = 0;
						g_GameState.iPCTarget = A;
						g_GameState.rHelper = false;
						g_GameState.gameMode = GAME_TOOL;
						DXUTSetCursorSettings(true, false);
					}
				}

				break;

			}
		} 
	}

	HandleDroidAI( fElapsedTime );

	if (g_GameState.gameMode == GAME_PRESHOP){
		g_Camera.FrameMove( fElapsedTime );
		UpdateAudio();
	}

	if (g_Shop.bOpen == false && g_GameState.gameMode == GAME_WAITING){
		g_Camera.FrameMove( fElapsedTime );
		UpdateAudio();
	}

    if( g_GameState.gameMode != GAME_RUNNING )
        return;

    // Update posisi kamera
	
    g_Camera.FrameMove( fElapsedTime );

    UpdateAudio();

    // Update warna ammo
    g_GameState.fAmmoColorLerp += fElapsedTime / 2.0f;
    if( g_GameState.fAmmoColorLerp > 1.0f )
    {
        g_GameState.fAmmoColorLerp = 0.0f;
        g_GameState.BlendFromColor = g_GameState.BlendToColor;
        switch( rand() % 5 )
        {
            case 0:
                g_GameState.BlendToColor = D3DXCOLOR( 0.6f, 0, 0, 1 ); break;
            case 1:
                g_GameState.BlendToColor = D3DXCOLOR( 0, 0.6f, 0, 1 ); break;
            case 2:
                g_GameState.BlendToColor = D3DXCOLOR( 0, 0, 0.6f, 1 ); break;
            case 3:
                g_GameState.BlendToColor = D3DXCOLOR( 0, 0, 0, 1 ); break;
            case 4:
                g_GameState.BlendToColor = D3DXCOLOR( 0.6f, 0.6f, 0, 1 ); break;
        }
    }

    // Check mouse/xinput dan langsung trigger ammo kalo ditembakan
    bool bFire = DXUTIsMouseButtonDown( VK_LBUTTON );
    for( DWORD iPort = 0; iPort < DXUT_MAX_CONTROLLERS; iPort++ )
    {
        if( g_GameState.gamePad[iPort].bConnected )
        {
            if( ( g_GameState.gamePad[iPort].wButtons & ( XINPUT_GAMEPAD_LEFT_SHOULDER |
                                                          XINPUT_GAMEPAD_RIGHT_SHOULDER | XINPUT_GAMEPAD_A ) ) ||
                g_GameState.gamePad[iPort].bLeftTrigger || g_GameState.gamePad[iPort].bRightTrigger )
            {
                bFire = true;
            }
        }
    }

    if( DXUTIsKeyDown( VK_SPACE ) )
        bFire = true;

    if( bFire )
    {
        static float fLastFired = 0.0f;
        if( fTime - fLastFired >= AUTOFIRE_DELAY )
        {
            FireAmmo();
            fLastFired = ( float )fTime;
        }

		//g_Camera.GetLookAtPt()
    }

    // DROD MOVE TOGGLE
    for( DWORD iPort = 0; iPort < DXUT_MAX_CONTROLLERS; iPort++ )
    {
        if( g_GameState.gamePad[iPort].bConnected )
        {
            if( g_GameState.gamePad[iPort].wPressedButtons & XINPUT_GAMEPAD_Y )
            {
                g_GameState.bDroidMove = !g_GameState.bDroidMove;
            }
        }
    }

    // DROID ADD TOGGLE
    for( DWORD iPort = 0; iPort < DXUT_MAX_CONTROLLERS; iPort++ )
    {
        if( g_GameState.gamePad[iPort].bConnected )
        {
            if( g_GameState.gamePad[iPort].wPressedButtons & XINPUT_GAMEPAD_X )
            {
                if( g_GameState.nDroidCount < MAX_DROID )
                    g_GameState.bDroidCreate = true;
            }
        }
    }
	

    // DROID ADD TOGGLE
    for( DWORD iPort = 0; iPort < DXUT_MAX_CONTROLLERS; iPort++ )
    {
        if( g_GameState.gamePad[iPort].bConnected )
        {
            if( g_GameState.gamePad[iPort].wButtons & XINPUT_GAMEPAD_X )
            {
                static float fLastDroidCreateTime = 0.0f;
                if( fTime - fLastDroidCreateTime >= 0.25f )
                {
                    if( g_GameState.nDroidCount < MAX_DROID )
                        g_GameState.bDroidCreate = true;
                    fLastDroidCreateTime = ( float )fTime;
                }
            }
        }
    }

    // DRID MASS KILL TOGGLE
    for( DWORD iPort = 0; iPort < DXUT_MAX_CONTROLLERS; iPort++ )
    {
        if( g_GameState.gamePad[iPort].bConnected )
        {
            if( g_GameState.gamePad[iPort].wPressedButtons & XINPUT_GAMEPAD_B )
            {
                g_GameState.bMassDroidKill = true;
            }
        }
    }

    // (Auto ADD DROID)
    if( g_GameState.bAutoAddDroids && g_GameState.nDroidCount < g_GameState.nMaxDroids )
    {
        if( !g_GameState.bDroidCreate )
        {
            g_GameState.fDroidCreateCountdown = ( rand() % DROID_CREATE_DELAY_FLUX ) / 1000.0f;
            g_GameState.bDroidCreate = true;
        }
    }

    if( g_GameState.bDroidCreate )
    {
        g_GameState.fDroidCreateCountdown -= fElapsedTime;
        if( g_GameState.fDroidCreateCountdown < 0.0f )
        {
			//CreateChest();
            CreateDroid();
            g_GameState.bDroidCreate = false;
        }
    }

    if( g_GameState.bMassDroidKill )
    {
        for( int B = 0; B < MAX_DROID; B++ )
        {
            if( !g_GameState.DroidQ[B].bActive || g_GameState.DroidQ[B].nHitPoints <= 0 )
                continue;

            g_GameState.DroidQ[B].nHitPoints = 0;
            g_GameState.DroidQ[B].fDeathAnimation = 0.0f;
            Play3DAudioCue( g_audioState.iDroidDestroyed, &g_GameState.DroidQ[B].vPosition );
        }
        g_GameState.bMassDroidKill = false;
    }

    HandleAmmoAI( fElapsedTime );
    
}


//--------------------------------------------------------------------------------------
void CreateAmmo( int nIndex, D3DXVECTOR4 Pos, D3DXVECTOR4 Vel )
{
    D3DXMatrixRotationYawPitchRoll( &g_GameState.AmmoQ[nIndex].mAmmoRot,
                                    rand() % 360 * D3DX_PI / 180.0f,
                                    rand() % 360 * D3DX_PI / 180.0f,
                                    rand() % 360 * D3DX_PI / 180.0f );

    g_GameState.AmmoQ[nIndex].bActive = true;
    g_GameState.AmmoQ[nIndex].fTimeCreated = DXUTGetTime();
    g_GameState.AmmoQ[nIndex].fLifeCountdown = 5.0f;
    g_GameState.AmmoQ[nIndex].fAlpha = 1.0f;
    g_GameState.AmmoQ[nIndex].bGround = false;
    g_GameState.AmmoQ[nIndex].vPosition = ( D3DXVECTOR3 )Pos;
    g_GameState.AmmoQ[nIndex].vVelocity = ( D3DXVECTOR3 )Vel;
    g_GameState.nAmmoCount++;

    D3DXCOLOR cBlendColor;
    D3DXColorLerp( &cBlendColor, &g_GameState.BlendFromColor, &g_GameState.BlendToColor, g_GameState.fAmmoColorLerp );
    g_GameState.AmmoQ[nIndex].Diffuse = ( D3DXVECTOR4 )cBlendColor;
}

void LoadLevel(int iLevel)
{
	int Modal,i, RyP;
	int ModalBot;
    int tipeinput;
    int *isiSimpul;
    int PetiMerah,PetiKuning,PetiHijau,jmlBot;
    int targetsekor, bataswktu, jumlahsimpul;
    float *LokasiX,*LokasiY,*LokasiZ;
    bool valids;
	float fStartRotation, C1, C2;
	D3DXVECTOR3 mEye, mPos;

	D3DXVECTOR3 Pos;

	switch (iLevel){
		case 0 :BacaFile("ai.txt", &Modal,&tipeinput,&PetiMerah,&PetiKuning,&PetiHijau,&jmlBot,isiSimpul,&targetsekor
             ,&bataswktu,&jumlahsimpul,LokasiX, LokasiY, LokasiZ,&valids); break;
		case 1 :BacaFile("level1.txt", &Modal,&tipeinput,&PetiMerah,&PetiKuning,&PetiHijau,&jmlBot,isiSimpul,&targetsekor
             ,&bataswktu,&jumlahsimpul,LokasiX, LokasiY, LokasiZ,&valids); break;
		case 2 :BacaFile("level2.txt", &Modal,&tipeinput,&PetiMerah,&PetiKuning,&PetiHijau,&jmlBot,isiSimpul,&targetsekor
             ,&bataswktu,&jumlahsimpul,LokasiX, LokasiY, LokasiZ,&valids); break;
		default :BacaFile("level3.txt", &Modal,&tipeinput,&PetiMerah,&PetiKuning,&PetiHijau,&jmlBot,isiSimpul,&targetsekor
             ,&bataswktu,&jumlahsimpul,LokasiX, LokasiY, LokasiZ,&valids); break;
	}

    if (valids) {
		g_GameState.iMoney = Modal * 100;
		g_Shop.iMoney = Modal * 100;
		ModalBot = Modal;

		g_GameState.iLimit = targetsekor * 100;
		g_GameState.iLevelT = bataswktu;
		g_GameState.iGetChest = 0;
		g_GameState.nDroidCount = 0;
		g_GameState.nChestCount = 0;

	

		for (i=1;i<=jumlahsimpul;i++)
		{
			switch(isiSimpul[i])
			{
				case 1 :	RyP = (( rand() % 360));
							g_GameState.ChestQ[g_GameState.nChestCount].bActive = true;
							g_GameState.ChestQ[g_GameState.nChestCount].bTargeted = false;
							g_GameState.ChestQ[g_GameState.nChestCount].bAvail = false;
							g_GameState.ChestQ[g_GameState.nChestCount].iChestType = 3;
							g_GameState.ChestQ[g_GameState.nChestCount].fRotator = (float)RyP;
							Pos = D3DXVECTOR3( LokasiX[i], LokasiY[i], LokasiZ[i] );
							g_GameState.ChestQ[g_GameState.nChestCount].vPosition = Pos;

							fStartRotation = ( rand() % 360 ) / 360.0f * D3DX_PI * 2.0f;

							g_GameState.ChestQ[g_GameState.nChestCount].fTargetRotation = fStartRotation;
							D3DXQuaternionRotationYawPitchRoll( &g_GameState.ChestQ[g_GameState.nChestCount].qTarget, g_GameState.ChestQ[g_GameState.nChestCount].fTargetRotation, 0.0f, 0.0f );

							g_GameState.ChestQ[g_GameState.nChestCount].qStart = g_GameState.ChestQ[g_GameState.nChestCount].qTarget;
							g_GameState.ChestQ[g_GameState.nChestCount].qCurrent = g_GameState.ChestQ[g_GameState.nChestCount].qTarget;
							g_GameState.ChestQ[g_GameState.nChestCount].fRotInterp = 0.0f;
							g_GameState.ChestQ[g_GameState.nChestCount].chState = CH_CLOSED;
							g_GameState.ChestQ[g_GameState.nChestCount].fTaskTimer = 0.0f;
							g_GameState.ChestQ[g_GameState.nChestCount].fExplodeAnimation = 0.0f;
							g_GameState.ChestQ[g_GameState.nChestCount].fAlpha = 1.0f;
							g_GameState.nChestCount++;
							break;

				case 2 :	RyP = (( rand() % 360));
							g_GameState.ChestQ[g_GameState.nChestCount].bActive = true;
							g_GameState.ChestQ[g_GameState.nChestCount].bAvail = false;
							g_GameState.ChestQ[g_GameState.nChestCount].bTargeted = false;
							g_GameState.ChestQ[g_GameState.nChestCount].iChestType = 2;
							g_GameState.ChestQ[g_GameState.nChestCount].fRotator = (float)RyP;
							Pos = D3DXVECTOR3( LokasiX[i], LokasiY[i], LokasiZ[i] );
							g_GameState.ChestQ[g_GameState.nChestCount].vPosition = Pos;

							fStartRotation = ( rand() % 360 ) / 360.0f * D3DX_PI * 2.0f;

							g_GameState.ChestQ[g_GameState.nChestCount].fTargetRotation = fStartRotation;
							D3DXQuaternionRotationYawPitchRoll( &g_GameState.ChestQ[g_GameState.nChestCount].qTarget, g_GameState.ChestQ[g_GameState.nChestCount].fTargetRotation, 0.0f, 0.0f );

							g_GameState.ChestQ[g_GameState.nChestCount].qStart = g_GameState.ChestQ[g_GameState.nChestCount].qTarget;
							g_GameState.ChestQ[g_GameState.nChestCount].qCurrent = g_GameState.ChestQ[g_GameState.nChestCount].qTarget;
							g_GameState.ChestQ[g_GameState.nChestCount].fRotInterp = 0.0f;
							g_GameState.ChestQ[g_GameState.nChestCount].chState = CH_CLOSED;
							g_GameState.ChestQ[g_GameState.nChestCount].fTaskTimer = 0.0f;
							g_GameState.ChestQ[g_GameState.nChestCount].fExplodeAnimation = 0.0f;
							g_GameState.ChestQ[g_GameState.nChestCount].fAlpha = 1.0f;
							g_GameState.nChestCount++;
							break;

				case 3 :	RyP = (( rand() % 360));
							g_GameState.ChestQ[g_GameState.nChestCount].bActive = true;
							g_GameState.ChestQ[g_GameState.nChestCount].bAvail = false;
							g_GameState.ChestQ[g_GameState.nChestCount].bTargeted = false;
							g_GameState.ChestQ[g_GameState.nChestCount].iChestType = 1;
							g_GameState.ChestQ[g_GameState.nChestCount].fRotator = (float)RyP;
							Pos = D3DXVECTOR3( LokasiX[i], LokasiY[i], LokasiZ[i] );
							g_GameState.ChestQ[g_GameState.nChestCount].vPosition = Pos;

							fStartRotation = ( rand() % 360 ) / 360.0f * D3DX_PI * 2.0f;

							g_GameState.ChestQ[g_GameState.nChestCount].fTargetRotation = fStartRotation;
							D3DXQuaternionRotationYawPitchRoll( &g_GameState.ChestQ[g_GameState.nChestCount].qTarget, g_GameState.ChestQ[g_GameState.nChestCount].fTargetRotation, 0.0f, 0.0f );

							g_GameState.ChestQ[g_GameState.nChestCount].qStart = g_GameState.ChestQ[g_GameState.nChestCount].qTarget;
							g_GameState.ChestQ[g_GameState.nChestCount].qCurrent = g_GameState.ChestQ[g_GameState.nChestCount].qTarget;
							g_GameState.ChestQ[g_GameState.nChestCount].fRotInterp = 0.0f;
							g_GameState.ChestQ[g_GameState.nChestCount].chState = CH_CLOSED;
							g_GameState.ChestQ[g_GameState.nChestCount].fTaskTimer = 0.0f;
							g_GameState.ChestQ[g_GameState.nChestCount].fExplodeAnimation = 0.0f;
							g_GameState.ChestQ[g_GameState.nChestCount].fAlpha = 1.0f;
							g_GameState.nChestCount++;
							break;


				case 4 :	
							mEye.x = g_Camera.GetLookAtPt()->x;
							mEye.y = g_Camera.GetLookAtPt()->y;
							mEye.z = g_Camera.GetLookAtPt()->z;

							mPos.x = LokasiX[i];
							mPos.y = LokasiY[i];
							mPos.z = LokasiZ[i];

							g_Camera.SetViewParams(&mPos, &mEye);
							break;

				case 5 :	g_GameState.DroidQ[g_GameState.nDroidCount].bActive = true;
							g_GameState.DroidQ[g_GameState.nDroidCount].bBusy = false;
							Pos = D3DXVECTOR3( LokasiX[i], LokasiY[i], LokasiZ[i] );
							g_GameState.DroidQ[g_GameState.nDroidCount].vPosition = Pos;
							fStartRotation = ( rand() % 360 ) / 360.0f * D3DX_PI * 2.0f;
							g_GameState.DroidQ[g_GameState.nDroidCount].fTargetRotation = fStartRotation;
							D3DXQuaternionRotationYawPitchRoll( &g_GameState.DroidQ[g_GameState.nDroidCount].qTarget, g_GameState.DroidQ[g_GameState.nDroidCount].fTargetRotation, 0.0f, 0.0f );
						
							g_GameState.DroidQ[g_GameState.nDroidCount].qStart = g_GameState.DroidQ[g_GameState.nDroidCount].qTarget;
							g_GameState.DroidQ[g_GameState.nDroidCount].qCurrent = g_GameState.DroidQ[g_GameState.nDroidCount].qTarget;
							g_GameState.DroidQ[g_GameState.nDroidCount].fRotInterp = 0.0f;
							g_GameState.DroidQ[g_GameState.nDroidCount].aiState = AI_MOVING;
							g_GameState.DroidQ[g_GameState.nDroidCount].fTaskTimer = 0.0f;
							g_GameState.DroidQ[g_GameState.nDroidCount].nHitPoints = DROID_HITPOINTS;
							g_GameState.DroidQ[g_GameState.nDroidCount].fDeathAnimation = 0.0f;
							g_GameState.DroidQ[g_GameState.nDroidCount].fAlpha = 0.5f;

							g_GameState.DroidQ[g_GameState.nDroidCount].iScore = ModalBot;
							g_GameState.DroidQ[g_GameState.nDroidCount].iTool1 = 0;
							g_GameState.DroidQ[g_GameState.nDroidCount].iTool2 = 0;

							while (g_GameState.DroidQ[g_GameState.nDroidCount].iScore > ModalBot/2) { // belanja sampai uangnya tinggal setengahnya
								if (g_GameState.DroidQ[g_GameState.nDroidCount].iScore > 3){
									g_GameState.DroidQ[g_GameState.nDroidCount].iScore -= 3;
									g_GameState.DroidQ[g_GameState.nDroidCount].iTool1++;
								}else{
									g_GameState.DroidQ[g_GameState.nDroidCount].iScore -= 2;
									g_GameState.DroidQ[g_GameState.nDroidCount].iTool2++;
								}
							}

							g_GameState.DroidQ[g_GameState.nDroidCount].iScore = g_GameState.DroidQ[g_GameState.nDroidCount].iScore * 100;
							D3DXMATRIXA16 mRot;
							D3DXMatrixRotationQuaternion( &mRot, &g_GameState.DroidQ[g_GameState.nDroidCount].qTarget );
							g_GameState.DroidQ[g_GameState.nDroidCount].vVelocity = D3DXVECTOR3( mRot._31, mRot._32, mRot._33 ) * DROID_VELOCITY;

							C1 = 0.75f;
							C2 = 0.35f;

							switch( rand() % 7 )
							{
								case 0:
									g_GameState.DroidQ[g_GameState.nDroidCount].Specular = D3DXCOLOR( C1, C1, C2, 1.0f ); break;
								case 1:
									g_GameState.DroidQ[g_GameState.nDroidCount].Specular = D3DXCOLOR( C1, C2, C1, 1.0f ); break;
								case 2:
									g_GameState.DroidQ[g_GameState.nDroidCount].Specular = D3DXCOLOR( C1, C2, C2, 1.0f ); break;
								case 3:
									g_GameState.DroidQ[g_GameState.nDroidCount].Specular = D3DXCOLOR( C2, C1, C1, 1.0f ); break;
								case 4:
									g_GameState.DroidQ[g_GameState.nDroidCount].Specular = D3DXCOLOR( C2, C1, C2, 1.0f ); break;
								case 5:
									g_GameState.DroidQ[g_GameState.nDroidCount].Specular = D3DXCOLOR( C2, C2, C1, 1.0f ); break;
								case 6:
									g_GameState.DroidQ[g_GameState.nDroidCount].Specular = D3DXCOLOR( C2, C2, C2, 1.0f ); break;
							}
							g_GameState.nDroidCount++;
							switch(iLevel){
								case 3 : SetNumDroidsForAudio( 50 ); break;
								case 2 : SetNumDroidsForAudio( 20 ); break;
								default: SetNumDroidsForAudio( 10 ); break;
							}
							break;
			}
        
		

		}
	
		for(i=g_GameState.nDroidCount;i<MAX_DROID;i++){
			g_GameState.DroidQ[i].bActive = false;
			g_GameState.DroidQ[i].iScore = -999;
		}

		for(i=g_GameState.nChestCount;i<MAX_CHEST;i++){
			g_GameState.ChestQ[i].bActive = false;
			g_GameState.ChestQ[i].vPosition = D3DXVECTOR3(99,99,99);
		}

		for (i=0;i<MAX_DROID;i++)
		{
			if (g_GameState.DroidQ [i].bActive ) {
				DroidGreedy(i);
			}
		}
    }
}

//--------------------------------------------------------------------------------------
void RenderAmmo( int A, D3DXMATRIXA16& mView, D3DXMATRIXA16& mProj )
{
    HRESULT hr;
    D3DXMATRIXA16 mWorldView;
    D3DXMATRIXA16 mWorldViewProjection;
    D3DXMATRIXA16 mWorld;
    D3DXMatrixTranslation( &mWorld, g_GameState.AmmoQ[A].vPosition.x,
                           g_GameState.AmmoQ[A].vPosition.y,
                           g_GameState.AmmoQ[A].vPosition.z );
    mWorld = g_GameState.AmmoQ[A].mAmmoRot * g_Render.mAmmoWorld * mWorld;
    mWorldViewProjection = mWorld * mView * mProj;
    mWorldView = mWorld * mView;
    V( g_Render.pEffect->SetMatrix( g_Render.hMatWVP, &mWorldViewProjection ) );
    V( g_Render.pEffect->SetMatrix( g_Render.hMatWV, &mWorldView ) );
    V( g_Render.pEffect->SetMatrix( g_Render.hMatW, &mWorld ) );

    g_Render.pEffect->SetTexture( g_Render.hDiffuseTexture, g_Render.pDefaultTex );
    g_GameState.AmmoQ[A].Diffuse.w = g_GameState.AmmoQ[A].fAlpha;
    g_Render.pEffect->SetVector( g_Render.hDiffuse, ( D3DXVECTOR4* )&g_GameState.AmmoQ[A].Diffuse );
    g_Render.pEffect->SetBool( "g_bUseSpecular", !g_Render.DisableSpecular );
	
    g_Render.meshAmmo.Render( g_Render.pEffect, 0, NULL, 0, g_Render.hSpecular, 0, g_Render.hPower, true, false );
}


// ------------------------------------------------------------------------------
void CreateChest()
{
	int A;
    for( A = 0; A < MAX_CHEST; ++A )
    {
        if( !g_GameState.ChestQ[A].bActive )
            break;
    }
    if( A == MAX_CHEST )
        return;

    D3DXVECTOR3 Pos;

	int TyP = (( rand() % 3) + 1);
	int RyP = (( rand() % 360));

	g_GameState.ChestQ[A].iChestType = TyP;
	g_GameState.ChestQ[A].fRotator = (float)RyP;

    for(; ; )
    {
        float fX = ( rand() % 100 ) / 100.0f;
        float fY = ( rand() % 100 ) / 100.0f;
		float fZ = ( rand() % 100 ) / 100.0f;

        Pos = D3DXVECTOR3( g_MinBound.x + fX * ( g_MaxBound.x - g_MinBound.x ),
                           g_MinBound.y + fY * ( g_MaxBound.y - g_MinBound.y ),
                           g_MinBound.z + fZ * ( g_MaxBound.z - g_MinBound.z ) );

        // Acak posisi chest
        bool bInterset = false;
        for( int B = 0; B < MAX_CHEST; B++ )
        {
            if( !g_GameState.ChestQ[B].bActive )
                continue;
            D3DXVECTOR3 vAToB = Pos - g_GameState.ChestQ[B].vPosition;
            float Dist = D3DXVec3Length( &vAToB );
            if( Dist < 2.0f * CHEST_SIZE )
            {
                bInterset = true;
                break;
            }
        }

        if( !bInterset ) break;
    }

    g_GameState.ChestQ[A].bActive = true;
    g_GameState.ChestQ[A].bTargeted = false;
	g_GameState.ChestQ[A].vPosition = Pos;


    float fStartRotation = ( rand() % 360 ) / 360.0f * D3DX_PI * 2.0f;
    g_GameState.ChestQ[A].fTargetRotation = fStartRotation;
    D3DXQuaternionRotationYawPitchRoll( &g_GameState.ChestQ[A].qTarget, g_GameState.ChestQ[A].fTargetRotation, 0.0f,
                                        0.0f );
    g_GameState.ChestQ[A].qStart = g_GameState.ChestQ[A].qTarget;
    g_GameState.ChestQ[A].qCurrent = g_GameState.ChestQ[A].qTarget;
    g_GameState.ChestQ[A].fRotInterp = 0.0f;
    g_GameState.ChestQ[A].chState = CH_CLOSED;
    g_GameState.ChestQ[A].fTaskTimer = 0.0f;
    g_GameState.ChestQ[A].fExplodeAnimation = 0.0f;
    g_GameState.ChestQ[A].fAlpha = 1.0f;

    g_GameState.nChestCount++;
    
}

//--------------------------------------------------------------------------------------
void CreateDroid()
{
    int A;
    for( A = 0; A < MAX_DROID; ++A )
    {
        if( !g_GameState.DroidQ[A].bActive )
            break;
    }
    if( A == MAX_DROID )
        return;

    D3DXVECTOR3 Pos;
	D3DXMATRIXA16 mRot;
    for(; ; )
    {
        float fX = ( rand() % 100 ) / 100.0f;
        float fZ = ( rand() % 100 ) / 100.0f;

        Pos = D3DXVECTOR3( g_MinBound.x + fX * ( g_MaxBound.x - g_MinBound.x ),
                           -2.45,
                           g_MinBound.z + fZ * ( g_MaxBound.z - g_MinBound.z ) );

        // Acak posisi droid
        bool bInterset = false;
        for( int B = 0; B < MAX_DROID; B++ )
        {
            if( !g_GameState.DroidQ[B].bActive )
                continue;
            D3DXVECTOR3 vAToB = Pos - g_GameState.DroidQ[B].vPosition;
            float Dist = D3DXVec3Length( &vAToB );
            if( Dist < 2.0f * DROID_SIZE )
            {
                bInterset = true;
                break;
            }
        }

        if( !bInterset ) break;
    }

    g_GameState.DroidQ[A].bActive = true;
    g_GameState.DroidQ[A].vPosition = Pos;

    float fStartRotation = ( rand() % 360 ) / 360.0f * D3DX_PI * 2.0f;
    g_GameState.DroidQ[A].fTargetRotation = fStartRotation;
    D3DXQuaternionRotationYawPitchRoll( &g_GameState.DroidQ[A].qTarget, g_GameState.DroidQ[A].fTargetRotation,  0.0f, 0.0f );
	D3DXMatrixRotationQuaternion( &mRot, &g_GameState.DroidQ[A].qTarget );
    g_GameState.DroidQ[A].qStart = g_GameState.DroidQ[A].qTarget;
    g_GameState.DroidQ[A].qCurrent = g_GameState.DroidQ[A].qTarget;
    g_GameState.DroidQ[A].fRotInterp = 0.0f;
	//g_GameState.DroidQ[A].iTool1 = 3;
    g_GameState.DroidQ[A].aiState = AI_MOVING;
    g_GameState.DroidQ[A].fTaskTimer = 0.0f;
    g_GameState.DroidQ[A].nHitPoints = DROID_HITPOINTS;
    g_GameState.DroidQ[A].fDeathAnimation = 0.0f;
    g_GameState.DroidQ[A].fAlpha = 0.5f;
    
    
    g_GameState.DroidQ[A].vVelocity = D3DXVECTOR3( mRot._31, mRot._32, mRot._33 ) * DROID_VELOCITY;
	
    // Pewarnaan berbeda untuk droid
    float C1 = 0.75f;
    float C2 = 0.35f;

    switch( rand() % 7 )
    {
        case 0:
            g_GameState.DroidQ[A].Specular = D3DXCOLOR( C1, C1, C2, 1.0f ); break;
        case 1:
            g_GameState.DroidQ[A].Specular = D3DXCOLOR( C1, C2, C1, 1.0f ); break;
        case 2:
            g_GameState.DroidQ[A].Specular = D3DXCOLOR( C1, C2, C2, 1.0f ); break;
        case 3:
            g_GameState.DroidQ[A].Specular = D3DXCOLOR( C2, C1, C1, 1.0f ); break;
        case 4:
            g_GameState.DroidQ[A].Specular = D3DXCOLOR( C2, C1, C2, 1.0f ); break;
        case 5:
            g_GameState.DroidQ[A].Specular = D3DXCOLOR( C2, C2, C1, 1.0f ); break;
        case 6:
            g_GameState.DroidQ[A].Specular = D3DXCOLOR( C2, C2, C2, 1.0f ); break;
    }

	
	DroidGreedy(A);
    g_GameState.nDroidCount++;
    SetNumDroidsForAudio( g_GameState.nDroidCount );
}


//--------------------------------------------------------------------------------------
void RenderChest( IDirect3DDevice9* pd3dDevice, int A, D3DXMATRIXA16& mView, D3DXMATRIXA16& mProj, bool bExplode )
{
	HRESULT hr;
    D3DXMATRIXA16 mWorldView;
    D3DXMATRIXA16 mWorldViewProjection;
    D3DXMATRIXA16 mWorld, mRot;
    D3DXMatrixTranslation( &mWorld, g_GameState.ChestQ[A].vPosition.x,
                           g_GameState.ChestQ[A].vPosition.y,
                           g_GameState.ChestQ[A].vPosition.z );
	
    D3DXMatrixRotationQuaternion( &mRot, &g_GameState.ChestQ[A].qCurrent );

    mWorld = g_Render.mChestWorld * mRot * mWorld;
    mWorldView = mWorld * mView;
    mWorldViewProjection = mWorldView * mProj;
    V( g_Render.pEffect->SetMatrix( g_Render.hMatWVP, &mWorldViewProjection ) );
    V( g_Render.pEffect->SetMatrix( g_Render.hMatWV, &mWorldView ) );
    V( g_Render.pEffect->SetMatrix( g_Render.hMatW, &mWorld ) );

   
    D3DXVECTOR4 vDiffuse = D3DXVECTOR4( 1, 1, 1, g_GameState.ChestQ[A].fAlpha );
    g_Render.pEffect->SetVector( g_Render.hDiffuse, ( D3DXVECTOR4* )&vDiffuse );
    V( g_Render.pEffect->SetFloat( g_Render.hAnimation, g_GameState.ChestQ[A].fExplodeAnimation ) );

    V( g_Render.pEffect->SetFloat( g_Render.hPower, 30.5f ) );

    switch (g_GameState.ChestQ[A].iChestType) {
	case 1 :  g_Render.meshChestGreen.Render( g_Render.pEffect, g_Render.hDiffuseTexture, NULL, 0, NULL, 0, NULL, true, false ); break;
	case 2 :  g_Render.meshChestYellow.Render( g_Render.pEffect, g_Render.hDiffuseTexture, NULL, 0, NULL, 0, NULL, true, false ); break;
	case 3 :  g_Render.meshChestRed.Render( g_Render.pEffect, g_Render.hDiffuseTexture, NULL, 0, NULL, 0, NULL, true, false ); break;
	}
}



//--------------------------------------------------------------------------------------
void RenderDroid( IDirect3DDevice9* pd3dDevice, int A, D3DXMATRIXA16& mView, D3DXMATRIXA16& mProj, bool bExplode )
{
	HRESULT hr;
    D3DXMATRIXA16 mWorldView;
    D3DXMATRIXA16 mWorldViewProjection;
    D3DXMATRIXA16 mWorld, mRot;
    D3DXMatrixTranslation( &mWorld, g_GameState.DroidQ[A].vPosition.x,
                           g_GameState.DroidQ[A].vPosition.y,
                           g_GameState.DroidQ[A].vPosition.z );

    D3DXMatrixRotationQuaternion( &mRot, &g_GameState.DroidQ[A].qCurrent );

    mWorld = g_Render.mDroidWorld * mRot * mWorld;
    mWorldView = mWorld * mView;
    mWorldViewProjection = mWorldView * mProj;
    V( g_Render.pEffect->SetMatrix( g_Render.hMatWVP, &mWorldViewProjection ) );
    V( g_Render.pEffect->SetMatrix( g_Render.hMatWV, &mWorldView ) );
    V( g_Render.pEffect->SetMatrix( g_Render.hMatW, &mWorld ) );

    D3DXVECTOR4 Diffuse = D3DXVECTOR4( 1, 1, 1, 1 );
    g_Render.pEffect->SetVector( g_Render.hDiffuse, ( D3DXVECTOR4* )&Diffuse );
    g_Render.pEffect->SetBool( "g_bUseSpecular", 1 );

    D3DXVECTOR4 vDiffuse = D3DXVECTOR4( 1, 1, 1, g_GameState.DroidQ[A].fAlpha );
    g_Render.pEffect->SetVector( g_Render.hDiffuse, ( D3DXVECTOR4* )&vDiffuse );
    V( g_Render.pEffect->SetFloat( g_Render.hAnimation, g_GameState.DroidQ[A].fDeathAnimation ) );

    V( g_Render.pEffect->SetVector( g_Render.hSpecular, ( D3DXVECTOR4* )&g_GameState.DroidQ[A].Specular ) );
    V( g_Render.pEffect->SetFloat( g_Render.hPower, 30.5f ) );

    //if( g_Render.pDroidNormalMap )
    //    g_Render.pEffect->SetTexture( g_Render.hNormalMap, g_Render.pDroidNormalMap );

    if( g_Render.UseHighResModels )
    {
            g_Render.meshDroid.Render( g_Render.pEffect, g_Render.hDiffuseTexture, NULL, 0, NULL, 0, NULL, true, false );
    }
    else
    {
            g_Render.meshDroidLow.Render( g_Render.pEffect, g_Render.hDiffuseTexture, NULL, 0, NULL, 0, NULL, true,
                                          false );
    }

    g_Render.pEffect->SetTexture( g_Render.hNormalMap, g_Render.pDefaultNormalMap );
    V( g_Render.pEffect->SetFloat( g_Render.hAnimation, 0.0f ) );
}


//--------------------------------------------------------------------------------------
// Fungsi yang dipanggil tepat sebelum frame di render ke buffer
// fungsi ini cocok untuk memasukan semua objek yang akan dirender ke buffer
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    HRESULT hr;
	
    D3DXMATRIXA16 mView;
    D3DXMATRIXA16 mProj;
    D3DXMATRIXA16 mWorldView;
    D3DXMATRIXA16 mWorldViewProjection;
	CDXUTTextHelper txtHelper;
	D3DXMATRIX trans;


    // Clear the render target and the zbuffer 
    V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 105, 105, 105, 125 ), 1.0f, 0 ) );

	
    // Memulai scene 3D
    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
        // Ambil matrix projection dan view 
        mProj = *g_Camera.GetProjMatrix();
        mView = *g_Camera.GetViewMatrix();

        mWorldViewProjection = g_Render.mCellWorld * mView * mProj;
        mWorldView = g_Render.mCellWorld * mView;

        // Update variable dari effect render
        V( g_Render.pEffect->SetMatrix( g_Render.hMatV, &mView ) );
        V( g_Render.pEffect->SetMatrix( g_Render.hMatP, &mProj ) );
        V( g_Render.pEffect->SetMatrix( g_Render.hMatWVP, &mWorldViewProjection ) );
        V( g_Render.pEffect->SetMatrix( g_Render.hMatWV, &mWorldView ) );
        V( g_Render.pEffect->SetMatrix( g_Render.hMatW, &g_Render.mCellWorld ) );
        g_Render.pEffect->SetBool( "g_bUseSpecular", false ); 

		if(g_GameState.gameMode != GAME_INTRO && g_GameState.gameMode != GAME_HELP && g_GameState.gameMode != GAME_VIDEO_INTRO) {
			g_Render.meshCell.Render( g_Render.pEffect, g_Render.hDiffuseTexture, g_Render.hDiffuse, 0, g_Render.hSpecular,
                                  0, g_Render.hPower, true, false );
		}
        
		// Render objek yang transparan terlebih dahulu
        if( g_GameState.nAmmoCount > 0 )
        {
            for( int A = 0; A < MAX_AMMO; A++ )
            {
                if( !g_GameState.AmmoQ[A].bActive )
                    continue;
                if( g_GameState.AmmoQ[A].fAlpha == 1.0f )
                    RenderAmmo( A, mView, mProj );
            }
        }
        if( g_GameState.nDroidCount > 0 )
        {
			
            for( int A = 0; A < MAX_DROID; A++ )
            {
                if( !g_GameState.DroidQ[A].bActive )
                    continue;

                if( g_GameState.DroidQ[A].fAlpha == 1.0f )
                {
                    RenderDroid( pd3dDevice, A, mView, mProj, g_GameState.DroidQ[A].nHitPoints <= 0 );
                }
            }
        }



        // Lalu render objek yang solid
        if( g_GameState.nAmmoCount > 0 )
        {
            for( int A = 0; A < MAX_AMMO; A++ )
            {
                if( !g_GameState.AmmoQ[A].bActive )
                    continue;
                if( g_GameState.AmmoQ[A].fAlpha < 1.0f )
                    RenderAmmo( A, mView, mProj );
            }
        }
        if( g_GameState.nDroidCount > 0 )
        {
            for( int A = 0; A < MAX_DROID; A++ )
            {
                if( !g_GameState.DroidQ[A].bActive )
                    continue;

                if( g_GameState.DroidQ[A].fAlpha < 1.0f )
                    RenderDroid( pd3dDevice, A, mView, mProj, g_GameState.DroidQ[A].nHitPoints <= 0 );
            }
        }

		if(g_GameState.gameMode != GAME_INTRO && g_GameState.gameMode != GAME_HELP && g_GameState.gameMode != GAME_VIDEO_INTRO) {
			for( int A = 0; A < MAX_CHEST; A++ )
				{
					if( g_GameState.ChestQ[A].chState == CH_EXPLODE )
						RenderChest( pd3dDevice, A, mView, mProj, true );

					if( !g_GameState.ChestQ[A].bActive )
						continue;

					if( g_GameState.ChestQ[A].fAlpha == 1.0f )
						RenderChest( pd3dDevice, A, mView, mProj, false );
				}
		}


		
		RenderText();

        switch( g_GameState.gameMode )
        {
			case GAME_WAITING:
				if (g_Shop.bOpen){
					if (g_Shop.iAlphaKey > 0) {
						g_Shop.iAlphaKey = g_Shop.iAlphaKey - fElapsedTime * 1000;
						if (g_Shop.iAlphaKey < 0) g_Shop.iAlphaKey = 0;
					}
					if (g_Shop.iAlphaKey == 0) {
						if (g_GameState.ChestQ[g_GameState.iPCTarget].bActive == false) {
							SetCursorPos(g_Render.iPixelW /2, g_Render.iPixelH /2);
							DXUTSetCursorSettings(false, false);
							g_Camera.SetEnablePositionMovement (true);
							g_Shop.bOpen = false;
							g_Render.fNotif1 = 400;
							g_GameState.gameMode = GAME_RUNNING;
						}else{
							SetCursorPos(g_Render.iPixelW /2, g_Render.iPixelH /2);
							DXUTSetCursorSettings(false, false);
							g_Camera.SetEnablePositionMovement (false);
							g_Shop.bOpen = false;
							g_Task[MAX_DROID].bEnabled = true;
							g_Task[MAX_DROID].IDC = g_GameState.iPCTarget;
							if (g_Shop.bT1 == true){
								g_Shop.iTool1--;
								g_Task[MAX_DROID].iTimer = 40;
							}
							if (g_Shop.bT2 == true){
								g_Shop.iTool2--;
								g_Task[MAX_DROID].iTimer = 20;
							}
							if (g_Shop.bT3 == true){
								g_Shop.iTool3--;
								g_Task[MAX_DROID].iTimer = 10;
							}
						}
					}
					RenderTool(g_GameState.ChestQ[g_GameState.iPCTarget].iChestType); 
				}else{
					if (g_Shop.iAlphaKey < 255) {
						g_Shop.iAlphaKey = g_Shop.iAlphaKey + fElapsedTime * 500;
						if (g_Shop.iAlphaKey > 255) g_Shop.iAlphaKey = 255;
					}
					txtHelper.Init( g_Render.pFont, g_Render.pTextSprite, NULL, NULL, 15 );
					txtHelper.Begin();
					txtHelper.SetInsertionPos( g_Render.iPixelW / 2 - 52, g_Render.iPixelH - 42 );
					txtHelper.SetForegroundColor( D3DXCOLOR( 0.0f, 0.0f, 0.0f, g_Shop.iAlphaKey/255.0f ) );
					txtHelper.DrawTextLine( L"Sedang membuka peti ..." );

					txtHelper.SetInsertionPos( g_Render.iPixelW / 2 - 54, g_Render.iPixelH - 44 );
					txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, g_Shop.iAlphaKey/255.0f ) );
					txtHelper.DrawTextLine( L"Sedang membuka peti ..." );

					txtHelper.Init( g_Render.pFont2, g_Render.pTextSprite, NULL, NULL, 60 );
					txtHelper.SetInsertionPos( g_Render.iPixelW / 2 - 18, g_Render.iPixelH - 107 );
					txtHelper.SetForegroundColor( D3DXCOLOR( 0.0f, 0.0f, 0.0f, g_Shop.iAlphaKey/255.0f ) );
					txtHelper.DrawFormattedTextLine( L"%2d", g_Task[MAX_DROID].iTimer / 10 + 1);

					txtHelper.SetInsertionPos( g_Render.iPixelW / 2 - 20, g_Render.iPixelH - 109 );
					txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, g_Shop.iAlphaKey/255.0f ) );
					txtHelper.DrawFormattedTextLine( L"%2d", g_Task[MAX_DROID].iTimer / 10 + 1);
			
					txtHelper.End();
				}
				break;
			case GAME_SHOP:
				if (g_Shop.bOpen){
					if (g_Shop.iAlphaKey < 255) {
						g_Shop.iAlphaKey = g_Shop.iAlphaKey + fElapsedTime * 500;
						if (g_Shop.iAlphaKey > 255) g_Shop.iAlphaKey = 255;
					}
					RenderShop();
				}else{
					if (g_Shop.iAlphaKey > 0) {
						g_Shop.iAlphaKey = g_Shop.iAlphaKey - fElapsedTime * 1000;
						if (g_Shop.iAlphaKey < 0) g_Shop.iAlphaKey = 0;
					}
					if (g_Shop.iAlphaKey == 0) {
						SetCursorPos(g_Render.iPixelW /2, g_Render.iPixelH / 2);
						g_GameState.iGT = g_GameState.iLevelT;
						g_GameState.iMoney = g_Shop.iMoney;
						if (g_Shop.bWeapon) g_GameState.iAmmo = 100;
						else g_GameState.iAmmo = 0;
						g_GameState.bDroidMove = true;
						g_Camera.SetEnablePositionMovement(true);
						DXUTSetCursorSettings(false, false);
						g_GameState.gameMode = GAME_RUNNING;
					}else{
						RenderShop();
					}
				}
				
				break;
			case GAME_HELP:
				 V( g_Render.HelpDlg.OnRender( fElapsedTime ) );
			case GAME_INTRO:
				RenderLogo(); 
				V( g_Render.IntroDlg.OnRender( fElapsedTime ) ); 	break;
			case GAME_TOOL:
				if (g_Shop.bOpen == true) {
					if (g_Shop.iAlphaKey < 255) {
						g_Shop.iAlphaKey = g_Shop.iAlphaKey + fElapsedTime * 500;
					}
					if (g_Shop.iAlphaKey > 255) g_Shop.iAlphaKey = 255;
				}else{
					if (g_Shop.iAlphaKey > 0) {
						g_Shop.iAlphaKey = g_Shop.iAlphaKey - fElapsedTime * 1000;
					}
					if (g_Shop.iAlphaKey < 0){
						g_Shop.iAlphaKey = 0;
						DXUTSetCursorSettings(false, false);
						SetCursorPos(g_Render.iPixelW /2, g_Render.iPixelH /2 );
						g_GameState.gameMode = GAME_RUNNING;
					}
				}
				RenderTool(g_GameState.ChestQ[g_GameState.iPCTarget].iChestType); 
				break;
            case GAME_RUNNING:
				if (g_Render.fNotif1 > 0) {
					g_Render.fNotif1 -= fElapsedTime * 350;

					if (g_Render.fNotif1 < 0) g_Render.fNotif1 = 0;
					D3DXMatrixTranslation(&trans, (float)g_Render.iPixelW/2 - 235,  (float)g_Render.iPixelH/2 - 235 ,0);
					g_Render.pStatus->Begin(0);
					g_Render.pStatus->SetTransform(&trans);
					if (g_Render.fNotif1 > 255)
						g_Render.pStatus->Draw( g_Render.pNotif1, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, 255));
					else
						g_Render.pStatus->Draw( g_Render.pNotif1, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Render.fNotif1));
					g_Render.pStatus->End();
				}
				if (g_Render.fNotif2 > 0) {
					g_Render.fNotif2 -= fElapsedTime * 350;

					if (g_Render.fNotif2 < 0) g_Render.fNotif2 = 0;
					D3DXMatrixTranslation(&trans, (float)g_Render.iPixelW/2 - 235,  (float)g_Render.iPixelH/2 - 235 ,0);
					g_Render.pStatus->Begin(0);
					g_Render.pStatus->SetTransform(&trans);
					if (g_Render.fNotif2 > 255)
						g_Render.pStatus->Draw( g_Render.pNotif2, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, 255));
					else
						g_Render.pStatus->Draw( g_Render.pNotif2, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Render.fNotif2));
					g_Render.pStatus->End();
				}
				if (g_Render.fNotif3 > 0) {
					g_Render.fNotif3 -= fElapsedTime * 350;

					if (g_Render.fNotif3 < 0) g_Render.fNotif3 = 0;
					D3DXMatrixTranslation(&trans, (float)g_Render.iPixelW/2 - 235,  (float)g_Render.iPixelH/2 - 235 ,0);
					g_Render.pStatus->Begin(0);
					g_Render.pStatus->SetTransform(&trans);
					if (g_Render.fNotif3 > 255)
						g_Render.pStatus->Draw( g_Render.pNotif3, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, 255));
					else
						g_Render.pStatus->Draw( g_Render.pNotif3, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Render.fNotif3));
					g_Render.pStatus->End();
				}
				if (g_Render.fNotif4 > 0) {
					g_Render.fNotif4 -= fElapsedTime * 350;

					if (g_Render.fNotif4 < 0) g_Render.fNotif4 = 0;
					D3DXMatrixTranslation(&trans, (float)g_Render.iPixelW/2 - 235,  (float)g_Render.iPixelH/2 - 235 ,0);
					g_Render.pStatus->Begin(0);
					g_Render.pStatus->SetTransform(&trans);
					if (g_Render.fNotif4 > 255)
						g_Render.pStatus->Draw( g_Render.pNotif4, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, 255));
					else
						g_Render.pStatus->Draw( g_Render.pNotif4, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Render.fNotif4));
					g_Render.pStatus->End();
				}
                 break;
            case GAME_MAIN_MENU:
                V( g_Render.MainMenuDlg.OnRender( fElapsedTime ) ); break;
            case GAME_AUDIO_MENU:
                V( g_Render.AudioMenuDlg.OnRender( fElapsedTime ) ); break;
            case GAME_VIDEO_MENU:
                V( g_Render.VideoMenuDlg.OnRender( fElapsedTime ) ); break;
			case GAME_VIDEO_INTRO:
                RenderLogo(); V( g_Render.IntroDlg.OnRender( fElapsedTime ) ); V( g_Render.VideoMenuDlg.OnRender( fElapsedTime ) ); break;

        }

        V( pd3dDevice->EndScene() );
    }
}

// -----------------------------------------------------------

void RenderTool(int T)
{
	CDXUTTextHelper txtHelper;
	RECT win;
	D3DXMATRIX trans;
	POINT cursor;
	
	GetCursorPos(&cursor);
	FLOAT x = (float)(g_Render.iPixelW - 638)  / 2;
	FLOAT y = (float)(g_Render.iPixelH - 332) / 2;
	D3DXMatrixTranslation(&trans, x,  y ,0);
	
	GetWindowRect(DXUTGetHWND(), &win);
	cursor.x = cursor.x - win.left;
	cursor.y = cursor.y - win.top;
	if (DXUTIsWindowed())
		cursor.y -= 20;

	g_Render.pTool->Begin(0);
	g_Render.pTool->SetTransform(&trans);

	if (T==1) {
		if (cursor.x > x + 20 && cursor.x < x + 200 && cursor.y > y + 105 && cursor.y < y + 266 && g_Shop.bOpen == true && g_Shop.iAlphaKey == 255) {
			if (DXUTIsMouseButtonDown( VK_LBUTTON ) && g_Shop.iTool1 > 0) {
				g_Render.pTool->Draw( g_Render.pToolRes10, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
				D3DXMatrixTranslation(&trans, x + 21,  y + 110 ,0);
				g_Render.pTool->SetTransform(&trans);
				g_Render.pTool->Draw( g_Render.pToolRes1, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
				g_Shop.bT1 = true;
			}else{
				if (g_Shop.bT1){
					g_GameState.gameMode = GAME_WAITING;
				}
				g_Render.pTool->Draw( g_Render.pToolRes10, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
				D3DXMatrixTranslation(&trans, x + 16,  y + 105 ,0);
				g_Render.pTool->SetTransform(&trans);
				g_Render.pTool->Draw( g_Render.pToolRes1, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
			}
		}else if (cursor.x > x + 228 && cursor.x < x + 430 && cursor.y > y + 105 && cursor.y < y + 266 && g_Shop.bOpen == true && g_Shop.iAlphaKey == 255) {
			if (DXUTIsMouseButtonDown( VK_LBUTTON )  && g_Shop.iTool2 > 0) {
				g_Render.pTool->Draw( g_Render.pToolRes10, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
				D3DXMatrixTranslation(&trans, x + 228,  y + 110 ,0);
				g_Render.pTool->SetTransform(&trans);
				g_Render.pTool->Draw( g_Render.pToolRes2, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
				g_Shop.bT2 = true;
			}else{
				if (g_Shop.bT2){
					g_GameState.gameMode = GAME_WAITING;
				}
				g_Render.pTool->Draw( g_Render.pToolRes10, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
				D3DXMatrixTranslation(&trans, x + 223,  y + 105 ,0);
				g_Render.pTool->SetTransform(&trans);
				g_Render.pTool->Draw( g_Render.pToolRes2, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
			}
		}else if (cursor.x > x + 450 && cursor.x < x + 610 && cursor.y > y + 105 && cursor.y < y + 266 && g_Shop.bOpen == true && g_Shop.iAlphaKey == 255) {
			if (DXUTIsMouseButtonDown( VK_LBUTTON )  && g_Shop.iTool3 > 0) {
				g_Render.pTool->Draw( g_Render.pToolRes10, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
				D3DXMatrixTranslation(&trans, x + 454,  y + 115 ,0);
				g_Render.pTool->SetTransform(&trans);
				g_Render.pTool->Draw( g_Render.pToolRes3, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
				g_Shop.bT3 = true;
			}else{
				if (g_Shop.bT3){
					g_GameState.gameMode = GAME_WAITING;
				}
				g_Render.pTool->Draw( g_Render.pToolRes10, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
				D3DXMatrixTranslation(&trans, x + 449,  y + 110 ,0);
				g_Render.pTool->SetTransform(&trans);
				g_Render.pTool->Draw( g_Render.pToolRes3, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
			}
		}else if (cursor.x > x + 580 && cursor.x < x + 630 && cursor.y > y && cursor.y < y + 25 && g_Shop.bOpen == true && g_Shop.iAlphaKey == 255) {
			if (DXUTIsMouseButtonDown( VK_LBUTTON )) {
				g_Render.pTool->Draw( g_Render.pToolRes10, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
				g_Shop.bClose = true;
			}else{
				if (g_Shop.bClose == true) {
					g_Shop.bOpen = false;
				}
				g_Render.pTool->Draw( g_Render.pToolRes10, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
				D3DXMatrixTranslation(&trans, x + 561,  y ,0);
				g_Render.pTool->SetTransform(&trans);
				g_Render.pTool->Draw( g_Render.pWindowX, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
			}
		}else{
			if (g_GameState.gameMode == GAME_TOOL) {
				g_Shop.bClose = false;
				g_Shop.bT1 = false;
				g_Shop.bT2 = false;
				g_Shop.bT3 = false;
			}
			g_Render.pTool->Draw( g_Render.pToolRes10, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
		}
	}else if(T==2) {
		if (cursor.x > x + 120 && cursor.x < x + 328 && cursor.y > y + 105 && cursor.y < y + 266 && g_Shop.bOpen == true && g_Shop.iAlphaKey == 255) {
			if (DXUTIsMouseButtonDown( VK_LBUTTON )  && g_Shop.iTool2 > 0) {
				g_Render.pTool->Draw( g_Render.pToolRes20, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
				D3DXMatrixTranslation(&trans, x + 123,  y + 90 ,0);
				g_Render.pTool->SetTransform(&trans);
				g_Render.pTool->Draw( g_Render.pToolRes2, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
				g_Shop.bT2 = true;
			}else{
				if (g_Shop.bT2){
					g_GameState.gameMode = GAME_WAITING;
				}
				g_Render.pTool->Draw( g_Render.pToolRes20, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
				D3DXMatrixTranslation(&trans, x + 118,  y + 85 ,0);
				g_Render.pTool->SetTransform(&trans);
				g_Render.pTool->Draw( g_Render.pToolRes2, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
			}
		}else if (cursor.x > x + 349 && cursor.x < x + 508 && cursor.y > y + 105 && cursor.y < y + 266 && g_Shop.bOpen == true && g_Shop.iAlphaKey == 255) {
			if (DXUTIsMouseButtonDown( VK_LBUTTON )  && g_Shop.iTool3 > 0) {
				g_Render.pTool->Draw( g_Render.pToolRes20, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
				D3DXMatrixTranslation(&trans, x + 349,  y + 95 ,0);
				g_Render.pTool->SetTransform(&trans);
				g_Render.pTool->Draw( g_Render.pToolRes3, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
				g_Shop.bT3 = true;
			}else{
				if (g_Shop.bT3){
					g_GameState.gameMode = GAME_WAITING;
				}
				g_Render.pTool->Draw( g_Render.pToolRes20, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
				D3DXMatrixTranslation(&trans, x + 344,  y + 90 ,0);
				g_Render.pTool->SetTransform(&trans);
				g_Render.pTool->Draw( g_Render.pToolRes3, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
			}
		}else if (cursor.x > x + 580 && cursor.x < x + 630 && cursor.y > y && cursor.y < y + 25 && g_Shop.bOpen == true && g_Shop.iAlphaKey == 255) {
			if (DXUTIsMouseButtonDown( VK_LBUTTON )) {
				g_Render.pTool->Draw( g_Render.pToolRes20, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
				g_Shop.bClose = true;
			}else{
				if (g_Shop.bClose == true) {
					g_Shop.bOpen = false;
				}
				g_Render.pTool->Draw( g_Render.pToolRes20, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
				D3DXMatrixTranslation(&trans, x + 561,  y ,0);
				g_Render.pTool->SetTransform(&trans);
				g_Render.pTool->Draw( g_Render.pWindowX, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
			}
		}else{
			if (g_GameState.gameMode == GAME_TOOL) {
				g_Shop.bClose = false;
				g_Shop.bT1 = false;
				g_Shop.bT2 = false;
				g_Shop.bT3 = false;
			}
			g_Render.pTool->Draw( g_Render.pToolRes20, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
		}
	}else if (T==3) {
		if (cursor.x > x + 220 && cursor.x < x + 430 && cursor.y > y + 105 && cursor.y < y + 266 && g_Shop.bOpen == true && g_Shop.iAlphaKey == 255) {
			if (DXUTIsMouseButtonDown( VK_LBUTTON )  && g_Shop.iTool3 > 0) {
				g_Shop.bT3 = true;
				g_Render.pTool->Draw( g_Render.pToolRes30, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
				D3DXMatrixTranslation(&trans, x + 249,  y + 106 ,0);
				g_Render.pTool->SetTransform(&trans);
				g_Render.pTool->Draw( g_Render.pToolRes3, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
			}else{
				if (g_Shop.bT3){
					g_GameState.gameMode = GAME_WAITING;
				}
				g_Render.pTool->Draw( g_Render.pToolRes30, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
				D3DXMatrixTranslation(&trans, x + 244,  y + 101 ,0);
				g_Render.pTool->SetTransform(&trans);
				g_Render.pTool->Draw( g_Render.pToolRes3, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
			}
		}else if (cursor.x > x + 580 && cursor.x < x + 630 && cursor.y > y && cursor.y < y + 25 && g_Shop.bOpen == true && g_Shop.iAlphaKey == 255) {
			if (DXUTIsMouseButtonDown( VK_LBUTTON )) {
				g_Render.pTool->Draw( g_Render.pToolRes30, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
				g_Shop.bClose = true;
			}else{
				g_Render.pTool->Draw( g_Render.pToolRes30, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
				D3DXMatrixTranslation(&trans, x + 561,  y ,0);
				g_Render.pTool->SetTransform(&trans);
				g_Render.pTool->Draw( g_Render.pWindowX, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
				if (g_Shop.bClose == true) {
					g_Shop.bOpen = false;
				}
			}
		}else{
			if (g_GameState.gameMode == GAME_TOOL) {
				g_Shop.bClose = false;
				g_Shop.bT1 = false;
				g_Shop.bT2 = false;
				g_Shop.bT3 = false;
			}
			g_Render.pTool->Draw( g_Render.pToolRes30, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
		}
	}

	g_Render.pTool->End();

	
	txtHelper.Init( g_Render.pFont1, g_Render.pTextSprite, NULL, NULL, 15 );
	txtHelper.Begin();
	switch (T) {
		case 1:	txtHelper.Init( g_Render.pFont1, g_Render.pTextSprite, NULL, NULL, 15 );
				txtHelper.SetForegroundColor( D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f ) );
				if (DXUTIsMouseButtonDown( VK_LBUTTON ) && g_Shop.iTool1 > 0 && (cursor.x > x + 20 && cursor.x < x + 200 && cursor.y > y + 105 && cursor.y < y + 266) && g_Shop.bOpen)
					txtHelper.SetInsertionPos( (int)x + 154 , (int)y + 246 );
				else
					txtHelper.SetInsertionPos( (int)x + 149 , (int)y + 241 );
				txtHelper.DrawFormattedTextLine(L"%d", g_Shop.iTool1);

				if (DXUTIsMouseButtonDown( VK_LBUTTON ) && g_Shop.iTool2 > 0 && (cursor.x > x + 228 && cursor.x < x + 430 && cursor.y > y + 105 && cursor.y < y + 266) && g_Shop.bOpen)
					txtHelper.SetInsertionPos( (int)x + 370 , (int)y + 246 );
				else
					txtHelper.SetInsertionPos( (int)x + 365 , (int)y + 241 );
				txtHelper.DrawFormattedTextLine(L"%d", g_Shop.iTool2);

				if (DXUTIsMouseButtonDown( VK_LBUTTON ) && g_Shop.iTool3 > 0 && (cursor.x > x + 450 && cursor.x < x + 610 && cursor.y > y + 105 && cursor.y < y + 266) && g_Shop.bOpen)
					txtHelper.SetInsertionPos( (int)x + 582 , (int)y + 248 );
				else
					txtHelper.SetInsertionPos( (int)x + 577 , (int)y + 244 );
				txtHelper.DrawFormattedTextLine(L"%d", g_Shop.iTool3);
				break;
		case 2: txtHelper.Init( g_Render.pFont1, g_Render.pTextSprite, NULL, NULL, 15 );
				txtHelper.SetForegroundColor( D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f ) );

				if (DXUTIsMouseButtonDown( VK_LBUTTON ) && g_Shop.iTool2 > 0 && (cursor.x > x + 120 && cursor.x < x + 328 && cursor.y > y + 105 && cursor.y < y + 266) && g_Shop.bOpen)
					txtHelper.SetInsertionPos( (int)x + 265 , (int)y + 226 );
				else
					txtHelper.SetInsertionPos( (int)x + 260 , (int)y + 221 );
				txtHelper.DrawFormattedTextLine(L"%d", g_Shop.iTool2);

				if (DXUTIsMouseButtonDown( VK_LBUTTON ) && g_Shop.iTool3 > 0 && (cursor.x > x + 349 && cursor.x < x + 508 && cursor.y > y + 105 && cursor.y < y + 266) && g_Shop.bOpen)
					txtHelper.SetInsertionPos( (int)x + 468 , (int)y + 228 );
				else
					txtHelper.SetInsertionPos( (int)x + 463 , (int)y + 224 );
				txtHelper.DrawFormattedTextLine(L"%d", g_Shop.iTool3);
				break;
		case 3: txtHelper.Init( g_Render.pFont1, g_Render.pTextSprite, NULL, NULL, 15 );
				txtHelper.SetForegroundColor( D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f ) );

				if (DXUTIsMouseButtonDown( VK_LBUTTON ) && g_Shop.iTool3 > 0 && (cursor.x > x + 220 && cursor.x < x + 430 && cursor.y > y + 105 && cursor.y < y + 266) && g_Shop.bOpen)
					txtHelper.SetInsertionPos( (int)x + 372 , (int)y + 239 );
				else
					txtHelper.SetInsertionPos( (int)x + 367 , (int)y + 234 );
				txtHelper.DrawFormattedTextLine(L"%d", g_Shop.iTool3);
				break;
	}
	txtHelper.End(); 


}

void RenderLogo()
{
	D3DXMATRIX  trans, transL, scaleL;
	POINT cursor;
	
	GetCursorPos(&cursor);
	FLOAT x = (float)(g_Render.iPixelW - 320);
	FLOAT y = (float)(g_Render.iPixelH - 418);
	D3DXMatrixTranslation(&transL, 10 ,  y + 290 ,0);
	D3DXMatrixTranslation(&trans, x,  y ,0);

	g_Render.pHelper->Begin(0);
	
	
	g_Render.pHelper->SetTransform(&transL);
	g_Render.pHelper->Draw( g_Render.pITB, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,255));

	g_Render.pHelper->SetTransform(&trans);
	g_Render.pHelper->Draw( g_Render.pHelperRes, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,255));
	g_Render.pHelper->End();
}


// ---------------------------------------------------------
void RenderShop()
{
	D3DXMATRIX trans;
	POINT cursor;
	RECT win;
	CDXUTTextHelper txtHelper;


	GetCursorPos(&cursor);
	GetWindowRect(DXUTGetHWND(), &win);
	cursor.x = cursor.x - win.left;
	cursor.y = cursor.y - win.top;
	if (DXUTIsWindowed())
		cursor.y -= 20;

	FLOAT x = (float)(g_Render.iPixelW - 642)  / 2;
	FLOAT y = (float)(g_Render.iPixelH - 412) / 2;
	D3DXMatrixTranslation(&trans, x,  y ,0);

	g_Render.pShop->Begin(0);

	g_Render.pShop->SetTransform(&trans);
	if (cursor.x > x + 32.5 && cursor.x < x + 208.7 && cursor.y > y + 110 && cursor.y < y + 270 && g_Shop.bOpen && g_Shop.iAlphaKey == 255) {
		
		if (DXUTIsMouseButtonDown( VK_LBUTTON ) && g_Shop.iMoney >= 100) {
			g_Render.pShop->Draw( g_Render.pShopRes, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255, (int)g_Shop.iAlphaKey));
			D3DXMatrixTranslation(&trans, x + 27,  y + 121 ,0);
			g_Render.pShop->SetTransform(&trans);
			g_Render.pShop->Draw( g_Render.pShopRes1, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,(int)g_Shop.iAlphaKey));
			g_Shop.bT1 = true;
		}else{
			g_Render.pShop->Draw( g_Render.pShopRes, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,(int)g_Shop.iAlphaKey));
			D3DXMatrixTranslation(&trans, x + 22,  y + 116 ,0);
			g_Render.pShop->SetTransform(&trans);
			g_Render.pShop->Draw( g_Render.pShopRes1, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,(int)g_Shop.iAlphaKey));

			if (g_Shop.bT1 == true){
				g_Shop.bT1 = false;
				if (g_Shop.iMoney >= 100){
					g_Shop.iMoney -= 100;
					g_Shop.iTool1++;
				}
			}
		}
		
	}else if (cursor.x > x + 212.5 && cursor.x < x + 433.75 && cursor.y > y + 110 && cursor.y < y + 270 && g_Shop.bOpen && g_Shop.iAlphaKey == 255) {
		if (DXUTIsMouseButtonDown( VK_LBUTTON ) && g_Shop.iMoney >= 200) {
			g_Render.pShop->Draw( g_Render.pShopRes, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,(int)g_Shop.iAlphaKey));
			D3DXMatrixTranslation(&trans, x + 221,  y + 103 ,0);
			g_Render.pShop->SetTransform(&trans);
			g_Render.pShop->Draw( g_Render.pShopRes2, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,(int)g_Shop.iAlphaKey));
			g_Shop.bT2 = true;
		}else{
			g_Render.pShop->Draw( g_Render.pShopRes, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,(int)g_Shop.iAlphaKey));
			D3DXMatrixTranslation(&trans, x + 216,  y + 98 ,0);
			g_Render.pShop->SetTransform(&trans);
			g_Render.pShop->Draw( g_Render.pShopRes2, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,(int)g_Shop.iAlphaKey));
			if (g_Shop.bT2 == true){
				g_Shop.bT2 = false;
				if (g_Shop.iMoney >= 200){
					g_Shop.iMoney -= 200;
					g_Shop.iTool2++;
				}
			}
		}
	}else if (cursor.x > x + 450 && cursor.x < x + 617 && cursor.y > y + 110 && cursor.y < y + 270 && g_Shop.bOpen && g_Shop.iAlphaKey == 255) {
		if (DXUTIsMouseButtonDown( VK_LBUTTON ) && g_Shop.iMoney >= 300) {
			g_Render.pShop->Draw( g_Render.pShopRes, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,(int)g_Shop.iAlphaKey));
			D3DXMatrixTranslation(&trans, x + 455,  y + 119 ,0);
			g_Render.pShop->SetTransform(&trans);
			g_Render.pShop->Draw( g_Render.pShopRes3, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,(int)g_Shop.iAlphaKey));
			g_Shop.bT3 = true;
		}else{
			g_Render.pShop->Draw( g_Render.pShopRes, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,(int)g_Shop.iAlphaKey));
			D3DXMatrixTranslation(&trans, x + 450,  y + 114 ,0);
			g_Render.pShop->SetTransform(&trans);
			g_Render.pShop->Draw( g_Render.pShopRes3, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,(int)g_Shop.iAlphaKey));
			if (g_Shop.bT3 == true){
				g_Shop.bT3 = false;
				if (g_Shop.iMoney >= 300){
					g_Shop.iMoney -= 300;
					g_Shop.iTool3++;
				}
			}
		}
	}else if (cursor.x > x + 80 && cursor.x < x + 547 && cursor.y > y + 285 && cursor.y < y + 357 && g_Shop.bWeapon == false && g_Shop.bOpen && g_Shop.iAlphaKey == 255) {
		if (DXUTIsMouseButtonDown( VK_LBUTTON ) && g_Shop.iMoney >= 300) {
			g_Render.pShop->Draw( g_Render.pShopRes, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,(int)g_Shop.iAlphaKey));
			D3DXMatrixTranslation(&trans, x + 90,  y + 285 ,0);
			g_Render.pShop->SetTransform(&trans);
			g_Render.pShop->Draw( g_Render.pShopRes4, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,(int)g_Shop.iAlphaKey));
			g_Shop.bW = true;
		}else{
			g_Render.pShop->Draw( g_Render.pShopRes, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,(int)g_Shop.iAlphaKey));
			D3DXMatrixTranslation(&trans, x + 85,  y + 280 ,0);
			g_Render.pShop->SetTransform(&trans);
			g_Render.pShop->Draw( g_Render.pShopRes4, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,(int)g_Shop.iAlphaKey));
			if (g_Shop.bW == true && g_Shop.bWeapon == false){
				g_Shop.bW = false;
				if (g_Shop.iMoney >= 300){
					g_Shop.iMoney -= 300;
					g_Shop.bWeapon = true;
				}
			}
		}
	}else if (cursor.x > x + 580 && cursor.x < x + 630 && cursor.y > y && cursor.y < y + 25 && g_Shop.bOpen && g_Shop.iAlphaKey == 255) {
		if (DXUTIsMouseButtonDown( VK_LBUTTON )) {
			g_Render.pShop->Draw( g_Render.pShopRes, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,(int)g_Shop.iAlphaKey));
			g_Shop.bClose = true;
		}else{
			g_Render.pShop->Draw( g_Render.pShopRes, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,(int)g_Shop.iAlphaKey));
			D3DXMatrixTranslation(&trans, x + 561,  y ,0);
			g_Render.pShop->SetTransform(&trans);
			g_Render.pShop->Draw( g_Render.pWindowX, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,(int)g_Shop.iAlphaKey));
			if (g_Shop.bClose == true){
				g_Shop.bClose = false;
				g_Shop.bOpen = false;
			}
		}
	}else{
		g_Shop.bClose = false;
		g_Shop.bW = false;
		g_Shop.bT1 = false;
		g_Shop.bT2 = false;
		g_Shop.bT3 = false;

		g_Render.pShop->Draw( g_Render.pShopRes, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,(int)g_Shop.iAlphaKey));
	}

	if (g_Shop.bWeapon == true) {
		D3DXMatrixTranslation(&trans, x + 171,  y + 277 ,0);
		g_Render.pShop->SetTransform(&trans);
		g_Render.pShop->Draw( g_Render.pShopRes5, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,(int)g_Shop.iAlphaKey));
	}

	
	g_Render.pShop->End();

	txtHelper.Init( g_Render.pFont1, g_Render.pTextSprite, NULL, NULL, 15 );
	txtHelper.Begin();
	txtHelper.Init( g_Render.pFont3, g_Render.pTextSprite, NULL, NULL, 15 );
	txtHelper.SetForegroundColor( D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f ) );

	txtHelper.SetInsertionPos( (int)x + 15 , (int)y + 60 );
	txtHelper.DrawFormattedTextLine(L"Sisa Uang : %d", g_Shop.iMoney);

	txtHelper.SetInsertionPos( (int)x + 500 , (int)y + 60 );
	txtHelper.DrawFormattedTextLine(L"Sisa Waktu : %d", g_GameState.iGT);

	txtHelper.Init( g_Render.pFont1, g_Render.pTextSprite, NULL, NULL, 15 );
	txtHelper.SetForegroundColor( D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f ) );
	if (DXUTIsMouseButtonDown( VK_LBUTTON ) && g_Shop.iMoney >= 100 && (cursor.x > x + 32.5 && cursor.x < x + 208.7 && cursor.y > y + 110 && cursor.y < y + 270) && g_Shop.bOpen)
		txtHelper.SetInsertionPos( (int)x + 154 , (int)y + 246 );
	else
		txtHelper.SetInsertionPos( (int)x + 149 , (int)y + 241 );
	txtHelper.DrawFormattedTextLine(L"%d", g_Shop.iTool1);

	if (DXUTIsMouseButtonDown( VK_LBUTTON ) && g_Shop.iMoney >= 200  && (cursor.x > x + 212.5 && cursor.x < x + 433.75 && cursor.y > y + 110 && cursor.y < y + 270) && g_Shop.bOpen)
		txtHelper.SetInsertionPos( (int)x + 370 , (int)y + 246 );
	else
		txtHelper.SetInsertionPos( (int)x + 365 , (int)y + 241 );
	txtHelper.DrawFormattedTextLine(L"%d", g_Shop.iTool2);

	if (DXUTIsMouseButtonDown( VK_LBUTTON ) && g_Shop.iMoney >= 300 && (cursor.x > x + 450 && cursor.x < x + 617 && cursor.y > y + 110 && cursor.y < y + 270) && g_Shop.bOpen)
		txtHelper.SetInsertionPos( (int)x + 582 , (int)y + 248 );
	else
		txtHelper.SetInsertionPos( (int)x + 577 , (int)y + 244 );
	txtHelper.DrawFormattedTextLine(L"%d", g_Shop.iTool3);


	txtHelper.End(); 
}


//--------------------------------------------------------------------------------------
void RenderText()
{
	int A;
	D3DXMATRIX trans;
	CDXUTTextHelper txtHelper;

	switch (g_GameState.gameMode) 
	{
		case GAME_FINISH:
			{
				RECT win;
				POINT cursor;
	
				GetCursorPos(&cursor);
				
				GetWindowRect(DXUTGetHWND(), &win);
				cursor.x = cursor.x - win.left;
				cursor.y = cursor.y - win.top;
				if (DXUTIsWindowed())
					cursor.y -= 20;

				g_Render.pStatus->Begin(0);
				D3DXMatrixTranslation(&trans, (float)(g_Render.iPixelW - 642) /2,  (float)(g_Render.iPixelH - 408)/2 ,0);
				g_Render.pStatus->SetTransform(&trans);
				if (cursor.x > (float)(g_Render.iPixelW - 642) /2 + 580 && cursor.x < (float)(g_Render.iPixelW - 642) /2 + 630 && cursor.y > (float)(g_Render.iPixelH - 408)/2 && cursor.y < (float)(g_Render.iPixelH - 408)/2 + 25) {
					if (DXUTIsMouseButtonDown( VK_LBUTTON )) {
						g_Render.pStatus->Draw( g_Render.pFinishRes, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,255));
						g_Shop.bClose = true;
					}else{
						if (g_Shop.bClose == true) {
							SetNumDroidsForAudio(0);
							UpdateAudio();
							g_Shop.bClose = false;
							
							for(int J=0; J<=MAX_DROID; J++){
								g_Task[J].bEnabled = false;
								g_GameState.DroidQ[J].bActive = false;
							}

							D3DXVECTOR3 vEye, vLook;
							vEye = D3DXVECTOR3 (0, -2.3,0);
							vLook = D3DXVECTOR3 (0, -2.3, 1);
							g_Camera.SetViewParams (&vEye, &vLook);

							for (int J=0; J<10; J++)
								CreateDroid();

							g_Shop.bWeapon = false;
							g_Shop.iAlphaKey = 0;
							g_Shop.bOpen = true;
							g_Shop.iMoney = 0;
							g_Shop.iTimer = 10;
							g_Shop.iTool1 = 0;
							g_Shop.iTool2 = 0;
							g_Shop.iTool3 = 0;
							g_Shop.bT1 = false;
							g_Shop.bT2 = false;
							g_Shop.bT3 = false;
							g_Shop.bW = false;


							g_Render.fNotif1 = 0;
							g_Render.fNotif2 = 0;
							g_Render.fNotif3 = 0;
							g_Render.fNotif4 = 0;


							g_GameState.rHelper = false;
							g_GameState.fSub = 0.0f;


							g_GameState.iCAlphaV = 0;
							g_GameState.iGT = 20;
							g_GameState.fScnd = 0;
							g_GameState.gameMode = GAME_INTRO;
						}
						g_Render.pStatus->Draw( g_Render.pFinishRes, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,255));
						D3DXMatrixTranslation(&trans, (float)(g_Render.iPixelW - 642) /2 + 563, (float)(g_Render.iPixelH - 408)/2 + 1 ,0);
						g_Render.pStatus->SetTransform(&trans);
						g_Render.pStatus->Draw( g_Render.pWindowX, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,255));
					}
				}else{
					g_Render.pStatus->Draw( g_Render.pFinishRes, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,255));
				}
				g_Render.pStatus->End();

				txtHelper.Init( g_Render.pFont3, g_Render.pTextSprite, NULL, NULL, 15 );
				txtHelper.Begin();
				txtHelper.SetInsertionPos( (g_Render.iPixelW - 642) /2 + 140 , (g_Render.iPixelH - 408)/2 + 64 );
				txtHelper.SetForegroundColor( D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f ) );
				txtHelper.DrawFormattedTextLine (L"%d Chest", g_GameState.nChestCount);
				txtHelper.DrawFormattedTextLine (L"%d Sekon", g_GameState.iGT);

				txtHelper.Init( g_Render.pFont1, g_Render.pTextSprite, NULL, NULL, 15 );
				txtHelper.SetForegroundColor( D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f ) );
				for(int i=0; i<g_GameState.iHS;i++) {
					txtHelper.SetInsertionPos( (g_Render.iPixelW - 642) /2 + 50 , (g_Render.iPixelH - 408)/2 + 130 + i * 15 );

					txtHelper.DrawFormattedTextLine (L"%d", i + 1);

					txtHelper.SetInsertionPos( (g_Render.iPixelW - 642) /2 + 110 , (g_Render.iPixelH - 408)/2 + 130 + i * 15 );
					switch(g_GameState.HS[i].iID) {
						case 999:	txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ) );
									txtHelper.DrawFormattedTextLine (L"Andre Susanto (HUMAN)"); break;
						case 888:	txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ) );
									txtHelper.DrawFormattedTextLine (L"Tony (HUMAN)"); break;
						case 777:	txtHelper.SetForegroundColor( D3DXCOLOR( 0.0f, 0.0f, 0.0f, 0.7f ) );
									txtHelper.DrawFormattedTextLine (L"Reinaldo Michael (DEWA)"); break;
						default	:	if (g_GameState.HS[i].iColor == 4) {
										txtHelper.SetForegroundColor( D3DXCOLOR( 0.0f, 0.0f, 0.0f, 0.7f ) );
										txtHelper.DrawFormattedTextLine (L"Sembah BOT [R.I.P] (ID : %d)", g_GameState.HS[i].iID + 1); break;
									}else{
										txtHelper.SetForegroundColor( D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f ) );
										txtHelper.DrawFormattedTextLine (L"Dodo BOT (ID : %d)", g_GameState.HS[i].iID + 1); break;
									}break;
					}

					if (g_GameState.HS[i].iSkor < g_GameState.iLimit)
						txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ) );
					else
						txtHelper.SetForegroundColor( D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f ) );

					txtHelper.SetInsertionPos( (g_Render.iPixelW - 642) /2 + 500 , (g_Render.iPixelH - 408)/2 + 130 + i * 15 );

					txtHelper.DrawFormattedTextLine (L"%d", g_GameState.HS[i].iSkor);
					txtHelper.SetForegroundColor( D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f ) );
				}
				
				txtHelper.End();
				break;
			}
		case GAME_WAITING:
		case GAME_TOOL:
		case GAME_RUNNING:
			{
				if (g_GameState.bAiMode){
					txtHelper.Init( g_Render.pFont, g_Render.pTextSprite, NULL, NULL, 15 );
					txtHelper.Begin();
					txtHelper.SetInsertionPos( 2, 0 );
					txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	

					txtHelper.DrawTextLine( L"BUSER STIMA - Rinaldi Fans Club" );
					txtHelper.DrawTextLine( DXUTGetFrameStats( true ) );
					txtHelper.DrawFormattedTextLine( L"Posisi: %0.2f, %0.2f, %0.2f", g_Camera.GetEyePt()->x, g_Camera.GetEyePt()->y,
													 g_Camera.GetEyePt()->z );
					txtHelper.DrawFormattedTextLine( L"Vektor Pandangan: %0.2f, %0.2f, %0.2f", g_Camera.GetLookAtPt()->x, g_Camera.GetLookAtPt()->y,
													 g_Camera.GetLookAtPt()->z );
					txtHelper.DrawFormattedTextLine( L"[AI MODE] Sisa Waktu : %d", g_GameState.iGT );

					txtHelper.End();

					break;
				}

				D3DXMatrixTranslation(&trans, (float)g_Render.iPixelW - 235,  10 ,0);
				g_Render.pStatus->Begin(0);
				g_Render.pStatus->SetTransform(&trans);
				g_Render.pStatus->Draw( g_Render.pStatusRes, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,255));
				D3DXMatrixTranslation(&trans, (float)g_Render.iPixelW - 190,  178 ,0);
				g_Render.pStatus->SetTransform(&trans);
	
				for(A=0; A<MAX_CHEST; A++){
					if(g_GameState.ChestQ[A].bAvail == true){
						if (g_GameState.iCAlphaV < 255) g_GameState.iCAlphaV++;
						g_Render.pStatus->Draw( g_Render.pStatusCRes, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,g_GameState.iCAlphaV));
						break;
					}else{
						if (A==MAX_CHEST-1){
							if (g_GameState.iCAlphaV > 0) {
								g_GameState.iCAlphaV--;
								g_Render.pStatus->Draw( g_Render.pStatusCRes, NULL , NULL, NULL, D3DCOLOR_RGBA(255,255,255,g_GameState.iCAlphaV));
							}
						}
					}
				}

				
				g_Render.pStatus->End();

				txtHelper.Init( g_Render.pFont, g_Render.pTextSprite, NULL, NULL, 15 );
				txtHelper.Begin();
				txtHelper.SetInsertionPos( 2, 0 );
				txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	

				txtHelper.DrawTextLine( L"BUSER STIMA - Rinaldi Fans Club" );
				txtHelper.DrawTextLine( DXUTGetFrameStats( true ) );
				txtHelper.DrawFormattedTextLine( L"Posisi: %0.2f, %0.2f, %0.2f", g_Camera.GetEyePt()->x, g_Camera.GetEyePt()->y,
												 g_Camera.GetEyePt()->z );
				txtHelper.DrawFormattedTextLine( L"Vektor Pandangan: %0.2f, %0.2f, %0.2f", g_Camera.GetLookAtPt()->x, g_Camera.GetLookAtPt()->y,
												 g_Camera.GetLookAtPt()->z );

				txtHelper.Init( g_Render.pFont2, g_Render.pTextSprite, NULL, NULL, 60 );

				txtHelper.SetInsertionPos( g_Render.iPixelW - 158 , 62 );
				txtHelper.SetForegroundColor( D3DXCOLOR( 0, 0, 0, 1.0f ) );
				txtHelper.DrawFormattedTextLine( L"%2d" , g_GameState.iGT );
				txtHelper.SetInsertionPos( g_Render.iPixelW - 160 , 60 );
				txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
				txtHelper.DrawFormattedTextLine( L"%2d" , g_GameState.iGT  );

	
	
				txtHelper.Init (g_Render.pFont1, g_Render.pTextSprite, NULL, NULL, 15);
				txtHelper.SetInsertionPos( g_Render.iPixelW - 80 , 130 );
				txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
				txtHelper.DrawFormattedTextLine( L"%2d" , g_GameState.iLimit );
				txtHelper.DrawFormattedTextLine( L"%2d" , g_GameState.iMoney );
				txtHelper.DrawFormattedTextLine( L"%2d" , g_GameState.iGetChest );
				txtHelper.DrawFormattedTextLine( L"%2d" , g_GameState.iAmmo );



				txtHelper.End();
			}
			break;


		case GAME_PRESHOP:	
			txtHelper.Init( g_Render.pFont, g_Render.pTextSprite, NULL, NULL, 15 );
			txtHelper.Begin();
			txtHelper.SetInsertionPos( g_Render.iPixelW / 2 - 142, g_Render.iPixelH - 42 );
			txtHelper.SetForegroundColor( D3DXCOLOR( 0.0f, 0.0f, 0.0f, 0.9f ) );
			txtHelper.DrawTextLine( L"Untuk mengakhiri mode analisis, tekan tombol [ENTER] ..." );

			txtHelper.SetInsertionPos( g_Render.iPixelW / 2 - 144, g_Render.iPixelH - 44 );
			txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
			txtHelper.DrawTextLine( L"Untuk mengakhiri mode analisis, tekan tombol [ENTER] ..." );

			txtHelper.Init( g_Render.pFont2, g_Render.pTextSprite, NULL, NULL, 60 );
			txtHelper.SetInsertionPos( g_Render.iPixelW / 2 - 18, g_Render.iPixelH - 107 );
			txtHelper.SetForegroundColor( D3DXCOLOR( 0.0f, 0.0f, 0.0f, 0.9f ) );
			txtHelper.DrawFormattedTextLine( L"%2d", g_GameState.iGT);

			txtHelper.SetInsertionPos( g_Render.iPixelW / 2 - 20, g_Render.iPixelH - 109 );
			txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
			txtHelper.DrawFormattedTextLine( L"%2d", g_GameState.iGT );
			

			txtHelper.End();
			break;

	}
}


//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                          void* pUserContext )
{
    
	g_Render.DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );

    switch( g_GameState.gameMode )
    {
		case GAME_HELP:
            g_Render.HelpDlg.MsgProc( hWnd, uMsg, wParam, lParam );  break;
        case GAME_RUNNING:
            g_Camera.HandleMessages( hWnd, uMsg, wParam, lParam ); 
			if (DXUTIsWindowed()) {
				SetWindowPos(hWnd, HWND_TOPMOST, (GetSystemMetrics(SM_CXSCREEN) - g_Render.iPixelW) / 2 , (GetSystemMetrics(SM_CYSCREEN) - g_Render.iPixelH) / 2, 0,0, SWP_NOSIZE); 
			}
			break;
        case GAME_MAIN_MENU:
            g_Render.MainMenuDlg.MsgProc( hWnd, uMsg, wParam, lParam ); break;
        case GAME_AUDIO_MENU:
            g_Render.AudioMenuDlg.MsgProc( hWnd, uMsg, wParam, lParam ); break;
		
		case GAME_VIDEO_INTRO:
        case GAME_VIDEO_MENU:
            g_Render.VideoMenuDlg.MsgProc( hWnd, uMsg, wParam, lParam ); break;

		case GAME_INTRO:
			g_Render.IntroDlg.MsgProc( hWnd, uMsg, wParam, lParam ); break;
    }

    return 0;
}


//--------------------------------------------------------------------------------------
void UpdateAspectRatioList( DXUTDeviceSettings* pDS )
{
    bool bFullScreenChecked = g_Render.VideoMenuDlg.GetCheckBox( IDC_FULLSCREEN )->GetChecked();

    CD3D9EnumDeviceSettingsCombo* pDeviceSettingsCombo = DXUTGetD3D9Enumeration()->GetDeviceSettingsCombo
        ( &pDS->d3d9 );
    if( pDeviceSettingsCombo == NULL )
        return;
    CGrowableArray <D3DDISPLAYMODE>* pDMList;
    if( !bFullScreenChecked )
        pDMList = &g_Render.aWindowedDMList;
    else
        pDMList = &pDeviceSettingsCombo->pAdapterInfo->displayModeList;

    UINT nAdapterMonitorWidth = g_Render.rcAdapterWork[pDS->d3d9.AdapterOrdinal].right -
        g_Render.rcAdapterWork[pDS->d3d9.AdapterOrdinal].left;
    UINT nAdapterMonitorHeight = g_Render.rcAdapterWork[pDS->d3d9.AdapterOrdinal].bottom -
        g_Render.rcAdapterWork[pDS->d3d9.AdapterOrdinal].top;

    CGrowableArray <float> aspectRatioList;
    for( int i = 0; i < pDMList->GetSize(); i++ )
    {
        D3DDISPLAYMODE dm = pDMList->GetAt( i );

        
		if( ( bFullScreenChecked && dm.Height >= 600 ) ||
            ( !bFullScreenChecked && dm.Width <= nAdapterMonitorWidth && dm.Height <= nAdapterMonitorHeight ) )
        {
            
			float fAspect = ( float )dm.Width / ( float )dm.Height;
            bool bFound = false;
            for( int iAspect = 0; iAspect < aspectRatioList.GetSize(); iAspect++ )
            {
                if( fabsf( aspectRatioList.GetAt( iAspect ) - fAspect ) < 0.05f )
                {
                    bFound = true;
                    break;
                }
            }
            if( !bFound )
                aspectRatioList.Add( fAspect );
        }
    }

    
	if( aspectRatioList.GetData() )
        qsort( aspectRatioList.GetData(), aspectRatioList.GetSize(), sizeof( float ), SortAspectRatios );

    
	float fCurrentlySelectedAspect = 0.0f;
    if( g_Render.VideoMenuDlg.GetComboBox( IDC_ASPECT )->GetNumItems() > 0 )
    {
        void* pD = g_Render.VideoMenuDlg.GetComboBox( IDC_ASPECT )->GetSelectedData();
        fCurrentlySelectedAspect = *( float* )( void* )&pD;
    }

    
	g_Render.VideoMenuDlg.GetComboBox( IDC_ASPECT )->RemoveAllItems();
    for( int i = 0; i < aspectRatioList.GetSize(); i++ )
    {
        WCHAR sz[256];
        float fAspect = aspectRatioList.GetAt( i );

        
		if( fabsf( fAspect - 1.3333f ) < 0.05f ) swprintf_s( sz, 256, L"4:3" );
        else if( fabsf( fAspect - 1.25f ) < 0.05f ) swprintf_s( sz, 256, L"5:4" );
        else if( fabsf( fAspect - 1.77f ) < 0.05f ) swprintf_s( sz, 256, L"16:9" );
        else if( fabsf( fAspect - 1.6f ) < 0.05f ) swprintf_s( sz, 256, L"16:10" );
        else if( fabsf( fAspect - 1.5f ) < 0.05f ) swprintf_s( sz, 256, L"3:2" );
        else if( fabsf( fAspect - 0.8f ) < 0.05f ) swprintf_s( sz, 256, L"4:5" );
        else if( fabsf( fAspect - 1.66f ) < 0.05f ) swprintf_s( sz, 256, L"5:3" );
        else if( fabsf( fAspect - 0.75f ) < 0.05f ) swprintf_s( sz, 256, L"3:4" );
        else if( fabsf( fAspect - 0.5625f ) < 0.05f ) swprintf_s( sz, 256, L"9:16" );
        else
            swprintf_s( sz, 256, L"%0.2f:1", fAspect );

        g_Render.VideoMenuDlg.GetComboBox( IDC_ASPECT )->AddItem( sz, UintToPtr( *( DWORD* )&fAspect ) );
    }

    
	if( fCurrentlySelectedAspect == 0.0f )
    {
        D3DDISPLAYMODE dmDesktop;
        DXUTGetDesktopResolution( pDS->d3d9.AdapterOrdinal, &dmDesktop.Width, &dmDesktop.Height );
        g_Render.fDesktopAspectRatio = dmDesktop.Width / ( FLOAT )dmDesktop.Height;
        fCurrentlySelectedAspect = g_Render.fDesktopAspectRatio;
    }
    g_Render.VideoMenuDlg.GetComboBox( IDC_ASPECT )->SetSelectedByData( UintToPtr
                                                                        ( *( DWORD* )&fCurrentlySelectedAspect ) );

    
	UpdateResolutionList( pDS );
}


//--------------------------------------------------------------------------------------
void UpdateResolutionList( DXUTDeviceSettings* pDS )
{
    bool bFullScreenChecked = g_Render.VideoMenuDlg.GetCheckBox( IDC_FULLSCREEN )->GetChecked();

    
	void* pData = g_Render.VideoMenuDlg.GetComboBox( IDC_ASPECT )->GetSelectedData();
    float fSelectedAspect = *( float* )( void* )&pData;

    
	void* pSelectedData = NULL;
    if( g_Render.VideoMenuDlg.GetComboBox( IDC_RESOLUTION )->GetNumItems() > 0 )
        pSelectedData = g_Render.VideoMenuDlg.GetComboBox( IDC_RESOLUTION )->GetSelectedData();

    UINT nAdapterMonitorWidth = g_Render.rcAdapterWork[pDS->d3d9.AdapterOrdinal].right -
        g_Render.rcAdapterWork[pDS->d3d9.AdapterOrdinal].left;
    UINT nAdapterMonitorHeight = g_Render.rcAdapterWork[pDS->d3d9.AdapterOrdinal].bottom -
        g_Render.rcAdapterWork[pDS->d3d9.AdapterOrdinal].top;

    CGrowableArray <D3DDISPLAYMODE>* pDMList;
    CD3D9EnumDeviceSettingsCombo* pDeviceSettingsCombo = DXUTGetD3D9Enumeration()->GetDeviceSettingsCombo
        ( &pDS->d3d9 );
    if( !bFullScreenChecked )
        pDMList = &g_Render.aWindowedDMList;
    else
        pDMList = &pDeviceSettingsCombo->pAdapterInfo->displayModeList;

    
	g_Render.VideoMenuDlg.GetComboBox( IDC_RESOLUTION )->RemoveAllItems();
    for( int i = 0; i < pDMList->GetSize(); i++ )
    {
        D3DDISPLAYMODE dm = pDMList->GetAt( i );

        
		if( ( bFullScreenChecked && dm.Height >= 600 ) ||
            ( !bFullScreenChecked && dm.Width <= nAdapterMonitorWidth && dm.Height <= nAdapterMonitorHeight ) )
        {
        
			float fAspect = ( float )dm.Width / ( float )dm.Height;
            if( fabsf( fSelectedAspect - fAspect ) < 0.05f )
            {
                WCHAR sz[256];
                swprintf_s( sz, 256, L"%d x %d", dm.Width, dm.Height );
                if( !g_Render.VideoMenuDlg.GetComboBox( IDC_RESOLUTION )->ContainsItem( sz ) )
                    g_Render.VideoMenuDlg.GetComboBox( IDC_RESOLUTION )->AddItem( sz, UIntToPtr( MAKELONG( dm.Width,
                                                                                                           dm.Height
                                                                                                           ) ) );
            }
        }
    }

    bool bSetToDefault = false;
    if( pSelectedData )
    {
        HRESULT hr = g_Render.VideoMenuDlg.GetComboBox( IDC_RESOLUTION )->SetSelectedByData( pSelectedData );
        if( FAILED( hr ) )
            bSetToDefault = true;
    }
    else
    {
        bSetToDefault = true;
    }

    if( bSetToDefault )
    {
        D3DDISPLAYMODE dmDesktop;
        DXUTGetDesktopResolution( pDS->d3d9.AdapterOrdinal, &dmDesktop.Width, &dmDesktop.Height );
        g_Render.VideoMenuDlg.GetComboBox( IDC_RESOLUTION )->SetSelectedByData( UintToPtr( MAKELONG( dmDesktop.Width,
                                                                                                     dmDesktop.Height
                                                                                                     ) ) );
    }
}


//--------------------------------------------------------------------------------------
// Penanganan event dari GUI (trigger dari menu)
//--------------------------------------------------------------------------------------
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
    switch( nControlID )
    {
		case IDC_AI_MODE:
			g_GameState.bAiMode = true;
			g_GameState.iAmmo = 0;
			SetCursorPos(g_Render.iPixelW / 2, g_Render.iPixelH / 2);
			DXUTSetCursorSettings( false, true );
			LoadLevel(0);
			g_GameState.gameMode = GAME_RUNNING;
            break;
		case IDC_LEVEL1:
			LoadLevel(1);
			g_GameState.bAiMode = false;
			g_GameState.iGT = 15;
			SetCursorPos(g_Render.iPixelW / 2, g_Render.iPixelH / 2);
			DXUTSetCursorSettings( false, true );
			g_GameState.bDroidMove = false;
			g_Camera.SetEnablePositionMovement(false);
			g_GameState.gameMode = GAME_PRESHOP;
			break;
		case IDC_LEVEL2:
			LoadLevel(2);
			g_GameState.bAiMode = false;
			g_GameState.iGT = 15;
			SetCursorPos(g_Render.iPixelW / 2, g_Render.iPixelH / 2);
			DXUTSetCursorSettings( false, true );
			g_GameState.bDroidMove = false;
			g_Camera.SetEnablePositionMovement(false);
			g_GameState.gameMode = GAME_PRESHOP;
			break;
		case IDC_LEVEL3:
			LoadLevel(3);
			g_GameState.bAiMode = false;
			g_GameState.iGT = 15;
			SetCursorPos(g_Render.iPixelW / 2, g_Render.iPixelH / 2);
			DXUTSetCursorSettings( false, true );
			g_GameState.bDroidMove = false;
			g_Camera.SetEnablePositionMovement(false);
			g_GameState.gameMode = GAME_PRESHOP;
			break;
        case IDC_RESUME:
			SetCursorPos(g_Render.iPixelW / 2, g_Render.iPixelH / 2);
            g_GameState.gameMode = GAME_RUNNING;
            DXUTSetCursorSettings( false, true );
            break;
        case IDC_BACK:
			switch(g_GameState.gameMode){
				case GAME_VIDEO_INTRO:	g_GameState.gameMode = GAME_INTRO; break;
				default				 :	g_GameState.gameMode = GAME_MAIN_MENU; break;
			}
            
            break;
        case IDC_AUDIO:
            g_GameState.gameMode = GAME_AUDIO_MENU;
            break;
        case IDC_VIDEO:
        {
            g_GameState.gameMode = GAME_VIDEO_MENU;

            // tampilkan setting yg aktif
            g_Render.VideoMenuDlg.EnableNonUserEvents( true );
            g_Render.VideoMenuDlg.GetCheckBox( IDC_FULLSCREEN )->SetChecked( !DXUTIsWindowed() );
            g_Render.VideoMenuDlg.EnableNonUserEvents( false );
            g_Render.VideoMenuDlg.GetCheckBox( IDC_ANTI_ALIASING )->SetChecked( DXUTGetDeviceSettings
                                                                                ().d3d9.pp.MultiSampleType !=
                                                                                D3DMULTISAMPLE_NONE );
            g_Render.VideoMenuDlg.GetComboBox( IDC_RESOLUTION )->SetSelectedByData(
                UintToPtr( MAKELONG( DXUTGetDeviceSettings().d3d9.pp.BackBufferWidth,
                                     DXUTGetDeviceSettings().d3d9.pp.BackBufferHeight ) ) );
            g_Render.VideoMenuDlg.GetCheckBox( IDC_HIGH_MODEL_RES )->SetChecked( g_Render.UseHighResModels );
            //g_Render.VideoMenuDlg.GetSlider( IDC_MAX_DROIDS )->SetValue( g_GameState.nMaxDroids );
            break;
        }
		case IDC_VIDEO2:
        {
            g_GameState.gameMode = GAME_VIDEO_INTRO;

            // tampilkan setting yg aktif
            g_Render.VideoMenuDlg.EnableNonUserEvents( true );
            g_Render.VideoMenuDlg.GetCheckBox( IDC_FULLSCREEN )->SetChecked( !DXUTIsWindowed() );
            g_Render.VideoMenuDlg.EnableNonUserEvents( false );
            g_Render.VideoMenuDlg.GetCheckBox( IDC_ANTI_ALIASING )->SetChecked( DXUTGetDeviceSettings
                                                                                ().d3d9.pp.MultiSampleType !=
                                                                                D3DMULTISAMPLE_NONE );
            g_Render.VideoMenuDlg.GetComboBox( IDC_RESOLUTION )->SetSelectedByData(
                UintToPtr( MAKELONG( DXUTGetDeviceSettings().d3d9.pp.BackBufferWidth,
                                     DXUTGetDeviceSettings().d3d9.pp.BackBufferHeight ) ) );
            g_Render.VideoMenuDlg.GetCheckBox( IDC_HIGH_MODEL_RES )->SetChecked( g_Render.UseHighResModels );
            //g_Render.VideoMenuDlg.GetSlider( IDC_MAX_DROIDS )->SetValue( g_GameState.nMaxDroids );
            break;
        }
        case IDC_QUIT:
            DXUTShutdown( 0 );
            break;
        case IDC_MUSIC_SCALE:
            g_audioState.fMusicVolume = ( float )( g_Render.AudioMenuDlg.GetSlider( IDC_MUSIC_SCALE )->GetValue() /
                                                   100.0f );
            SetMusicVolume( g_audioState.fMusicVolume );
            break;
        case IDC_SOUNDFX_SCALE:
            g_audioState.fSoundFXVolume = ( float )( g_Render.AudioMenuDlg.GetSlider( IDC_SOUNDFX_SCALE )->GetValue() /
                                                     100.0f );
            SetSoundFxVolume( g_audioState.fSoundFXVolume );
            break;
		case IDC_HELPS:
			g_GameState.gameMode = GAME_HELP;
			break;
		case IDC_CLOSE_HELP:
			g_GameState.gameMode = GAME_INTRO;
			break;
        case IDC_FULLSCREEN:
        {
            // update screen sesuai yg full screen
            DXUTDeviceSettings ds = DXUTGetDeviceSettings();
            UpdateAspectRatioList( &ds );
            break;
        }
        case IDC_ASPECT:
        {
            // update screen sesuai aspek rasio yg dipilih
            DXUTDeviceSettings ds = DXUTGetDeviceSettings();
            UpdateResolutionList( &ds );
            break;
        }
        case IDC_APPLY:
        {
            // Ambil setting yg dipilih user
            bool bAA = g_Render.VideoMenuDlg.GetCheckBox( IDC_ANTI_ALIASING )->GetChecked();
            UINT nRes = PtrToInt( g_Render.VideoMenuDlg.GetComboBox( IDC_RESOLUTION )->GetSelectedData() );
            int nWidth = LOWORD( nRes ); int nHeight = HIWORD( nRes );
            bool bFullscreen = g_Render.VideoMenuDlg.GetCheckBox( IDC_FULLSCREEN )->GetChecked();

            DXUTDeviceSettings ds = DXUTGetDeviceSettings();
            ds.d3d9.pp.MultiSampleType = ( bAA ) ? D3DMULTISAMPLE_4_SAMPLES : D3DMULTISAMPLE_NONE;
            ds.d3d9.pp.Windowed = ( BOOL )!bFullscreen;
            ds.d3d9.pp.BackBufferWidth = nWidth; ds.d3d9.pp.BackBufferHeight = nHeight;

            // Terapkan settingnya
            g_Render.bDetectOptimalSettings = false;
            DXUTCreateDeviceFromSettings( &ds );
            g_Render.bDetectOptimalSettings = true;

            //g_GameState.nMaxDroids = g_Render.VideoMenuDlg.GetSlider( IDC_MAX_DROIDS )->GetValue();
            g_Render.UseHighResModels = g_Render.VideoMenuDlg.GetCheckBox( IDC_HIGH_MODEL_RES )->GetChecked();


            while( g_GameState.nDroidCount > g_GameState.nMaxDroids )
            {
                for( int A = 0; A < MAX_DROID; A++ )
                {
                    if( g_GameState.DroidQ[A].bActive )
                    {
                        g_GameState.DroidQ[A].bActive = false;
                        g_GameState.nDroidCount--;
                        SetNumDroidsForAudio( g_GameState.nDroidCount );
                        break;
                    }
                }
            }
        }
    }
}


//--------------------------------------------------------------------------------------
void ToggleMenu()
{
    switch( g_GameState.gameMode )
    {
        case GAME_RUNNING:
            g_GameState.gameMode = GAME_MAIN_MENU; 
			DXUTSetCursorSettings( true, true );
			break;
        case GAME_MAIN_MENU:
            g_GameState.gameMode = GAME_RUNNING; 
			DXUTSetCursorSettings( false, true );break;
        case GAME_AUDIO_MENU:
            g_GameState.gameMode = GAME_MAIN_MENU; DXUTSetCursorSettings( true, true );
			break;
        case GAME_VIDEO_MENU:
            g_GameState.gameMode = GAME_MAIN_MENU; DXUTSetCursorSettings( true, true );
			break;
    }
}


//--------------------------------------------------------------------------------------
void CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
    if( bKeyDown )
    {
        switch( nChar )
        {
            case VK_ESCAPE:
                ToggleMenu();
                break;

            case VK_RETURN: // Droid move toggle
				switch(g_GameState.gameMode) {
					case GAME_RUNNING : g_GameState.bDroidMove = !g_GameState.bDroidMove; break;
					case GAME_PRESHOP : g_GameState.iGT = 15; DXUTSetCursorSettings(true, false); g_GameState.gameMode = GAME_SHOP; break;
					case GAME_SHOP	  : g_GameState.iGT = 15; DXUTSetCursorSettings(true, false); g_GameState.gameMode = GAME_SHOP; break;
				}
                
                break;

            case 'M': // Droid auto-add 
                g_GameState.bAutoAddDroids = !g_GameState.bAutoAddDroids;
                break;

            case 'X': // Droid add
                if( g_GameState.nDroidCount < MAX_DROID )
                    g_GameState.bDroidCreate = true;
                break;

            case 'B': // Mass droid kill
                g_GameState.bMassDroidKill = true;
                break;
        }
    }
}


//--------------------------------------------------------------------------------------
// Fungsi yang dijalankan ketika D3D mengalami Lost Device.
//--------------------------------------------------------------------------------------
void CALLBACK OnLostDevice( void* pUserContext )
{
    g_Render.DialogResourceManager.OnD3D9LostDevice();
    if( g_Render.pFont )
        g_Render.pFont->OnLostDevice();
	if( g_Render.pFont1 )
        g_Render.pFont1->OnLostDevice();
	if( g_Render.pFont2 )
			g_Render.pFont2->OnLostDevice();
	if( g_Render.pFont3 )
			g_Render.pFont3->OnLostDevice();

	if( g_Render.pShop )
		g_Render.pShop->OnLostDevice();
	if (g_Render.pTool )
		g_Render.pTool->OnLostDevice();
    if( g_Render.pEffect )
        g_Render.pEffect->OnLostDevice();
	if( g_Render.pHelper )
        g_Render.pHelper->OnLostDevice();
	if( g_Render.pStatus )
        g_Render.pStatus->OnLostDevice();

    SAFE_RELEASE( g_Render.pTextSprite );
    g_Render.meshCell.InvalidateDeviceObjects();
    g_Render.meshAmmo.InvalidateDeviceObjects();
    g_Render.meshDroid.InvalidateDeviceObjects();
    g_Render.meshDroidExplosion.InvalidateDeviceObjects();
    g_Render.meshDroidLow.InvalidateDeviceObjects();
    g_Render.meshDroidExplosionLow.InvalidateDeviceObjects();
	g_Render.meshChestGreen.InvalidateDeviceObjects();
	g_Render.meshChestYellow.InvalidateDeviceObjects();
	g_Render.meshChestRed.InvalidateDeviceObjects();
}


//--------------------------------------------------------------------------------------
// Destruktor dari D3D, handle objek yang sudah dibuat disini
//--------------------------------------------------------------------------------------
void CALLBACK OnDestroyDevice( void* pUserContext )
{
    g_Render.DialogResourceManager.OnD3D9DestroyDevice();
    SAFE_RELEASE( g_Render.pEffect );
    SAFE_RELEASE( g_Render.pFont );
	SAFE_RELEASE( g_Render.pFont1 );
	SAFE_RELEASE( g_Render.pFont2 );
	SAFE_RELEASE( g_Render.pFont3 );

    SAFE_RELEASE( g_Render.pDefaultTex );
    SAFE_RELEASE( g_Render.pDefaultNormalMap );
    SAFE_RELEASE( g_Render.pDroidNormalMap );
	SAFE_RELEASE( g_Render.pShopRes );
	SAFE_RELEASE( g_Render.pShopRes1 );
	SAFE_RELEASE( g_Render.pShopRes2 );
	SAFE_RELEASE( g_Render.pShopRes3 );
	SAFE_RELEASE( g_Render.pShopRes4 );
	SAFE_RELEASE( g_Render.pShopRes5 );

	SAFE_RELEASE( g_Render.pNotif1 );
	SAFE_RELEASE( g_Render.pNotif2 );
	SAFE_RELEASE( g_Render.pNotif3 );
	SAFE_RELEASE( g_Render.pNotif4 );

	SAFE_RELEASE( g_Render.pHelper );
	SAFE_RELEASE( g_Render.pHelperRes );
	SAFE_RELEASE( g_Render.pITB );

	SAFE_RELEASE( g_Render.pStatus );
	SAFE_RELEASE( g_Render.pStatusRes );
	SAFE_RELEASE( g_Render.pStatusCRes );

	SAFE_RELEASE( g_Render.pShop );
	SAFE_RELEASE( g_Render.pTool );

	SAFE_RELEASE( g_Render.pWindowX );
	SAFE_RELEASE( g_Render.pFinishRes );

	SAFE_RELEASE( g_Render.pToolRes1 );
	SAFE_RELEASE( g_Render.pToolRes2 );
	SAFE_RELEASE( g_Render.pToolRes3 );
	SAFE_RELEASE( g_Render.pToolRes10 );
	SAFE_RELEASE( g_Render.pToolRes20 );
	SAFE_RELEASE( g_Render.pToolRes30 );

    g_Render.meshCell.Destroy();
    g_Render.meshAmmo.Destroy();
    g_Render.meshDroid.Destroy();
    g_Render.meshDroidExplosion.Destroy();
    g_Render.meshDroidLow.Destroy();
    g_Render.meshDroidExplosionLow.Destroy();
	g_Render.meshChestGreen.Destroy();
	g_Render.meshChestYellow.Destroy();
	g_Render.meshChestRed.Destroy();
    SAFE_RELEASE( g_Render.pMeshDroidExplosion );
}


