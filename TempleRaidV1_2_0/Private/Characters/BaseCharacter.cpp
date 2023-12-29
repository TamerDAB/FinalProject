#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Items/Weapons/Sword.h"
#include "Components/AttributeComponent.h"
#include "Kismet/GamePlayStatics.h"
#include "TempleRaidV1_2_0/DebugMacros.h"
#include "Characters/CharacterTypes.h"


ABaseCharacter::ABaseCharacter()
{
 	// Character call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	// Create a subobject of type UAttributeComponent and give it the name 'Attributes'.
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

}


// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


/* GetHit_Implementation function check firsly if the character is alive and if it not so do Die() function,
 then continue to math directional hit react and play hit sound also spawn hit particles */ 
void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	else Die();

	PlayHitSound(ImpactPoint);

	SpawnHitParticles(ImpactPoint);
}


// Attack function check if the combat target is dead and convert it to null.
void ABaseCharacter::Attack()
{
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead")))
	{
		CombatTarget = nullptr;
	}
}


// give a dead tag and play death animation.
void ABaseCharacter::Die()
{
	Tags.Add(FName("Dead"));
	PlayDeathMontage();
}


bool ABaseCharacter::CanAttack()
{
	return false;
}


// check if is alive
bool ABaseCharacter::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}


// Disable mesh collision
void ABaseCharacter::DisableMeshCollision()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::DodgeEnd()
{
}


// PLayHitReactMontage function plays hit react animation with specific section name.
void ABaseCharacter::PLayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}


// calculation for the directional hit react.
void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();

	// lower impact point to the enemy's actor location z.
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1, so Forward * ToHit= cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);

	// take the inverse cosine (arc-cosline) of cos(theta) to get theta.
	double Theta = FMath::Acos(CosTheta);

	// convert from radians to degrees.
	Theta = FMath::RadiansToDegrees(Theta);

	//if crossproduct points down, theta should be negative.
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}


	FName Section("FromBack");

	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}

	PLayHitReactMontage(FName(Section));

}


// this function to play hit sound.
void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
}


// this function to spawn hit particles.
void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}
}


// this function update health attributes.
void ABaseCharacter::HandleDamge(float DamageAmount)
{
	if (Attributes)
	{
		Attributes->ReceiveDamge(DamageAmount);
	}
}


//play montage and jump to section
void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}


// Play random monatage Section(play random animation).
int32 ABaseCharacter::PlayRandomMonatageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	//make sure that attack montage is not empty.
	if (SectionNames.Num() <= 0)return -1;

	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);

	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}


// this function to play attack animation.
int32 ABaseCharacter::PlayAttackMontage()
{
	 
	return PlayRandomMonatageSection(AttackMontage, AttackMontageSection);

}


// this function plays death animation.
int32 ABaseCharacter::PlayDeathMontage()
{
	const int32 Selection = PlayRandomMonatageSection(DeathtMontage, DeathMontageSection);
	TEnumAsByte<EDeathPose> Pose(Selection);
	if (Pose < EDeathPose::EDP_Max)
	{
		DeathPose = Pose;
	}

	return Selection;
}


// function to play dodge animation there are one section name called deafault.
void ABaseCharacter::PlayDodgeMontage()
{
	PlayMontageSection(DodgeMontage, FName("Default"));
}


// function for stopping enemy to do attack animation.
void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0.25f, AttackMontage);
	}
}


/*  this function calculates a translation vector to move the enemy away from
the combat target (lara) by a certain distance. used to implement a mechanic where the enemy 
moves around a target during combat. */
FVector ABaseCharacter::GetTranslationWrapTarget()
{
	if(CombatTarget ==nullptr) return FVector();

	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();

	FVector TargetToMe = ( Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WrapTargetDistance;


	return	CombatTargetLocation + TargetToMe;

}


/* providing a rotation wrap target position for the character based on the combat target's position.
the enemy rotate toward or focus on the combat target(lara) position. */
FVector ABaseCharacter::GetRotationWrapTarget()
{
	if (CombatTarget)
	{
		return CombatTarget->GetActorLocation();
	}
	return FVector();
}


// Disable capsule component
void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


// Set the sword collision enabled
void ABaseCharacter::SetSwordCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetSwordBox())
	{

		EquippedWeapon->GetSwordBox()->SetCollisionEnabled(CollisionEnabled);
	    EquippedWeapon->IgnoreActors.Empty();
	}

}

