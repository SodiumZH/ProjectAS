// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/BoxSlots/SNaBoxSlot.h"
#include "NaItemStack.h"
#include "Fonts/SlateFontInfo.h"
#include "SNaItemSlot.generated.h"

class UNaGameModeItemSystemComponent;
class SNaInventoryWrappedBox;

/** 
 * Item slot styles that are often constant for a whole slot list.
* It can also be used to define single slots, and is not limited in slot lists.
* This struct is used in SNaItemSlot input params which can easily defined in slot list BPs and transferred as a whole to each slot.
*/
USTRUCT(BlueprintType)
struct NAITEMSYSTEM_API FNaItemSlotStyle {

	GENERATED_BODY()

public:
	
	static const FNaItemSlotStyle DefaultStyle;
	
	/* Slot size (in pixel). */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D SlotSize = FVector2D(64.f, 64.f);

	/* Base icon for empty slots */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UObject* IconEmpty = nullptr;

	/* Border icon for empty and occupied slots, stacking above the base icon */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UObject* IconBorder = nullptr;

	/* Base icon for disabled slot */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UObject* IconDisabled = nullptr;

	/* Border icon for disabled slots, stacking above the base icon */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UObject* IconBorderDisabled = nullptr;

	/* Additive image for pointed item slot (stack above base and border) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UObject* IconPointed = nullptr;

	/* Additive image for selected item slot (stack above all other icons) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UObject* IconSelected = nullptr;

	/* Font for subscript (amount indicator). */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FSlateFontInfo SubscriptFont = FTextBlockStyle::GetDefault().Font;

	/* Color for subscript (amount indicator). */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FLinearColor SubscriptColor = FLinearColor(1, 1, 1, 1);

	/* If true, the disabled item slots will enable to be pointed */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bCanPointAtDisabled = false;

	/* If true, the disabled item slots will enable to be pointed */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bCanSelectDisabled = false;

	/* If true, the subscript will be hidden when amount is 1. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bHideAmountWhenOne = true;

	FNaItemSlotStyle() {};
	
};

/**
 * 
 */
class NAITEMSYSTEM_API SNaItemSlot : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SNaItemSlot)
	{
		_StylePtr = nullptr;
		_WorldContext = nullptr;
		_Stack = nullptr;
		_bIsDisabled = false;
	}

	SLATE_ATTRIBUTE(FNaItemSlotStyle*, StylePtr)
	SLATE_ATTRIBUTE(UObject*, WorldContext)	// object as world context (indicating world)
	SLATE_ATTRIBUTE(UNaItemStack*, Stack)	/* Ptr to corresponding item stack. Null ptr means empty */
	SLATE_ATTRIBUTE(bool, bIsDisabled) /* If true, this slot will be regarded as disabled, ignoring the value of ItemEntry and bIsEmpty() */
	
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	/* Make slot params for box slot construction, from style and item stack */
	void MakeParams(FNaBoxSlotParams& OutParams);

protected:

	// Only for setting slot. Temporary param.
	FNaBoxSlotParams TempParams;

protected:

	/* Subwidget */

	TSharedPtr<SNaBoxSlot> BoxSlot;

	/* Style */
	
	FNaItemSlotStyle* StylePtr = nullptr;

	FSlateFontInfo Font = FTextBlockStyle::GetDefault().Font;

	FLinearColor SubscriptColor = FLinearColor(1, 1, 1, 1);

	/* State and general properties */

	bool bIsDisabled = true;

	bool bHideAmountWhenOne = true;

	/* Properties for getting item information */

	UNaGameModeItemSystemComponent * GMComponent;

	UNaItemStack* ItemStack = nullptr;

public:

	// Check if it's an empty slot
	bool IsEmpty() { return !ItemStack || ItemStack->IsEmpty(); };

public:

	// Get item stack
	UNaItemStack* GetStack() { return ItemStack; };

	// Get child as box slot
	TSharedPtr<SNaBoxSlot> GetBoxSlot() { return BoxSlot; };

	// Get a new item stack and reset the slot from it
	void ResetItemStack(UNaItemStack* NewStack);

	// Set if the slot is disabled
	void SetDisabled(bool NewDisabledState);

protected:

	/* Functions for interaction with inventory widget */

	/* If this ptr is null, this slot is not in an SNaItemSlotList
	*/
	SNaInventoryWrappedBox* ItemSlotList = nullptr;

	int PositionInSlotList = -1;

	/* Bind this slot's pointing and selecting state to item slot list */
	void BindItemSlotListEvents();

public:

	/* 
	 * Functions for transfer events to ItemSlotList.
	* Warning: no validity check inside. Check validity before calling.
	*/
	// SNaItemSlot only transfers pointing/selecting/mouse events from SNaBoxSlot to SNaItemSlotList.
	// SNaItemSlot itself doesn't detect pointing/selecting/mouse events.
	void SlotPointedToList();
	void SlotUnpointedToList();
	void SlotSelectedToList();
	void SlotUnselectedToList();
	FReply SlotClickedToList();
	void SlotHoveredToList();
	void SlotUnhoveredToList();

	FReply SlotMouseButtonDownToList(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
	FReply SlotMouseButtonUpToList(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
	FReply SlotMouseMoveToList(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
	FReply SlotDoubleClickedToList(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

	// Empty function when no item slot list. When the slot is not in a list, skip transferring events to list.
	void ExecNoList() { return; };
	FReply ExecNoListClicked() { return FReply::Handled(); };
	FReply ExecNoListMouse(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) { return FReply::Handled(); };


	// Call this function ONLY when constructing slots in item slot list.
	void SetItemSlotList(SNaInventoryWrappedBox* List, int Position);

	/* SNaItemSlotList end */
	/***********************/

};
