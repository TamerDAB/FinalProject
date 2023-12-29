//* LaraOverlay Class *//
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LaraOverlay.generated.h"



UCLASS()
class TEMPLERAIDV1_2_0_API ULaraOverlay : public UUserWidget
{
	GENERATED_BODY()


public:
	//set lara health
	void SetHealthBarPercent(float Percent);

	//set lara stamina
	void SetStaminaBarPercent(float Percent);

	//set golds
	void SetGold(int32 Gold);

	//set souls
	void SetSouls(int32 Souls);

private:

	//Health Progress Bar
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthProgressBar;

	//Stamina Progress Bar
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* StaminaProgressBar;

	//GoldText
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* GoldText;

	//SoulsText
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SoulsText;

};
