//* Enemy Class *//
#include "GuardsEnemy/Guards.h"
#include "AIController.h"
#include "Items/Weapons/Sword.h"
#include "HUD/HealthBarComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AttributeComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Soul.h"


AGuards::AGuards()
{
 	//  Enemy  call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;


	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	
	// create and attach a UHealthBarComponent to the root component of the object.
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	/* create and configure a UPawnSensingComponent, used to give AI-controlled enemies
	 the ability to sense nearby pawns (Lara character) and react to them. */
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(80.f);

}


// Called every frame
void AGuards::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// if enemy is dead exit
	if (IsDead()) return;

	 
	if (QuardState > EQuardState::EQS_Patrolling)
	{
		//combat target is lara
		CheckCombatTarget();
	}
	else
	{
		//continue patrolling
		CheckPatrolTarget();
	}

}


/* Handle damge for enemy, updating healthbar, 
 setting the combat target, and adjusting the state based on attack radius,
 preparing for another attack if necessary. */
float AGuards::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamge(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();

	if (IsInsideAttackRadius())
	{
		QuardState = EQuardState::EQS_Attacking;
	}
	else if(IsOutsideAttackRadius())
	{
		//when we hit the enemy he will attack again.
	    ChaseTarget();
	}

	return DamageAmount;
}



// destroy Weapon mesh.
void AGuards::Destroyed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}

}


/* This function manages the impact on the guard, 
 displaying the health bar if alive, halting patrols and attacks,
 disabling sword collision, and stopping attack montages. */
void AGuards::GetHit_Implementation(const FVector& ImpactPoint)
{
	Super::GetHit_Implementation(ImpactPoint);
	if (!IsDead()) ShowHealthBar();
	ClearPatrolTimer();
	ClearAttackTimer();
	//set weapon collision to no-collision.
	SetSwordCollisionEnabled(ECollisionEnabled::NoCollision);
	//stop attack montage when enemy get hit.
	StopAttackMontage();

}


// Called when the game starts or when spawned
void AGuards::BeginPlay()
{
	Super::BeginPlay();

	if (PawnSensing)
	{
		//bind callback , this->user object, Adress of enemy->&AGuards::PawnSeen.
		PawnSensing->OnSeePawn.AddDynamic(this, &AGuards::PawnSeen);
	}

	InitializeEnemy();
	Tags.Add(FName("Enemy"));

}


// play death animation.change state to dead.
void AGuards::Die()
{
	
	Super::Die();

	QuardState = EQuardState::EQS_Dead;

	ClearAttackTimer();
	HideHealthBar();
	DisableCapsule();
	//enemy disapear from world after die.
	SetLifeSpan(DeathLifeSpan);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	//Disable wepon collision when Guard die.
	SetSwordCollisionEnabled(ECollisionEnabled::NoCollision);
	// spawn soul
	SpawnSoul();

}


// spawn soul , (For use when enemy die).
void AGuards::SpawnSoul()
{
	UWorld* World = GetWorld();
	if (World && SoulClass)
	{
		
		ASoul* SpawnedSoul =  World->SpawnActor<ASoul>(SoulClass, GetActorLocation(), GetActorRotation());
		if (SpawnedSoul)
		{
          SpawnedSoul->SetSouls(Attributes->GetSouls());
		}
		
	}
}


// start attack animation to enemy and change state to Engaged.
void AGuards::Attack()
{
	Super::Attack();
	//if (CombatTarget == nullptr) return;

	QuardState = EQuardState::EQS_Engaged;
	PlayAttackMontage();
}


// returns true if enemy can attack otherwise false.
bool AGuards::CanAttack()
{
	bool bCanAttack =
		IsInsideAttackRadius() &&
		!IsAttacking() &&
		!IsEngaged() &&
		!IsDead();
	return bCanAttack;
}


// Attack end for enemy, convert his state to no state.and check the combat target.
void AGuards::AttackEnd()
{
	QuardState = EQuardState::EQS_NoState;
	CheckCombatTarget();
}


// updates the enemy healthbar after taking damage.
void AGuards::HandleDamge(float DamageAmount)
{
	Super::HandleDamge(DamageAmount);

	if (Attributes && HealthBarWidget)
	{
		HealthBarWidget->SetHealthPrecent(Attributes->GetHealthPercent());
	}
}



//Initialize Enemy
void AGuards::InitializeEnemy()
{
	QuardController = Cast<AAIController>(GetController());
	MoveToTarget(PatrolTarget);
	HideHealthBar();
	SpawnDefaultWeapon();
}



