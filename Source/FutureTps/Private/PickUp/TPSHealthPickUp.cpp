// FutureTPS Game All Rights Reserved


#include "PickUp/TPSHealthPickUp.h"

#include "Components/TPSHealthComponent.h"
#include "TPSUtil/TPSUtils.h"

bool ATPSHealthPickUp::CanPickup(AActor *Actor)
{
	const UTPSHealthComponent *HealthComponent = FTPSUtils::GetComponentByCurrentPlayer<
		UTPSHealthComponent>(Actor);
	if (!HealthComponent) { return false; }

	return !HealthComponent->IsFullHealth();
}

void ATPSHealthPickUp::Pickup(AActor *Actor)
{
	Super::Pickup(Actor);
	UTPSHealthComponent *HealthComponent = FTPSUtils::GetComponentByCurrentPlayer<
		UTPSHealthComponent>(Actor);
	if (!HealthComponent) { return; }
	HealthComponent->Heal(ResupplyPercent);
}
