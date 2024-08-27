// Copyright 2016-2018 AlphaSoft LLC. All Rights Reserved.
// Copyright 2024 VK LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FGameCenterPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static inline FGameCenterPluginModule& Get() {
		return FModuleManager::LoadModuleChecked<FGameCenterPluginModule>("GameCenterPlugin");
	}
	static inline bool IsAvailable() {
		return FModuleManager::Get().IsModuleLoaded("GameCenterPlugin");
	}
};
