
#include "Characters/LaraCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GroomComponent.h"
#include "Items/Items.h"
#include "Items/Weapons/Sword.h"
#include "Animation/AnimMontage.h"
#include "Components/BoxComponent.h"
#include "HUD/LaraHUD.h"
#include "HUD/MyLaraController.h"
#include "Components/AttributeComponent.h"
#include "HUD/LaraOverlay.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/LaraAnimInstance.h"
#include "GuardsEnemy/Guards.h"
#include "ItemStorge.h"
#include "MySaveGame.h"


// Lara constructor
ALaraCharacter::ALaraCharacter()
{

 	// character call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Lara rotation will automatically align with the direction it's moving in.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	//Camera that show lara set the distance to 300.  
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;


	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	/* lara Hair and Eyebrows come separate, so should to attach them to right place. */

	// Attach lara hair to her head.
	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	// Attach lara eyebrows to head.
	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");


}


//Tick function Called every frame.
void ALaraCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (Attributes && LaraOverlay)
	{
		//Regenrate Health and stamina just if player alive.
		if (ActionState != EActionState::EAS_Dead)
		{
			//regenerate Stamina
			Attributes->RegenStamina(DeltaTime);
			LaraOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
			//regenerate Health
			Attributes->RegenHealth(DeltaTime);
			LaraOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
		}
			
	}

}


// Called to bind functionality to input.
void ALaraCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ALaraCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ALaraCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ALaraCharacter::LookUp);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ALaraCharacter::MoveRight);
	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ALaraCharacter::Jump);
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &ALaraCharacter::EKeyPressed);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ALaraCharacter::Attack);
	PlayerInputComponent->BindAction(FName("Dodge"), IE_Pressed, this, &ALaraCharacter::Dodge);


}


//Lara(Player) jump action extends from Character Class from unreal engine.
void ALaraCharacter::Jump()
{
	if (IsUnoccupied())
	{
		Super::Jump();
	}
	
}


//  Handle player damage and update the health bar.
float ALaraCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamge(DamageAmount);

	SetHUDHealth();

	return DamageAmount;
}


// laraCharacteer extends GetHit_Implementation from BaseCharacter class, calc in which direction lara get hit,
// and change her action state to HitReaction. 
void ALaraCharacter::GetHit_Implementation(const FVector& ImpactPoint)
{
	Super::GetHit_Implementation(ImpactPoint);
	SetSwordCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Attributes && Attributes->GetHealthPercent() > 0.f)
	{
		ActionState = EActionState::ECS_HitReaction;
	}
	
}


// Set the OverlappingItem variable of a character when they come into contact with an item in the game.
void ALaraCharacter::SetOverlappingItem(AItems* Item)
{
	OverlappingItem = Item;
}


// Add souls to lara Attributes.
void ALaraCharacter::AddSouls(ASoul* Soul)
{

	if (Attributes && LaraOverlay)
	{
		Attributes->AddSouls(Soul->GetSouls());
		//update Soul to HUD(Heads-up Display).
		LaraOverlay->SetSouls(Attributes->GetSouls());
	}

}


// Add Golds to lara Attributes.
void ALaraCharacter::AddGold(ATreasure* Treasure)
{
	if (Attributes && LaraOverlay)
	{
		Attributes->AddGold(Treasure->GetGold());
		//update gold to HUD
		LaraOverlay->SetGold(Attributes->GetGold());
	}

}


// Called when the game start
void ALaraCharacter::BeginPlay()
{
	Super::BeginPlay();

	//we used this tag in Guards.cpp in PawnSeen() function.to know if guards sees the player.
	Tags.Add(FName("LaraCharacter"));
	
	//Initializing the overlay (Health & Stamina Bar).
	InitializeLaraOverlay();

}


//  MoveForward function.
void ALaraCharacter::MoveForward(float Value)
{
	 //If the ActionState is not EAS_Unoccupied, exit the function.
	if (ActionState != EActionState::EAS_Unoccupied) return;
	if ((Controller) && (Value != 0.f))
	{
		// Find out which way is forward based on the control rotation.
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		// Get the forward direction vector based on the YawRotation.
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
	
	}
}


// Move Right function.
void ALaraCharacter::MoveRight(float Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;
	if ((Controller) && (Value != 0.f))
	{
		// find out wich way is right
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		//Get the Right direction
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}

}


/* Handle horizontal rotation input(yaw) for the character(Lara) controller,
   allowing the player to look around left and right  */
void ALaraCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}


/* Handle vertical rotation input (pitch) for the character(Lara) controller,
  allowing the player to look up and down.*/
void ALaraCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}


