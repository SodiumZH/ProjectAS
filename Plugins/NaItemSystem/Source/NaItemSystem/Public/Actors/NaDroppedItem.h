// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NaDroppedItem.generated.h"

UCLASS()
class NAITEMSYSTEM_API ANaDroppedItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANaDroppedItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
