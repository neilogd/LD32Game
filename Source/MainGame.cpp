/**************************************************************************
*
* File:		MainGame.cpp
* Author: 	Neil Richardson 
* Ver/Date:	
* Description:
*		Main Game Entrypoint.
*		
*
*
* 
**************************************************************************/

#include "Psybrus.h"

#include "System/Content/CsCore.h"
#include "System/Os/OsCore.h"
#include "System/Scene/ScnCore.h"

//////////////////////////////////////////////////////////////////////////
// GPsySetupParams
PsySetupParams GPsySetupParams( "LD32Game", psySF_GAME_DEV, 1.0f / 60.0f );	
extern BcU32 GResolutionWidth;
extern BcU32 GResolutionHeight;

//////////////////////////////////////////////////////////////////////////
// PsyGameInit
void PsyGameInit()
{
	//GResolutionWidth = 900;
	//GResolutionHeight = 600;
}

//////////////////////////////////////////////////////////////////////////
// PsyLaunchGame
void PsyLaunchGame()
{
	ScnCore::pImpl()->spawnEntity( ScnEntitySpawnParams( "SceneEntity", "default", "RootEntity", MaMat4d(), nullptr ) );
}