/* The function checks if the enemy is within the patrol radius of its current target.
 If yes, it chooses a new target,starts the patrol timer, and
 handling patrol behavior after a 2 second. */
void AGuards::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AGuards::PatrolTimerFinished, 2.f);
	}
}


/* This function manages the enemy actions based on its position relative to combat and attack ranges. 
It determines whether the enemy should lose interest, patrol, chase the target, or attack. */
void AGuards::CheckCombatTarget()
{

	if (IsOutsideCombatRadius())
	{
		ClearAttackTimer();
		//ouitside combat radius, lose intrest.
		LoseInterest();
		if (!IsEngaged())
		{
			StartPatrolling();
		}


	}
	else if (IsOutsideAttackRadius() && !IsChasing())
	{
		ClearAttackTimer();
		if (!IsEngaged())
		{
			//outside attack range, chase character.
			ChaseTarget();
		}

	}
	else if (CanAttack())
	{
		// inside attack range , attack character
		//Attack montage play.
		StartAttackTimer();

	}


}


// when timmer finished enemy will move to target
void AGuards::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}


// Hide enemy healthbar
void AGuards::HideHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}

}


// Show healthbar
void AGuards::ShowHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}


// enemy lose interest , convert combat target to null and hide his healthbar.
void AGuards::LoseInterest()
{
	//CombatTarget = nullptr;
	HideHealthBar();
}


/* enemy start patrolling, set his action state to 
 patrolling change walk speed to 75 and attach to the target.*/
void AGuards::StartPatrolling()
{
	QuardState = EQuardState::EQS_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = 75.f;
	MoveToTarget(PatrolTarget);
}


/* enemy start chasing(chasing after lara character), set max walk speed to chasingSpeed variable
  can be changed in enemy blueprint class in the editor, and attach to target which is lara.*/
void AGuards::ChaseTarget()
{
	QuardState = EQuardState::EQS_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}


// return true if the enemy is not within the combat radius of its combat target.
bool AGuards::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}


// return true when the enemy is not within the attack radius of its combat target.
bool AGuards::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}


// return true if the enemy is within the attack radius of its combat target.
bool AGuards::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}


// return true if the enemy is in chasing state
bool AGuards::IsChasing()
{
	return QuardState == EQuardState::EQS_Chasing;
}


// return true if the enemy is in attacking state
bool AGuards::IsAttacking()
{
	return QuardState == EQuardState::EQS_Attacking;
}


// return true if the enemy is in dead state
bool AGuards::IsDead()
{
	return QuardState == EQuardState::EQS_Dead;
}


// return true if the enemy is in Engaged state
bool AGuards::IsEngaged()
{
	return QuardState == EQuardState::EQS_Engaged;
}


// stop and clear the patrol timer
void AGuards::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}


// initializes the attack timer, triggering the Attack function after a randomized time within the specified range.
void AGuards::StartAttackTimer()
{
	QuardState = EQuardState::EQS_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AGuards::Attack, AttackTime);
}


// stop and clear the attack timer
void AGuards::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}


/* calculates whether the enemy is within the specified radius of the target. 
 It returns true if the distance is less than or equal to the radius, otherwise it returns false.*/
bool AGuards::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr)return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();

	return DistanceToTarget <= Radius;
}


// Move the enemy towards the specified target using AI-controlled movement.
void AGuards::MoveToTarget(AActor* Target)
{
	if (QuardController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(40.f);
	QuardController->MoveTo(MoveRequest);
	
}


/* The function selects a new patrol target from a targets array,
 excluding the current one to avoid immediate revisiting.*/
AActor* AGuards::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (auto Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}

	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
	}
	return nullptr;
}


// equip weapon to enemy and attach it to RightHandSocket.
void AGuards::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		ASword* DefultWeapon = World->SpawnActor<ASword>(WeaponClass);
		DefultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		EquippedWeapon = DefultWeapon;
	}
}


/* When a pawn is detected, the function determines if the
 enemy should chase based on its state and the type of pawn("LaraCharacter"). */
void AGuards::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChaseTarget =
		QuardState != EQuardState::EQS_Dead &&
		QuardState != EQuardState::EQS_Chasing &&
		QuardState < EQuardState::EQS_Attacking &&
		SeenPawn->ActorHasTag(FName("LaraCharacter"));


	if (bShouldChaseTarget)
	{
		CombatTarget = SeenPawn;
		//we have timmer(for stopping bettwen target to target) running so we have to clear it.
		ClearPatrolTimer();

		//we know then the Player has seen ,so we start Chasing.
		//set the speed to 300. (gurad speed when seeing LaraCharacter).
		//Enemy run after Lara.
		ChaseTarget();
	}

}




