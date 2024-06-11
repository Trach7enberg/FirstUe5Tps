// FutureTPS Game All Rights Reserved


#include "Effects/TPSWeaponFXComponent.h"
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
		// 将特效系统生成到Hit的冲击点位置
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystemEffect, Hit.ImpactPoint,
		                                               Hit.ImpactPoint.Rotation(), FVector(1), true);
	}
}
