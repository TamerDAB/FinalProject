//* HeralthBarComponent Class *//

#include "HUD/HealthBarComponent.h"
#include "HUD/HealthBar.h"
#include "Components/ProgressBar.h"



/* Function updates the health bar display percentage. */
void UHealthBarComponent::SetHealthPrecent(float Percent)
{
	if (HealthBarWidget == nullptr) 
	{

		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
    
	}
	if (HealthBarWidget && HealthBarWidget->HealthBar)
	{
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}
