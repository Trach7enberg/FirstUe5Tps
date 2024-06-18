// FutureTPS Game All Rights Reserved


#include "AI/Decorators/TPSIsAmmoEmptyBTDecorator.h"

#include "AIController.h"
#include "Components/TPSWeaponLogicComponent.h"
#include "TPSUtil/TPSUtils.h"

DEFINE_LOG_CATEGORY_STATIC(MyUTPSIsAmmoEmptyBTDecoratorLog, All, All);

UTPSIsAmmoEmptyBTDecorator::UTPSIsAmmoEmptyBTDecorator() { NodeName = "IsAmmoEmpty"; }

bool UTPSIsAmmoEmptyBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
	const auto AiController = OwnerComp.GetAIOwner();

	if (!AiController) { return false; }

	const auto WeaponComp = FTPSUtils::GetComponentByCurrentPlayer<UTPSWeaponLogicComponent>(AiController->GetPawn());
	if (!WeaponComp) { return false; }

	// UE_LOG(MyUTPSIsAmmoEmptyBTDecoratorLog, Error, TEXT("AmmoLeftPercent: %f"),
	//        WeaponComp->GetCurrentWeaponAmmoLeftPercent());

	return WeaponComp->GetCurrentWeaponAmmoLeftPercent() <= AmmoLeftPercent;
}
