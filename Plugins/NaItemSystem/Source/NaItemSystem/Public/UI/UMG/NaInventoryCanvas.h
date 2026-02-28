// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "UI/Widgets/SNaInventoryCanvas.h"
#include "UI/Widgets/SNaItemSlot.h"
#include "NaInventoryCanvas.generated.h"

class SNaInventoryCanvas;
class UNaItemInventoryComponent;

/**
 * UNaInventoryCanvas - UMG widget wrapping SNaInventoryCanvas.
 * Displays inventory slots on a canvas panel, where slot positions are
 * determined by GetSlotPosition(). Override GetSlotPosition in a Blueprint
 * or C++ subclass to customize slot layout.
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNaInventoryCanvasSlotEvent, int, Position);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FNaInventoryCanvasSlotPointerEvent, int, Position, const FGeometry&, MyGeometry, const FPointerEvent&, MouseEvent);


UCLASS()
class NAITEMSYSTEM_API UNaInventoryCanvas : public UWidget
{
	GENERATED_BODY()

protected:

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

	virtual TSharedRef<SWidget> RebuildWidget() override;

public:

	/* Style */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryCanvas")
	FNaItemSlotStyle PublicStyle = FNaItemSlotStyle();

	/**
	 * Overall slot amount including disabled slots.
	 * If <= 0, defaults to inventory size.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryCanvas")
	int SlotAmount = -1;

protected:

	TSharedPtr<SNaInventoryCanvas> CanvasWidget;

public:

	UPROPERTY(BlueprintReadOnly)
	UNaItemInventoryComponent* ContainerComponent = nullptr;

	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|UI|InventoryCanvas")
	void SetContainerComponent(UNaItemInventoryComponent* NewComponent);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Refresh Inventory Canvas"), Category = "NaItemSystem|UI|InventoryCanvas")
	void Refresh();

public:

	/* Selection related */

	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|UI|InventoryCanvas")
	void SelectSlot(int Position);

	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|UI|InventoryCanvas")
	void UnselectAll();

	UFUNCTION(BlueprintPure, Category = "NaItemSystem|UI|InventoryCanvas")
	int GetSelectedPosition();

public:

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|InventoryCanvas")
	FNaInventoryCanvasSlotEvent OnSlotPointed;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|InventoryCanvas")
	FNaInventoryCanvasSlotEvent OnSlotUnpointed;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|InventoryCanvas")
	FNaInventoryCanvasSlotEvent OnSlotSelected;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|InventoryCanvas")
	FNaInventoryCanvasSlotEvent OnSlotUnselected;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|InventoryCanvas")
	FNaInventoryCanvasSlotEvent OnSlotClicked;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|InventoryCanvas")
	FNaInventoryCanvasSlotEvent OnSlotHovered;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|InventoryCanvas")
	FNaInventoryCanvasSlotEvent OnSlotUnhovered;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|InventoryCanvas")
	FNaInventoryCanvasSlotPointerEvent OnSlotMouseButtonDown;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|InventoryCanvas")
	FNaInventoryCanvasSlotPointerEvent OnSlotMouseButtonUp;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|InventoryCanvas")
	FNaInventoryCanvasSlotPointerEvent OnSlotMouseMove;

	UPROPERTY(BlueprintAssignable, Category = "NaItemSystem|UI|InventoryCanvas")
	FNaInventoryCanvasSlotPointerEvent OnSlotDoubleClicked;

public:

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

};
