#include "NavBlockerBoxComponent.h"

UNavBlockerBoxComponent::UNavBlockerBoxComponent()
{
	// Только для навигации
	bDynamicObstacle = true;
	bUseSystemDefaultObstacleAreaClass = false;
	AreaClassOverride = UNavArea_Null::StaticClass();

	// Навсегда статик
	SetMobility(EComponentMobility::Static);

	// Полностью отключаем коллизию
	SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	SetGenerateOverlapEvents(false);
	CanCharacterStepUpOn = ECB_No;

	// Оптимизация
	PrimaryComponentTick.bCanEverTick = false;
	bCanEverAffectNavigation = true;

	// Невидим полностью
	bHiddenInGame = true;   // скрыт в игре
	SetVisibility(false);   // скрыт в редакторе (нет гизмо)
}
