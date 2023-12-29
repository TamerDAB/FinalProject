//* LaraAnimInstance Class *//

#include "Characters/LaraAnimInstance.h"
#include "Characters/LaraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


/* This function readies animations by connecting to thecharacter
 owner and its movement,enabling interaction during playback. */
void ULaraAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	LaraCharacter = Cast<ALaraCharacter>(TryGetPawnOwner());
	if (LaraCharacter)
	{
		LaraCharacterMovement = LaraCharacter->GetCharacterMovement();
	}
}


/* This function updates animations using the character
 movement, state, and actions for a responsive animation experience. */
void ULaraAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (LaraCharacterMovement)
	{
	  GroundSpeed =  UKismetMathLibrary::VSizeXY(LaraCharacterMovement->Velocity);
	  IsFalling = LaraCharacterMovement->IsFalling();
	  CharacterState = LaraCharacter->GetCharacterState();
	  ActionState = LaraCharacter->GetActionState();
	  DeathPose = LaraCharacter->GetDeathPose();
	}
}
