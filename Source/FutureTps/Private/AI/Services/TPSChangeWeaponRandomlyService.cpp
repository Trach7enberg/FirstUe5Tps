// FutureTPS Game All Rights Reserved


#include "AI/Services/TPSChangeWeaponRandomlyService.h"

#include "AIController.h"
#include "Components/TPSWeaponLogicComponent.h"
#include "TPSUtil/TPSUtils.h"

DEFINE_LOG_CATEGORY_STATIC(MyUTPSChangeWeaponRandomlyServiceLog, All, All);

UTPSChangeWeaponRandomlyService::UTPSChangeWeaponRandomlyService()
{
	NodeName = "RandomChangeWeapon";
	Interval = 3.0f;
}

void UTPSChangeWeaponRandomlyService::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();
	if (!Controller) { return; }

	const auto AIWeaponComp = FTPSUtils::GetComponentByCurrentPlayer<UTPSWeaponLogicComponent>(
		Controller->GetPawn());
	if (!AIWeaponComp) { return; }


	if (Probability > 0 && FMath::FRand() <= Probability)
	{
		UE_LOG(MyUTPSChangeWeaponRandomlyServiceLog, Display, TEXT("AiChangeWeaponRandomly"));
		if (!AIWeaponComp->IsReloading()) { AIWeaponComp->SwitchWeapon(); }
	}


	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
