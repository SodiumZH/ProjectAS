#include "NaItemInventory.h"
#include "NatriumItemSystem.h"

UNaItemInventory::UNaItemInventory()
{
}

int32 UNaItemInventory::GetSize() const
{
	return Slots.Num();
}

bool UNaItemInventory::IsValidSlot(int32 Slot) const
{
	return Slot >= 0 && Slot < Slots.Num();
}

UNaItemStack* UNaItemInventory::GetSlot(int32 Slot) const
{
	if (!IsValidSlot(Slot))
	{
		return nullptr;
	}
	return Slots[Slot];
}

bool UNaItemInventory::IsSlotEmpty(int32 Slot) const
{
	if (!IsValidSlot(Slot))
	{
		return true;
	}
	UNaItemStack* Stack = Slots[Slot];
	return !Stack || Stack->IsEmpty();
}

int32 UNaItemInventory::FindFirstEmptySlot() const
{
	for (int32 i = 0; i < Slots.Num(); ++i)
	{
		if (IsSlotEmpty(i))
		{
			return i;
		}
	}
	return -1;
}

int32 UNaItemInventory::FindItemByType(const UNaItemType* Type, int32 StartFrom) const
{
	if (!Type)
	{
		return -1;
	}
	for (int32 i = FMath::Max(0, StartFrom); i < Slots.Num(); ++i)
	{
		UNaItemStack* Stack = Slots[i];
		if (Stack && !Stack->IsEmpty() && Stack->ItemType == Type)
		{
			return i;
		}
	}
	return -1;
}

int32 UNaItemInventory::CountItemByType(const UNaItemType* Type) const
{
	if (!Type)
	{
		return 0;
	}
	int32 Total = 0;
	for (const UNaItemStack* Stack : Slots)
	{
		if (Stack && !Stack->IsEmpty() && Stack->ItemType == Type)
		{
			Total += Stack->Count;
		}
	}
	return Total;
}

void UNaItemInventory::SetSlot(int32 Slot, UNaItemStack* Stack)
{
	if (!IsValidSlot(Slot))
	{
		UE_LOG(LogNaItem, Warning, TEXT("UNaItemInventory::SetSlot: Invalid slot %d"), Slot);
		return;
	}
	if (Stack && Stack->IsEmpty())
	{
		Slots[Slot] = nullptr;
	}
	else
	{
		Slots[Slot] = Stack;
	}
}

void UNaItemInventory::ClearSlot(int32 Slot)
{
	if (!IsValidSlot(Slot))
	{
		UE_LOG(LogNaItem, Warning, TEXT("UNaItemInventory::ClearSlot: Invalid slot %d"), Slot);
		return;
	}
	Slots[Slot] = nullptr;
}

void UNaItemInventory::SwapSlots(int32 SlotA, int32 SlotB)
{
	if (!IsValidSlot(SlotA) || !IsValidSlot(SlotB))
	{
		UE_LOG(LogNaItem, Warning, TEXT("UNaItemInventory::SwapSlots: Invalid slot(s) %d, %d"), SlotA, SlotB);
		return;
	}
	Slots.SwapMemory(SlotA, SlotB);
}

bool UNaItemInventory::MergeSlots(int32 SourceSlot, int32 DestSlot)
{
	if (!IsValidSlot(SourceSlot) || !IsValidSlot(DestSlot))
	{
		return false;
	}
	UNaItemStack* Source = Slots[SourceSlot];
	UNaItemStack* Dest = Slots[DestSlot];

	if (!Source || Source->IsEmpty())
	{
		return false;
	}

	if (!Dest || Dest->IsEmpty())
	{
		// Move entire source to dest slot
		Slots[DestSlot] = Source;
		Slots[SourceSlot] = nullptr;
		return true;
	}

	const int32 Moved = Dest->MergeFrom(Source, true);
	if (Moved > 0 && Source->IsEmpty())
	{
		Slots[SourceSlot] = nullptr;
	}
	return Moved > 0;
}

int32 UNaItemInventory::GiveItem(UNaItemStack* Stack)
{
	if (!Stack || Stack->IsEmpty())
	{
		return 0;
	}

	// First pass: merge into existing stacks of the same type
	int32 StartSearch = 0;
	while (Stack->Count > 0)
	{
		const int32 SlotIdx = FindItemByType(Stack->ItemType, StartSearch);
		if (SlotIdx == -1)
		{
			break;
		}
		UNaItemStack* ExistingStack = Slots[SlotIdx];
		if (ExistingStack && !ExistingStack->IsFullStack() && ExistingStack->CanStackWith(Stack, true))
		{
			ExistingStack->MergeFrom(Stack, true);
		}
		StartSearch = SlotIdx + 1;
	}

	// Second pass: place remainder into empty slots
	while (Stack->Count > 0)
	{
		const int32 EmptySlot = FindFirstEmptySlot();
		if (EmptySlot == -1)
		{
			break;
		}

		const int32 MaxSize = Stack->GetMaxStackSize();
		if (Stack->Count <= MaxSize)
		{
			// All remaining items fit in one slot
			Slots[EmptySlot] = UNaItemStack::CreateItemStack(this, Stack->ItemType, Stack->Count);
			Stack->SetCount(0);
		}
		else
		{
			// Fill a full stack in this slot
			Slots[EmptySlot] = UNaItemStack::CreateItemStack(this, Stack->ItemType, MaxSize);
			Stack->Shrink(MaxSize);
		}
	}

	return Stack->Count;
}

