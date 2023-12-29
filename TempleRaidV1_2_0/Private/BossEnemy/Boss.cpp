
#include "BossEnemy/Boss.h"
#include "AIController.h"
#include "Items/Weapons/Sword.h"
#include "HUD/HealthBarComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AttributeComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Soul.h"


ABoss::ABoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);



	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;


	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(80.f);

}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead()) return;

	if (QuardState > EQuardState::EQS_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}

}

float ABoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamge(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();

	if (IsInsideAttackRadius())
	{
		QuardState = EQuardState::EQS_Attacking;
	}
	else if (IsOutsideAttackRadius())
	{
		//when we hit the Guard he will attack again.
		ChaseTarget();
	}

	return DamageAmount;
}

void ABoss::Destroyed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}

}

void ABoss::GetHit_Implementation(const FVector& ImpactPoint)
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
void ABoss::BeginPlay()
{
	Super::BeginPlay();

	if (PawnSensing)
	{
		//bind callback , this->user object, Adress of enemy->&AGuards::PawnSeen.
		PawnSensing->OnSeePawn.AddDynamic(this, &ABoss::PawnSeen);
	}

	InitializeEnemy();
	Tags.Add(FName("Enemy"));

}

void ABoss::Die()
{
	Super::Die();

	QuardState = EQuardState::EQS_Dead;

	//Callplay play death montage
	ClearAttackTimer();
	HideHealthBar();
	DisableCapsule();
	SetLifeSpan(DeathLifeSpan);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	//Disable wepon collision when Guard die.
	SetSwordCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawnSoul();

}

void ABoss::SpawnSoul()
{
	UWorld* World = GetWorld();
	if (World && SoulClass)
	{

		ASoul* SpawnedSoul = World->SpawnActor<ASoul>(SoulClass, GetActorLocation(), GetActorRotation());
		if (SpawnedSoul)
		{
			SpawnedSoul->SetSouls(Attributes->GetSouls());
		}

	}
}

void ABoss::Attack()
{
	Super::Attack();
	if (CombatTarget == nullptr) return;

	QuardState = EQuardState::EQS_Engaged;
	PlayAttackMontage();
}

bool ABoss::CanAttack()
{
	bool bCanAttack =
		IsInsideAttackRadius() &&
		!IsAttacking() &&
		!IsEngaged() &&
		!IsDead();
	return bCanAttack;
}

void ABoss::AttackEnd()
{
	QuardState = EQuardState::EQS_NoState;
	CheckCombatTarget();
}

void ABoss::HandleDamge(float DamageAmount)
{
	Super::HandleDamge(DamageAmount);

	if (Attributes && HealthBarWidget)
	{
		HealthBarWidget->SetHealthPrecent(Attributes->GetHealthPercent());
	}
}



//Initialize Enemy
void ABoss::InitializeEnemy()
{
	QuardController = Cast<AAIController>(GetController());
	MoveToTarget(PatrolTarget);
	HideHealthBar();
	SpawnDefaultWeapon();
}

void ABoss::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &ABoss::PatrolTimerFinished, 2.f);
	}
}

void ABoss::CheckCombatTarget()
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

void ABoss::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

void ABoss::HideHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}

}

void ABoss::ShowHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}

void ABoss::LoseInterest()
{
	CombatTarget = nullptr;
	HideHealthBar();
}

void ABoss::StartPatrolling()
{
	QuardState = EQuardState::EQS_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = 75.f;
	MoveToTarget(PatrolTarget);
}

void ABoss::ChaseTarget()
{
	QuardState = EQuardState::EQS_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}

bool ABoss::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}

bool ABoss::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}

bool ABoss::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}

bool ABoss::IsChasing()
{
	return QuardState == EQuardState::EQS_Chasing;
}

bool ABoss::IsAttacking()
{
	return QuardState == EQuardState::EQS_Attacking;
}

bool ABoss::IsDead()
{
	return QuardState == EQuardState::EQS_Dead;
}

bool ABoss::IsEngaged()
{
	return QuardState == EQuardState::EQS_Engaged;
}

void ABoss::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

void ABoss::StartAttackTimer()
{
	QuardState = EQuardState::EQS_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &ABoss::Attack, AttackTime);
}

void ABoss::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

bool ABoss::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr)return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();

	return DistanceToTarget <= Radius;
}

void ABoss::MoveToTarget(AActor* Target)
{
	if (QuardController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(40.f);
	QuardController->MoveTo(MoveRequest);

}

AActor* ABoss::ChoosePatrolTarget()
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

void ABoss::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		ASword* DefultWeapon = World->SpawnActor<ASword>(WeaponClass);
		DefultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		EquippedWeapon = DefultWeapon;
	}
}

void ABoss::PawnSeen(APawn* SeenPawn)
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
		//we now then the Player has seen ,so we start Chasing.
		//set the speed to 300. (gurad speed when seeing LaraCharacter).
		//Enemy run after Lara.
		ChaseTarget();
	}

}
