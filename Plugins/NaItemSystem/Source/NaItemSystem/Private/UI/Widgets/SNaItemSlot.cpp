// By Sodium


#include "UI/Widgets/SNaItemSlot.h"
#include "SlateOptMacros.h"
#include "BPLibraries/NaItemStatics.h"
#include "BPLibraries/NaItemDataStatics.h"
#include "Components/NaGameModeItemSystemComponent.h"
#include "UI/Widgets/SNaItemSlotList.h"
#include "Widgets/BoxSlots/SNaBoxSlot.h"
#include "NaUtilityMinimal.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SNaItemSlot::Construct(const FArguments& InArgs)
{

	GMComponent = UNaItemStatics::GetGameModeItemSystemComponent(InArgs._WorldContext.Get());
	
#if NAPACK_DO_COMMON_CHECK
	if (!IsValid(GMComponent)) {
		UE_LOG(LogNaItem, Error, TEXT("NaItemSlot: invalid game mode component."));
		return;
	}
#endif

	/* Copy properties */
	ItemEntryPtr = InArgs._EntryPtr.Get() ;
	bIsDisabled = InArgs._bIsDisabled.Get();
	bHideAmountWhenOne = InArgs._bHideAmountWhenOne.Get();
	Font = InArgs._Font.Get();
	Size = InArgs._Size.Get();

	/* Start NaItemSlotList part */
	SlotListRef = InArgs.InSlotList.Get();
	PositionInSlotList = InArgs.PositionInSlotList.Get();
	BindItemSlotListEvents();	// This function contains validity check, so no extra check
	/* End NaItemSlotList part */

	ParamsFromEntry(TempParams);

	ChildSlot
	[
		SAssignNew(BoxSlot, SNaBoxSlot)
		.Params(&TempParams)
		.SubscriptFont(Font)
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SNaItemSlot::ParamsFromEntry(FNaBoxSlotParams& OutParams) {

#if NAPACK_DO_VERBOSE_CHECK
	if (!IsValid(GMComponent)) {
		UE_LOG(LogNaItem, Error, TEXT("NaItemSlot::ParamsFromEntry: invalid game mode component."));
		return;
	}
#endif

	if (bIsDisabled) {
		OutParams.ImageFrame = GMComponent->DefaultSlotSettings.IconBorderDisabled;
		OutParams.ImageSelected = GMComponent->DefaultSlotSettings.bCanSelectDisabled ? GMComponent->DefaultSlotSettings.IconSelected : nullptr;
		OutParams.ImagePointed = GMComponent->DefaultSlotSettings.bCanPointAtDisabled ? GMComponent->DefaultSlotSettings.IconPointed : nullptr;
		OutParams.ImageBase = GMComponent->DefaultSlotSettings.IconDisabled;
		OutParams.SuperscriptText = FText();
		OutParams.SubscriptText = FText();

	}
	else if (IsEmpty()) {
		OutParams.ImageFrame = GMComponent->DefaultSlotSettings.IconBorder;
		OutParams.ImageSelected = GMComponent->DefaultSlotSettings.IconSelected;
		OutParams.ImagePointed = GMComponent->DefaultSlotSettings.IconPointed;
		OutParams.ImageBase = GMComponent->DefaultSlotSettings.IconEmpty;
		OutParams.SuperscriptText = FText();
		OutParams.SubscriptText = FText();

	}
	else {
		OutParams.ImageFrame = GMComponent->DefaultSlotSettings.IconBorder;
		OutParams.ImageSelected = GMComponent->DefaultSlotSettings.IconSelected;
		OutParams.ImagePointed = GMComponent->DefaultSlotSettings.IconPointed;
		OutParams.ImageBase = UNaItemDataStatics::GetItemDisplayDataFromID(GMComponent, ItemEntryPtr->TypeDescriptor.ItemTypeID).BrushImage;
		OutParams.SuperscriptText = FText();
		OutParams.SubscriptText = (bHideAmountWhenOne && ItemEntryPtr->Amount == 1) ? FText() : FText::FromString(FString::FromInt(ItemEntryPtr->Amount));
	}
	
}

void SNaItemSlot::ResetItemEntry(TSharedPtr<FNaItemEntry> NewEntryPtr) {
	ItemEntryPtr = NewEntryPtr;
	ParamsFromEntry(TempParams);
	BoxSlot->Reset(TempParams);
}

void SNaItemSlot::SetDisabled(bool NewDisabledState) {
	bIsDisabled = NewDisabledState;
	ParamsFromEntry(TempParams);
	BoxSlot->Reset(TempParams);
}

void SNaItemSlot::BindItemSlotListEvents() {

	if (ItemSlotList.IsValid() && !ItemSlotList.Pin()->IsInvalid() && ItemSlotList.Pin()->GetContainer()->Container.IsInSize(PositionInSlotList)) {
		BoxSlot->OnPointed.BindSP(this, &SlotPointedToList);
		BoxSlot->OnUnpointed.BindSP(this, &SlotUnpointedToList);
		BoxSlot->OnSelected.BindSP(this, &SlotSelectedToList);
		BoxSlot->OnUnselected.BindSP(this, &SlotUnselectedToList);
	}

}

void SNaItemSlot::SlotPointedToList() {
	ItemSlotList.Pin()->OnSlotPointed.ExecuteIfBound(PositionInSlotList);
}

void SNaItemSlot::SlotUnpointedToList() {
	ItemSlotList.Pin()->OnSlotUnpointed.ExecuteIfBound(PositionInSlotList);
};

void SNaItemSlot::SlotSelectedToList() {
	ItemSlotList.Pin()->OnSlotSelected.ExecuteIfBound(PositionInSlotList);
}

void SNaItemSlot::SlotUnselectedToList() {
	ItemSlotList.Pin()->OnSlotUnselected.ExecuteIfBound(PositionInSlotList);
}