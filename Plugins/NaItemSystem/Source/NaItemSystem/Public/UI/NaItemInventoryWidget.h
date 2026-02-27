// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NaItemInventoryWidget.generated.h"

UINTERFACE()
class UNaItemInventoryWidget : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NAITEMSYSTEM_API INaItemInventoryWidget
{
	GENERATED_BODY()

public:
	/**
	 * Get the inventory this widget describes.
	 */
	UFUNCTION(BlueprintPure)
	virtual UNaItemInventory* GetInventory();

	/**
	 * Get overall slot amount, including disabled slots. 
	 * This amount is not necessarily equal to the inventory size. If this amount is smaller, the odd inventory slots 
	 * will be ignored. If this amount is larger, disabled slots will be added in the tail.
	 */
	UFUNCTION(BlueprintPure)
	virtual int32 GetSlotAmount();

	/**
	 * Function mapping slot indexes to XY positions.
	 */
	UFUNCTION(BlueprintPure)
	virtual FVector2D GetSlotPosition(int32 index);
	
};
