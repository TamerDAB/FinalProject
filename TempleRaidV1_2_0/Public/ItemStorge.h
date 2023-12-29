//* ItemStorage Class *//
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemStorge.generated.h"

UCLASS()
class TEMPLERAIDV1_2_0_API AItemStorge : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemStorge();

    // Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	// TMap <String (Weapon name), TSubclassOf<class ASword> (blueprint class of weapon).
	UPROPERTY(EditDefaultsOnly, Category = "SaveData")
	TMap<FString, TSubclassOf<class ASword>> WeaponMap;

};
