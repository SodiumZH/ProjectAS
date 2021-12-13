// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "NaGameModeSubunitComponent.generated.h"

class UNaGameModeBaseComponent;

UCLASS( Blueprintable, ClassGroup=(NaPublicDependencies), meta=(BlueprintSpawnableComponent) )
class NAUTILITY_API UNaGameModeSubunitComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNaGameModeSubunitComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Get game mode base component.
	UNaGameModeBaseComponent* GetBase();




};
