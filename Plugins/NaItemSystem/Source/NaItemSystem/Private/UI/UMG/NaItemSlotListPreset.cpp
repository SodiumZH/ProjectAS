// By Sodium


#include "UI/UMG/NaItemSlotListPreset.h"

TSharedRef<SWidget> UNaItemSlotListPreset::RebuildWidget() {

	TSharedRef<SWidget> Widget = Super::RebuildWidget();
	
	List->OnSlotClicked.AddUObject(this, &UNaItemSlotListPreset::EventClickedPreset);
	List->OnSlotDoubleClicked.AddUObject(this, &UNaItemSlotListPreset::EventDoubleClickedPreset);

	return Widget;
}

void UNaItemSlotListPreset::EventClickedPreset(int Position) {
	if (GetSelectedPosition() == Position)
		UnselectAll();
	else SelectSlot(Position);
}

void UNaItemSlotListPreset::EventDoubleClickedPreset(int Position, const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	return;
}