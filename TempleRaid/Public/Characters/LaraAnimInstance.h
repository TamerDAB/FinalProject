//* LaraAnimInstance Class *//

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LaraCharacter.h"
#include "CharacterTypes.h"
#include "LaraAnimInstance.generated.h"


UCLASS()
class TEMPLERAIDV1_2_0_API ULaraAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	class ALaraCharacter* LaraCharacter;


	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* LaraCharacterMovement;
	

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = Movement )
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly, Category = Movement )
	EActionState ActionState;

	UPROPERTY(BlueprintReadOnly, Category = Movement )
	TEnumAsByte<EDeathPose> DeathPose;

};
