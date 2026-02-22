#include "NaItemStack.h"
#include "Engine/Texture2D.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

UNaItemStack::UNaItemStack()
{
	ItemType = nullptr;
	Count = 0;
	CustomData = nullptr;
}

bool UNaItemStack::IsEmpty() const
{
	return !ItemType || Count <= 0;
}

bool UNaItemStack::IsValid() const
{
	return ItemType != nullptr && Count > 0;
}

int32 UNaItemStack::GetMaxStackSize() const
{
	return ItemType ? ItemType->MaxStackSize : 1;
}

bool UNaItemStack::IsFullStack() const
{
	if (!ItemType)
	{
		return true;
	}
	return Count >= ItemType->MaxStackSize;
}

int32 UNaItemStack::GetRemainingCapacity() const
{
	if (!ItemType)
	{
		return 0;
	}
	return FMath::Max(0, ItemType->MaxStackSize - Count);
}

int32 UNaItemStack::Grow(int32 Amount)
{
	if (!ItemType || Amount <= 0)
	{
		return 0;
	}

	const int32 OldCount = Count;
	const int32 MaxSize = ItemType->MaxStackSize;
	Count = FMath::Clamp(Count + Amount, 0, MaxSize);
	
	return Count - OldCount;
}

int32 UNaItemStack::Shrink(int32 Amount)
{
	if (Amount <= 0)
	{
		return 0;
	}

	const int32 OldCount = Count;
	Count = FMath::Max(0, Count - Amount);
	
	return OldCount - Count;
}

void UNaItemStack::SetCount(int32 NewCount)
{
	Count = NewCount;
	ClampCount();
}

void UNaItemStack::Clear()
{
	ItemType = nullptr;
	Count = 0;
	CustomData = nullptr;
}

bool UNaItemStack::CanStackWith(const UNaItemStack* Other, bool bCheckCustomData) const
{
	// Both empty = can stack (both nothing)
	if (IsEmpty() && (!Other || Other->IsEmpty()))
	{
		return true;
	}

	// One empty = can't stack
	if (IsEmpty() || !Other || Other->IsEmpty())
	{
		return false;
	}

	// Different item types = can't stack
	if (ItemType != Other->ItemType)
	{
		return false;
	}

	// If checking custom data, must match exactly
	if (bCheckCustomData)
	{
		return AreJsonObjectsEqual(CustomData, Other->CustomData);
	}

	// Not checking custom data = can stack if same type
	return true;
}

int32 UNaItemStack::MergeFrom(UNaItemStack* Other, bool bConsumeOther)
{
	if (!Other || !CanStackWith(Other, true))
	{
		return 0;
	}

	const int32 Capacity = GetRemainingCapacity();
	const int32 TransferAmount = FMath::Min(Capacity, Other->Count);

	if (TransferAmount > 0)
	{
		Grow(TransferAmount);
		
		if (bConsumeOther)
		{
			Other->Shrink(TransferAmount);
		}
	}

	return TransferAmount;
}

UNaItemStack* UNaItemStack::Split(int32 SplitCount)
{
	if (SplitCount <= 0 || Count <= 0 || !ItemType)
	{
		return nullptr;
	}

	const int32 ActualSplitCount = FMath::Min(SplitCount, Count);
	
	// Create new stack
	UNaItemStack* NewStack = NewObject<UNaItemStack>(GetTransientPackage());
	NewStack->ItemType = ItemType;
	NewStack->Count = ActualSplitCount;
	
	// Deep copy custom data
	if (CustomData.IsValid())
	{
		FString JsonString = ExportCustomDataToString();
		NewStack->ImportCustomDataFromString(JsonString);
	}

	// Shrink this stack
	Shrink(ActualSplitCount);

	return NewStack;
}

bool UNaItemStack::Use(AActor* User, AActor* Target)
{
	if (!ItemType || IsEmpty())
	{
		return false;
	}

	return ItemType->OnUse(User, Target);
}

bool UNaItemStack::Consume(AActor* User)
{
	if (!ItemType || IsEmpty())
	{
		return false;
	}

	const bool bConsumed = ItemType->OnConsume(User);
	
	if (bConsumed)
	{
		Shrink(1);
	}

	return bConsumed;
}

