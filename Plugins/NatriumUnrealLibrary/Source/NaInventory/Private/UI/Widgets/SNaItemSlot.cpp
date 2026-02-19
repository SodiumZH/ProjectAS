// By Sodium


#include "UI/Widgets/SNaItemSlot.h"
#include "SlateOptMacros.h"
#include "BPLibraries/NaItemStatics.h"
#include "BPLibraries/NaItemDataStatics.h"
#include "Components/NaGameModeItemSystemComponent.h"
#include "UI/Widgets/SNaItemSlotList.h"
#include "Widgets/BoxSlots/SNaBoxSlot.h"
#include "NaUtilityMinimal.h"
#include "UI/UMG/NaItemSlotList.h"


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
	StylePtr = InArgs._StylePtr.Get();
	if (StylePtr) {
		Font = StylePtr->SubscriptFont;
		bHideAmountWhenOne = StylePtr->bHideAmountWhenOne;
		SubscriptColor = StylePtr->SubscriptColor;
	}

	MakeParams(TempParams);

	ChildSlot
	[
		SAssignNew(BoxSlot, SNaBoxSlot)
		.Params(&TempParams)
		.SubscriptFont(Font)
		.SubscriptColor(SubscriptColor)
	];

}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SNaItemSlot::MakeParams(FNaBoxSlotParams& OutParams) {

	if (!IsValid(GMComponent)) {
		UE_LOG(LogNaItem, Error, TEXT("NaItemSlot::MakeParams: invalid game mode component."));
		return;
	}
	if (!StylePtr) {
		UE_LOG(LogNaItem, Error, TEXT("NaItemSlot::MakeParams: Missing Style reference."));
		return;
	}


	if (bIsDisabled) {
		OutParams.ImageFrame = StylePtr->IconBorderDisabled;
		OutParams.ImageSelected = StylePtr->bCanSelectDisabled ? StylePtr->IconSelected : nullptr;
		OutParams.ImagePointed = StylePtr->bCanPointAtDisabled ? StylePtr->IconPointed : nullptr;
		OutParams.ImageBase = StylePtr->IconDisabled;
		OutParams.SuperscriptText = FText();
		OutParams.SubscriptText = FText();

	}
	else if (IsEmpty()) {
		OutParams.ImageFrame = StylePtr->IconBorder;
		OutParams.ImageSelected = StylePtr->IconSelected;
		OutParams.ImagePointed = StylePtr->IconPointed;
		OutParams.ImageBase = StylePtr->IconEmpty;
		OutParams.SuperscriptText = FText();
		OutParams.SubscriptText = FText();

	}
	else {
		OutParams.ImageFrame = StylePtr->IconBorder;
		OutParams.ImageSelected = StylePtr->IconSelected;
		OutParams.ImagePointed = StylePtr->IconPointed;
		OutParams.ImageBase = UNaItemDataStatics::GetItemDisplayDataFromID(GMComponent, ItemEntryPtr->TypeDescriptor.ItemTypeID).BrushImage;
		OutParams.SuperscriptText = FText();
		OutParams.SubscriptText = (bHideAmountWhenOne && ItemEntryPtr->Amount == 1) ? FText() : FText::FromString(FString::FromInt(ItemEntryPtr->Amount));
	}
	
}

void SNaItemSlot::ResetItemEntry(TSharedPtr<FNaItemEntry> NewEntryPtr) {
	ItemEntryPtr = NewEntryPtr;
	MakeParams(TempParams);
	BoxSlot->Reset(TempParams);
}

void SNaItemSlot::SetDisabled(bool NewDisabledState) {
	bIsDisabled = NewDisabledState;
	MakeParams(TempParams);
	BoxSlot->Reset(TempParams);
}

