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
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|Inventory")
	virtual UNaItemInventory* GetInventory() const;

	/**
	 * Get overall slot amount, including disabled slots. 
	 * This amount is not necessarily equal to the inventory size. If this amount is smaller, the odd inventory slots 
	 * will be ignored. If this amount is larger, disabled slots will be added in the tail.
	 */
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|Inventory")
	virtual int32 GetSlotAmount() const;

	/**
	 * Function mapping slot indexes to XY positions.
	 */
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|Inventory")
	virtual FVector2D GetSlotPosition(int32 index) const;
	
	virtual TWeakPtr<NaItemInventoryWidgetInterface> GetNativeInterface() const = 0;
	
};

/**
 * Base interface for inventory slate classes. 
 */
class NaItemInventoryWidgetInterface
{
	
public:
	
	/**
	 * Get the inventory this widget describes.
	 */
	virtual UNaItemInventory* GetInventory() = 0;

	/**
	 * Get overall slot amount, including disabled slots. 
	 * This amount is not necessarily equal to the inventory size. If this amount is smaller, the odd inventory slots 
	 * will be ignored. If this amount is larger, disabled slots will be added in the tail.
	 */
	virtual int32 GetSlotAmount() const = 0;

	/**
	 * Function mapping slot indexes to XY positions.
	 */
	virtual FVector2D GetSlotPosition(int32 index) = 0;
};

