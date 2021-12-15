// Copyright Epic Games, Inc. All Rights Reserved.

#include "NaPublicDependencies/NaGameModeBaseComponent.h"
#include "NaPublicDependencies/NaGameModeSubunitComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Utility/BPLibraries/NaObjectStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utility/DebugUtil/NaDebugUtility.h"


ENaGameModeComponentErrorType UNaGameModeBaseComponent::CheckGameModeHierarchy(bool bAssertWhenFailed) {

#if WITH_EDITOR
	ENaGameModeComponentErrorType ErrorType = ENaGameModeComponentErrorType::GMCET_Correct;

	if (!GetOwner())	// Default object, no owner
		return ENaGameModeComponentErrorType::GMCET_Correct;

	AGameModeBase* OwnerGameMode = dynamic_cast<AGameModeBase*>(GetOwner());	// Owner; null if invalid


	/* Check game mode type */
	if (!IsValid(OwnerGameMode)) {
		ErrorType = ENaGameModeComponentErrorType::GMCET_InvalidGameMode;
	}

	/* Check base duplication */
	if (ErrorType == ENaGameModeComponentErrorType::GMCET_Correct) {
		const TArray<USceneComponent*> & AttachedToRoot = OwnerGameMode->GetRootComponent()->GetAttachChildren();	// All components attached to root; for checking duplication 
		int i = 0; int dup = 0;
		for (1; i < AttachedToRoot.Num(); ++i) {
			if (dynamic_cast<UNaGameModeBaseComponent*>(AttachedToRoot[i])) {
				++dup;
				if (dup > 1) {
					ErrorType = ENaGameModeComponentErrorType::GMCET_BaseDuplicate;
					break;
				}
			}
		}
	}

	/* Check attachment of this base to root */
	if (ErrorType == ENaGameModeComponentErrorType::GMCET_Correct) {	// Passed
		if (GetAttachParent() != OwnerGameMode->GetRootComponent())
			ErrorType = ENaGameModeComponentErrorType::GMCET_BaseNotAttachedToRoot;
	}

	/* Check if any non-subunit component attached to base */
	if (ErrorType == ENaGameModeComponentErrorType::GMCET_Correct) {

		const TArray<USceneComponent*> & AttachedToThis = GetAttachChildren();
		for (auto & obj : AttachedToThis) {
			if (UKismetMathLibrary::ClassIsChildOf(obj->GetClass(), UNaGameModeSubunitComponent::StaticClass())) {
				ErrorType = ENaGameModeComponentErrorType::GMCET_NonSubunitAttachedToBase;
					break;
			}
		}
	}

	/* Check if any "wild" subunits (not attached to base) */
	TArray<USceneComponent*> AllSubunits = TArray<USceneComponent*>();
	if (ErrorType == ENaGameModeComponentErrorType::GMCET_Correct) {
		UNaObjectStatics::GetAllAttachedComponents(AllSubunits, OwnerGameMode->GetRootComponent(), UNaGameModeSubunitComponent::StaticClass(), true);
		for (auto & obj : AllSubunits) {
			if (obj->GetAttachParent() != this) {
				ErrorType = ENaGameModeComponentErrorType::GMCET_SubunitNotAttachedToBase;
				break;
			}
		}
	}

	/* Check if any subunit not allow duplication duplicated */
	if (ErrorType == ENaGameModeComponentErrorType::GMCET_Correct) {
		TSet<UClass*> ExistingClasses;
		for (auto & obj : AllSubunits) {
			if (ExistingClasses.Contains(obj->GetClass())) {
				if (dynamic_cast<UNaGameModeSubunitComponent*>(obj)->bAllowDuplication == false) {
					ErrorType = ENaGameModeComponentErrorType::GMCET_SubunitDuplicate;
					break;
				}
			}
		}
	}
	
	if (ErrorType != ENaGameModeComponentErrorType::GMCET_Correct) {
		if (bAssertWhenFailed) {
			switch (ErrorType) {
			case ENaGameModeComponentErrorType::GMCET_InvalidGameMode: {
				checkf(false, TEXT("NaGameModeComponents hierarchy error: invalid game mode. Maybe the NaGameModeBaseComponent is attached to a non-gamemode actor, or running on a client."));
			}
			case ENaGameModeComponentErrorType::GMCET_BaseDuplicate: {
				checkf(false, TEXT("NaGameModeComponents hierarchy error: base component duplicated. Only one NaGameModeBaseComponent is allowed."));
			}
			case ENaGameModeComponentErrorType::GMCET_BaseNotAttachedToRoot: {
				checkf(false, TEXT("NaGameModeComponentst hierarchy error: base component is not attached to the root of game mode."));
			}
			case ENaGameModeComponentErrorType::GMCET_NonSubunitAttachedToBase: {
				checkf(false, TEXT("NaGameModeComponents hierarchy error: a non-subunit component is directly attached to the base component."));
			}
			case ENaGameModeComponentErrorType::GMCET_SubunitNotAttachedToBase: {
				checkf(false, TEXT("NaGameModeComponents hierarchy error: a subunit component is not directly attached to the base component."));
			}
			case ENaGameModeComponentErrorType::GMCET_SubunitDuplicate: {
				checkf(false, TEXT("NaGameModeComponents hierarchy error: a subunit not allowing duplication appeared twice."));
			}
			default: {
				checkNoEntry();
			}
			}
		}
		else {
			switch (ErrorType) {
			case ENaGameModeComponentErrorType::GMCET_InvalidGameMode: {
				LogError("NaGameModeComponents hierarchy error: invalid game mode. Maybe the NaGameModeBaseComponent is attached to a non-gamemode actor, or running on a client.");
				break;
			}
			case ENaGameModeComponentErrorType::GMCET_BaseDuplicate: {
				LogError("NaGameModeComponents hierarchy error: base component duplicated. Only one NaGameModeBaseComponent is allowed.");
				break;
			}
			case ENaGameModeComponentErrorType::GMCET_BaseNotAttachedToRoot: {
				LogError("NaGameModeComponents hierarchy error: base component is not attached to the root of game mode.");
				break;
			}
			case ENaGameModeComponentErrorType::GMCET_NonSubunitAttachedToBase: {
				LogError("NaGameModeComponents hierarchy error: a non-subunit component is directly attached to the base component.");
				break;
			}
			case ENaGameModeComponentErrorType::GMCET_SubunitNotAttachedToBase: {
				LogError("NaGameModeComponents hierarchy error: a subunit component is not directly attached to the base component.");
				break;
			}
			case ENaGameModeComponentErrorType::GMCET_SubunitDuplicate: {
				LogError("NaGameModeComponents hierarchy error: a subunit not allowing duplication appeared twice.");
				break;
			}
			default: {
				checkNoEntry();
			}
			}
		}
	}
	return ErrorType;

#else
	// On runtime always return correct to save resource
	return ENaGameModeComponentErrorType::GMCET_Correct;
#endif
}