bool UNaItemInventory::CanGiveItemComplete(const UNaItemStack* Stack) const
{
	if (!Stack || Stack->IsEmpty())
	{
		return true;
	}

	int32 Remaining = Stack->Count;

	// Count space in existing stacks
	for (const UNaItemStack* ExistingStack : Slots)
	{
		if (Remaining <= 0)
		{
			break;
		}
		if (ExistingStack && !ExistingStack->IsEmpty() && ExistingStack->CanStackWith(Stack, true))
		{
			Remaining -= ExistingStack->GetRemainingCapacity();
		}
	}

	if (Remaining <= 0)
	{
		return true;
	}

	// Count available empty slots
	const int32 MaxSize = Stack->GetMaxStackSize();
	for (const UNaItemStack* SlotEntry : Slots)
	{
		if (Remaining <= 0)
		{
			break;
		}
		if (!SlotEntry || SlotEntry->IsEmpty())
		{
			Remaining -= MaxSize;
		}
	}

	return Remaining <= 0;
}

bool UNaItemInventory::GiveItemComplete(UNaItemStack* Stack)
{
	if (!Stack || Stack->IsEmpty())
	{
		return true;
	}

	if (!CanGiveItemComplete(Stack))
	{
		return false;
	}

	GiveItem(Stack);
	return true;
}

int32 UNaItemInventory::ConsumeItemByType(UNaItemType* Type, int32 Amount)
{
	if (!Type || Amount <= 0)
	{
		return 0;
	}

	int32 Remaining = Amount;
	for (int32 i = 0; i < Slots.Num() && Remaining > 0; ++i)
	{
		UNaItemStack* Stack = Slots[i];
		if (!Stack || Stack->IsEmpty() || Stack->ItemType != Type)
		{
			continue;
		}
		const int32 ToRemove = FMath::Min(Remaining, Stack->Count);
		Stack->Shrink(ToRemove);
		Remaining -= ToRemove;
		if (Stack->IsEmpty())
		{
			Slots[i] = nullptr;
		}
	}

	return Amount - Remaining;
}

bool UNaItemInventory::CanConsumeItemByType(const UNaItemType* Type, int32 Amount) const
{
	return CountItemByType(Type) >= Amount;
}

UNaItemStack* UNaItemInventory::TakeFromSlot(int32 Slot, int32 Amount)
{
	if (!IsValidSlot(Slot) || Amount <= 0)
	{
		return nullptr;
	}

	UNaItemStack* Stack = Slots[Slot];
	if (!Stack || Stack->IsEmpty())
	{
		return nullptr;
	}

	const int32 ActualAmount = FMath::Min(Amount, Stack->Count);
	UNaItemStack* NewStack = UNaItemStack::CreateItemStack(this, Stack->ItemType, ActualAmount);
	Stack->Shrink(ActualAmount);
	if (Stack->IsEmpty())
	{
		Slots[Slot] = nullptr;
	}
	return NewStack;
}

bool UNaItemInventory::Resize(int32 NewSize, bool bForce)
{
	if (NewSize < 0)
	{
		UE_LOG(LogNaItem, Warning, TEXT("UNaItemInventory::Resize: NewSize %d is negative"), NewSize);
		return false;
	}

	const int32 OldSize = Slots.Num();
	if (NewSize == OldSize)
	{
		return true;
	}

	if (NewSize < OldSize)
	{
		// Check for occupied slots in the range to be removed
		if (!bForce)
		{
			for (int32 i = NewSize; i < OldSize; ++i)
			{
				if (!IsSlotEmpty(i))
				{
					UE_LOG(LogNaItem, Warning,
						TEXT("UNaItemInventory::Resize: Slot %d is occupied; use bForce to force shrink"), i);
					return false;
				}
			}
		}
		Slots.SetNum(NewSize);
	}
	else
	{
		// Growing — add null slots
		Slots.SetNum(NewSize);
	}

	return true;
}

void UNaItemInventory::ClearAll()
{
	for (int32 i = 0; i < Slots.Num(); ++i)
	{
		Slots[i] = nullptr;
	}
}

void UNaItemInventory::ClearEmptyStacks()
{
	for (int32 i = 0; i < Slots.Num(); ++i)
	{
		UNaItemStack* Stack = Slots[i];
		if (Stack && Stack->IsEmpty())
		{
			Slots[i] = nullptr;
		}
	}
}

UNaItemInventory* UNaItemInventory::CreateInventory(UObject* Outer, int32 Size)
{
	UNaItemInventory* Inv = NewObject<UNaItemInventory>(Outer ? Outer : GetTransientPackage());
	Inv->Slots.SetNum(FMath::Max(0, Size));
	return Inv;
}
