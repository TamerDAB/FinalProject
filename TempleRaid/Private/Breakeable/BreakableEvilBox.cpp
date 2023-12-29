//* ERvilBoxBreakableActor Class *//
#include "Breakeable/BreakableEvilBox.h"
#include "Blueprint/UserWidget.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

// Sets default values
ABreakableEvilBox::ABreakableEvilBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	GeometryCollection = CreateAbstractDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

// Called when the game starts or when spawned
void ABreakableEvilBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABreakableEvilBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableEvilBox::GetHit_Implementation(const FVector& ImpactPoint)
{
	//Add to view port Game Over Widget if lara destroy the EviBox.
	UWorld* World = GetWorld();
	if (World)
	{
		// Get player controller.
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (EndGameWidget)
		{
			// create gameOver widget and added to view port.
			UUserWidget* EndGame = CreateWidget<UUserWidget>(PlayerController, EndGameWidget);
			EndGame->AddToViewport();
			FInputModeUIOnly InputMode;
			PlayerController->SetInputMode(InputMode);
			PlayerController->bShowMouseCursor = true;

		}

	}

}

