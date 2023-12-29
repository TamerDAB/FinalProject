//* ItemStorage Class *//

#include "ItemStorge.h"

// Sets default values
AItemStorge::AItemStorge()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AItemStorge::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemStorge::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

