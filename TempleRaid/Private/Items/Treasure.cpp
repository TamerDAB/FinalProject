//* Gold Class *//


#include "Items/Treasure.h"
#include "Interfaces/PickupInterface.h"
#include "Kismet/GamePlayStatics.h"


/* This function adds gold to the actor that overlaps with the Golds, triggers a sound effect,
and then destroys the treasure item to simulate a successful pickup. */
void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->AddGold(this);

		SpawnPickupSound();
		Destroy();

	}
}
