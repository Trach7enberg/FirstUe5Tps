// FutureTPS Game All Rights Reserved


#include "AI/EQS/EnvQueryTest/EnvQueryTest_CanPickup.h"

#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "PickUp/TPSBasePickUp.h"


UEnvQueryTest_CanPickup::UEnvQueryTest_CanPickup(const FObjectInitializer &ObjectInitializer):
	Super(ObjectInitializer)
{
	// 没有什么数学运算,所以级别设置为低
	Cost = EEnvTestCost::Low;

	// Test面板上我们要测试的值的类型,这里选ActorBase
	ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
	SetWorkOnFloatValues(false);
}

void UEnvQueryTest_CanPickup::RunTest(FEnvQueryInstance &QueryInstance) const
{
	const auto DataOwner = QueryInstance.Owner.Get();
	BoolValue.BindData(DataOwner, QueryInstance.QueryID);

	// 面板上BoolMatch的值
	const auto BoolMatch = BoolValue.GetValue();


	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		// 获取EQS查询到的Actor
		const auto ItemActor = GetItemActor(QueryInstance, It.GetIndex());
		const auto PickupActor = Cast<ATPSBasePickUp>(ItemActor);
		if (!PickupActor) { continue; }

		const auto CanBeTaken = PickupActor->CanBeTaken();

		// 能被拾取,则被EQS查询到
		// 第三个参数是我们查询测试得出的结果,第四个是期望值
		It.SetScore(TestPurpose, FilterType, CanBeTaken, BoolMatch);
	}
}
