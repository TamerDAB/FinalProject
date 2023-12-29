//* Lara Character Class*//

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "Interfaces/PickupInterface.h"
#include "LaraCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItems;
class ASoul;
class ATreasure;
class UAnimMontage;
class ULaraOverlay;


UCLASS()
class TEMPLERAIDV1_2_0_API ALaraCharacter : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	ALaraCharacter();

	// Weapon storage data.
	UPROPERTY(EditDefaultsOnly, Category = "SaveData")
	TSubclassOf<class AItemStorge> WeaponStorge;

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual void SetOverlappingItem(AItems* Item)override;
	virtual void AddSouls(ASoul* Soul) override;
	virtual void AddGold(ATreasure* Treasure)override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	/**
	* Callbacks for inputs
	**/
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void EKeyPressed();
	virtual void Attack() override;
	void Dodge();
	
	/**
	* Play Montage functions
	**/
	virtual void AttackEnd() override;
	virtual void DodgeEnd() override;
	virtual bool CanAttack() override;
	bool CanDisarm();
	bool CanArm();
	void PlayEquipMontage(const FName& SectionName);
	virtual void Die() override;
	bool HasEnoughStamina();
	bool IsOccupied();

	/** Attach weapon to back */
	UFUNCTION(BlueprintCallable)
		void Disarm();

	/** Attach weapon to hand */
	UFUNCTION(BlueprintCallable)
		void Arm();

	UFUNCTION(BlueprintCallable)
		void FinishEquipping();

	UFUNCTION(BlueprintCallable)
		void HitReactEnd();


private:

	bool IsUnoccupied();
	void InitializeLaraOverlay();
	void SetHUDHealth();

	/** Character Component */

	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* ViewCamera;

	// Lara Hair
	UPROPERTY(VisibleAnywhere, Category = Hair)
		UGroomComponent* Hair;

	// Lara Eyebrows
	UPROPERTY(VisibleAnywhere, Category = Hair)
		UGroomComponent* Eyebrows;

	//variable of type AItems
	UPROPERTY(VisibleInstanceOnly)
		AItems* OverlappingItem;

	// Equip montage from laraCharacter blueprint class. 
	UPROPERTY(EditDefaultsOnly, Category = Montage)
		UAnimMontage* EquipMontage;


	/* Deafault character state, action state. */
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EActionState ActionState = EActionState::EAS_Unoccupied;


	// Healthbar,stamina,golds,souls. widget. 
	UPROPERTY()
	ULaraOverlay* LaraOverlay;

	

public:
	
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }


	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame(bool SetPosition);
 
	void SwitchLevel(FName LevelName);

	//to display in game "Game Over" widget.
	UFUNCTION(BlueprintCallable)
	bool IsLaraDead();

	//Door puzzle1
	UFUNCTION(BlueprintCallable)
	bool DoorPuzzle1();

	//Door puzzle2
	UFUNCTION(BlueprintCallable)
	bool DoorPuzzle2();

	//Door puzzle3
	UFUNCTION(BlueprintCallable)
	bool DoorPuzzle3();

	UFUNCTION(BlueprintCallable)
		bool AroundEvilBox();

	/*Get gameOver widget from LaraCharacter blueprint class,
	 to add it to view port from here.*/
	UPROPERTY(EditAnywhere, Category = Lara)
		TSubclassOf<UUserWidget> GameOverWidget;

};