void UNaGameModeBaseComponent::BeginPlay() {
	Super::BeginPlay();
	CheckGameModeHierarchy(true);
}

UNaGameModeSubunitComponent* UNaGameModeBaseComponent::GetSubunit(TSubclassOf<UNaGameModeSubunitComponent> Class) {
	const TArray<USceneComponent*> Children = GetAttachChildren();
	for (auto & elem : Children) {
		if (UKismetMathLibrary::ClassIsChildOf(elem->GetClass(), Class))
			return static_cast<UNaGameModeSubunitComponent*> (elem);
	}
	return nullptr;
}

UNaGameModeSubunitComponent* UNaGameModeBaseComponent::GetSubunitSpecificClass(TSubclassOf<UNaGameModeSubunitComponent> Class) {
	const TArray<USceneComponent*> Children = GetAttachChildren();
	for (auto & elem : Children) {
		if (elem->GetClass()== Class)
			return static_cast<UNaGameModeSubunitComponent*> (elem);
	}
	return nullptr;
}


void UNaGameModeBaseComponent::GetAllSubunits(TArray<UNaGameModeSubunitComponent*>& Out, TSubclassOf<UNaGameModeSubunitComponent> Class) {
	const TArray<USceneComponent*> Children = GetAttachChildren();
	Out.Empty();
	for (auto& elem : Children) {
		if (UKismetMathLibrary::ClassIsChildOf(elem->GetClass(), Class))
			Out.Emplace(static_cast<UNaGameModeSubunitComponent*> (elem));
	}
	return;
}
void UNaGameModeBaseComponent::GetAllSubunitsSpecificClass(TArray<UNaGameModeSubunitComponent*>& Out, TSubclassOf<UNaGameModeSubunitComponent> Class){
	const TArray<USceneComponent*> Children = GetAttachChildren();
	Out.Empty();
	for (auto& elem : Children) {
		if (elem->GetClass() == Class)
			Out.Emplace(static_cast<UNaGameModeSubunitComponent*> (elem));
	}
	return;
}
