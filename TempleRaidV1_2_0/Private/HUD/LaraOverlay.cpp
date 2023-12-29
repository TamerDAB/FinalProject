//* LaraOverlay Class *//

#include "HUD/LaraOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


//Function to set the health bar percent to the HealthProgressBar
void ULaraOverlay::SetHealthBarPercent(float Percent)
{

	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
	}

}

// Function to set the stamina bar percent to the StaminaProgressBar
void ULaraOverlay::SetStaminaBarPercent(float Percent)
{

	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(Percent);
	}

}
// Function to set the golds to the GoldText
void ULaraOverlay::SetGold(int32 Gold)
{
	if (GoldText)
	{
		GoldText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Gold)));
	}
}


// Function to set the souls to the SoulsText
void ULaraOverlay::SetSouls(int32 Souls)
{
	if (SoulsText)
	{
		SoulsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Souls)));
	}
}
