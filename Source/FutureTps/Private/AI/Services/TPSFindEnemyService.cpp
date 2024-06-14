// FutureTPS Game All Rights Reserved


#include "AI/Services/TPSFindEnemyService.h"
#include "AIController.h"
#include "AI/TPSAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TPSUtil/TPSUtils.h"

DEFINE_LOG_CATEGORY_STATIC(MyUTPSFindEnemyService, All, All)

UTPSFindEnemyService::UTPSFindEnemyService() { NodeName = "FindEnemy"; }

void UTPSFindEnemyService::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{


	const auto BlackBoard = OwnerComp.GetBlackboardComponent();

	if (BlackBoard)
	{
		const auto Controller = OwnerComp.GetAIOwner();
		const auto PerceptionComp = FTPSUtils::GetComponentByCurrentPlayer<UTPSAIPerceptionComponent>(Controller);

		if (PerceptionComp)
		{
			//动态更新黑板上EnemyActorKeySelector的值,这个值可能为Null
			BlackBoard->SetValueAsObject(EnemyActorKeySelector.SelectedKeyName, PerceptionComp->GetClosestEnemy());
		}
	}


	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
