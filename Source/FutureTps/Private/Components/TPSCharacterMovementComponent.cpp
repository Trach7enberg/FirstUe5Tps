// FutureTPS Game All Rights Reserved


#include "Components/TPSCharacterMovementComponent.h"

#include "Characters/TPSBaseCharacter.h"

UTPSCharacterMovementComponent::UTPSCharacterMovementComponent()
{

}



float UTPSCharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();
	const ATPSBaseCharacter *Player = Cast<ATPSBaseCharacter>(GetPawnOwner());
	return (Player && Player->BIsRush) ? MaxSpeed * RunFactor : MaxSpeed;
}
