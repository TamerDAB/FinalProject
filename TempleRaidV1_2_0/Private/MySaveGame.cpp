//* MySaveGame Class *//


#include "MySaveGame.h"

UMySaveGame::UMySaveGame()
{//constructor

	//we have just one player
	PlayerName = TEXT("Default");

	//number slot
	UserIndex = 0;

	//for weapon
	CharacterStats.WeaponName = TEXT("");

	//level name
	CharacterStats.LevelName = TEXT("");

}
