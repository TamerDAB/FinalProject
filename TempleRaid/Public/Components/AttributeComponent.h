//* AttributeComponent Class *//
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEMPLERAIDV1_2_0_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//regenrate the stamina
	void RegenStamina(float DeltaTime);
	//regenrate the health
	void RegenHealth(float DeltaTime);



private:
	//current health
	UPROPERTY(EditAnyWhere, Category = "Actor Attributes")
	float Health;

	//max health
	UPROPERTY(EditAnyWhere, Category = "Actor Attributes")
	float MaxHealth;

	//Current stamina
	UPROPERTY(EditAnyWhere, Category = "Actor Attributes")
		float Stamina;

	//max stamina
	UPROPERTY(EditAnyWhere, Category = "Actor Attributes")
		float MaxStamina;

	//gold
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Gold;

	//souls
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		int32 Souls;

	//Dodge Cost
	UPROPERTY(EditAnyWhere, Category = "Actor Attributes")
		float DodgeCost = 14.f;

	//Stamina Regenrate amount also can be changed in the editor
	UPROPERTY(EditAnyWhere, Category = "Actor Attributes")
		float StaminaRegenrate = 8.f;

	//Health Regenrate amount also can be changed in the editor
	UPROPERTY(EditAnyWhere, Category = "Actor Attributes")
		float HealthRegenrate = 1.f;

public:

	void ReceiveDamge(float Damge);
	void UseStamina(float StaminaCost);
	float GetHealthPercent();
	float GetStaminaPercent();
	bool IsAlive();
	void AddSouls(int32 NumberOfSouls);
	void AddGold(int32 AmountOfGold);
	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE int32 GetSouls() const { return Souls; }
	FORCEINLINE float GetDodgeCost()const { return DodgeCost; }
	FORCEINLINE float GetStamina() const { return Stamina; }
	FORCEINLINE float GetHealth() const { return Health; }
};
