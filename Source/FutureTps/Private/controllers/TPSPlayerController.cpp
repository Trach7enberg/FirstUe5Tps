// FutureTPS Game All Rights Reserved


#include "controllers/TPSPlayerController.h"

#include "Components/TPSRespawnComponent.h"
#include "GameFramework/GameModeBase.h"
#include "GameInstances/TPSGameInstance.h"
#include "GameMode/TPSGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(MyATPSPlayerControllerLog, All, All);

ATPSPlayerController::ATPSPlayerController()
{
	RespawnComponent = CreateDefaultSubobject<UTPSRespawnComponent>("RespawnComponent");
}

void ATPSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!InputComponent) { return; }

	InputComponent->BindAction("GamePause", IE_Pressed, this, &ATPSPlayerController::PauseGame);
	InputComponent->BindAction("MuteSound", IE_Pressed, this, &ATPSPlayerController::OnMuteSound);

	
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

void ATPSPlayerController::OnMuteSound()
{
	if (!GetWorld()) { return; }

	const auto GameInstance = GetWorld()->GetGameInstance<UTPSGameInstance>();
	if (!GameInstance) { return; }

	// UE_LOG(MyATPSPlayerControllerLog, Error, TEXT("Mute sound"));
	GameInstance->ToggleVolume();
}


