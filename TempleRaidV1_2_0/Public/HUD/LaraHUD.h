//* LaraHUD Class *//
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LaraHUD.generated.h"


class ULaraOverlay;


UCLASS()
class TEMPLERAIDV1_2_0_API ALaraHUD : public AHUD
{
 GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	
private:

	//blueprint widget from editor (UE).
	UPROPERTY(EditAnywhere, Category = Lara)
		TSubclassOf<ULaraOverlay> LaraOverlayClass;

	//to set all attributes in LaraOverlay.
	UPROPERTY()
	   ULaraOverlay* LaraOverlay;
    

public:

	//returns LaraOverlay
	FORCEINLINE ULaraOverlay* GetLaraOverlay() const { return LaraOverlay; }

};
