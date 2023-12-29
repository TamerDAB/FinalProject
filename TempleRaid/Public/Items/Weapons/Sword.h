//* Weapon Class *//

#pragma once

#include "CoreMinimal.h"
#include "Items/Items.h"
#include "Sword.generated.h"


class UBoxComponent;




UCLASS()
class TEMPLERAIDV1_2_0_API ASword : public AItems
{
	GENERATED_BODY()
public:
	ASword();

	//give weapon a name (for saving usage).
	UPROPERTY(EditDefaultsOnly, Category = "SaveData")
	FString Name;

	//equip the sword.
	void Equip(USceneComponent* Inparent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	void DeactivateEmbers();
	void DisableSphereCollision();
	void AttachMeshToSocket(USceneComponent* Inparent, const FName& InSocketName);

	TArray<AActor*> IgnoreActors;

protected:

	virtual void BeginPlay() override;
	
		virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
		virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

		UFUNCTION()
		void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

		bool ActorIsSameType(AActor* OtherActor);

		void ExecuteGetHit(FHitResult& BoxHit);
        

		//represents the location where fields
		UFUNCTION(BlueprintImplementableEvent)
			void CreateFields(const FVector& FieldLocation);



private:

	void BoxTrace(FHitResult& BoxHit);
	// BoxTrace Extent
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FVector BoxTraceExtent = FVector(5.f);
	//bShow Box Debug
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	bool bShowBoxDebug = false;
	//Weapon Box
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* SwordBox;
	//Box Trace Start
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;
	//Box Trace End
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;
	// weapon damage deafault value 20 ,can be changed in editor.
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 20.f;

public:

	FORCEINLINE UBoxComponent* GetSwordBox() const { return SwordBox; }
};
