//* CharacterTypes *//
#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{//varables to equip sword usage.

	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon")
};


UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_HitReaction UMETA(DisplayName = "HitReaction"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_EquippingWeapon UMETA(DisplayName = "Equipping Weapon"),
	EAS_Dodge UMETA(DisplayName = "Dodge"),
	EAS_Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum EDeathPose
{
	EDP_Death1 UMETA(DisplayName = "Death1"),
	EDP_Death2 UMETA(DisplayName = "Death2"),
	EDP_Death3 UMETA(DisplayName = "Death3"),
	EDP_Death4 UMETA(DisplayName = "Death4"),
	EDP_Max UMETA(DisplayName = "DefaultMax")

};

UENUM(BlueprintType)
//enum class called qurad state, to know the quard in wich state.
enum class EQuardState : uint8
{
	EQS_NoState UMETA(DisplayName = "NoState"),
	EQS_Dead UMETA(DisplayName = "Dead"),
	EQS_Patrolling UMETA(DisplayName = "Patrolling"),
	EQS_Chasing UMETA(DisplayName = "Chasing"),
	EQS_Attacking UMETA(DisplayName = "Attacking"),
	EQS_Engaged UMETA(DisplayName = "Engaged")

	
	
};