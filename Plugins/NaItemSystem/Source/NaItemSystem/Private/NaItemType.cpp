#include "NaItemType.h"
#include "Engine/DataTable.h"
#include "BPLibraries/NaItemStatics.h"
#include "Components/NaGameModeItemSystemComponent.h"
#include "NaItemSystem.h"

UNaItemType::UNaItemType()
{
	RegistryName = NAME_None;
	DisplayName = FText::FromString(TEXT("Unknown Item"));
	MaxStackSize = 64;
	bRequiresNBT = false;
	ItemIcon = nullptr;
	RarityColor = FLinearColor::White;
	Description = FText::FromString(TEXT("An item."));
}

bool UNaItemType::CanStackWith(const UNaItemType* Other) const
{
	if (!Other)
	{
		return false;
	}

	// Same type can stack
	return this == Other;
}

bool UNaItemType::OnUse_Implementation(AActor* User, AActor* Target)
{
	// Base implementation: do nothing
	UE_LOG(LogTemp, Log, TEXT("Item type %s used by %s"), 
		*GetRegistryNameString(), 
		User ? *User->GetName() : TEXT("None"));
	
	return false;
}

bool UNaItemType::OnConsume_Implementation(AActor* User)
{
	// Base implementation: do nothing, don't consume
	return false;
}

FText UNaItemType::GetTooltipText_Implementation() const
{
	FString TooltipString = FString::Printf(
		TEXT("%s\n%s\nMax Stack: %d"),
		*DisplayName.ToString(),
		*Description.ToString(),
		MaxStackSize
	);

	return FText::FromString(TooltipString);
}

bool UNaItemType::AreItemTypesEqual(const UNaItemType* A, const UNaItemType* B)
{
	// Both null = equal
	if (!A && !B)
	{
		return true;
	}

	// One null = not equal
	if (!A || !B)
	{
		return false;
	}

	// Same pointer = equal (singletons!)
	return A == B;
}

FNaItemProperties & UNaItemType::GetProperties() const
{
	
}

TOptional<FName> UNaItemType::GetRegistryName() const
{
	UNaGameModeItemSystemComponent* GM = UNaItemStatics::GetGameModeItemSystemComponent(this);
	const FNaItemTableRow* Row = GM->ItemTypeDataTable
		->FindRow<FNaItemTableRow>(this->RegistryName, TEXT("UNaItemType::GetRegistryName"));
	if (!Row || Row->Type != this)
	{
		UE_LOG(LogNaItem, Error, TEXT("Item type %s missing registry entry. Is its name field identical to its data table key?"), this->RegistryName.ToString());
	}
}