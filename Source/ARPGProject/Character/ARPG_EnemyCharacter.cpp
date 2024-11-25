#include "ARPG_EnemyCharacter.h"

void AARPG_EnemyCharacter::WeaponGrip()
{
	bIsMainWeaponGrip = true;
	WeaponAttach("Sword_Grip");
}
