//* PickupInterface Class *//
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickupInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupInterface : public UInterface
{
	GENERATED_BODY()
};


/* interface with three virtual functions for
 picking up items,
 adding souls,
 and adding gold.
 Other classes can implement this interface to provide these
 functionalities by overriding these functions. */

class TEMPLERAIDV1_2_0_API IPickupInterface
{
	GENERATED_BODY()

public:


	virtual void SetOverlappingItem(class AItems* Item);
	virtual void AddSouls(class ASoul* Soul);
	virtual void AddGold(class ATreasure* Treasure);
};
