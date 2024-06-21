// FutureTPS Game All Rights Reserved


#include "controllers/TPSPlayerController.h"

#include "Components/TPSRespawnComponent.h"
#include "GameFramework/GameModeBase.h"
#include "GameMode/TPSGameModeBase.h"

ATPSPlayerController::ATPSPlayerController()
{
	RespawnComponent = CreateDefaultSubobject<UTPSRespawnComponent>("RespawnComponent");
}

void ATPSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!InputComponent) { return; }

	InputComponent->BindAction("GamePause", IE_Pressed, this, &ATPSPlayerController::PauseGame);

}


void ATPSPlayerController::PauseGame()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) { return; }

	GetWorld()->GetAuthGameMode()->SetPause(this);
}

void ATPSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld())
	{
		const auto GameMode = Cast<ATPSGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode) { GameMode->OnMatchStateChanged.AddUObject(this, &ATPSPlayerController::OnMatchStateChanged); }
	}
}

void ATPSPlayerController::OnMatchStateChanged(ETPSMatchState MatchState)
{
	if (MatchState == ETPSMatchState::InProgress)
	{
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
	}
}
