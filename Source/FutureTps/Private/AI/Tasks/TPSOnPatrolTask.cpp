// FutureTPS Game All Rights Reserved


#include "AI/Tasks/TPSOnPatrolTask.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

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

	const bool bRandomReachablePointInRadius = NavSys->GetRandomReachablePointInRadius(
		AIPawn->GetActorLocation(), Radius, FoundedNavLocation);
	// 以Radius为半径,AI位置为中心,返回一个可以在导航系统(区内)到达的点

	// 找不到可以走的点,返回Failed
	if (!bRandomReachablePointInRadius) { return EBTNodeResult::Failed; }
	BlackBoard->SetValueAsVector(LocationKey.SelectedKeyName, FoundedNavLocation.Location);

	return EBTNodeResult::Succeeded;
}
