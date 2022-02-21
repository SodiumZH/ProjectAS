// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/BoxSlots/SNaBoxSlot.h"
#include "NaItemEntry.h"

/**
 * 
 */
class NAITEMSYSTEM_API SNaItemSlot : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SNaItemSlot)
	{}

	SLATE_ATTRIBUTE(TSharedPtr<FNaItemEntry>, EntryPtr)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);



};
