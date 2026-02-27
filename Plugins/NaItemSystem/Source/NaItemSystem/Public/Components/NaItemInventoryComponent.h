#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NaItemInventory.h"
#include "NaItemInventoryComponent.generated.h"

/**
 * UNaItemInventoryComponent - ActorComponent that owns and exposes a UNaItemInventory.
 */
UCLASS(ClassGroup=(NaItemSystem), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class NAITEMSYSTEM_API UNaItemInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UNaItemInventoryComponent();

	virtual void InitializeComponent() override;

	/** Initial inventory size */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NaItemSystem|Inventory")
	int32 InitSize = 64;

	/** The owned inventory */
	UPROPERTY(BlueprintReadOnly, Category = "NaItemSystem|Inventory")
	UNaItemInventory* Inventory;

	/** Get the owned inventory */
	UFUNCTION(BlueprintPure, Category = "NaItemSystem|Inventory")
	UNaItemInventory* GetInventory() const;

	/***** Convenience delegates to inventory *****/

	UFUNCTION(BlueprintPure, Category = "NaItemSystem|Inventory")
	UNaItemStack* GetSlot(int32 Slot) const;

	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|Inventory")
	int32 GiveItem(UNaItemStack* Stack);

	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|Inventory")
	int32 ConsumeItemByType(UNaItemType* Type, int32 Amount);
};
