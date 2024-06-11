

#include "GameMode/TPSGameModeBase.h"

#include "Characters/TPSBaseCharacter.h"
#include "controllers/TPSPlayerController.h"

ATPSGameModeBase::ATPSGameModeBase()
{
	DefaultPawnClass = ATPSBaseCharacter::StaticClass();
	PlayerControllerClass = ATPSPlayerController::StaticClass();
	
}
