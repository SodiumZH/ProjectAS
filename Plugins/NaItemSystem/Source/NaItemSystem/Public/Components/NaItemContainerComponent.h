// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NaItemEntry.h"
#include "NaItemContainerComponent.generated.h"

struct FNaItemContainer;

/** NaItemContainerComponent is a component bringing an item container for attachment to actors.
*/
UCLASS( ClassGroup=(NaItemSystem), meta=(BlueprintSpawnableComponent), Blueprintable )
class NAITEMSYSTEM_API UNaItemContainerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNaItemContainerComponent();

	virtual void InitializeComponent() override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Initial size */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ItemContainer|ContainerInitialization")
	int InitSize = 64;

	/* If true, container will be initialized with InitializerContent */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ItemContainer|ContainerInitialization")
	bool bInitFromInitializer = true;

	/* Initializer if bInitFromInitializer is true. Key integer = index. Indexes out of size will be ignored. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ItemContainer|ContainerInitialization")
	TMap<int, FNaItemEntry> Initializer;

	UPROPERTY(BlueprintReadWrite)
	FNaItemContainer Container;

};
