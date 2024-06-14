// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TPSOnPatrolTask.generated.h"

/**
 * 行为树的任务-获得在导航系统允许的范围内的随机点 ,并将该值更新到黑板上
 */
UCLASS()
class FUTURETPS_API UTPSOnPatrolTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	explicit UTPSOnPatrolTask();

	// 行为树任务开始时会调用该函数
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;

protected:
	// 巡逻半径
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AI)
	float Radius = 1000.0f;

	// 黑板上名字为LocationKey的键值对结构
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AI)
	FBlackboardKeySelector LocationKey;

	// 是否以自己为中心,在附近生成随机巡逻点
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AI)
	bool SelfCenter = true;

	// 黑板上存储着敌人类的键值对结构
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AI,meta=(EditCondition="!SelfCenter"))
	FBlackboardKeySelector EnemyActorKey;
};
