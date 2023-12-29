//* Enemy Class *//

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Guards.generated.h"




class UHealthBarComponent;
class UPawnSensingComponent;


UCLASS()
class TEMPLERAIDV1_2_0_API AGuards : public ABaseCharacter
{
	GENERATED_BODY()

public:

	AGuards();
    // Called every frame
	virtual void Tick(float DeltaTime) override;

	/** <IHitInterface>*/
    virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	/** </IHitInterface>*/

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;
	virtual void Destroyed() override;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

	/**  <ABaseCharacter>  */
    virtual void Die() override;
	void SpawnSoul();
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void AttackEnd() override;
	virtual void HandleDamge(float DamageAmount) override;
	/**  </ABaseCharacter>  */
	


	//GuardState Default value.
	UPROPERTY(BlueprintReadOnly)
	EQuardState QuardState = EQuardState::EQS_Patrolling;

	
	

private:
   /** AI Behavior */
	void InitializeEnemy();
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished();
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	void SpawnDefaultWeapon();
	

	/*  Callback for OnPawnSeen in UPawnSensingComponent.  */
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);
	/**/

	//enemy HealthBarWidget
	UPROPERTY(VisibleAnyWhere)
		UHealthBarComponent* HealthBarWidget;

	//enemy pawn sensing 
	UPROPERTY(VisibleAnyWhere)
		UPawnSensingComponent* PawnSensing;

	//weapon class from editor
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ASword> WeaponClass;


	//Attack Radius
	UPROPERTY(EditAnywhere)
		double AttackRadius = 150.f;

	//Patrol Radius
	UPROPERTY(EditAnywhere)
		double PatrolRadius = 200.f;

	//AI Controller for the enemy
	UPROPERTY()
		class AAIController* QuardController;

	//Combat Radius
	UPROPERTY(EditAnywhere)
		double CombatRadius = 1500.f;

	//current patrol target
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		AActor* PatrolTarget;

	//Array of patrol target
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		TArray<AActor*> PatrolTargets;


	FTimerHandle PatrolTimer;

	// PatrollingSpeed for enemy
	UPROPERTY(EditAnywhere, Category = Combat)
		float PatrollingSpeed = 75.f;

	// ChasingSpeed for enemy
	UPROPERTY(EditAnywhere, Category = Combat)
		float ChasingSpeed = 300.f;

	// AttackMin for enemy
	FTimerHandle AttackTimer;
	UPROPERTY(EditAnywhere, Category = Combat)
		float AttackMin = 0.1f;

	// AttackMax for enemy
	UPROPERTY(EditAnywhere, Category = Combat)
		float AttackMax = 0.2f;
	// DeathLifeSpan time in second
	UPROPERTY(EditAnywhere, Category = Combat)
		float DeathLifeSpan = 60.f;
	
	// soul class from editor
	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<class ASoul> SoulClass;
};
