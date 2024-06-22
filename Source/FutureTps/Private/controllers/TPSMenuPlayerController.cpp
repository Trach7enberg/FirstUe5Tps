// FutureTPS Game All Rights Reserved


#include "controllers/TPSMenuPlayerController.h"

void ATPSMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}
