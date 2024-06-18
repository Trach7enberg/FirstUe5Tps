// FutureTPS Game All Rights Reserved


#include "AI/TPSAIPerceptionComponent.h"

#include "AIController.h"
#include "Components/TPSHealthComponent.h"
#include "Perception/AISense_Sight.h"
#include "TPSUtil/TPSUtils.h"

DEFINE_LOG_CATEGORY_STATIC(MyUTPSAIPerceptionComponentLog, All, All)

AActor *UTPSAIPerceptionComponent::GetClosestEnemy() const
{
	TArray<AActor *> PerceiveActors;

	// 根据视力配置,获得当前已感知到的所有Actors
	// 注意是UAISense_Sight::StaticClass(),不是UAISense::StaticClass()
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceiveActors);

	const auto AiController = Cast<AAIController>(GetOwner());

	if (!AiController) { return nullptr; }

	const auto AiPawn = AiController->GetPawn();
	if (!AiPawn) { return nullptr; }


	float MinDistance = MAX_FLT;
	AActor *ClosestActor = nullptr;

	// 找最小值算法
	for (const auto PerceiveActor : PerceiveActors)
	{
		const auto HealthComp = FTPSUtils::GetComponentByCurrentPlayer<UTPSHealthComponent>(PerceiveActor);
		// PerceiveActor->FindComponentByClass<UTPSHealthComponent>();

		// 判断当前AI和感知到的角色是否是敌人
		const auto IsEnemy = FTPSUtils::AreEnemies(AiController, Cast<APawn>(PerceiveActor)->GetController());

		// 如果当前感知到的角色是活着的敌人
		if (HealthComp && !HealthComp->IsDead() && IsEnemy)
		{
			// 获取当前ai 到 ai感知到的角色的之间的距离
			const float AiToActorDistance = (PerceiveActor->GetActorLocation() - AiPawn->GetActorLocation()).Length();
			if (AiToActorDistance < MinDistance)
			{
				MinDistance = AiToActorDistance;
				ClosestActor = PerceiveActor;
			}
		}
	}


	return ClosestActor;
}
