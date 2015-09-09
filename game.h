#include "resource.h"
#include "DXUTcamera.h"
#include "DXUTsettingsdlg.h"
#include "SDKmesh.h"
//#include "fExternal.h"

// Consts
#define MAXANISOTROPY 8 
#define GROUND_Y 3.0f				// -GROUND_Y = kordinat ground.
#define GROUND_ABSORBANCE 0.2f		// power yg diserap oleh tanah untuk peluru.
#define AMMO_ABSORBANCE 0.1f		// power yg diserap oleh ammo yg bertabrakan.
#define MAX_AMMO 100  
#define MAX_DROID 50
#define MAX_CHEST 100
#define DROID_HITPOINTS 20
#define AMMO_SIZE 0.050f 
#define DROID_SIZE 0.5f
#define DROID_MIN_HEIGHT 0.5f
#define DROID_HEIGHT_FLUX 0.5f
#define DROID_TURN_AI_PERCENT 40
#define DROID_MOVE_AI_PERCENT 40
#define DROID_MOVE_TIME_MIN  2000
#define DROID_MOVE_TIME_FLUX 3000
#define DROID_CREATE_DELAY_FLUX 2500
#define DROID_DEATH_SPEED 3.0f;
#define CHEST_SIZE 0.3f
#define CHEST_MIN_HEIGHT 0.5f
#define CHEST_HEIGHT_FLUX 0.5f
#define AUTOFIRE_DELAY 0.1f 
#define CAMERA_SIZE 0.2f
#define GRAVITY 5.0f  
#define DROID_VELOCITY 2.0f 
#define BOUNCE_TRANSFER 0.8f 
#define BOUNCE_LOST 0.1f  
#define REST_THRESHOLD 0.005f 
#define PHYSICS_FRAMELENGTH 0.003f 
#define MAX_SOUND_VELOCITY 1.0f 
#define MIN_SOUND_VELOCITY 0.07f 
#define MIN_VOL_ADJUST 0.8f 



// Boundaris untuk objek
const D3DXVECTOR3           g_MinBound( -6.0f, -GROUND_Y, -6.0f );
const D3DXVECTOR3           g_MaxBound( 6.0f, GROUND_Y, 6.0f );

enum AI_STATE
{
    AI_TURNING = 1,
    AI_MOVING,
    AI_STOPPED,
};

enum CH_STATE
{
    CH_CLOSED = 1,
    CH_OPEN,
    CH_EXPLODE,
};

enum GAME_MODE
{
    GAME_RUNNING = 1,
    GAME_MAIN_MENU,
    GAME_AUDIO_MENU,
    GAME_VIDEO_MENU,
	GAME_VIDEO_INTRO,
	GAME_INTRO,
	GAME_PRESHOP,
	GAME_SHOP,
	GAME_TOOL,
	GAME_HELP,
	GAME_WAITING,
	GAME_FINISH
};

struct OUTFILE
{
	int iRobot[MAX_DROID];
	int iJenis[MAX_DROID];
	int iSimpul[MAX_DROID];
	int N;
};

struct SHOP_STATE
{
	bool bWeapon;
	bool bOpen;
	bool bT1;
	bool bT2;
	bool bT3;
	bool bW;
	bool bClose;

	float iAlphaKey;
	int iTool1;
	int iTool2;
	int iTool3;
	int iMoney;
	int iTimer;
};


struct RENDER_STATE
{
    D3DXMATRIXA16 mCellWorld; 
    D3DXMATRIXA16 mAmmoWorld;  
    D3DXMATRIXA16 mDroidWorld; 
	D3DXMATRIXA16 mChestWorld; 

	int iPixelW;
	int iPixelH;

    ID3DXFont* pFont;  
	ID3DXFont* pFont1;  
	ID3DXFont* pFont2;
	ID3DXFont* pFont3;  

	ID3DXSprite* pShop;
	ID3DXSprite* pTool;
	ID3DXSprite* pHelper;
	ID3DXSprite* pStatus;


    ID3DXSprite* pTextSprite; 
	IDirect3DTexture9 *pHelperRes;

	IDirect3DTexture9 *pShopRes;
	IDirect3DTexture9 *pShopRes1;
	IDirect3DTexture9 *pShopRes2;
	IDirect3DTexture9 *pShopRes3;
	IDirect3DTexture9 *pShopRes4;
	IDirect3DTexture9 *pShopRes5;

	IDirect3DTexture9 *pToolRes1; 
	IDirect3DTexture9 *pToolRes2;
	IDirect3DTexture9 *pToolRes3;
	IDirect3DTexture9 *pToolRes10;
	IDirect3DTexture9 *pToolRes20;
	IDirect3DTexture9 *pToolRes30;

	IDirect3DTexture9 *pFinishRes;

	IDirect3DTexture9 *pNotif1;
	IDirect3DTexture9 *pNotif2;
	IDirect3DTexture9 *pNotif3;
	IDirect3DTexture9 *pNotif4;


	IDirect3DTexture9 *pWindowX;

	IDirect3DTexture9 *pStatusRes;
	IDirect3DTexture9 *pStatusCRes; //chest


	IDirect3DTexture9 *pITB;					

    CDXUTDialogResourceManager DialogResourceManager;
	CDXUTDialog IntroDlg; 
    CDXUTDialog MainMenuDlg;  
    CDXUTDialog VideoMenuDlg; 
    CDXUTDialog AudioMenuDlg; 
	CDXUTDialog HelpDlg; 

