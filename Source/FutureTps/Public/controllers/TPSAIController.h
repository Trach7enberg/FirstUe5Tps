// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TPSAIController.generated.h"

class UTPSAIPerceptionComponent;
/**
 *  AI的控制器
 */
UCLASS()
class FUTURETPS_API ATPSAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATPSAIController();

	virtual void Tick(float DeltaTime) override;

protected:
	/// 当AI控制器控制某个Pawn时
	/// @param InPawn 被控制的Pawn
	virtual void OnPossess(APawn *InPawn) override;
	virtual void BeginPlay() override;
	
	// AI感知组件
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components)
	UTPSAIPerceptionComponent *TPSAIPerceptionComponent;

	// 黑板上某个键的名字,通过这个名字获取对应的黑板值
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=AI)
	FName FocusOnEnemyKeyName = "EnemyActor";

private:
	/// 获取当前Ai正在看的Actor
	/// @return 被当前Ai看的Actor
	AActor* GetCurrentFocusActor();

};
