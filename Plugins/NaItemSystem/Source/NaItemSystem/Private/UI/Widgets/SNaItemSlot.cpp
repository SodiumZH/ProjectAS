#include "UI/Widgets/SNaItemSlot.h"
#include "SlateOptMacros.h"
#include "BPLibraries/NaItemStatics.h"
#include "Components/NaGameModeItemSystemComponent.h"
#include "UI/Widgets/SNaInventoryWrappedBox.h"
#include "Widgets/BoxSlots/SNaBoxSlot.h"
#include "NaUtilityMinimal.h"
#include "UI/UMG/NaItemSlotList.h"
#include "NatriumItemSystem.h"

const FNaItemSlotStyle FNaItemSlotStyle::DefaultStyle = FNaItemSlotStyle();

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
	ItemStack = InArgs._Stack.Get();
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
		OutParams.ImageBase = ItemStack ? ItemStack->GetIcon() : nullptr;
		OutParams.SuperscriptText = FText();
		if (ItemStack && bHideAmountWhenOne && ItemStack->Count == 1)
			OutParams.SubscriptText = FText();
		else if (ItemStack)
			OutParams.SubscriptText = FText::FromString(FString::FromInt(ItemStack->Count));
		else
			OutParams.SubscriptText = FText();
	}
	
}

void SNaItemSlot::ResetItemStack(UNaItemStack* NewStack) {
	ItemStack = NewStack;
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
	if (ItemSlotList && !ItemSlotList->IsInvalid() && ItemSlotList->GetContainer()->Inventory->IsValidSlot(PositionInSlotList) && BoxSlot.IsValid()) {
		BoxSlot->OnPointed.BindRaw(this, &SNaItemSlot::SlotPointedToList);
		BoxSlot->OnUnpointed.BindRaw(this, &SNaItemSlot::SlotUnpointedToList);
		BoxSlot->OnSelected.BindRaw(this, &SNaItemSlot::SlotSelectedToList);
		BoxSlot->OnUnselected.BindRaw(this, &SNaItemSlot::SlotUnselectedToList);
		BoxSlot->GetButton()->SetOnClicked(FOnClicked::CreateRaw(this, &SNaItemSlot::SlotClickedToList));
		BoxSlot->GetButton()->SetOnHovered(FSimpleDelegate::CreateRaw(this, &SNaItemSlot::SlotHoveredToList));
		BoxSlot->GetButton()->SetOnUnhovered(FSimpleDelegate::CreateRaw(this, &SNaItemSlot::SlotUnhoveredToList));


		BoxSlot->GetButton()->SetOnMouseButtonDown(FPointerEventHandler::CreateRaw(this, &SNaItemSlot::SlotMouseButtonDownToList));
		BoxSlot->GetButton()->SetOnMouseButtonUp(FPointerEventHandler::CreateRaw(this, &SNaItemSlot::SlotMouseButtonUpToList));
		BoxSlot->GetButton()->SetOnMouseMove(FPointerEventHandler::CreateRaw(this, &SNaItemSlot::SlotMouseMoveToList));
		BoxSlot->GetButton()->SetOnMouseDoubleClick(FPointerEventHandler::CreateRaw(this, &SNaItemSlot::SlotDoubleClickedToList));

	}

	// Or bind to null functions (no behavior).
	else {
		BoxSlot->OnPointed.BindRaw(this, &SNaItemSlot::ExecNoList);
		BoxSlot->OnUnpointed.BindRaw(this, &SNaItemSlot::ExecNoList);
		BoxSlot->OnSelected.BindRaw(this, &SNaItemSlot::ExecNoList);
		BoxSlot->OnUnselected.BindRaw(this, &SNaItemSlot::ExecNoList);
		BoxSlot->GetButton()->SetOnClicked(FOnClicked::CreateRaw(this, &SNaItemSlot::ExecNoListClicked));
		BoxSlot->GetButton()->SetOnHovered(FSimpleDelegate::CreateRaw(this, &SNaItemSlot::ExecNoList));
		BoxSlot->GetButton()->SetOnUnhovered(FSimpleDelegate::CreateRaw(this, &SNaItemSlot::ExecNoList));

		BoxSlot->GetButton()->SetOnMouseButtonDown(FPointerEventHandler::CreateRaw(this, &SNaItemSlot::ExecNoListMouse));
		BoxSlot->GetButton()->SetOnMouseButtonUp(FPointerEventHandler::CreateRaw(this, &SNaItemSlot::ExecNoListMouse));
		BoxSlot->GetButton()->SetOnMouseMove(FPointerEventHandler::CreateRaw(this, &SNaItemSlot::ExecNoListMouse));
		BoxSlot->GetButton()->SetOnMouseDoubleClick(FPointerEventHandler::CreateRaw(this, &SNaItemSlot::ExecNoListMouse));
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

void SNaItemSlot::SetItemSlotList(SNaInventoryWrappedBox* List, int Position) {
	check(List);
	ItemSlotList = List;
	PositionInSlotList = Position;
	BindItemSlotListEvents();

}