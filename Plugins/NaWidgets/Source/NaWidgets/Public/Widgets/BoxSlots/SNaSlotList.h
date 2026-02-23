// By Sodium
// SNaSlotList.h - Declares SNaSlotList, a grid/list container of SNaBoxSlot widgets,
// and FNaSlotListDisplayInfo, the configuration struct that drives it.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Fonts/SlateFontInfo.h"
#include "Widgets/BoxSlots/SNaBoxSlot.h"
#include "SNaSlotList.generated.h"

/**
 * @brief Hard upper limit on the number of slots a single SNaSlotList may contain.
 *
 * If the requested length exceeds this value the slot list will refuse to construct
 * and print an error message.  Exists to protect against runaway allocations.
 */
#define SLOT_LIST_MAX_LENGTH 10000

/**
 * @brief Soft performance-warning threshold for slot count.
 *
 * When SLOT_LIST_ENABLE_LENGTH_WARNING is true and the requested length exceeds this
 * value a warning is printed to the log, indicating that a large number of slots
 * is being created and may affect performance.
 */
#define SLOT_LIST_WARNING_LENGTH 200

/**
 * @brief Controls whether the performance warning for large slot counts is emitted.
 *
 * Set to false to suppress warnings when intentionally creating slot lists with more
 * than SLOT_LIST_WARNING_LENGTH entries.
 */
#define SLOT_LIST_ENABLE_LENGTH_WARNING true


struct FNaBoxSlotParams;
class SWrapBox;
class SNaBoxSlot;


/**
 * @brief Configuration data that drives the appearance of every slot in an SNaSlotList.
 *
 * FNaSlotListDisplayInfo aggregates per-slot arrays (base images, frame images, text
 * overlays) with shared properties (pointed image, selected image) so that a single
 * struct can describe the full visual state of all slots in a list.
 *
 * ## Array contract
 * After construction, all per-slot arrays must have exactly `Length` elements.
 * Call FixArrays() to enforce this invariant automatically, or check it with
 * AreArraysFixed().  GetSlotParams() also assumes the arrays are fixed.
 *
 * ## Lifecycle
 * 1. Construct with `FNaSlotListDisplayInfo(Length)` (or set `Length` manually).
 * 2. Populate `ImageBaseArray`, `ImageFrameArray`, `SuperscriptTextArray`, etc.
 * 3. Call `FixArrays()` to synchronise array sizes with `Length`.
 * 4. Pass a pointer to the struct to `SNaSlotList` via the `DisplayInfo` attribute.
 */
USTRUCT(BlueprintType)
struct NAWIDGETS_API FNaSlotListDisplayInfo {

	GENERATED_BODY()

public:

	/** Total number of slots in the list.  Must be > 0 and ≤ SLOT_LIST_MAX_LENGTH. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaSlotListDisplayInfo")
	int Length = 8;

	/** Visual size (width × height) of each individual slot in Slate units. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaSlotListDisplayInfo")
	FVector2D BoxSize = FVector2D(64.f, 64.f);

	/**
	 * Per-slot base images (bottom layer, e.g. item icons or slot backgrounds).
	 * After FixArrays() this array has exactly `Length` elements.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaSlotListDisplayInfo", meta = (AllowedClasses = "/Script/Engine.Texture, /Script/Engine.MaterialInterface, /Script/Engine.SlateTextureAtlasInterface", DisallowedClasses = "/Script/MediaAssets.MediaTexture"))
	TArray<UObject*> ImageBaseArray;

	/**
	 * Per-slot frame images (second layer, e.g. decorative borders).
	 * After FixArrays() this array has exactly `Length` elements.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaSlotListDisplayInfo", meta = (AllowedClasses = "/Script/Engine.Texture, /Script/Engine.MaterialInterface, /Script/Engine.SlateTextureAtlasInterface", DisallowedClasses = "/Script/MediaAssets.MediaTexture"))
	TArray<UObject*> ImageFrameArray;

	/**
	 * Shared hover-highlight image used by every slot in the list.
	 * Drawn above base and frame when a slot is pointed (hovered).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaSlotListDisplayInfo", meta = (AllowedClasses = "/Script/Engine.Texture, /Script/Engine.MaterialInterface, /Script/Engine.SlateTextureAtlasInterface", DisallowedClasses = "/Script/MediaAssets.MediaTexture"))
	UObject* ImagePointed = nullptr;

	/**
	 * Shared selection-highlight image used by every slot in the list.
	 * Drawn on the top layer when a slot is selected.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaSlotListDisplayInfo", meta = (AllowedClasses = "/Script/Engine.Texture, /Script/Engine.MaterialInterface, /Script/Engine.SlateTextureAtlasInterface", DisallowedClasses = "/Script/MediaAssets.MediaTexture"))
	UObject* ImageSelected = nullptr;

	/**
	 * Per-slot superscript text overlays (top-right corner, e.g. stack counts).
	 * After FixArrays() this array has exactly `Length` elements.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaSlotListDisplayInfo")
	TArray<FText> SuperscriptTextArray;

	/**
	 * Per-slot subscript text overlays (bottom-right corner, e.g. cooldown timers).
	 * After FixArrays() this array has exactly `Length` elements.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaSlotListDisplayInfo")
	TArray<FText> SubscriptTextArray;

	/** Default constructor — creates a single-slot list and fixes all arrays. */
	FNaSlotListDisplayInfo() { Length = 1; FixArrays(); };