// Equip weapon when player press 'E' Key, also player can equip and unequip the weapon.
void ALaraCharacter::EKeyPressed()
{
	ASword* OverlappingWeapon = Cast<ASword>(OverlappingItem);
	if (OverlappingWeapon)
	{
		//respawn the equiped weapon when the player collect another at the player location.
		if (EquippedWeapon)
		{
			AItemStorge* WeaponsData = GetWorld()->SpawnActor<AItemStorge>(WeaponStorge);
			FString WeaponName = EquippedWeapon->Name;
			//ASword* WeaponToSpawn = GetWorld()->SpawnActor<ASword>(WeaponsData->WeaponMap[WeaponName]);
			ASword* SpawnWeapon = Cast<ASword>(OverlappingItem);

			//get player loaction
			ALaraCharacter* PlayerCharacter = Cast<ALaraCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
			FVector SpawnLocation = PlayerCharacter->GetActorLocation();

			FActorSpawnParameters SpawnParams;
			FRotator SpawnRotation(0.f, 0.f, 0.f);
			SpawnLocation.X = SpawnLocation.X + 20.f;
			SpawnLocation.Y = SpawnLocation.Y - 100.f;
			SpawnLocation.Z = SpawnLocation.Z + 20.f;

			//Spawn the equiped weapon.
			GetWorld()->SpawnActor<ASword>(WeaponsData->WeaponMap[WeaponName], SpawnLocation, SpawnRotation, SpawnParams);

			//destroy the wepon that in Lara hand.
			EquippedWeapon->Destroy();
		}

		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		OverlappingItem = nullptr;
		EquippedWeapon = OverlappingWeapon;
	}
	else
	{
		if (CanDisarm()) 
		{
			PlayEquipMontage(FName("Unequip"));
			CharacterState = ECharacterState::ECS_Unequipped;
		}
		else if (CanArm())
		{
			PlayEquipMontage(FName("Equip"));
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		}
	}
}


// Attack function, first check if lara can attack, then play attack animation.
void ALaraCharacter::Attack()
{
	Super::Attack();

	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
	
}


// Dodge function, first check if lara in occuppied state and has enough stamina, then play dodge animation.
void ALaraCharacter::Dodge()
{
	if (IsOccupied() || !HasEnoughStamina()) return;
	
	PlayDodgeMontage();
	ActionState = EActionState::EAS_Dodge;
	if (Attributes && LaraOverlay)
	{
		Attributes->UseStamina(Attributes->GetDodgeCost());
		LaraOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
	
}


// when attack ends change Lara action state to unoccupied. 
void ALaraCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}


// when Dodge animation ends change Lara action state to unoccupied. 
void ALaraCharacter::DodgeEnd()
{
	Super::DodgeEnd();

	ActionState = EActionState::EAS_Unoccupied;
}


// helper for attack function
bool ALaraCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}


// helper function to equip and unequip the sword.
bool ALaraCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}


// helper function to equip and unequip the sword.
bool ALaraCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped && 
		EquippedWeapon;
}


// Play equip montage, playing the animation and jumps to a specific section within the montage.
void ALaraCharacter::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}

}


// Die function that firstly play Lara death animation, then display GameOver widget.
void ALaraCharacter::Die()
{
	Super::Die();

	ActionState = EActionState::EAS_Dead;


	 //Add to view port Game Over Widget if lara is dead.
	UWorld* World = GetWorld();
	if (World)
	{
		 // Get player controller.
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (GameOverWidget)
		{
			// create gameOver widget and added to view port.
		    UUserWidget* GameOver = CreateWidget<UUserWidget>(PlayerController, GameOverWidget);
			GameOver->AddToViewport();
		    FInputModeUIOnly InputMode;
			PlayerController->SetInputMode(InputMode);
			PlayerController->bShowMouseCursor = true;

		}

	}
	
}


// return true if lara have enough stamina for dodge animation.
bool ALaraCharacter::HasEnoughStamina()
{
	return Attributes && Attributes->GetStamina() > Attributes->GetDodgeCost();
}


// return ture if lara IsOcuupied.
bool ALaraCharacter::IsOccupied()
{
	return ActionState != EActionState::EAS_Unoccupied;
}


// attach weapon to lara back.
void ALaraCharacter::Disarm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
		ActionState = EActionState::EAS_EquippingWeapon;
	}
}


// attach weapon to lara hand.
void ALaraCharacter::Arm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}


// In FinishEquipping, convert action state to unoccupied. 
void ALaraCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}


// In HitReactEnd, convert action state to unoccupied.
void ALaraCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}


// return true if the action state is unoccupied.
bool ALaraCharacter::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}


// Initialize Lara Overlay.
void ALaraCharacter::InitializeLaraOverlay()
{
	// Get player controller.
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{ 

		ALaraHUD* LaraHUD = Cast<ALaraHUD>(PlayerController->GetHUD());
		
		
		if (LaraHUD)
		{
			LaraOverlay = LaraHUD->GetLaraOverlay();
			if (LaraOverlay && Attributes)
			{
				LaraOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				LaraOverlay->SetStaminaBarPercent(1.f);
				LaraOverlay->SetGold(0);
				LaraOverlay->SetSouls(0);
			}
		}
	}
}


