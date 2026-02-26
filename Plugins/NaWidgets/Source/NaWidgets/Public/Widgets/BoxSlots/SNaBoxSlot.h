// By Sodium
// SNaBoxSlot.h - Declares SNaBoxSlot, a single interactive Slate box slot widget
// used for displaying items, shop entries, skills, etc.

#pragma once

#include "CoreMinimal.h"
#include "Fonts/SlateFontInfo.h"
#include "Widgets/Input/SButton.h"
#include "SNaBoxSlot.generated.h"

class SButton;
class SOverlay;
class SLayeredImage;
class STextBlock;

/**
 * @brief Initialization and configuration parameters for an SNaBoxSlot widget.
 *
 * This struct carries all the data needed to construct or reset an SNaBoxSlot.
 * It is passed by pointer during construction (via the `Params` SLATE_ATTRIBUTE)
 * and is copied into the widget — the external pointer is not retained after
 * `Construct()` returns.
 *
 * ## Image layer rendering order (bottom to top)
 * 1. **ImageBase**    — background / item icon
 * 2. **ImageFrame**   — decorative border / frame overlay
 * 3. **ImagePointed** — highlight shown while the cursor hovers the slot
 * 4. **ImageSelected** — highlight shown while the slot is in the selected state
 */
USTRUCT(BlueprintType)
struct FNaBoxSlotParams {

	GENERATED_BODY()

public:

	/** Visual size (width × height) of the rendered slot in Slate units. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaBoxSlotParams")
	FVector2D Size = FVector2D(64.f, 64.f);

	/** Base image drawn on the bottom layer (e.g. an item icon or slot background). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaBoxSlotParams", meta = (AllowedClasses = "/Script/Engine.Texture, /Script/Engine.MaterialInterface, /Script/Engine.SlateTextureAtlasInterface", DisallowedClasses = "/Script/MediaAssets.MediaTexture"))
	UObject* ImageBase = nullptr;

	/** Slot frame drawn above the base layer (e.g. a decorative border). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaBoxSlotParams", meta = (AllowedClasses = "/Script/Engine.Texture, /Script/Engine.MaterialInterface, /Script/Engine.SlateTextureAtlasInterface", DisallowedClasses = "/Script/MediaAssets.MediaTexture"))
	UObject* ImageFrame = nullptr;

	/** Overlay image shown while the cursor is hovering (pointed) over the slot. Drawn above base and frame. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaBoxSlotParams", meta = (AllowedClasses = "/Script/Engine.Texture, /Script/Engine.MaterialInterface, /Script/Engine.SlateTextureAtlasInterface", DisallowedClasses = "/Script/MediaAssets.MediaTexture"))
	UObject* ImagePointed = nullptr;

	/** Overlay image shown when the slot is in the selected state. Drawn on the top layer. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaBoxSlotParams", meta = (AllowedClasses = "/Script/Engine.Texture, /Script/Engine.MaterialInterface, /Script/Engine.SlateTextureAtlasInterface", DisallowedClasses = "/Script/MediaAssets.MediaTexture"))
	UObject* ImageSelected = nullptr;

	/** Text overlay drawn in the top-right corner of the slot (e.g. a stack count or level indicator). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaBoxSlotParams")
	FText SuperscriptText = FText::FromString(TEXT(""));

	/** Text overlay drawn in the bottom-right corner of the slot (e.g. a cooldown timer or quantity). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaBoxSlotParams")
	FText SubscriptText = FText::FromString(TEXT(""));

	/** Default constructor — all images null, texts empty, size 64×64. */
	FNaBoxSlotParams() {};

	/** Shared default params instance used when no explicit params are provided. */
	static FNaBoxSlotParams DefaultParams;
};

/**
 * @brief Identifies one of the four image layers composited inside an SNaBoxSlot.
 *
 * Layers are rendered in ascending order so higher values appear on top:
 * - `BSIL_Base`     → bottom-most layer (background / icon)
 * - `BSIL_Frame`    → decorative frame above the base
 * - `BSIL_Pointed`  → hover highlight, above frame
 * - `BSIL_Selected` → selection highlight, top-most layer
 */
UENUM(BlueprintType)
enum class ENaBoxSlotImageLayer :uint8 {
	BSIL_Base		UMETA(DisplayName = "Base"),
	BSIL_Frame		UMETA(DisplayName = "Frame"),
	BSIL_Pointed	UMETA(DisplayName = "ImagePointed"),
	BSIL_Selected	UMETA(DisplayName = "ImageSelected")
};


