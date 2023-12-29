//* Items Class *//

#include "Items/Items.h"
#include "TempleRaidV1_2_0/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Interfaces/PickupInterface.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"




// Sets default values
AItems::AItems()
{
 	// actor  call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	/*  sets up the ItemMesh component as the root component,
	 configures it to ignore collisions and have no collision response,
	 effectively making it a non-colliding visual component. */
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;

	//create a spher collision component (USphereComponent) named 'Sphere' and attaches it to the root component.
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	/* creates a Niagara component named 'ItemEffect',used to handle particle effects. 
	 then attached to the root component of the class. */
	ItemEffect = CreateAbstractDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	ItemEffect->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AItems::BeginPlay()
{
	Super::BeginPlay();

	/* Binds dynamic event handlers to the 'OnSphereOverlap' and 'OnSphereEndOverlap' member functions within the class.
	These handlers are triggered when other components overlap with or end overlap with the Sphere component. */
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItems::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItems::OnSphereEndOverlap);


}


/*this function calculates a scaled sine wave value using Amplitude, RunningTime, and TimeConstant.
  used in animations. */
float AItems::Transformedsin()
{
	return Amplitude * FMath::Sin( RunningTime * TimeConstant);
}


/* function calculates a scaled cosine wave value using Amplitude, RunningTime, and TimeConstant,
which used to animations. */
float AItems::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}


/* this function signals to the actor that this item is currently overlapping with it, allowing it to handle pickup-related behavior. */
void AItems::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->SetOverlappingItem(this);
	}
}


/* This function notifies an actor that the overlap with the item has ended,
enabling the actor to respond accordingly when the item is no longer nearby. */
void AItems::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->SetOverlappingItem(nullptr);
	}
}


/* function triggers the visual particle effect defined in
PickupEffect at the item location when it's picked up or interacted with. */
void AItems::SpawnPickupSystem()
{
	if (PickupEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			PickupEffect,
			GetActorLocation()
		);
	}
}

// play sound when pick up item.
void AItems::SpawnPickupSound()
{
	if (PickupSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			this,
			PickupSound,
			GetActorLocation()
		);
	}
}

// Called every frame
/*  function updates the item's behavior based on its state.
 If it's hovering (EIS_Hovering), it adds a hovering effect using the transformed sine function.
 Additionally, it calculates the average vector between the item's location and the zero vector */
void AItems::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	RunningTime += DeltaTime;

	if (ItemState == EItemState::EIS_Hovering)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, Transformedsin()));
	}

	FVector AvgVector = Avg<FVector>(GetActorLocation(), FVector::ZeroVector);
	
}

