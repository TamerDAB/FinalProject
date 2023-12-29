//* HitInterface Class *//
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface
{
	GENERATED_BODY()
};


class TEMPLERAIDV1_2_0_API IHitInterface
{
	GENERATED_BODY()

public:
	// provide the GetHit event, which can be overridden in Blueprint classes.
	UFUNCTION(BlueprintNativeEvent)
     void GetHit(const FVector& ImpactPoint) ;

};
