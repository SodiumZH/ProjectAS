// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "NaItemContainer.h"
#include "Widgets/BoxSlots/SNaSlotList.h"
#include "UI/Widgets/SNaItemSlot.h"

class UNaGameModeItemSystemComponent;

/**
 * 
 */
class NAITEMSYSTEM_API SNaItemSlotList : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SNaItemSlotList)
	{
		_WorldContext = nullptr;
		_ContainerPtr = nullptr;
		_BoxSize = FVector2D;
		_bHideAmountWhenOne = true;
		_Font = FTextBlockStyle::GetDefault().Font;
		_bFillDisabledToCompleteRectangle = true;
		_RowLength = 8;
	}
	SLATE_ATTRIBUTE(UObject*, WorldContext)
	SLATE_ATTRIBUTE(TSharedPtr<FNaItemContainer>, ContainerPtr)
	SLATE_ATTRIBUTE(FVector2D, BoxSize)
	SLATE_ATTRIBUTE(bool, bHideAmountWhenOne)
	SLATE_ATTRIBUTE(FSlateFontInfo, Font)
	SLATE_ATTRIBUTE(bool, bFillDisabledToCompleteRectangle)	/* If true, it will add disabled slots to the end to fill a complete rectangle. */
	SLATE_ATTRIBUTE(int, RowLength)	/* How many boxes in a row */

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

protected:


	/* Input copies */

	UNaGameModeItemSystemComponent GMComponent;
	TSharedPtr<FNaItemContainer> ContainerPtr;
	FVector2D BoxSize;
	bool bHideAmountWhenOne;
	FSlateFontInfo Font;
	bool bFillDisabledToCompleteRectangle;
	int RowLength;

	/**/
	int ActualLength;

	/* Subwidgets */
	TSharedPtr<SWrapBox> WrapBox;
	TArray<TSharedPtr<SNaItemSlot>> Slots;

};
