//* LaraHUD Class *//

#include "HUD/LaraHUD.h"
#include "HUD/LaraOverlay.h"



/* Function initializes and displays a ULaraOverlay widget on the game viewport */
void ALaraHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && LaraOverlayClass)
		{
			LaraOverlay = CreateWidget<ULaraOverlay>(Controller, LaraOverlayClass);
			LaraOverlay->AddToViewport();

		}

	}
	
}
