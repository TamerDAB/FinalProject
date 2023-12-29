//* BreakableActor Class *//

#include "Breakeable/BreakeableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure.h"

// Default values
ABreakeableActor::ABreakeableActor()
{
 	// this actor call Tick() every frame.
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollection = CreateAbstractDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

// Called when the game starts or when spawned
void ABreakeableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABreakeableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
 
/* When BreakeableActor is hit. 
 It spawns a Treasure actor(Gold) above the BreakeableActor at the specified location. */
void ABreakeableActor::GetHit_Implementation(const FVector& ImpactPoint)
{
	UWorld* World = GetWorld();
	if (World && TreasureClass) 
	{
		FVector Location = GetActorLocation();
		Location.Z += 75.f;
		World->SpawnActor<ATreasure>(TreasureClass, Location, GetActorRotation());
	}
}

