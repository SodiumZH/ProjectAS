// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NaPublicDependencyStatics.generated.h"

/**
 * 
 */

enum class ENaGameModeComponentErrorType:uint8;

UCLASS()
class NAUTILITY_API UNaPublicDependencyStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/* Check if NaGameModeBaseComponent and subunits have correct hierarchy.
	* Correct hierarchy: NaGameModeBaseComponent attached to root; all subunits attach to base component. No other components should be attached to NaGameModeBaseComponent.
	* Also check: Game mode base component not duplicated.
	* @Param WorldContext Any actor that can refer its world. Usually you can use "self".
	* @Param bAssertWhenFailed If true, it will CRASH when check failed.
	* @ReturnValue Whether check succeeded.
	*/
	UFUNCTION(BlueprintCallable, Category = "NaPublicDependencies")
	ENaGameModeComponentErrorType CheckGameModeHierarchy(AActor* WorldContext, bool bAssertWhenFailed = false);

	/**
	* Get NaGameModeBase.
	* If there is no NaGameModeBase, return null.
	* ASSERT if duplicated!
	* @Param WorldContext Any actor that can get world.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get NaGameModeBase"), Category = "NaPublicDependencies")
	static UNaGameModeBaseComponent* GetNaGameModeBase(AActor* WorldContext);


	/**
	* Get NaGameMode subunit from class.
	* If there is not one, return null.
	* If there is multiple, return the first hit one.
	* @Param WorldContext Any actor that can get world.
	* @Param Class Subunit class. Subclasses are included.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get NaGameMode Subunit"), Category = "NaPublicDependencies")
	static UNaGameModeSubunitComponent* GetNaGameModeSubunit(AActor* WorldContext, TSubclassOf<UNaGameModeSubunitComponent> Class);

	/**
	* Get NaGameMode subunit from class. NOT including subclasses.
	* If there is not one, return null.
	* If there is multiple, return the first hit one.
	* @Param WorldContext Any actor that can get world.
	* @Param Class Subunit class. Subclasses are NOT included.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get NaGameMode Subunit(Specific Class)"), Category = "NaPublicDependencies")
	static UNaGameModeSubunitComponent* GetNaGameModeSubunitSpecific(AActor* WorldContext, TSubclassOf<UNaGameModeSubunitComponent> SpecificClass);

	/**
	* Get all NaGameMode subunits from class.
	* @Param WorldContext Any actor that can get world.
	* @Param Class Subunit class. Subclasses are included.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get All NaGameMode Subunits"), Category = "NaPublicDependencies")
	static void GetAllNaGameModeSubunits(TArray<UNaGameModeSubunitComponent*>& Out, AActor* WorldContext, TSubclassOf<UNaGameModeSubunitComponent> Class);

	/**
	* Get all NaGameMode subunits from class. NOT including subclasses.
	* @Param WorldContext Any actor that can get world.
	* @Param Class Subunit class. Subclasses are NOT included.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get All NaGameMode Subunits(Specific Class)"), Category = "NaPublicDependencies")
	static void GetAllNaGameModeSubunitsSpecific(TArray<UNaGameModeSubunitComponent*>& Out, AActor* WorldContext, TSubclassOf<UNaGameModeSubunitComponent> SpecificClass);

};
