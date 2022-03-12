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

	if (ItemSlotList.IsValid() && !ItemSlotList.Pin()->IsInvalid() && ItemSlotList.Pin()->GetContainer()->Container.IsInSize(PositionInSlotList) && BoxSlot.IsValid()) {
		BoxSlot->OnPointed.BindRaw(this, &SNaItemSlot::SlotPointedToList);
		BoxSlot->OnUnpointed.BindRaw(this, &SNaItemSlot::SlotUnpointedToList);
		BoxSlot->OnSelected.BindRaw(this, &SNaItemSlot::SlotSelectedToList);
		BoxSlot->OnUnselected.BindRaw(this, &SNaItemSlot::SlotUnselectedToList);
		BoxSlot->GetButton()->SetOnClicked(FSimpleDelegate::CreateRaw(this, &SNaItemSlot::SlotClickedToList));
		BoxSlot->GetButton()->SetOnHovered(FSimpleDelegate::CreateRaw(this, &SNaItemSlot::SlotHoveredToList));
		BoxSlot->GetButton()->SetOnUnhovered(FSimpleDelegate::CreateRaw(this, &SNaItemSlot::SlotUnhoveredToList));

		BoxSlot->GetButton()->SetOnMouseButtonDown(SNaItemSlot::SlotMouseButtonDownToList);
		BoxSlot->GetButton()->SetOnMouseButtonUp(SNaItemSlot::SlotMouseButtonUpToList);
		BoxSlot->GetButton()->SetOnMouseMove(SNaItemSlot::SlotMouseMoveToList);
	}
	else {
		BoxSlot->OnPointed.BindRaw(this, &SNaItemSlot::ExecNoList);
		BoxSlot->OnUnpointed.BindRaw(this, &SNaItemSlot::ExecNoList);
		BoxSlot->OnSelected.BindRaw(this, &SNaItemSlot::ExecNoList);
		BoxSlot->OnUnselected.BindRaw(this, &SNaItemSlot::ExecNoList);
		BoxSlot->GetButton()->SetOnClicked(FSimpleDelegate::CreateRaw(this, &SNaItemSlot::ExecNoListMouse));
		BoxSlot->GetButton()->SetOnHovered(FSimpleDelegate::CreateRaw(this, &SNaItemSlot::ExecNoListMouse));
		BoxSlot->GetButton()->SetOnUnhovered(FSimpleDelegate::CreateRaw(this, &SNaItemSlot::ExecNoListMouse));

		BoxSlot->GetButton()->SetOnMouseButtonDown(this, &SNaItemSlot::ExecNoListMouse);
		BoxSlot->GetButton()->SetOnMouseButtonUp(SNaItemSlot::SlotMouseButtonUpToList);
		BoxSlot->GetButton()->SetOnMouseMove(SNaItemSlot::SlotMouseMoveToList);
	}
}

/* Events */

void SNaItemSlot::SlotPointedToList() {
	ItemSlotList.Pin()->OnSlotPointed.ExecuteIfBound(PositionInSlotList);
}
void SNaItemSlot::SlotUnpointedToList() {
	ItemSlotList.Pin()->OnSlotUnpointed.ExecuteIfBound(PositionInSlotList);
}
void SNaItemSlot::SlotSelectedToList() {
	ItemSlotList.Pin()->OnSlotSelected.ExecuteIfBound(PositionInSlotList);
}
void SNaItemSlot::SlotUnselectedToList() {
	ItemSlotList.Pin()->OnSlotUnselected.ExecuteIfBound(PositionInSlotList);
}
void SNaItemSlot::SlotClickedToList() {
	ItemSlotList.Pin()->OnSlotClicked.ExecuteIfBound(PositionInSlotList);
}
void SNaItemSlot::SlotHoveredToList() {
	// This will override binding in SNaBoxSlot::Construct(), So the previously bound function should be executed here
	BoxSlot->SetPointed(true);
	ItemSlotList.Pin()->OnSlotHovered.ExecuteIfBound(PositionInSlotList);
}
void SNaItemSlot::SlotUnhoveredToList() {
	// This will override binding in SNaBoxSlot::Construct(), So the previously bound function should be executed here
	BoxSlot->SetPointed(false);
	ItemSlotList.Pin()->OnSlotUnhovered.ExecuteIfBound(PositionInSlotList);
}

FReply SNaItemSlot::SlotMouseButtonDownToList(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	ItemSlotList.Pin()->OnSlotMouseButtonDown.ExecuteIfBound(PositionInSlotList, MyGeometry, MouseEvent);
	return FReply::Handled();
}
FReply SNaItemSlot::SlotMouseButtonUpToList(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	ItemSlotList.Pin()->OnSlotMouseButtonUp.ExecuteIfBound(PositionInSlotList, MyGeometry, MouseEvent);
	return FReply::Handled();
}
FReply SNaItemSlot::SlotMouseMoveToList(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	ItemSlotList.Pin()->OnSlotMouseMove.ExecuteIfBound(PositionInSlotList, MyGeometry, MouseEvent);
	return FReply::Handled();
}



/* Events end */

void SNaItemSlot::SetItemSlotList(TSharedPtr<SNaItemSlotList> List, int Position) {
	ItemSlotList = List;
	PositionInSlotList = Position;
	BindItemSlotListEvents();

}