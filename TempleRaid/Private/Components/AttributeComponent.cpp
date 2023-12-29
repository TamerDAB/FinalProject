//* AttributeComponent Class *//

#include "Components/AttributeComponent.h"

// default values 
UAttributeComponent::UAttributeComponent()
{
	// component initialized when the game starts.
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

// Regenrate Stamina
void UAttributeComponent::RegenStamina(float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina + StaminaRegenrate * DeltaTime, 0.f, MaxStamina);
}

//Regenrate Health
void UAttributeComponent::RegenHealth(float DeltaTime)
{
	Health = FMath::Clamp(Health + HealthRegenrate * DeltaTime, 0.f, MaxHealth);
}

//Receive Damge and update health
void UAttributeComponent::ReceiveDamge(float Damge)
{
	Health = FMath::Clamp(Health - Damge, 0.f, MaxHealth);
}

//use stamina and Update it 
void UAttributeComponent::UseStamina(float StaminaCost)
{
	Stamina = FMath::Clamp(Stamina - StaminaCost, 0.f, MaxStamina);
}

// return Health Percent
float UAttributeComponent::GetHealthPercent()
{
	return Health / MaxHealth;
}

//return Stamina Percent
float UAttributeComponent::GetStaminaPercent()
{
	return Stamina / MaxStamina;
}

//return true if character Alive
bool UAttributeComponent::IsAlive()
{
	return Health >0.f;
}


//add souls
void UAttributeComponent::AddSouls(int32 NumberOfSouls)
{
	Souls += NumberOfSouls;
}

//add golds
void UAttributeComponent::AddGold(int32 AmountOfGold)
{
	Gold += AmountOfGold;
}

