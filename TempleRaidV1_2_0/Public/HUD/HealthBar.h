//* HealthBar Class *//

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"


UCLASS()
class TEMPLERAIDV1_2_0_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Bound to a widget in a User Interface Blueprint.
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;


};
