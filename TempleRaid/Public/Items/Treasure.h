//* Gold Class *//
#pragma once

#include "CoreMinimal.h"
#include "Items/Items.h"
#include "Treasure.generated.h"




UCLASS()
class TEMPLERAIDV1_2_0_API ATreasure : public AItems
{
	GENERATED_BODY()


protected:

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	

private:
	//Gold variable
	UPROPERTY(EditAnywhere, Category = "Treasure Properties")
	int32 Gold;


public:
	//returns Gold
	FORCEINLINE int32 GetGold() const { return Gold; }
};
