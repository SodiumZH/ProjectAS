// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "NaItemSlotList.generated.h"


class UNaItemContainerComponent;
class SNaItemSlotList;


/**
 * 
 */
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

	/* Input settings */

	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|UI|ItemSlotList")
	void SelectSlot();

public:

	UFUNCTION(BlueprintNativeEvent, Category = "NaItemSystem|UI|ItemSlotList")
	void OnSlotPointed(int Position);
	void OnSlotPointed_Implementation(int Position) {};

	UFUNCTION(BlueprintNativeEvent, Category = "NaItemSystem|UI|ItemSlotList")
	void OnSlotUnpointed(int Position);
	void OnSlotUnpointed_Implementation(int Position) {};

	UFUNCTION(BlueprintNativeEvent, Category = "NaItemSystem|UI|ItemSlotList")
	void OnSlotSelected(int Position);
	void OnSlotSelected_Implementation(int Position) {};

	UFUNCTION(BlueprintNativeEvent, Category = "NaItemSystem|UI|ItemSlotList")
	void OnSlotUnselected(int Position);
	void OnSlotUnselected_Implementation(int Position) {};

	UFUNCTION(BlueprintNativeEvent, Category = "NaItemSystem|UI|ItemSlotList")
	void OnSlotClicked(int Position);
	void OnSlotClicked_Implementation(int Position) {};

	UFUNCTION(BlueprintNativeEvent, Category = "NaItemSystem|UI|ItemSlotList")
	void OnSlotHovered(int Position);
	void OnSlotHovered_Implementation(int Position) {};

	UFUNCTION(BlueprintNativeEvent, Category = "NaItemSystem|UI|ItemSlotList")
	void OnSlotUnhovered(int Position);
	void OnSlotUnhovered_Implementation(int Position) {};

	UFUNCTION(BlueprintNativeEvent, Category = "NaItemSystem|UI|ItemSlotList")
	void OnSlotMouseButtonDown(int Position, const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
	void OnSlotUnhovered_Implementation(int Position, const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {};

	UFUNCTION(BlueprintNativeEvent, Category = "NaItemSystem|UI|ItemSlotList")
	void OnSlotMouseButtonUp(int Position, const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
	void OnSlotUnhovered_Implementation(int Position, const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {};

	UFUNCTION(BlueprintNativeEvent, Category = "NaItemSystem|UI|ItemSlotList")
	void OnSlotMouseMove(int Position, const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
	void OnSlotUnhovered_Implementation(int Position, const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {};
};

