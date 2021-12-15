// Copyright Epic Games, Inc. All Rights Reserved.

#include "NaPublicDependencies/NaGameModeBaseComponent.h"
#include "NaPublicDependencies/NaGameModeSubunitComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Utility/BPLibraries/NaObjectStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utility/DebugUtil/NaDebugUtility.h"

ANaGameMode::ANaGameMode()
{
	


}

UNaGameModeBaseComponent* UNaGameModeBaseComponent::GetNaGameModeBase(AGameModeBase* GameMode) {
	TArray<UNaGameModeBaseComponent*> Res;
	GameMode->GetComponents<UNaGameModeBaseComponent>(Res);
	checkf(Res.Num() <= 1, TEXT("Error: Duplicated NaGameModeBase component detected."));
	if (Res.Num())
		return Cast<UNaGameModeBaseComponent>(Res[0]);
	else return nullptr;
}

bool UNaGameModeBaseComponent::CheckGameModeHierarchy(bool bAssertWhenFailed) {

	ENaGameModeComponentErrorType ErrorType = ENaGameModeComponentErrorType::GMCET_Correct;

	if (!GetOwner())	// Default object, no owner
		return true;

	AGameModeBase* OwnerGameMode = dynamic_cast<AGameModeBase*>(GetOwner());	// Owner; null if invalid


	/* Check game mode type */
	if (!IsValid(OwnerGameMode)) {
		ErrorType = ENaGameModeComponentErrorType::GMCET_InvalidGameMode;
	}

	/* Check base duplication */
	if (ErrorType == ENaGameModeComponentErrorType::GMCET_Correct) {
		const TArray<UNaGameModeBaseComponent*> & AttachedToRoot = OwnerGameMode->GetRootComponent()->GetAttachChildren();	// All components attached to root; for checking duplication 
		for (int i = 0, int dup = 0; i < AttachedToRoot.Num(); ++i) {
			if (dynamic_cast<UNaGameModeBaseComponent*>(AttachedToRoot[i])) {
				++dup;
				if (dup > 1) {
					ErrorType = ENaGameModeComponentErrorType::GMCET_BaseDuplicate();
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
			if (UKismetMathLibrary::ClassIsChildOf(obj->GetClass(), UNaGameModeSubunitComponent::StaticClass()) {
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
				if (dynamic_cast<UNaGameModeSubunitComponent>(obj)->bAllowDuplication == false)) {
					ErrorType = ENaGameModeComponentErrorType::SubunitDuplicate;
					break;
				}
			}
		}
	}
	
	if (ErrorType != ENaGameModeComponentErrorType::GMCET_Correct) {
		if (bAssertWhenFailed) {
			switch (ErrorType) {
			case ENaGameModeComponentErrorType::GMCET_InvalidGameMode: {
				checkf(false, TEXT("Na Game Mode Component hierarchy error: invalid game mode. Maybe the NaGameModeBaseComponent is attached to a non-gamemode actor, or running on a client."));
			}
			case ENaGameModeComponentErrorType::GMCET_BaseDuplicate: {
				checkf(false, TEXT("Na Game Mode Component hierarchy error: base component duplicated. Only one NaGameModeBaseComponent is allowed."));
			}
			case ENaGameModeComponentErrorType::GMCET_BaseNotAttachedToRoot: {
				checkf(false, TEXT("Na Game Mode Component hierarchy error: base component is not attached to the root of game mode."));
			}
			case ENaGameModeComponentErrorType::GMCET_NonSubunitAttachedToBase: {
				checkf(false, TEXT("Na Game Mode Component hierarchy error: a non-subunit component is directly attached to the base component."));
			}
			case ENaGameModeComponentErrorType::GMCET_SubunitNotAttachedToBase: {
				checkf(false, TEXT("Na Game Mode Component hierarchy error: a subunit component is not directly attached to the base component."));
			}
			case ENaGameModeComponentErrorType::GMCET_SubunitDuplicate: {
				checkf(false, TEXT("Na Game Mode Component hierarchy error: a subunit not allowing duplication appeared twice."));
			}
			default: {
				checkNoEntry();
			}
			}
		}
		else {
			switch (ErrorType) {
			case ENaGameModeComponentErrorType::GMCET_InvalidGameMode: {
				LogError(TEXT("Na Game Mode Component hierarchy error: invalid game mode. Maybe the NaGameModeBaseComponent is attached to a non-gamemode actor, or running on a client."));
			}
			case ENaGameModeComponentErrorType::GMCET_BaseDuplicate: {
				LogError(TEXT("Na Game Mode Component hierarchy error: base component duplicated. Only one NaGameModeBaseComponent is allowed."));
			}
			case ENaGameModeComponentErrorType::GMCET_BaseNotAttachedToRoot: {
				LogError(TEXT("Na Game Mode Component hierarchy error: base component is not attached to the root of game mode."));
			}
			case ENaGameModeComponentErrorType::GMCET_NonSubunitAttachedToBase: {
				LogError(TEXT("Na Game Mode Component hierarchy error: a non-subunit component is directly attached to the base component."));
			}
			case ENaGameModeComponentErrorType::GMCET_SubunitNotAttachedToBase: {
				LogError( TEXT("Na Game Mode Component hierarchy error: a subunit component is not directly attached to the base component."));
			}
			case ENaGameModeComponentErrorType::GMCET_SubunitDuplicate: {
				LogError(TEXT("Na Game Mode Component hierarchy error: a subunit not allowing duplication appeared twice."));
			}
			default: {
				checkNoEntry();
			}
			}
		}
		return false;
	}
	return true;
}