// get the health from lara attributes and set to HUD.
void ALaraCharacter::SetHUDHealth()
{
	if (LaraOverlay)
	{
		LaraOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}


// Used to store game data.
void ALaraCharacter::SaveGame()
{
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));

	SaveGameInstance->CharacterStats.Health = Attributes->GetHealthPercent();
	SaveGameInstance->CharacterStats.Stamina = Attributes->GetStaminaPercent();
	SaveGameInstance->CharacterStats.Golds = Attributes->GetGold();
	SaveGameInstance->CharacterStats.Souls = Attributes->GetSouls();


	FString MapName = GetWorld()->GetMapName();
	MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);


	 // (Saving the name of the map)->every time when saving the game we got the corrent map name of the world where player was in.
	SaveGameInstance->CharacterStats.LevelName = MapName;

	
	 // save weapon just if the player have weapon .
	if (EquippedWeapon)
	{
	   SaveGameInstance->CharacterStats.WeaponName = EquippedWeapon->Name;
	}

	// Save player loaction
	SaveGameInstance->CharacterStats.Location = GetActorLocation();
	// Save player rotation
	SaveGameInstance->CharacterStats.Rotation = GetActorRotation();


	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->PlayerName, SaveGameInstance->UserIndex);

}


// Used to load game from saved data.
void ALaraCharacter::LoadGame(bool SetPosition)
{
	UMySaveGame* LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));

	LoadGameInstance =Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));


	LaraOverlay->SetHealthBarPercent(LoadGameInstance->CharacterStats.Health);
	LaraOverlay->SetStaminaBarPercent(LoadGameInstance->CharacterStats.Stamina);
	LaraOverlay->SetGold(LoadGameInstance->CharacterStats.Golds);
	LaraOverlay->SetSouls(LoadGameInstance->CharacterStats.Souls);


	 // if player have weapon in saved data. load it and attach it again to lara hand.
	if (WeaponStorge) 
	{

		AItemStorge* Weapons = GetWorld()->SpawnActor<AItemStorge>(WeaponStorge);

		if (Weapons)
		{

			FString WeaponName = LoadGameInstance->CharacterStats.WeaponName;
	
			if (Weapons->WeaponMap.Contains(WeaponName))
			{

				ASword* WeaponToEquip = GetWorld()->SpawnActor<ASword>(Weapons->WeaponMap[WeaponName]);

				ASword* OverlappingWeapon = Cast<ASword>(OverlappingItem);

				OverlappingWeapon = WeaponToEquip;
				OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
				CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
				OverlappingItem = nullptr;
				EquippedWeapon = OverlappingWeapon;
			}

			
		}
	}
	

	if (SetPosition)
	{
		// Load Lara location
		SetActorLocation(LoadGameInstance->CharacterStats.Location);
		// Load Lara rotation 
		SetActorRotation(LoadGameInstance->CharacterStats.Rotation);
	}


	 //the player won't be in death state in case saving while dying.
	if (ActionState == EActionState::EAS_Dead) 
	{

		ActionState = EActionState::EAS_Unoccupied;

		

		Jump();
		PlayEquipMontage(FName("Equip"));

	}


	 //continue if the level name is not empty string-> call switch level
	if (LoadGameInstance->CharacterStats.LevelName == TEXT(""))
	{
		FName LevelName(*LoadGameInstance->CharacterStats.LevelName);
	    
		SwitchLevel(LevelName);
	
	}

}


//Open level by name if not the current(saved in).
void ALaraCharacter::SwitchLevel(FName LevelName)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FString CurrentLevel = World->GetMapName();


		FName CurrentLevelName(*CurrentLevel);

		if (CurrentLevelName != LevelName) 
		{
			UGameplayStatics::OpenLevel(World, LevelName);
		}

	}

}


//Check if the player is dead -> to display in game "Game Over" widget.  
bool ALaraCharacter::IsLaraDead()
{
	if (ActionState == EActionState::EAS_Dead)
	{
		return true;
	}
	return false;
}


//(Door 1 Puzzle) Lara must have 5 golds -> the Door opens
bool ALaraCharacter::DoorPuzzle1()
{
	if (Attributes->GetGold() >= 0 /*0*/)
	{
		return true;
	}

	return false;
}


//(Door 2 Puzzle) Lara must have 10 golds & 5 souls -> the Door opens
bool ALaraCharacter::DoorPuzzle2()
{
	if (Attributes->GetGold() >= 10 /*0*/ && Attributes->GetSouls() >= 5 /*0*/ )
	{
		return true;
	}

	return false;
}


//(Door 3 Puzzle) Lara must have 18 golds & 15 souls -> the Door opens.
bool ALaraCharacter::DoorPuzzle3()
{
	if (Attributes->GetGold() >= 18 /*0*/ && Attributes->GetSouls() >= 15 /*0*/)
	{
		return true;
	}

	return false;
}


//Lara must have 22 golds & 16 souls to reach evil box that surrounded by imagery.
bool ALaraCharacter::AroundEvilBox()
{
	if (Attributes->GetGold() >= 22/*0*/ && Attributes->GetSouls() >= 16/*0*/)
	{
		return true;
	}

	return false;
}