/**
 * @brief A single interactive box slot widget for displaying items, shop entries, skills, etc.
 *
 * SNaBoxSlot is an SCompoundWidget that composes a button, a layered image stack, and two
 * optional text overlays (superscript / subscript).  It supports two interactive states:
 * - **Pointed** (hovered) — automatically set when the mouse enters/leaves the button area.
 * - **Selected** — must be set explicitly by calling SetSelected().
 *
 * ## Image layers (bottom → top)
 * 1. Base image  (`BSIL_Base`)
 * 2. Frame image (`BSIL_Frame`)
 * 3. Pointed image (`BSIL_Pointed`) — visible only while pointed (or also while selected if `bShowPointedWhenSelected` is true)
 * 4. Selected image (`BSIL_Selected`) — visible only while selected
 *
 * ## Usage example
 * @code
 * FNaBoxSlotParams SlotParams;
 * SlotParams.Size          = FVector2D(64.f, 64.f);
 * SlotParams.ImageBase     = MyItemTexture;
 * SlotParams.ImageFrame    = MyFrameTexture;
 * SlotParams.ImagePointed  = MyHoverTexture;
 * SlotParams.ImageSelected = MySelectTexture;
 * SlotParams.SuperscriptText = FText::FromString(TEXT("99"));
 *
 * TSharedRef<SNaBoxSlot> BoxSlot = SNew(SNaBoxSlot)
 *     .Params(&SlotParams)
 *     .bShowPointedWhenSelected(true)
 *     .OnPointed_Lambda([]() { UE_LOG(LogTemp, Log, TEXT("Hovered")); })
 *     .OnSelected_Lambda([]() { UE_LOG(LogTemp, Log, TEXT("Selected")); });
 * @endcode
 *
 * @see SNaSlotList, FNaBoxSlotParams, ENaBoxSlotImageLayer
 */
class NAWIDGETS_API SNaBoxSlot : public SCompoundWidget
{
public:
	
	SLATE_BEGIN_ARGS(SNaBoxSlot)
	{
		_Params = nullptr;
		_SubscriptFont = FTextBlockStyle::GetDefault().Font;
		_SuperscriptFont = FTextBlockStyle::GetDefault().Font;
		_SubscriptColor = FLinearColor(1, 1, 1, 1);
		_SuperscriptColor = FLinearColor(1, 1, 1, 1);
		_bShowPointedWhenSelected = true;
	}

	/** Initialization parameters. Only read during Construct(); the pointer is not retained afterwards. */
	SLATE_ATTRIBUTE(const FNaBoxSlotParams*, Params)
	/** Font used to render the subscript (bottom-right) text overlay. */
	SLATE_ATTRIBUTE(FSlateFontInfo, SubscriptFont)
	/** Font used to render the superscript (top-right) text overlay. */
	SLATE_ATTRIBUTE(FSlateFontInfo, SuperscriptFont)
	/** Color of the subscript text overlay. */
	SLATE_ATTRIBUTE(FLinearColor, SubscriptColor)
	/** Color of the superscript text overlay. */
	SLATE_ATTRIBUTE(FLinearColor, SuperscriptColor)
	/** When true, the pointed (hover) image remains visible while the slot is also selected. */
	SLATE_ATTRIBUTE(bool, bShowPointedWhenSelected)

	/** Fired when the slot becomes pointed (mouse enters). Bound automatically to mouse-enter. */
	SLATE_EVENT(FSimpleDelegate, OnPointed)

	/** Fired when the slot stops being pointed (mouse leaves). Bound automatically to mouse-leave. */
	SLATE_EVENT(FSimpleDelegate, OnUnpointed)

	/** Fired when the slot enters the selected state via SetSelected(true). */
	SLATE_EVENT(FSimpleDelegate, OnSelected)

	/** Fired when the slot leaves the selected state via SetSelected(false). */
	SLATE_EVENT(FSimpleDelegate, OnUnselected)
		
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

protected:

	/** Copy of the params used to build this slot (not a pointer — lifetime-safe). */
	FNaBoxSlotParams Params;
	/** Font for the subscript text overlay. */
	FSlateFontInfo SubscriptFont;
	/** Font for the superscript text overlay. */
	FSlateFontInfo SuperscriptFont;
	FLinearColor SubscriptColor;
	FLinearColor SuperscriptColor;

	/** Slate brush for the base layer image. */
	TSharedPtr<FSlateBrush> BrushBase;
	/** Slate brush for the frame layer image. */
	TSharedPtr<FSlateBrush> BrushFrame;
	/** Slate brush for the pointed (hover) layer image. */
	TSharedPtr<FSlateBrush> BrushPointed;
	/** Slate brush for the selected layer image. */
	TSharedPtr<FSlateBrush> BrushSelected;

