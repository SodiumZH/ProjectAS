// By Sodium

#include "Components/NaItemContainerComponent.h"
#include "NaItemEntry.h"
#include "NaItemContainer.h"

// Sets default values for this component's properties
UNaItemContainerComponent::UNaItemContainerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	// ...
}

void UNaItemContainerComponent::InitializeComponent() {
	Super::InitializeComponent();
	if (bInitFromInitializer)
		Container = FNaItemContainer(InitSize, Initializer);
	else
		Container = FNaItemContainer(InitSize);
}

// Called when the game starts
void UNaItemContainerComponent::BeginPlay()
{
	Super::BeginPlay();

	
	// ...
	
}


// Called every frame
void UNaItemContainerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