FText UNaItemStack::GetDisplayName() const
{
	if (!ItemType)
	{
		return FText::FromString(TEXT("Empty"));
	}

	// Check for custom name in data
	if (HasCustomDataKey(TEXT("CustomName")))
	{
		FString CustomName = GetCustomString(TEXT("CustomName"));
		if (!CustomName.IsEmpty())
		{
			return FText::FromString(CustomName);
		}
	}

	// Use item type name
	return ItemType->DisplayName;
}

FText UNaItemStack::GetTooltipText() const
{
	if (!ItemType)
	{
		return FText::FromString(TEXT("Empty slot"));
	}

	FText BaseTooltip = ItemType->GetTooltipText();
	
	// Add count if more than 1
	if (Count > 1)
	{
		FString TooltipString = FString::Printf(
			TEXT("%s\nCount: %d"),
			*BaseTooltip.ToString(),
			Count
		);
		return FText::FromString(TooltipString);
	}

	return BaseTooltip;
}

UTexture2D* UNaItemStack::GetIcon() const
{
	return ItemType ? ItemType->ItemIcon : nullptr;
}

void UNaItemStack::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	// Serialize ItemType (handled by UPROPERTY)
	// Serialize Count (handled by UPROPERTY)

	// Manually serialize CustomData
	if (Ar.IsSaving())
	{
		FString JsonString = ExportCustomDataToString();
		Ar << JsonString;
	}
	else if (Ar.IsLoading())
	{
		FString JsonString;
		Ar << JsonString;
		ImportCustomDataFromString(JsonString);
	}
}

FString UNaItemStack::ExportCustomDataToString() const
{
	if (!CustomData.IsValid())
	{
		return TEXT("{}");
	}

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(CustomData.ToSharedRef(), Writer);
	return OutputString;
}

bool UNaItemStack::ImportCustomDataFromString(const FString& JsonString)
{
	if (JsonString.IsEmpty() || JsonString == TEXT("{}"))
	{
		CustomData = nullptr;
		return true;
	}

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	return FJsonSerializer::Deserialize(Reader, CustomData);
}

UNaItemStack* UNaItemStack::CreateItemStack(UObject* WorldContextObject, UNaItemType* Type, int32 InCount)
{
	if (!Type || InCount <= 0)
	{
		return CreateEmptyStack(WorldContextObject);
	}

	UNaItemStack* NewStack = NewObject<UNaItemStack>(
		WorldContextObject ? WorldContextObject : GetTransientPackage()
	);
	
	NewStack->ItemType = Type;
	NewStack->Count = InCount;
	NewStack->ClampCount();

	return NewStack;
}

UNaItemStack* UNaItemStack::CreateEmptyStack(UObject* WorldContextObject)
{
	UNaItemStack* NewStack = NewObject<UNaItemStack>(
		WorldContextObject ? WorldContextObject : GetTransientPackage()
	);
	
	return NewStack;
}

UNaItemStack* UNaItemStack::CopyItemStack(UObject* WorldContextObject, const UNaItemStack* Source)
{
	if (!Source)
	{
		return CreateEmptyStack(WorldContextObject);
	}

	UNaItemStack* NewStack = NewObject<UNaItemStack>(
		WorldContextObject ? WorldContextObject : GetTransientPackage()
	);
	
	NewStack->ItemType = Source->ItemType;
	NewStack->Count = Source->Count;
	
	// Deep copy custom data
	if (Source->HasCustomData())
	{
		FString JsonString = Source->ExportCustomDataToString();
		NewStack->ImportCustomDataFromString(JsonString);
	}

	return NewStack;
}

bool UNaItemStack::AreStacksEqual(const UNaItemStack* A, const UNaItemStack* B)
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

	// Different types = not equal
	if (A->ItemType != B->ItemType)
	{
		return false;
	}

	// Different counts = not equal
	if (A->Count != B->Count)
	{
		return false;
	}

	// Check custom data equality
	return AreJsonObjectsEqual(A->CustomData, B->CustomData);
}