    ID3DXEffect* pEffect; 
    IDirect3DTexture9* pDefaultTex; 
    IDirect3DTexture9* pDefaultNormalMap;

    IDirect3DTexture9* pDroidNormalMap;	

    CDXUTXFileMesh meshCell;   
    CDXUTXFileMesh meshAmmo;   
    CDXUTXFileMesh meshDroid;
    CDXUTXFileMesh meshDroidLow;
    CDXUTXFileMesh meshDroidExplosion;
    CDXUTXFileMesh meshDroidExplosionLow;
	CDXUTXFileMesh meshChestGreen;
	CDXUTXFileMesh meshChestYellow;
	CDXUTXFileMesh meshChestRed;
    ID3DXMesh* pMeshDroidExplosion;

    D3DXHANDLE hShaderTech;     
    D3DXHANDLE hMatW;           
    D3DXHANDLE hMatV;
    D3DXHANDLE hMatP;
    D3DXHANDLE hMatWV;
    D3DXHANDLE hMatWVP;
    D3DXHANDLE hDiffuse;  
    D3DXHANDLE hSpecular;
    D3DXHANDLE hDiffuseTexture;
    D3DXHANDLE hPower;
    D3DXHANDLE hAnimation;
    D3DXHANDLE hNormalMap;

    float UseFixedFunction;
    int ForceShader;
    float MaximumResolution;
    float DisableSpecular;
    bool UseAnisotropicFilter;
    bool UseHighResModels;
    bool bDetectOptimalSettings;
    float fDesktopAspectRatio;

	float fNotif1;
	float fNotif2;
	float fNotif3;
	float fNotif4;

    RECT rcAdapterWork[10];
    CGrowableArray <D3DDISPLAYMODE> aWindowedDMList;
};


struct DROID_VERTEX
{
    D3DXVECTOR3 pos;
    D3DXVECTOR3 normal;
    D3DXVECTOR2 uv;
};

struct CH_TASKING
{
	int IDC;
	bool bEnabled;
	int iTimer;
};

struct AMMO_STATE
{
    D3DXMATRIXA16 mAmmoRot;
    bool bActive;   
    D3DXVECTOR3 vPosition;
    D3DXVECTOR3 vVelocity;

    D3DXVECTOR4 Diffuse;
    double fTimeCreated;
    float fLifeCountdown;
    float fAlpha;
    bool bGround;   
};

struct DROID_STATE
{
    bool bActive;
	bool bBusy;
    D3DXVECTOR3 vPosition;
    D3DXVECTOR3 vVelocity;

	int iScore;

	int iTool1; // tool spesial 3 (1s)
	int iTool2; // tool 2 (2s)
	// tidak pake tool 1 (4s)

	int min;

    D3DXVECTOR3 vNudgeVelocity;
    AI_STATE aiState;
    float fTargetRotation;
    D3DXQUATERNION qTarget;
    D3DXQUATERNION qStart;
    D3DXQUATERNION qCurrent;
    float fRotInterp;
    float fTaskTimer;
    int nHitPoints;
    float fDeathAnimation;
    float fAlpha;
    D3DXCOLOR Specular;
};

struct CHEST_STATE
{
    bool bActive;
	bool bAvail;
	bool bTargeted;
    D3DXVECTOR3 vPosition;
    int iChestType;
	float fRotator;

    CH_STATE chState;
    float fTargetRotation;
    D3DXQUATERNION qTarget;
    D3DXQUATERNION qStart;
    D3DXQUATERNION qCurrent;
    float fRotInterp;
    float fTaskTimer;
    float fExplodeAnimation;
    float fAlpha;
};

struct HIGH_SCORE
{
	int iColor;
	int iID;
	int iSkor;
};

struct GAME_STATE
{
    AMMO_STATE      AmmoQ[MAX_AMMO];  
    int nAmmoCount;    
	
    float fAmmoColorLerp;
    D3DXCOLOR BlendToColor;
    D3DXCOLOR BlendFromColor;

    DROID_STATE     DroidQ[MAX_DROID];
    int nDroidCount;
    int nMaxDroids;

	int iPCTarget;

	int iCAlphaV;
	int iGT;
	float fScnd;
	float fSub;

	int iHS;
	HIGH_SCORE HS[10];

	int iLevelT;
	int iLimit;
	int iMoney;
	int iGetChest;
	int iAmmo;

	bool rHelper;

    bool bDroidCreate;
    bool bMassDroidKill;
	bool bAiMode;
    float fDroidCreateCountdown;

    bool bDroidMove;
    bool bAutoAddDroids;

	CHEST_STATE		ChestQ[MAX_CHEST];
	int nChestCount;
	
    DXUT_GAMEPAD    gamePad[DXUT_MAX_CONTROLLERS];  

    GAME_MODE gameMode;
};


// Global variables

extern CFirstPersonCamera   g_Camera;
extern RENDER_STATE         g_RenderState;
extern GAME_STATE           g_GameState;


// External functions

bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed,
                                  void* pUserContext );
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                 void* pUserContext );
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                void* pUserContext );
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                          void* pUserContext );
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
void CALLBACK OnLostDevice( void* pUserContext );
void CALLBACK OnDestroyDevice( void* pUserContext );
void CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void InitApp();


