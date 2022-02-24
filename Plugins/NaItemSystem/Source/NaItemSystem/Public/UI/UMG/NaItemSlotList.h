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

	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|UI")
	void SetContainerComponent(UNaItemContainerComponent* NewComponent);
};
