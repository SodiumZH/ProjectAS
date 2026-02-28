#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "NaItemProperties.h"
#include "NaItemType.generated.h"

/**
 * UNaItemType - Singleton item type definition (like Minecraft's Item class)
 * Each item type has ONE instance that defines its properties and behavior.
 * 
 * Do NOT instantiate directly - use registry to obtain instances. (Registered in NaGameModeItemSystemComponent data table)
 */
UCLASS(BlueprintType, Abstract)
class NAITEMSYSTEM_API UNaItemType : public UObject
{
	GENERATED_BODY()

public:
	UNaItemType();

	/**
	 * Registry name in data table (in NaGameModeItemSystemComponent).
	 * MUST BE IDENTICAL WITH DATA TABLE ROW NAME!!!! Otherwise, GetRegistryName will always return empty.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Natrium|Item System")
	FName RegistryName;
	
	/***** Functions *****/

	/**
	 * Check if this item type can stack with another.
	 * Base implementation checks if they're the same type.
	 * Override for custom stacking rules.
	 */
	UFUNCTION(BlueprintPure, Category = "Natrium|Item System")
	virtual bool CanStackWith(const UNaItemType* Other) const;

	/**
	 * Called when item is used (right-clicked/activated).
	 * Override in child classes for custom use behavior.
	 * @param User The actor using the item
	 * @param Target Optional target actor
	 * @return Whether the item was successfully used
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Natrium|Item System")
	bool OnUse(AActor* User, AActor* Target);
	virtual bool OnUse_Implementation(AActor* User, AActor* Target);

	/**
	 * Called when item is consumed.
	 * Override in child classes for custom consume behavior.
	 * @param User The actor consuming the item
	 * @return Whether the item was successfully consumed (true = remove 1 from stack)
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Natrium|Item System")
	bool OnConsume(AActor* User);
	virtual bool OnConsume_Implementation(AActor* User);

	/**
	 * Get formatted tooltip text for display.
	 * Override to customize tooltip.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Natrium|Item System")
	FText GetTooltipText() const;
	virtual FText GetTooltipText_Implementation() const;

	/***** Static Helpers *****/

	/**
	 * Check if two item types are the same.
	 */
	UFUNCTION(BlueprintPure, Category = "Natrium|Item System", meta = (CompactNodeTitle = "=="))
	static bool AreItemTypesEqual(const UNaItemType* A, const UNaItemType* B);
	
	/**
	 * Get the registry name in data table. If the cached name is wrong or unavailable, browse the data table and 
	 * try updating the name.
	 * @return name if valid, or empty if updating failed (meaning this item type is not correctly registered).
	 */
	UFUNCTION(BlueprintPure, Category = "Natrium|Item System", DisplayName= "Get Registry Name")
	TOptional<FName> GetOrUpdateName();
	
	UFUNCTION(BlueprintPure, Category = "Natrium|Item System")
	FNaItemProperties & GetProperties() const;

	UFUNCTION(BlueprintPure, Category = "Natrium|Item System")
	TOptional<FName> GetRegistryName() const;
	
	UFUNCTION(BlueprintPure, Category = "Natrium|Item System")
	virtual UNaItemStack* CreateDefaultInstance() const;
	
};