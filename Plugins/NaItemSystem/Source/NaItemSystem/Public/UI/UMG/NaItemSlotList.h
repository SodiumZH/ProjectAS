// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "UI/Widgets/SNaItemSlotList.h"
#include "UI/Widgets/SNaItemSlot.h"
#include "NaItemSlotList.generated.h"

class SNaItemSlotList;
class UNaItemInventoryComponent;

/**
 * 
 */

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

public:

	/* Style */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSlotList")
	FNaItemSlotPublicStyle PublicStyle = FNaItemSlotPublicStyle();

	// Amount of slots each row
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSlotList")
	int RowLength = 8;
	
	// Amount of rows. If this count is not enough for container, it will be ignored and the row count will be automatically
	// calculated from container.
	// If this value is applied, the slot will always be a complete rectangle.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSlotList")
	int RowCount = 0;

	// If true, it will automatically add disabled slots at the end to make it a complete rectangle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSlotList")
	bool bFillDisabledToCompleteRectangle = true;



protected:

	TSharedPtr<SNaItemSlotList> List;

public:

	UPROPERTY(BlueprintReadOnly)
	UNaItemInventoryComponent* ContainerComponent = nullptr;

	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|UI|ItemSlotList")
	void SetContainerComponent(UNaItemInventoryComponent* NewComponent);

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

