// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "UI/UMG/NaItemSlotList.h"
#include "NaItemSlotListPreset.generated.h"

/**
 * 
 */
UCLASS()
class NAITEMSYSTEM_API UNaItemSlotListPreset : public UNaItemSlotList
{
	GENERATED_BODY()
	
public:

	virtual TSharedRef<SWidget> RebuildWidget() override;

protected:
	
	void EventClickedPreset(int Position);

	void EventDoubleClickedPreset(int Position, const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

};
