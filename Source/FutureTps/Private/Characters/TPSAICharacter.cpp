// FutureTPS Game All Rights Reserved


#include "Characters/TPSAICharacter.h"

#include "Components/TPSAIWeaponLogicComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ATPSAICharacter::ATPSAICharacter(const FObjectInitializer &ObjectInitializer):
	Super(ObjectInitializer.SetDefaultSubobjectClass<UTPSAIWeaponLogicComponent>("WeaponLogicComponent"))
{
	// 生成pawn时,就会被AI控制器控制
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ATPSAICharacter::StaticClass();

	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		// 设置AI转向的丝滑度
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0, 0, 200.0f);
	}
}
