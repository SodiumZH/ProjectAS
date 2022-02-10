// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SNaSlotList.generated.h"

/* Max length of slot list. If length is longer than this, slot list will not work correctly and an error message will be printed. */
#define SLOT_LIST_MAX_LENGTH 10000
/* If length is longer than this, a warning message will be printed to show it may be generating too many slots. */
#define SLOT_LIST_WARNING_LENGTH 200
/* If true, length warning will be enabled. */
#define SLOT_LIST_ENABLE_LENGTH_WARNING true


struct FNaBoxSlotParams;
class SWrapBox;
class SNaBoxSlot;

USTRUCT(BlueprintType)
struct NAWIDGETS_API FNaSlotListDisplayInfo {

	GENERATED_BODY()

public:

	/* Total box count */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaSlotListDisplayInfo")
	int Length = 8;

	/* Size for each box */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaSlotListDisplayInfo")
	FVector2D BoxSize = FVector2D(64.f, 64.f);

	/* Base image drawn on the bottom layer */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaSlotListDisplayInfo", meta = (AllowedClasses = "Texture,MaterialInterface,SlateTextureAtlasInterface", DisallowedClasses = "MediaTexture"))
	TArray<UObject*> ImageBaseArray;

	/* Slot frame. Drawn above the base. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaSlotListDisplayInfo", meta = (AllowedClasses = "Texture,MaterialInterface,SlateTextureAtlasInterface", DisallowedClasses = "MediaTexture"))
	TArray<UObject*> ImageFrameArray;

	/* Additional image when being pointed at. Drawn above base and frame. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaSlotListDisplayInfo", meta = (AllowedClasses = "Texture,MaterialInterface,SlateTextureAtlasInterface", DisallowedClasses = "MediaTexture"))
	UObject* ImagePointed = nullptr;

	/* Additional image when selected. Drawn on the top. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaSlotListDisplayInfo", meta = (AllowedClasses = "Texture,MaterialInterface,SlateTextureAtlasInterface", DisallowedClasses = "MediaTexture"))
	UObject* ImageSelected = nullptr;

	/* Text for superscript (drawn on the top right). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaSlotListDisplayInfo")
	TArray<FText> SuperscriptTextArray;

	/* Text for subscript (drawn on the bottom right). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaSlotListDisplayInfo")
	TArray<FText> SubscriptTextArray;

	/* Defaults */
	FNaSlotListDisplayInfo() { Length = 1; FixArrays(); };
	static FNaSlotListDisplayInfo Defaults;

	/* Fit array length to suit the list length
	* If array is longer than list length, it will be truncated (and will print log info for data loss).
	* Shorter arrays will be appended with copies of the last element (or null/empty if array is empty).
	*/
	void FixArrays();
	
	/* Check if array lengths are fixed.
	* @Param bDoAssert If true, it will assert (causing crash) when check failed.
	*/
	bool AreArraysFixed(bool bDoAssert = false);

	/* Check if length is valid. Length warning will be also executed here. */
	bool IsValidLength();

	/* Make FNaBoxSlotParams for slot initializaion. 
	* In-situ writing version 
	* If input position is invalid, do nothing.
	* @bNeedInit If true, non-array properties will be rewritten. If Size, ImageSelected and ImagePointed are already correct, this param can be be set false to
	* save resources.
	*/
	void GetSlotParams(FNaBoxSlotParams & Out, int Position, bool bNeedInit = true);

	/* Make FNaBoxSlotParams for slot initializaion. 
	* This will make a new FNaBoxSlotParams struct and return a ptr. 
	* If input position is invalid, return nullptr.
	* Not recommended for cost of creating a new struct.
	*/
	TSharedPtr<FNaBoxSlotParams> GetSlotParams(int Position);
};


/**
 * 
 */
class NAWIDGETS_API SNaSlotList : public SCompoundWidget
{
public:
	
	SLATE_BEGIN_ARGS(SNaSlotList)
	{
		_RowLength = 8;
		_DisplayInfo = &FNaSlotListDisplayInfo::Defaults;
	}
	
	SLATE_ATTRIBUTE(int, RowLength) /* Count of boxes for each row */
	SLATE_ATTRIBUTE(const FNaSlotListDisplayInfo* , DisplayInfo)	/* Display data, including length, images and texts */

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

public:

	int RowLength;

	FNaSlotListDisplayInfo DisplayInfo;

protected:

	/* Temp params for initializing or reset slots. 
	* During initing or resetting multiple slots this value will change dynamically
	* So don't depend on it under any other situations.
	*/
	FNaBoxSlotParams TempSlotParams;
	
public:

	/* Object ptrs */

	TSharedPtr<SWrapBox> WrapBox;

	TArray<TSharedPtr<SNaBoxSlot>> BoxSlots;

public:

	/* Dynamic actions of slots */

	/* Set all params of a slot.
	* This will not set non-variable params e.g. pointed and selected image.
	*/
	void SetSlot(int Position, const FNaBoxSlotParams & Params);

	void SetSlotBaseImage(int Position, UObject* NewImage);

	void SetSlotFrameImage(int Position, UObject* NewImage);

	/* Set text.
	* @Param bSetSuperscript True = SetSuperscript. False = SetSubscript.
	*/
	void SetSlotText(int Position, FText NewText, bool bSetSuperscript);

};