void SNaItemSlot::BindItemSlotListEvents() {

	// If in item slot list, bind mouse events
	if (ItemSlotList && !ItemSlotList->IsInvalid() && ItemSlotList->GetContainer()->Container.IsInSize(PositionInSlotList) && BoxSlot.IsValid()) {
		BoxSlot->OnPointed.BindSP(this, &SNaItemSlot::SlotPointedToList);
		BoxSlot->OnUnpointed.BindSP(this, &SNaItemSlot::SlotUnpointedToList);
		BoxSlot->OnSelected.BindSP(this, &SNaItemSlot::SlotSelectedToList);
		BoxSlot->OnUnselected.BindSP(this, &SNaItemSlot::SlotUnselectedToList);
		BoxSlot->GetButton()->SetOnClicked(FOnClicked::CreateSP(this, &SNaItemSlot::SlotClickedToList));
		BoxSlot->GetButton()->SetOnHovered(FSimpleDelegate::CreateSP(this, &SNaItemSlot::SlotHoveredToList));
		BoxSlot->GetButton()->SetOnUnhovered(FSimpleDelegate::CreateSP(this, &SNaItemSlot::SlotUnhoveredToList));


		BoxSlot->GetButton()->SetOnMouseButtonDown(FPointerEventHandler::CreateSP(this, &SNaItemSlot::SlotMouseButtonDownToList));
		BoxSlot->GetButton()->SetOnMouseButtonUp(FPointerEventHandler::CreateSP(this, &SNaItemSlot::SlotMouseButtonUpToList));
		BoxSlot->GetButton()->SetOnMouseMove(FPointerEventHandler::CreateSP(this, &SNaItemSlot::SlotMouseMoveToList));
		BoxSlot->GetButton()->SetOnMouseDoubleClick(FPointerEventHandler::CreateSP(this, &SNaItemSlot::SlotDoubleClickedToList));

	}

	// Or bind to null functions (no behavior).
	else {
		BoxSlot->OnPointed.BindSP(this, &SNaItemSlot::ExecNoList);
		BoxSlot->OnUnpointed.BindSP(this, &SNaItemSlot::ExecNoList);
		BoxSlot->OnSelected.BindSP(this, &SNaItemSlot::ExecNoList);
		BoxSlot->OnUnselected.BindSP(this, &SNaItemSlot::ExecNoList);
		BoxSlot->GetButton()->SetOnClicked(FOnClicked::CreateSP(this, &SNaItemSlot::ExecNoListClicked));
		BoxSlot->GetButton()->SetOnHovered(FSimpleDelegate::CreateSP(this, &SNaItemSlot::ExecNoList));
		BoxSlot->GetButton()->SetOnUnhovered(FSimpleDelegate::CreateSP(this, &SNaItemSlot::ExecNoList));

		BoxSlot->GetButton()->SetOnMouseButtonDown(FPointerEventHandler::CreateSP(this, &SNaItemSlot::ExecNoListMouse));
		BoxSlot->GetButton()->SetOnMouseButtonUp(FPointerEventHandler::CreateSP(this, &SNaItemSlot::ExecNoListMouse));
		BoxSlot->GetButton()->SetOnMouseMove(FPointerEventHandler::CreateSP(this, &SNaItemSlot::ExecNoListMouse));
		BoxSlot->GetButton()->SetOnMouseDoubleClick(FPointerEventHandler::CreateSP(this, &SNaItemSlot::ExecNoListMouse));
	}
}

/* Events */

void SNaItemSlot::SlotPointedToList() {
	ItemSlotList->OnSlotPointed.Broadcast(PositionInSlotList);
}
void SNaItemSlot::SlotUnpointedToList() {
	ItemSlotList->OnSlotUnpointed.Broadcast(PositionInSlotList);
}
void SNaItemSlot::SlotSelectedToList() {
	ItemSlotList->OnSlotSelected.Broadcast(PositionInSlotList);
}
void SNaItemSlot::SlotUnselectedToList() {
	ItemSlotList->OnSlotUnselected.Broadcast(PositionInSlotList);
}
FReply SNaItemSlot::SlotClickedToList() {
	ItemSlotList->OnSlotClicked.Broadcast(PositionInSlotList);
	return FReply::Handled();
}
void SNaItemSlot::SlotHoveredToList() {
	// This will override binding in SNaBoxSlot::Construct(), So the previously bound function should be executed here
	BoxSlot->SetPointed(true);
	ItemSlotList->OnSlotHovered.Broadcast(PositionInSlotList);
}
void SNaItemSlot::SlotUnhoveredToList() {
	// This will override binding in SNaBoxSlot::Construct(), So the previously bound function should be executed here
	BoxSlot->SetPointed(false);
	ItemSlotList->OnSlotUnhovered.Broadcast(PositionInSlotList);
}

FReply SNaItemSlot::SlotMouseButtonDownToList(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	ItemSlotList->OnSlotMouseButtonDown.Broadcast(PositionInSlotList, MyGeometry, MouseEvent);
	return FReply::Handled();
}
FReply SNaItemSlot::SlotMouseButtonUpToList(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	ItemSlotList->OnSlotMouseButtonUp.Broadcast(PositionInSlotList, MyGeometry, MouseEvent);
	return FReply::Handled();
}
FReply SNaItemSlot::SlotMouseMoveToList(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	ItemSlotList->OnSlotMouseMove.Broadcast(PositionInSlotList, MyGeometry, MouseEvent);
	return FReply::Handled();
}
FReply SNaItemSlot::SlotDoubleClickedToList(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	ItemSlotList->OnSlotDoubleClicked.Broadcast(PositionInSlotList, MyGeometry, MouseEvent);
	return FReply::Handled();
}

/* Events end */

void SNaItemSlot::SetItemSlotList(SNaItemSlotList* List, int Position) {
	check(List);
	ItemSlotList = List;
	PositionInSlotList = Position;
	BindItemSlotListEvents();

}