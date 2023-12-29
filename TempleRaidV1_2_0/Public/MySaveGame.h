//* MySaveGame Class *//
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStats
{// all variables for save data
	GENERATED_BODY()

	// lara Health
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		float Health;
	// lara MaxHealth
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		float MaxHealth;
	// lara Stamina
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		float Stamina;
	// lara MaxStamina
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		float MaxStamina;
	// lara Golds
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		int32 Golds;
	// lara Souls
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		int32 Souls;
	// lara Location
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		FVector Location;
	// lara Rotation
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		FRotator Rotation;
	//WeaponName
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FString WeaponName;
	//LevelName
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
		FString LevelName;
};


UCLASS()
class TEMPLERAIDV1_2_0_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	
	UMySaveGame();

	//PlayerName
	UPROPERTY(VisibleAnywhere, Category= Basic)
		FString PlayerName;
	//UserIndex
	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;
	// lara Character Stats
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FCharacterStats CharacterStats;
	
};
