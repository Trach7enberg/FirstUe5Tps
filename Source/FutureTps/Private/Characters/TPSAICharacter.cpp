// FutureTPS Game All Rights Reserved


#include "Characters/TPSAICharacter.h"

ATPSAICharacter::ATPSAICharacter(const FObjectInitializer &ObjectInitializer):
	Super(ObjectInitializer)
{
	// 生成pawn时,就会被AI控制器控制
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ATPSAICharacter::StaticClass();
}
