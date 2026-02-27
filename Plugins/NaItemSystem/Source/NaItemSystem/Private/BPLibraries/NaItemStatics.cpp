#pragma once
#include "BPLibraries/NaItemStatics.h"
#include "NaPublicDependencies/NaPublicDependencies.h"
#include "Components/NaGameModeItemSystemComponent.h"

UNaGameModeItemSystemComponent* UNaItemStatics::GetGameModeItemSystemComponent(UObject* WorldContext) {
	return Cast<UNaGameModeItemSystemComponent>(UNaPublicDependencyStatics::GetNaGameModeSubunit(WorldContext, UNaGameModeItemSystemComponent::StaticClass()));
}