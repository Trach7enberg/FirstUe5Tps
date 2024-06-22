// FutureTPS Game All Rights Reserved


#include "GameMode/TPSMenuGameModeBase.h"

#include "GameInstances/TPSGameInstance.h"
#include "UI/Hud/TPSMenuHUD.h"
#include "controllers/TPSMenuPlayerController.h"

ATPSMenuGameModeBase::ATPSMenuGameModeBase()
{
	PlayerControllerClass = ATPSMenuPlayerController::StaticClass();
	HUDClass = ATPSMenuHUD::StaticClass();
	
}
