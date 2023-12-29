//* BreakableActor Class *//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakeableActor.generated.h"


/*  class in Unreal Engine that is used to manage and
 simulate destructible geometry collections */ 
class UGeometryCollectionComponent;

UCLASS()
class TEMPLERAIDV1_2_0_API ABreakeableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakeableActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// reference to a UGeometryCollectionComponent within an blueprint class in UE.
	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryCollection;
	
	//Golds
	UPROPERTY(EditAnyWhere)
	UClass* TreasureClass;

};
