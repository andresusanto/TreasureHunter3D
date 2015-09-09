#include <xact3.h>
#include <xact3d3.h>

struct XACTGAME_3DCUE
{
    bool bActive;
    IXACT3Cue* pCue;
    D3DXVECTOR3 vEmitterPosition;
};

struct AUDIO_STATE
{
    CRITICAL_SECTION cs;
    bool bInitialized;
    bool bMusicPlaying;
    bool bUse3DAudio;


    XACTINDEX iAmmoBounce;
    XACTINDEX iAmmoFire;
    XACTINDEX iDroidDestroyed;
    XACTINDEX iDroidScan;
    XACTINDEX iBackgroundMusic;
    XACTINDEX iRoomRumble;

    
	XACTINDEX iNumDroidDestroyedCueInstances;
    XACTINDEX iNumDroids;

    XACTVARIABLEVALUE fNumDroidDestroyedCueInstances;
    bool bHandleCueInstanceChange;

    XACTCATEGORY iMusicCategory;
    XACTCATEGORY iSoundFXCategory;
    float fSoundFXVolume;
    float fMusicVolume;

    
	IXACT3WaveBank* pWaveBank;
    IXACT3SoundBank* pSoundBank;

    
	IXACT3Engine* pEngine;

    
	X3DAUDIO_HANDLE x3DInstance;

    IXACT3Cue* pBackgroundMusic;
    IXACT3Cue* pRoomRumble;

    VOID* pbWaveBank; 
    VOID* pbSoundBank;

    int nFrameToApply3DAudio;
    X3DAUDIO_DSP_SETTINGS dspSettings;
    X3DAUDIO_LISTENER listener;
    X3DAUDIO_EMITTER emitter;
    FLOAT32 DelayTimes[2];
    FLOAT32 MatrixCoefficients[2 * 8];
    CGrowableArray <XACTGAME_3DCUE> v3DCues;
};


// Global variables
extern AUDIO_STATE g_audioState;


// External functions
HRESULT PrepareAudio();
HRESULT UpdateAudio();
HRESULT PlayAudioCue( XACTINDEX iCueIndex );
HRESULT Play3DAudioCue( XACTINDEX iCueIndex, D3DXVECTOR3* pvPosition );
void SetNumDroidsForAudio( int nDroidCount );
void SetMusicVolume( float fVolume );
void SetSoundFxVolume( float fVolume );
VOID CleanupAudio();
HRESULT PlayBGMusic();



