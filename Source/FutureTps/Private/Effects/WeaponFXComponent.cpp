// FutureTPS Game All Rights Reserved


#include "Effects/WeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


UWeaponFXComponent::UWeaponFXComponent() { PrimaryComponentTick.bCanEverTick = true; }


// Called when the game starts
void UWeaponFXComponent::BeginPlay() { Super::BeginPlay(); }


// Called every frame
void UWeaponFXComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWeaponFXComponent::PlayImpactFX(const FHitResult &Hit) const
{
	if (Hit.bBlockingHit)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystemEffect, Hit.ImpactPoint,
		                                               Hit.ImpactPoint.Rotation(), FVector(1), true);
	}
}
