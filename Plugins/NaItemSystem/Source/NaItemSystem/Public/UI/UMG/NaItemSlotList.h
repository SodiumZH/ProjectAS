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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNaItemListSlotEvent_UMG, int, Position);

// List slot event for pointer events, with geometry and pointer event input
//UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FNaItemListSlotPointerEvent_UMG, int, Position, const FGeometry&, MyGeometry, const FPointerEvent&, MouseEvent);


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
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|UI|ItemSlotList")
	void SelectSlot(int Position);
	
	// Unselect all slots
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|UI|ItemSlotList")
	void UnselectAll();

	// Get selected position
	UFUNCTION(BlueprintPure, Category = "NaItemSystem|UI|ItemSlotList")
	int GetSelectedPosition();

public:

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|ItemSlotList")
	FNaItemListSlotEvent_UMG OnSlotPointed;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|ItemSlotList")
	FNaItemListSlotEvent_UMG OnSlotUnpointed;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|ItemSlotList")
	FNaItemListSlotEvent_UMG OnSlotSelected;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|ItemSlotList")
	FNaItemListSlotEvent_UMG OnSlotUnselected;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|ItemSlotList")
	FNaItemListSlotEvent_UMG OnSlotClicked;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|ItemSlotList")
	FNaItemListSlotEvent_UMG OnSlotHovered;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|ItemSlotList")
	FNaItemListSlotEvent_UMG OnSlotUnhovered;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|ItemSlotList")
	FNaItemListSlotPointerEvent_UMG OnSlotMouseButtonDown;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|ItemSlotList")
	FNaItemListSlotPointerEvent_UMG OnSlotMouseButtonUp;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|ItemSlotList")
	FNaItemListSlotPointerEvent_UMG OnSlotMouseMove;
};

