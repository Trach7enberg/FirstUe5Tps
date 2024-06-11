// FutureTPS Game All Rights Reserved


#include "Animations/AnimNotifys/TPSAnimNotify.h"

void UTPSAnimNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation,
                            const FAnimNotifyEventReference &EventReference)
{
	OnNotified.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation, EventReference);
}