	/** Clickable button that also drives pointer (hover) state changes. */
	TSharedPtr<SButton> Button;
	/** Overlay that stacks the layered image and text blocks. */
	TSharedPtr<SOverlay> Overlay;
	/** Layered image widget compositing the four image layers. */
	TSharedPtr<SLayeredImage> Images;
	/** Text block for the subscript (bottom-right) overlay. */
	TSharedPtr<STextBlock> Subscript;
	/** Text block for the superscript (top-right) overlay. */
	TSharedPtr<STextBlock> Superscript;
	
protected:

	/** Whether the slot is currently in the selected state. */
	bool bIsSelected = false;

	/** Whether the slot is currently being pointed at (hovered). */
	bool bIsPointed = false;

	/** Cached value of the bShowPointedWhenSelected construction argument. */
	bool bShowPointedWhenSelected;

	/** Shows or hides the pointed (hover) image layer. */
	void SetShowPointedImage(bool Val);

	/** Shows or hides the selected image layer. */
	void SetShowSelectedImage(bool Val);

public:

	/** Returns the underlying SButton subwidget, e.g. to bind additional click events. */
	TSharedPtr<SButton> GetButton() { return Button; };

public:

	/** Returns true if the slot is currently in the selected state. */
	FORCEINLINE bool IsSelected() { return bIsSelected; };
	/** Returns true if the slot is currently being pointed at (hovered). */
	FORCEINLINE bool IsPointed() { return bIsPointed; };

	/**
	 * @brief Sets the selected state of the slot.
	 *
	 * Calling with true shows the selected image layer and fires OnSelected.
	 * Calling with false hides the selected image layer and fires OnUnselected.
	 *
	 * @param NewSelectedState The desired selected state.
	 */
	void SetSelected(bool NewSelectedState);

	/**
	 * @brief Sets the pointed (hover) state of the slot.
	 *
	 * Calling with true shows the pointed image layer and fires OnPointed.
	 * Calling with false hides the pointed image layer and fires OnUnpointed.
	 * This is wired automatically to mouse enter/leave events on the button.
	 *
	 * @param NewPointedState The desired pointed state.
	 */
	void SetPointed(bool NewPointedState);

	/**
	 * @brief Replaces the image on the specified layer.
	 *
	 * @param Layer  The target image layer (Base, Frame, Pointed, or Selected).
	 * @param NewImage  A UTexture2D, UMaterialInterface, or other compatible asset, or nullptr to clear.
	 */
	void SetImage(ENaBoxSlotImageLayer Layer, UObject* NewImage);

	/**
	 * @brief Reinitializes the slot with a completely new set of parameters.
	 *
	 * This updates all images, text overlays, and the slot size to match NewParams.
	 *
	 * @param NewParams  The new parameter set to apply.
	 */
	void Reset(const FNaBoxSlotParams& NewParams);

	/**
	 * @brief Sets the superscript or subscript text overlay.
	 *
	 * @param bSetSuperscript  true → set the superscript (top-right); false → set the subscript (bottom-right).
	 * @param InText           The text to display.
	 */
	void SetText(bool bSetSuperscript, const FText& InText);

	/**
	 * @brief Sets the font for the superscript or subscript text overlay.
	 *
	 * @param bSetSuperscript  true → set the superscript font; false → set the subscript font.
	 * @param NewFont          The new font info to apply.
	 */
	void SetFont(bool bSetSuperscript, const FSlateFontInfo& NewFont);

	/**
	 * @brief Returns the current font for the superscript or subscript text overlay.
	 *
	 * @param bGetSuperscriptFont  true → return the superscript font; false → return the subscript font.
	 * @return The requested FSlateFontInfo.
	 */
	FSlateFontInfo GetFont(bool bGetSuperscriptFont);

	/** Delegate fired when the slot becomes pointed. Automatically bound to mouse-enter on the button. */
	FSimpleDelegate OnPointed;

	/** Delegate fired when the slot stops being pointed. Automatically bound to mouse-leave on the button. */
	FSimpleDelegate OnUnpointed;

	/** Delegate fired when the slot enters the selected state. Must be triggered via SetSelected(true). */
	FSimpleDelegate OnSelected;

	/** Delegate fired when the slot leaves the selected state. Must be triggered via SetSelected(false). */
	FSimpleDelegate OnUnselected;

protected:

	void ExecOnPointed() { SetPointed(true); };
	void ExecOnUnpointed() { SetPointed(false); };
	void ExecOnSelected() { SetSelected(true); };
	void ExecOnUnselected() { SetSelected(false); };

};
