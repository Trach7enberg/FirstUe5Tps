// FutureTPS Game All Rights Reserved


#include "controllers/TPSAIController.h"

#include "AI/TPSAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/TPSAICharacter.h"
#include "Components/TPSRespawnComponent.h"

DEFINE_LOG_CATEGORY_STATIC(MyATPSAIControllerLog, All, All);

ATPSAIController::ATPSAIController()
{
	TPSAIPerceptionComponent = CreateDefaultSubobject<UTPSAIPerceptionComponent>("AIPerceptionComponent");

	RespawnComponent = CreateDefaultSubobject<UTPSRespawnComponent>("RespawnComponent");

	// AI控制器生成的AI角色类也会有PlayerState
	bWantsPlayerState = true;

	// AI控制器中本身就有AI感知组件,所以我们得覆盖它
	// 参数要求是引用,我们的组件是指针,所以通过解引用符号(*)即可
	SetPerceptionComponent(*TPSAIPerceptionComponent);
	
}


void ATPSAIController::BeginPlay()
{
	Super::BeginPlay();
	check(TPSAIPerceptionComponent);
}

void ATPSAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	SetFocus(GetCurrentFocusActor());
}

void ATPSAIController::OnPossess(APawn *InPawn)
{
	Super::OnPossess(InPawn);
	ATPSAICharacter *AICharacter = Cast<ATPSAICharacter>(InPawn);
	if (!AICharacter) { return; }

	RunBehaviorTree(AICharacter->BehaviorTreeAsset);
}

AActor *ATPSAIController::GetCurrentFocusActor()
{
	if (!GetBlackboardComponent())
	{
		// UE_LOG(MyATPSAIControllerLog, Error, TEXT("GetBlackboardComponent->Nullptr"));
		return nullptr;
	}
	// 通过FocusOnEnemyKeyName获取黑板中对应的值,这里的值已通AIService动态更新
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnEnemyKeyName));
}
