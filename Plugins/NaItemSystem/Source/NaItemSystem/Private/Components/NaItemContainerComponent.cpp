// By Sodium

#include "Components/NaItemContainerComponent.h"
#include "NaItemContainer.h"
#include "UI/UMG/NaItemSlotList.h"
#include "UI/Widgets/SNaItemSlotList.h"
#include "NaItemEffect.h"

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

ENaItemContainerUsageResult UNaItemContainerComponent::UseItemFromSelf(int Position, AActor* Target) {
	checkf(Container.IsInSize(Position), TEXT("NaItemContainerComponent use item error: invalid position. Position: %d"), Position);
	if(IsValid(GetOwner()))
		return Container.UseItem(this, Position, GetOwner(), Target);
	else return ENaItemContainerUsageResult::ICUR_Failed;
}
