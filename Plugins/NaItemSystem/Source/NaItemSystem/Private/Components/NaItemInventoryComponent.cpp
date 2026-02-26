#include "Components/NaItemInventoryComponent.h"
#include "NatriumItemSystem.h"

UNaItemInventoryComponent::UNaItemInventoryComponent()
{
	bWantsInitializeComponent = true;
	Inventory = nullptr;
}

void UNaItemInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();
	Inventory = UNaItemInventory::CreateInventory(this, InitSize);
}

UNaItemInventory* UNaItemInventoryComponent::GetInventory() const
{
	return Inventory;
}

UNaItemStack* UNaItemInventoryComponent::GetSlot(int32 Slot) const
{
	if (!Inventory)
	{
		return nullptr;
	}
	return Inventory->GetSlot(Slot);
}

int32 UNaItemInventoryComponent::GiveItem(UNaItemStack* Stack)
{
	if (!Inventory)
	{
		UE_LOG(LogNaItem, Warning, TEXT("UNaItemInventoryComponent::GiveItem: Inventory is null"));
		return Stack ? Stack->Count : 0;
	}
	return Inventory->GiveItem(Stack);
}

int32 UNaItemInventoryComponent::ConsumeItemByType(UNaItemType* Type, int32 Amount)
{
	if (!Inventory)
	{
		UE_LOG(LogNaItem, Warning, TEXT("UNaItemInventoryComponent::ConsumeItemByType: Inventory is null"));
		return 0;
	}
	return Inventory->ConsumeItemByType(Type, Amount);
}
