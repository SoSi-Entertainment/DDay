// Copyright 2016-2018 AlphaSoft LLC. All Rights Reserved.
// Copyright 2024 VK LLC. All Rights Reserved.

#include "GameCenterPlugin.h"

#define LOCTEXT_NAMESPACE "FGameCenterPluginModule"


#include "Windows/WindowsSystemIncludes.h"
const Windows::HMODULE cINVALID_HANDLE_VALUE = (Windows::HMODULE)(void *) - 1;
static Windows::HMODULE s_gcOverlay_hModule = cINVALID_HANDLE_VALUE;


#if defined(_WIN64)
const TCHAR * GCOverlayPathName = TEXT("GC_GCLAY64_PATHNAME");
#elif defined(_WIN32)
const TCHAR * GCOverlayPathName = TEXT("GC_GCLAY_PATHNAME");
#endif

void FGameCenterPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FString overlayPath = FPlatformMisc::GetEnvironmentVariable(GCOverlayPathName);

	GLog->Logf(ELogVerbosity::Display, TEXT("[GameCenterPlugin] LibraryPath: %s"), *overlayPath);

	if (!overlayPath.IsEmpty())
	{
		s_gcOverlay_hModule = Windows::LoadLibraryW(*overlayPath);

		if (s_gcOverlay_hModule != cINVALID_HANDLE_VALUE)
		{
			GLog->Logf(ELogVerbosity::Display, TEXT("[GameCenterPlugin] Library loaded"));
		}
		else
		{
			GLog->Logf(ELogVerbosity::Warning, TEXT("[GameCenterPlugin] Can not load library"));
		}
	}
}

void FGameCenterPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	if (s_gcOverlay_hModule != cINVALID_HANDLE_VALUE)
	{
		Windows::FreeLibrary(s_gcOverlay_hModule);
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGameCenterPluginModule, GameCenterPlugin)