	/**
	 * @brief Constructs a display info with a given slot count and fixes arrays.
	 * @param InLength  Desired number of slots.  Must be > 0.
	 */
	FNaSlotListDisplayInfo(int InLength) { check(InLength > 0); Length = InLength; FixArrays(); };

	/** Shared default instance used when no explicit DisplayInfo is provided to SNaSlotList. */
	static FNaSlotListDisplayInfo Defaults;

	/**
	 * @brief Synchronises all per-slot arrays so each has exactly `Length` elements.
	 *
	 * - Arrays longer than `Length` are truncated (a log message is printed to warn
	 *   about potential data loss).
	 * - Arrays shorter than `Length` are extended by repeating the last element, or
	 *   by appending null / empty values if the array was empty.
	 *
	 * Call this after changing `Length` or any of the per-slot arrays.
	 */
	void FixArrays();
	
	/**
	 * @brief Checks whether all per-slot arrays have exactly `Length` elements.
	 *
	 * @param bDoAssert  When true, triggers an assert (crash) if validation fails
	 *                   rather than returning false.
	 * @return true if all arrays are correctly sized, false otherwise.
	 */
	bool AreArraysFixed(bool bDoAssert = false);

	/**
	 * @brief Validates `Length` against the hard and soft limits.
	 *
	 * Prints an error and returns false if Length > SLOT_LIST_MAX_LENGTH.
	 * Prints a warning (when SLOT_LIST_ENABLE_LENGTH_WARNING is true) if
	 * Length > SLOT_LIST_WARNING_LENGTH.
	 *
	 * @return true if the length is within acceptable bounds.
	 */
	bool IsValidLength();

	/**
	 * @brief Writes FNaBoxSlotParams for the slot at the given position into an existing struct.
	 *
	 * This in-situ variant avoids allocating a new struct, making it preferred for
	 * batch initialization of many slots.
	 *
	 * @param Out        Struct to write the parameters into.
	 * @param Position   Zero-based slot index.  Does nothing if out of range.
	 * @param bNeedInit  When true (default), non-array properties (Size, ImageSelected,
	 *                   ImagePointed) are also written.  Pass false when those values are
	 *                   already correct to save a small amount of work.
	 */
	void GetSlotParams(FNaBoxSlotParams& Out, int Position, bool bNeedInit = true);

	/**
	 * @brief Creates and returns a new FNaBoxSlotParams for the slot at the given position.
	 *
	 * @param Position  Zero-based slot index.
	 * @return A shared pointer to the new params struct, or nullptr if Position is invalid.
	 * @note Prefer the in-situ overload when initializing many slots to avoid repeated heap allocations.
	 */
	TSharedPtr<FNaBoxSlotParams> GetSlotParams(int Position);
};


/**
 * @brief A Slate widget that displays a grid or list of SNaBoxSlot widgets.
 *
 * SNaSlotList wraps an SWrapBox and automatically populates it with SNaBoxSlot
 * children based on the data provided through an FNaSlotListDisplayInfo pointer.
 * Slots flow left-to-right and wrap to a new row every `RowLength` slots.
 *
 * ## Features
 * - Configurable row width via `RowLength`
 * - Batch initialization from FNaSlotListDisplayInfo (images, text overlays)
 * - Runtime per-slot and all-slot setters for images and text
 * - Safety limits (SLOT_LIST_MAX_LENGTH / SLOT_LIST_WARNING_LENGTH) to prevent
 *   accidentally creating thousands of widgets
 *
 * ## Usage example
 * @code
 * FNaSlotListDisplayInfo Info(16);
 * Info.ImagePointed  = MyHoverTexture;
 * Info.ImageSelected = MySelectTexture;
 * // Fill per-slot data ...
 *
 * TSharedRef<SNaSlotList> SlotList = SNew(SNaSlotList)
 *     .RowLength(8)
 *     .DisplayInfo(&Info);
 *
 * // At runtime, update individual slots:
 * SlotList->SetSlotBaseImage(3, NewItemTexture);
 * SlotList->SetSlotText(3, FText::FromString(TEXT("5")), false); // subscript
 * @endcode
 *
 * @see SNaBoxSlot, FNaSlotListDisplayInfo
 */
