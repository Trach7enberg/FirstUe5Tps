// FutureTPS Game All Rights Reserved


#include "AI/Services/TPSFireService.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/TPSWeaponLogicComponent.h"
#include "TPSUtil/TPSUtils.h"

DEFINE_LOG_CATEGORY_STATIC(MyUTPSFireServiceLog, All, All);

UTPSFireService::UTPSFireService() { NodeName = "Fire"; }

void UTPSFireService::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
	const auto BlackBoard = OwnerComp.GetBlackboardComponent();
	auto WeaponLogicComp = FTPSUtils::GetComponentByCurrentPlayer<UTPSWeaponLogicComponent>(
		OwnerComp.GetAIOwner()->GetPawn());

	if (!BlackBoard || !WeaponLogicComp)
	{
		UE_LOG(MyUTPSFireServiceLog, Error, TEXT("Nullptr"));
		return;
	}

	// 如果通过黑板键选择器获得到的值不为null
	if (const auto HasTarget = BlackBoard->GetValueAsObject(EnemyActorKey.SelectedKeyName))
	{
		WeaponLogicComp->Fire(); 
	}
	else { WeaponLogicComp->StopFire(); }


	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
