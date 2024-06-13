// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/TPSBaseCharacter.h"
#include "TPSAICharacter.generated.h"

/**
 * AI角色类,继承自玩家的基础角色类
 */
UCLASS()
class FUTURETPS_API ATPSAICharacter : public ATPSBaseCharacter
{
	GENERATED_BODY()

public:
	// 调用父类构造器,(父类构造器通过这种构造器重写了角色移动组件)
	ATPSAICharacter(const FObjectInitializer &ObjectInitializer);
};
