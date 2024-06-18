// FutureTPS Game All Rights Reserved


#include "TPSEnemyEnvQueryContext.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UTPSEnemyEnvQueryContext::ProvideContext(FEnvQueryInstance &QueryInstance, FEnvQueryContextData &ContextData) const
{
	const auto QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());

	// 通过UAIBlueprintHelperLibrary这个函数库
	// 以当前我们这个EQS的拥有者这个Actor为参数,查找这个Actor身上的行为树组件,然后找到黑板
	const auto BlackBoard = UAIBlueprintHelperLibrary::GetBlackboard(QueryOwner);

	// 将当前EQS查询的对象设置为我们黑板上已经设置好了的actor
	if (!BlackBoard) { return; }
	const auto ContextActor = Cast<AActor>(BlackBoard->GetValueAsObject(EnemyActorKeyName));
	if (!ContextActor) { return; }


	UEnvQueryItemType_Actor::SetContextHelper(ContextData, ContextActor);
}
