// FutureTPS Game All Rights Reserved


#include "Characters/TPSAICharacter.h"

#include "BrainComponent.h"
#include "Components/TPSAIWeaponLogicComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "controllers/TPSAIController.h"

DEFINE_LOG_CATEGORY_STATIC(MyATPSAICharacterLog,All,All);

ATPSAICharacter::ATPSAICharacter(const FObjectInitializer &ObjectInitializer):
	Super(ObjectInitializer.SetDefaultSubobjectClass<UTPSAIWeaponLogicComponent>("WeaponLogicComponent"))
{
	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = ATPSAICharacter::StaticClass(); 
	
	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		// 设置AI转向的丝滑度
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		// 注意是YAW!! Z 对应 YAW
		GetCharacterMovement()->RotationRate = FRotator(0, 200.0f, 0);
	}

}

void ATPSAICharacter::OnDeath()
{
	Super::OnDeath();

	const auto TPSAiController = Cast<AAIController>(GetController());
	if (TPSAiController && TPSAiController->BrainComponent)
	{
		// BrainComponent是 底层的行为树组件,行为树组件通过它来Cast的,所以我们调用Cleanup函数
		// 当AI死亡时关闭行为树
		TPSAiController->BrainComponent->Cleanup();
		// UE_LOG(MyATPSAICharacterLog,Error,TEXT("CleanUpBehaviorTree"));
	}

}