bool UNaItemStack::AreStacksSameTypeAndData(const UNaItemStack* A, const UNaItemStack* B)
{
	// Both null = same
	if (!A && !B)
	{
		return true;
	}

	// One null = different
	if (!A || !B)
	{
		return false;
	}

	// Use CanStackWith (which checks type and custom data)
	return A->CanStackWith(B, true);
}

TSharedPtr<FJsonObject> UNaItemStack::GetCustomDataObject()
{
	if (!CustomData.IsValid())
	{
		CustomData = MakeShared<FJsonObject>();
	}
	return CustomData;
}

TSharedPtr<FJsonObject> UNaItemStack::GetCustomDataObject() const
{
	return CustomData;
}

void UNaItemStack::SetCustomDataObject(TSharedPtr<FJsonObject> InData)
{
	CustomData = InData;
}

void UNaItemStack::MergeCustomData(const UNaItemStack* Other, bool bOverwrite)
{
	if (!Other || !Other->CustomData.IsValid())
	{
		return;
	}

	TSharedPtr<FJsonObject> Data = GetCustomDataObject();

	for (const auto& Pair : Other->CustomData->Values)
	{
		if (bOverwrite || !Data->HasField(Pair.Key))
		{
			Data->SetField(Pair.Key, Pair.Value);
		}
	}
}

void UNaItemStack::ClampCount()
{
	if (ItemType)
	{
		Count = FMath::Clamp(Count, 0, ItemType->MaxStackSize);
	}
	else
	{
		Count = FMath::Max(0, Count);
	}
}

bool UNaItemStack::AreJsonObjectsEqual(const TSharedPtr<FJsonObject>& A, const TSharedPtr<FJsonObject>& B)
{
	// Both null = equal
	if (!A.IsValid() && !B.IsValid())
	{
		return true;
	}

	// One null = not equal
	if (!A.IsValid() || !B.IsValid())
	{
		return false;
	}

	// Different number of fields = not equal
	if (A->Values.Num() != B->Values.Num())
	{
		return false;
	}

	// Compare each field
	for (const auto& Pair : A->Values)
	{
		if (!B->HasField(Pair.Key))
		{
			return false;
		}

		const TSharedPtr<FJsonValue>& ValueA = Pair.Value;
		const TSharedPtr<FJsonValue>& ValueB = B->Values[Pair.Key];

		// Different types = not equal
		if (ValueA->Type != ValueB->Type)
		{
			return false;
		}

		// Compare based on type
		switch (ValueA->Type)
		{
		case EJson::None:
		case EJson::Null:
			// Both null/none = equal
			break;

		case EJson::String:
			if (ValueA->AsString() != ValueB->AsString())
			{
				return false;
			}
			break;

		case EJson::Number:
			if (ValueA->AsNumber() != ValueB->AsNumber())
			{
				return false;
			}
			break;

		case EJson::Boolean:
			if (ValueA->AsBool() != ValueB->AsBool())
			{
				return false;
			}
			break;

		case EJson::Array:
		{
			const TArray<TSharedPtr<FJsonValue>>& ArrayA = ValueA->AsArray();
			const TArray<TSharedPtr<FJsonValue>>& ArrayB = ValueB->AsArray();

			if (ArrayA.Num() != ArrayB.Num())
			{
				return false;
			}

			// Simple comparison - could be made more sophisticated
			for (int32 i = 0; i < ArrayA.Num(); i++)
			{
				if (ArrayA[i]->Type != ArrayB[i]->Type)
				{
					return false;
				}

				// For simplicity, compare as strings
				// Could recursively compare for nested arrays/objects
				FString StrA, StrB;
				TSharedRef<TJsonWriter<>> WriterA = TJsonWriterFactory<>::Create(&StrA);
				TSharedRef<TJsonWriter<>> WriterB = TJsonWriterFactory<>::Create(&StrB);
				FJsonSerializer::Serialize(ArrayA[i], WriterA);
				FJsonSerializer::Serialize(ArrayB[i], WriterB);

				if (StrA != StrB)
				{
					return false;
				}
			}
			break;
		}

		case EJson::Object:
			// Recursive comparison for nested objects
			if (!AreJsonObjectsEqual(ValueA->AsObject(), ValueB->AsObject()))
			{
				return false;
			}
			break;

		default:
			return false;
		}
	}

	return true;
}