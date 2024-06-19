// FutureTPS Game All Rights Reserved


#include "Components/TPSRespawnComponent.h"

#include "GameMode/TPSGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(MyUTPSRespawnComponentLog, All, All);

UTPSRespawnComponent::UTPSRespawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UTPSRespawnComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void UTPSRespawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTPSRespawnComponent::Respawn(float Time)
{
	if(!GetWorld()){return;}

	RespawnTimeCountDown = Time;
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle,this,&UTPSRespawnComponent::RespawnTimerUpdate,1.f,true);
}

void UTPSRespawnComponent::RespawnTimerUpdate()
{
	if(!GetWorld()){return;}
	
	if(--RespawnTimeCountDown == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

		const auto GameMode = Cast<ATPSGameModeBase>(GetWorld()->GetAuthGameMode());
		if(!GameMode){return;}

		const auto Controller = Cast<AController>(GetOwner());
		if(!Controller){return;}

		GameMode->RespawnPlayer(Controller);
	}
}

