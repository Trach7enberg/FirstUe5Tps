// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "TPSAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotified, USkeletalMeshComponent*);

/**
 * 动画蒙太奇的通知基类
 */
UCLASS

()
class FUTURETPS_API UTPSAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation,
	                    const FAnimNotifyEventReference &EventReference) override;

	FOnNotified OnNotified;
};
