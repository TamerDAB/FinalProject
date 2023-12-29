//* HeralthBarComponent Class *//
#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"



UCLASS()
class TEMPLERAIDV1_2_0_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:

	//updates the healthbar
	void SetHealthPrecent(float Precent);

private:

	//instance of the UHealthBar class.
	UPROPERTY()
	class UHealthBar* HealthBarWidget;
	
};
