#pragma once
#include "UI/NaItemInventoryWidget.h"

UNaItemInventory* INaItemInventoryWidget::GetInventory() const
{
	return this->GetNativeInterface().Pin().Get()->GetInventory();
}

int32 INaItemInventoryWidget::GetSlotAmount() const
{
	return this->GetNativeInterface().Pin().Get()->GetSlotAmount();
}

FVector2D INaItemInventoryWidget::GetSlotPosition(int32 index) const
{
	return this->GetNativeInterface().Pin().Get()->GetSlotPosition(index);
}
