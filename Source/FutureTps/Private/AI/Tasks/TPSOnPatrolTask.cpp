// FutureTPS Game All Rights Reserved


#include "AI/Tasks/TPSOnPatrolTask.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

DEFINE_LOG_CATEGORY_STATIC(MyUTPSOnPatrolTaskLog, All, All);

UTPSOnPatrolTask::UTPSOnPatrolTask()
{
	// 显示的结点名字
	NodeName = "GetNextLocation";
}

EBTNodeResult::Type UTPSOnPatrolTask::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
	// 通过当前行为树组件获取AI的控制器
	const auto AiController = OwnerComp.GetAIOwner();

	// 获取当前行为树里的黑板组件
	const auto BlackBoard = OwnerComp.GetBlackboardComponent();

	if (!AiController || !BlackBoard) { return EBTNodeResult::Failed; }

	const auto AIPawn = AiController->GetPawn();
	if (!AIPawn) { return EBTNodeResult::Failed; }

	// 获取当前导航系统的指针
	const auto NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys) { return EBTNodeResult::Failed; }

	// 保存 导航系统成功找到可走的点的数据
	FNavLocation FoundedNavLocation;

	FVector Center =
		AIPawn->GetActorLocation();

	if (!SelfCenter)
	{
		// 通过黑板获取Object
		auto Actor = Cast<AActor>(BlackBoard->GetValueAsObject(EnemyActorKey.SelectedKeyName));
		if (!Actor) { return EBTNodeResult::Failed; }
		
		Center = Actor->GetActorLocation();
	}

	const bool bRandomReachablePointInRadius = NavSys->GetRandomReachablePointInRadius(
		Center, Radius, FoundedNavLocation);
	// 以Radius为半径,AI位置为中心(SelfCenter为ture时),返回一个可以在导航系统(区内)到达的点

	// 找不到可以走的点,返回Failed
	if (!bRandomReachablePointInRadius) { return EBTNodeResult::Failed; }
	BlackBoard->SetValueAsVector(LocationKey.SelectedKeyName, FoundedNavLocation.Location);

	return EBTNodeResult::Succeeded;
}
