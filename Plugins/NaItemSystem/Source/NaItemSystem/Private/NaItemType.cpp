#include "NaItemType.h"
#include "Engine/DataTable.h"
#include "BPLibraries/NaItemStatics.h"
#include "Components/NaGameModeItemSystemComponent.h"
#include "NatriumItemSystem.h"
#include "Data/NaItemTableRow.h"

UNaItemType::UNaItemType()
{
	RegistryName = NAME_None;
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
		*RegistryName.ToString(), 
		User ? *User->GetName() : TEXT("None"));
	
	return false;
}

bool UNaItemType::OnConsume_Implementation(AActor* User)
{
	// Base implementation: do not consume
	return false;
}

FText UNaItemType::GetTooltipText_Implementation() const
{
	const FNaItemProperties& Props = GetProperties();
	FString TooltipString = FString::Printf(
		TEXT("%s\n%s\nMax Stack: %d"),
		*Props.DisplayName.ToString(),
		*Props.Description.ToString(),
		Props.MaxStackSize
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

FNaItemProperties& UNaItemType::GetProperties() const
{
	UNaGameModeItemSystemComponent* GM = UNaItemStatics::GetGameModeItemSystemComponent(const_cast<UNaItemType*>(this));
	if (GM && GM->ItemTypeDataTable)
	{
		FNaItemTableRow* Row = GM->ItemTypeDataTable->FindRow<FNaItemTableRow>(RegistryName, TEXT("UNaItemType::GetProperties"));
		if (Row)
		{
			return Row->Properties;
		}
	}
	UE_LOG(LogNaItem, Error, TEXT("UNaItemType::GetProperties: Failed to find properties for item type '%s'"), *RegistryName.ToString());
	static FNaItemProperties DefaultProperties;
	return DefaultProperties;
}

TOptional<FName> UNaItemType::GetRegistryName() const
{
	UNaGameModeItemSystemComponent* GM = UNaItemStatics::GetGameModeItemSystemComponent((UObject*)this);
	if (!GM || !GM->ItemTypeDataTable)
	{
		return TOptional<FName>();
	}
	const FNaItemTableRow* Row = GM->ItemTypeDataTable
		->FindRow<FNaItemTableRow>(this->RegistryName, TEXT("UNaItemType::GetRegistryName"));
	if (!Row || Row->Type != this)
	{
		UE_LOG(LogNaItem, Error, TEXT("Item type %s missing registry entry. Is its name field identical to its data table key?"), *this->RegistryName.ToString());
		return TOptional<FName>();
	}
	return TOptional<FName>(RegistryName);
}

TOptional<FName> UNaItemType::GetOrUpdateName()
{
	// First try current registry name
	TOptional<FName> Result = GetRegistryName();
	if (Result.IsSet())
	{
		return Result;
	}

	// Try to find by scanning all rows
	UNaGameModeItemSystemComponent* GM = UNaItemStatics::GetGameModeItemSystemComponent(this);
	if (!GM || !GM->ItemTypeDataTable)
	{
		return TOptional<FName>();
	}

	TArray<FName> RowNames = GM->ItemTypeDataTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		const FNaItemTableRow* Row = GM->ItemTypeDataTable->FindRow<FNaItemTableRow>(RowName, TEXT("UNaItemType::GetOrUpdateName"));
		if (Row && Row->Type == this)
		{
			RegistryName = RowName;
			return TOptional<FName>(RegistryName);
		}
	}

	return TOptional<FName>();
}