class NAWIDGETS_API SNaSlotList : public SCompoundWidget
{
public:
	
	SLATE_BEGIN_ARGS(SNaSlotList)
	{
		_RowLength = 8;
		_DisplayInfo = &FNaSlotListDisplayInfo::Defaults;
		_SubscriptFont = FTextBlockStyle::GetDefault().Font;
		_SuperscriptFont = FTextBlockStyle::GetDefault().Font;
	}

	/** Number of slots per row.  Slots wrap to the next row after this many are placed. */
	SLATE_ATTRIBUTE(int, RowLength)
	/** Pointer to display configuration (length, images, text).  Read during Construct(). */
	SLATE_ATTRIBUTE(const FNaSlotListDisplayInfo*, DisplayInfo)
	/** Font applied to the subscript (bottom-right) text overlay of every slot. */
	SLATE_ATTRIBUTE(FSlateFontInfo, SubscriptFont)
	/** Font applied to the superscript (top-right) text overlay of every slot. */
	SLATE_ATTRIBUTE(FSlateFontInfo, SuperscriptFont)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

public:

	/** Number of SNaBoxSlot widgets per row in the wrap-box layout. */
	int RowLength;

	/** A copy of the display info provided at construction time. */
	FNaSlotListDisplayInfo DisplayInfo;

	/** Font used for the superscript overlay of all child slots. */
	FSlateFontInfo SuperscriptFont;

	/** Font used for the subscript overlay of all child slots. */
	FSlateFontInfo SubscriptFont;

protected:

	/**
	 * @brief Scratch struct reused while initializing or resetting multiple slots.
	 *
	 * Its content is only meaningful during a batch init/reset pass.  Do not read
	 * this member outside of those operations.
	 */
	FNaBoxSlotParams TempSlotParams;
	
protected:

	/** The underlying SWrapBox that lays out child SNaBoxSlot widgets. */
	TSharedPtr<SWrapBox> WrapBox;

	/** Ordered array of all child SNaBoxSlot shared pointers. Index matches slot position. */
	TArray<TSharedPtr<SNaBoxSlot>> BoxSlots;

protected:

	/**
	 * @brief Verifies that the BoxSlots array length matches DisplayInfo.Length.
	 *
	 * Prints an error to the log if the sizes differ.
	 * @return true if the lengths match, false otherwise.
	 */
	bool IsSlotArrayLengthRight();

public:

	/** @name Per-slot setters */
	///@{

	/**
	 * @brief Replaces all variable parameters (images, text) for a single slot.
	 *
	 * Note: shared parameters such as ImagePointed and ImageSelected are not
	 * updated by this method.  Use SetSlotPointedImage / SetSlotSelectedImage instead.
	 *
	 * @param Position  Zero-based slot index.
	 * @param Params    New parameters to apply.
	 */
	void SetSlot(int Position, const FNaBoxSlotParams& Params);

	/**
	 * @brief Sets the base (bottom-layer) image of a single slot.
	 * @param Position  Zero-based slot index.
	 * @param NewImage  The new image asset, or nullptr to clear.
	 */
	void SetSlotBaseImage(int Position, UObject* NewImage);

	/**
	 * @brief Sets the frame (second-layer) image of a single slot.
	 * @param Position  Zero-based slot index.
	 * @param NewImage  The new image asset, or nullptr to clear.
	 */
	void SetSlotFrameImage(int Position, UObject* NewImage);

	/**
	 * @brief Sets the superscript or subscript text overlay of a single slot.
	 * @param Position        Zero-based slot index.
	 * @param NewText         The text to display.
	 * @param bSetSuperscript true → update the superscript (top-right); false → subscript (bottom-right).
	 */
	void SetSlotText(int Position, const FText& NewText, bool bSetSuperscript);

	///@}

	/** @name All-slot setters */
	///@{

	/**
	 * @brief Updates the shared hover-highlight (pointed) image for all slots.
	 * @param NewImage  The new image asset, or nullptr to clear.
	 */
	void SetSlotPointedImage(UObject* NewImage);

	/**
	 * @brief Updates the shared selection-highlight (selected) image for all slots.
	 * @param NewImage  The new image asset, or nullptr to clear.
	 */
	void SetSlotSelectedImage(UObject* NewImage);

	/**
	 * @brief Updates the superscript or subscript font for all slots.
	 * @param bSetSuperscript  true → set superscript font; false → set subscript font.
	 * @param NewFont          The new font info to apply.
	 */
	void SetFont(bool bSetSuperscript, const FSlateFontInfo& NewFont);

	///@}
};
