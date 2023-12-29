#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Boss.generated.h"


class UHealthBarComponent;
class UPawnSensingComponent;

UCLASS()
class TEMPLERAIDV1_2_0_API ABoss : public ABaseCharacter
{
	GENERATED_BODY()

public:
	
	ABoss();
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


	UPROPERTY(VisibleAnyWhere)
		UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnyWhere)
		UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ASword> WeaponClass;



	UPROPERTY(EditAnywhere)
		double AttackRadius = 150.f;

	UPROPERTY(EditAnywhere)
		double PatrolRadius = 200.f;

	UPROPERTY()
		class AAIController* QuardController;

	UPROPERTY(EditAnywhere)
		double CombatRadius = 1500.f;

	//current patrol target
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		AActor* PatrolTarget;

	//Array of patrol target
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		TArray<AActor*> PatrolTargets;


	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
		float PatrollingSpeed = 75.f;

	UPROPERTY(EditAnywhere, Category = Combat)
		float ChasingSpeed = 300.f;

	FTimerHandle AttackTimer;
	UPROPERTY(EditAnywhere, Category = Combat)
		float AttackMin = 0.2f;
	UPROPERTY(EditAnywhere, Category = Combat)
		float AttackMax = 0.5f;

	UPROPERTY(EditAnywhere, Category = Combat)
		float DeathLifeSpan = 60.f;

	UPROPERTY(EditAnywhere, Category = Combat)
		TSubclassOf<class ASoul> SoulClass;
};
