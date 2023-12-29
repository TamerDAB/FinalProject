//* Weapon Class *//

#include "Items/Weapons/Sword.h"
#include "Characters/LaraCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"
#include "NiagaraComponent.h"




ASword::ASword()
{
	// create a box-shaped collision component and attaches it to the root component of the class.
	SwordBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Sword Box"));
	SwordBox->SetupAttachment(GetRootComponent());
	SwordBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SwordBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SwordBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	//creates Uscene component named 'BoxTraceStart' and attaches it to the root component of the class.
	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	//creates Uscene component named Box 'Trace End' and attaches it to the root component of the class.
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}


void ASword::BeginPlay()
{
	Super::BeginPlay();
	// Respond when overlaps occur with the SwordBox collision area.
	SwordBox->OnComponentBeginOverlap.AddDynamic(this, &ASword::OnBoxOverlap);
}



/*  Equip function manages equipping the weapon, updating its state, setting the owner and instigator,
attaching its mesh to a socket, disabling collision, and deactivating associated effects. */
void ASword::Equip(USceneComponent* Inparent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	ItemState = EItemState::EIS_Equipped;
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSocket(Inparent, InSocketName);
	DisableSphereCollision();
	DeactivateEmbers();
}


// Turning off any embers effect associated with the weapon if the ItemEffect is available.
void ASword::DeactivateEmbers()
{
	if (ItemEffect)
	{
		ItemEffect->Deactivate();
	}
}

// Disable collision for the weapon Sphere if it exists.
void ASword::DisableSphereCollision()
{
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}


// This Function attaching the weapon mesh to a specific socket.
void ASword::AttachMeshToSocket(USceneComponent* Inparent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(Inparent, TransformRules, InSocketName);
}


// calling the base class function with the same arguments
void ASword::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}


// calling the base class function with the same arguments
void ASword::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}


/* this function handles interactions and damage calculations when
the weapon box component overlaps with another actor.
It applies damage, triggers hit effects, and creates visual effects at the impact point. */
void ASword::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (ActorIsSameType(OtherActor)) return;

	FHitResult BoxHit;
	BoxTrace(BoxHit);

	if (BoxHit.GetActor())
	{
		if (ActorIsSameType(BoxHit.GetActor())) return;

		UGameplayStatics::ApplyDamage(
			BoxHit.GetActor(),
			Damage,
			GetInstigator()->GetController(),
			this,
			UDamageType::StaticClass());

		ExecuteGetHit(BoxHit);

		CreateFields(BoxHit.ImpactPoint);
	}
}


/* If both the weapon's owner and the provided actor have the "Enemy" tag, the function returns true */
bool ASword::ActorIsSameType(AActor* OtherActor)
{
	return GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
}


/* function checks if the actor hit by the box implements the <IHitInterface>, 
 and if so, it calls the GetHit() function on that actor,
 passing in the impact point of the box hit. This allows the hit actor to respond to being hit by the weapon. */
void ASword::ExecuteGetHit(FHitResult& BoxHit)
{
	IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
	if (HitInterface)
	{
		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint);
	}

}


/*  function conducts a box trace collision check,
taking into account start and end locations, collision extent,rotation,
ignored actors,and debug visualization. The result is stored in BoxHit,
and the hit actor is added to the list of ignored actors. */
void ASword::BoxTrace(FHitResult& BoxHit)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();



	TArray<AActor*> ActorToIgnore;
	ActorToIgnore.Add(this);

	for (AActor* Actor : IgnoreActors)
	{
		ActorToIgnore.AddUnique(Actor);
	}

	
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		BoxTraceExtent,
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorToIgnore,
		bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		BoxHit,
		true
	);
	IgnoreActors.AddUnique(BoxHit.GetActor());
}
