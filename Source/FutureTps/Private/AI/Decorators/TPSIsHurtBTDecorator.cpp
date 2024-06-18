// FutureTPS Game All Rights Reserved


#include "AI/Decorators/TPSIsHurtBTDecorator.h"

#include "AIController.h"
#include "Components/TPSHealthComponent.h"
#include "TPSUtil/TPSUtils.h"

DEFINE_LOG_CATEGORY_STATIC(MyUTPSIsHurtBTDecoratorLog, All, All);

UTPSIsHurtBTDecorator::UTPSIsHurtBTDecorator() { NodeName = "IsHurt"; }

bool UTPSIsHurtBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
	const auto AiController = OwnerComp.GetAIOwner();

	if (!AiController) { return false; }

	const auto HealthComp = FTPSUtils::GetComponentByCurrentPlayer<UTPSHealthComponent>(AiController->GetPawn());
	if (!HealthComp || HealthComp->IsDead()) { return false; }

	// UE_LOG(MyUTPSIsHurtBTDecoratorLog, Error, TEXT("FindHealthPickup:	%d"),
	//        HealthComp->GetHealthPercent() <= HealthPercent);
	return HealthComp->GetHealthPercent() <= HealthPercent;
}
