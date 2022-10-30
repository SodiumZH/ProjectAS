// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"

#include "NaItemSlotList.generated.h"

class SNaItemSlotList;
class UNaItemContainerComponent;

/**
 * 
 */

/* FNaItemOperation is a pack defining operation to item container. 
* All container change requested by UI should be pack into this.
*/

USTRUCT()
struct NAITEMSYSTEM_API FNaItemContainerOperation {
	
	GENERATED_BODY()

public:

	enum Type {
		Null,	// No operation
		Add,	// Add items to container
		AddNew, // Add new item (stack)
		Use,	// Use an item
		Delete,	// Delete items
		Empty,	// Delete all items and empty a slot
		Exchange	// Exchange items of two slots
	};

	Type OperationType = Type::Null;
	int Position = -1;
	int Param0 = -1;

};





DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNaItemListSlotEvent, int, Position);
// List slot event for pointer events, with geometry and pointer event input
//UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FNaItemListSlotPointerEvent, int, Position, const FGeometry&, MyGeometry, const FPointerEvent&, MouseEvent);


UCLASS()
class NAITEMSYSTEM_API UNaItemSlotList : public UWidget
{
	GENERATED_BODY()
	
protected:


#if WITH_EDITOR
	//~ Begin UWidget Interface
	virtual const FText GetPaletteCategory() override;
	//~ End UWidget Interface
#endif

	virtual TSharedRef<SWidget> RebuildWidget() override;

protected:

	TSharedPtr<SNaItemSlotList> List;

public:

	UPROPERTY(BlueprintReadOnly)
	UNaItemContainerComponent* ContainerComponent = nullptr;

	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|UI|ItemSlotList")
	void SetContainerComponent(UNaItemContainerComponent* NewComponent);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Refresh Item Slot List"), Category = "NaItemSystem|UI|ItemSlotList")
	void Refresh();

public:

	/* Selection related */

	// Select a slot. DO NOT SET -1 TO UNSELECT! Use UnselectAll() instead
	// Selecting condition is not defaultly defined. If selecting is needed, condition should be defined in BP.
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|UI|ItemSlotList")
	void SelectSlot(int Position);
	
	// Unselect all slots
	// Selecting condition is not defaultly defined. If selecting is needed, condition should be defined in BP.
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|UI|ItemSlotList")
	void UnselectAll();

	// Get selected position
	UFUNCTION(BlueprintPure, Category = "NaItemSystem|UI|ItemSlotList")
	int GetSelectedPosition();

public:

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|ItemSlotList")
	FNaItemListSlotEvent OnSlotPointed;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|ItemSlotList")
	FNaItemListSlotEvent OnSlotUnpointed;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|ItemSlotList")
	FNaItemListSlotEvent OnSlotSelected;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|ItemSlotList")
	FNaItemListSlotEvent OnSlotUnselected;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|ItemSlotList")
	FNaItemListSlotEvent OnSlotClicked;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|ItemSlotList")
	FNaItemListSlotEvent OnSlotHovered;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|ItemSlotList")
	FNaItemListSlotEvent OnSlotUnhovered;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|ItemSlotList")
	FNaItemListSlotPointerEvent OnSlotMouseButtonDown;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|ItemSlotList")
	FNaItemListSlotPointerEvent OnSlotMouseButtonUp;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|ItemSlotList")
	FNaItemListSlotPointerEvent OnSlotMouseMove;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|ItemSlotList")
	FNaItemListSlotPointerEvent OnSlotDoubleClicked;

public:

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

};

