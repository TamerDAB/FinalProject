//* Souls Class *//

#pragma once

#include "CoreMinimal.h"
#include "Items/Items.h"
#include "Soul.generated.h"


UCLASS()
class TEMPLERAIDV1_2_0_API ASoul : public AItems
{
	GENERATED_BODY()
	


protected:

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private: 

	//soul variable
	UPROPERTY(EditAnyWhere, Category = "Soul Properties")
	int32 Souls;

public:
	//returns souls
	FORCEINLINE int32 GetSouls() const { return Souls; }
	//set souls
	FORCEINLINE void SetSouls(int32 NumberOfSouls) { Souls = NumberOfSouls; }
};
