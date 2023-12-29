//* Items Class *//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "Items.generated.h"


class USphereComponent;
class UNiagaraSystem;

enum class  EItemState :uint8
{
	EIS_Hovering,
	EIS_Equipped
};


UCLASS()
class TEMPLERAIDV1_2_0_API AItems : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItems();
    // Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "sine parameters")
	float Amplitude = 0.25f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "sine parameters")
	float TimeConstant = 5.f;

	UFUNCTION(BlueprintPure)
	float Transformedsin();

	UFUNCTION(BlueprintPure)
	float TransformedCos();

	template<typename T>
	T Avg(T First, T Second);


	UFUNCTION()
	 virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	 virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
	virtual void SpawnPickupSystem();
	virtual void SpawnPickupSound();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;


	EItemState ItemState = EItemState::EIS_Hovering;

    UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;



	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* ItemEffect;

	UPROPERTY(EditAnywhere)
		USoundBase* PickupSound;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RunningTime;


	UPROPERTY(EditAnywhere)
		 UNiagaraSystem* PickupEffect;


};

template<typename T>
inline T AItems::Avg(T First, T Second)
{
	return (First + Second) / 2;
}
