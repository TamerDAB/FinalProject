//* BaseCharacter Class *//
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Characters/CharacterTypes.h"
#include "BaseCharacter.generated.h"




class ASword;
class UAttributeComponent;
class UAnimMontage;



UCLASS()
class TEMPLERAIDV1_2_0_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



protected:
	/* Combat */
	virtual void BeginPlay() override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual void Attack();
	virtual void Die();
	void DirectionalHitReact(const FVector& ImpactPoint);
	virtual void HandleDamge(float DamageAmount);
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);
	void DisableCapsule();
	virtual bool CanAttack();
	bool IsAlive();
	void DisableMeshCollision();

	/* Montage */
	void PLayHitReactMontage(const FName& SectionName);
	//returns attack montage.
	virtual int32 PlayAttackMontage();
	//returns Death montage .
	virtual int32 PlayDeathMontage();
	virtual void PlayDodgeMontage();
	void StopAttackMontage();

	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWrapTarget();

	UFUNCTION(BlueprintCallable)
	FVector GetRotationWrapTarget();

	UFUNCTION(BlueprintCallable)
		virtual	void AttackEnd();

	UFUNCTION(BlueprintCallable)
		virtual	void DodgeEnd();

	UFUNCTION(BlueprintCallable)
		void SetSwordCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	UPROPERTY(VisibleAnywhere, Category = Weapon)
		ASword* EquippedWeapon;

	// Attribute components to manage various attributes of game objects(Health, Stamina, Golds, Souls).
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UAttributeComponent* Attributes;

	// Enemy combat target.the target is lara.
	UPROPERTY(BlueprintReadOnly, Category = Combat)
		AActor* CombatTarget;

	// Wrap target distance.
	UPROPERTY(EditAnywhere, Category = Combat)
	double WrapTargetDistance = 75.f;


	UPROPERTY(BlueprintReadOnly)
		TEnumAsByte<EDeathPose> DeathPose;

private:

	//play montage and jump to section
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);

	//this function to return random montage.
	int32 PlayRandomMonatageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	UPROPERTY(EditAnywhere, Category = Combat)
		USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = Combat)
		UParticleSystem* HitParticles;

	/**
	*Animation montages:
	**/
	UPROPERTY(EditDefaultsOnly, Category = Combat)
		UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
		UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
		UAnimMontage* DeathtMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
		UAnimMontage* DodgeMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
		TArray<FName> AttackMontageSection;

	UPROPERTY(EditAnywhere, Category = Combat)
		TArray<FName> DeathMontageSection;

public:

	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }
};
