// Fill out your copyright notice in the Description page of Project Settings.


#include "NaPublicDependencies/NaPublicDependencyStatics.h"
#include "GameFramework/Actor.h"
#include "NaPublicDependencies/NaGameModeBaseComponent.h"
#include "NaPublicDependencies/NaGameModeSubunitComponent.h"
#include "Utility/BPLibraries/NaObjectStatics.h"
#include "Utility/DebugUtil/NaDebugUtility.h"
#include "Components/SceneComponent.h"
#include "GameFramework/GameModeBase.h"

ENaGameModeComponentErrorType UNaPublicDependencyStatics::CheckGameModeHierarchy(UObject* WorldContext, bool bAssertWhenFailed) {
	if (!IsValid(WorldContext->GetWorld())) {
		UE_LOG(LogNaUtil, Warning, TEXT("NaPublicDependencies: Invalid world context object."));
		return ENaGameModeComponentErrorType::GMCET_NoBase;
	}
	AGameModeBase* GM = WorldContext->GetWorld()->GetAuthGameMode();
	if (!GM)	// No gamemode, maybe default object or client
		return ENaGameModeComponentErrorType::GMCET_Correct;

	const TArray<USceneComponent*> & RootChildren = GM->GetRootComponent()->GetAttachChildren();
	UNaGameModeBaseComponent* Base = nullptr;
	for (auto& obj : RootChildren) {
		Base = dynamic_cast<UNaGameModeBaseComponent*>(obj);
		if (Base)
			break;
	}
	if (!Base) {
		return ENaGameModeComponentErrorType::GMCET_NoBase;	// No NaGameModeBase. It's not an error, so no assert.
	}

	return Base->CheckGameModeHierarchy(bAssertWhenFailed);
}

UNaGameModeBaseComponent* UNaPublicDependencyStatics::GetNaGameModeBase(UObject* WorldContext) {
	if (!IsValid(WorldContext->GetWorld())) {
		UE_LOG(LogNaUtil, Warning, TEXT("NaPublicDependencies: Invalid world context object."));
		return nullptr;
	}
	if (CheckGameModeHierarchy(WorldContext) != ENaGameModeComponentErrorType::GMCET_Correct) {
		UE_LOG(LogNaUtil, Error, TEXT("NaPublicDependencies: Game mode component hierarchy error."));
		return nullptr;
	}
	TArray<UNaGameModeBaseComponent*> Res;
	AGameModeBase* GameMode = WorldContext->GetWorld()->GetAuthGameMode();
	if (!GameMode) {
		UE_LOG(LogNaUtil, Warning, TEXT("GetNaGameModeBase: GameMode is not valid."));
		return nullptr;
	}
	GameMode->GetComponents<UNaGameModeBaseComponent>(Res);
	checkf(Res.Num() <= 1, TEXT("Error: Duplicated NaGameModeBase component detected."));
	if (Res.Num())
		return Cast<UNaGameModeBaseComponent>(Res[0]);
	else return nullptr;
}

UNaGameModeSubunitComponent* UNaPublicDependencyStatics::GetNaGameModeSubunit(UObject* WorldContext, TSubclassOf<UNaGameModeSubunitComponent> Class) {
	UNaGameModeBaseComponent* Base = GetNaGameModeBase(WorldContext);
	if (!Base) {
		LogWriteNoContext("GetNaGameModeSubunit: Base component is invalid.");
		return nullptr;
	}
	return Base->GetSubunit(Class);
}

UNaGameModeSubunitComponent* UNaPublicDependencyStatics::GetNaGameModeSubunitSpecific(UObject* WorldContext, TSubclassOf<UNaGameModeSubunitComponent> SpecificClass) {
	UNaGameModeBaseComponent* Base = GetNaGameModeBase(WorldContext);
	if (!Base) {
		LogWriteNoContext("GetNaGameModeSubunit: Base component is invalid.");
		return nullptr;
	}
	return Base->GetSubunitSpecificClass(SpecificClass);
}

void UNaPublicDependencyStatics::GetAllNaGameModeSubunits(TArray<UNaGameModeSubunitComponent*>& Out, UObject* WorldContext, TSubclassOf<UNaGameModeSubunitComponent> Class) {
	if (!IsValid(WorldContext->GetWorld())) {
		UE_LOG(LogNaUtil, Warning, TEXT("NaPublicDependencies: Invalid world context object."));
		return;
	}
	UNaGameModeBaseComponent* Base = GetNaGameModeBase(WorldContext);
	Out.Empty();
	if (!Base) {
		LogWriteNoContext("GetNaGameModeSubunit: Base component is invalid.");
		return;
	}
	Base->GetAllSubunits(Out, Class);
}

void UNaPublicDependencyStatics::GetAllNaGameModeSubunitsSpecific(TArray<UNaGameModeSubunitComponent*>& Out, UObject* WorldContext, TSubclassOf<UNaGameModeSubunitComponent> SpecificClass) {
	UNaGameModeBaseComponent* Base = GetNaGameModeBase(WorldContext);
	Out.Empty();
	if (!Base) {
		LogWriteNoContext("GetNaGameModeSubunit: Base component is invalid.");
		return;
	}
	Base->GetAllSubunitsSpecificClass(Out, SpecificClass);
}

