// FutureTPS Game All Rights Reserved


#include "controllers/TPSPlayerController.h"

#include "Components/TPSRespawnComponent.h"

ATPSPlayerController::ATPSPlayerController()
{
	RespawnComponent = CreateDefaultSubobject<UTPSRespawnComponent>("RespawnComponent");
}
 