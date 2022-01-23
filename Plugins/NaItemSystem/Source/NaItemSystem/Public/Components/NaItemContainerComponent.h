// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NaItemContainerComponent.generated.h"

struct FNaItemContainer;

/** NaItemContainerComponent is a component bringing an item container for attachment to actors.
*/
UCLASS( ClassGroup=(NaItemSystem), meta=(BlueprintSpawnableComponent) )
class NAITEMSYSTEM_API UNaItemContainerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNaItemContainerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ItemContainer|ContainerInitialization")
	int InitSize = 64;

	UPROPERTY(BlueprintReadWrite)
	FNaItemContainer Container;

};
