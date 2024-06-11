// FutureTPS Game All Rights Reserved


#include "PickUp/TPSAmmoPickUp.h"

#include "Components/TPSWeaponLogicComponent.h"
#include "TPSUtil/TPSUtils.h"


bool ATPSAmmoPickUp::CanPickup(AActor *Actor)
{
	const UTPSWeaponLogicComponent *WeaponLogicComponent = FTPSUtils::GetComponentByCurrentPlayer<
		UTPSWeaponLogicComponent>(Actor);
	if (!WeaponLogicComponent || WeaponLogicComponent->IsReloading()) { return false; }
	return !(WeaponLogicComponent->IsFullAmmo());
}

void ATPSAmmoPickUp::Pickup(AActor *Actor)
{
	Super::Pickup(Actor);
	UTPSWeaponLogicComponent *WeaponLogicComponent = FTPSUtils::GetComponentByCurrentPlayer<
		UTPSWeaponLogicComponent>(Actor);
	if (!WeaponLogicComponent) { return; }

	WeaponLogicComponent->Resupply(ResupplyPercent);
}
