// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "Fonts/SlateFontInfo.h"
#include "Widgets/SButton.h"
#include "SNaBoxSlot.generated.h"

class SButton;
class SOverlay;
class SLayeredImage;
class STextBlock;

/**
* Params to initialize and construct FNaBoxSlot
*/
USTRUCT(BlueprintType)
struct FNaBoxSlotParams {

	GENERATED_BODY()

public:

	/* Drawed size */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaBoxSlotParams")
	FVector2D Size = FVector2D(64.f, 64.f);

	/* Base image drawn on the bottom layer */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaBoxSlotParams", meta = (AllowedClasses = "Texture,MaterialInterface,SlateTextureAtlasInterface", DisallowedClasses = "MediaTexture"))
	UObject* ImageBase = nullptr;

	/* Slot frame. Drawn above the base. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaBoxSlotParams", meta = (AllowedClasses = "Texture,MaterialInterface,SlateTextureAtlasInterface", DisallowedClasses = "MediaTexture"))
	UObject* ImageFrame = nullptr;

	/* Additional image when pointed at. Drawn above base and frame. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaBoxSlotParams", meta = (AllowedClasses = "Texture,MaterialInterface,SlateTextureAtlasInterface", DisallowedClasses = "MediaTexture"))
	UObject* ImagePointed = nullptr;

	/* Additional image when selected. Drawn on the top. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaBoxSlotParams", meta = (AllowedClasses = "Texture,MaterialInterface,SlateTextureAtlasInterface", DisallowedClasses = "MediaTexture"))
	UObject* ImageSelected = nullptr;

	/* Text for superscript (drawn on the top right). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaBoxSlotParams")
	FText SuperscriptText = FText::FromString(TEXT(""));

	/* Text for subscript (drawn on the bottom right). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NaBoxSlotParams")
	FText SubscriptText = FText::FromString(TEXT(""));

	/* Defaults */
	FNaBoxSlotParams() {};
	static FNaBoxSlotParams DefaultParams;


};

UENUM(BlueprintType)
enum class ENaBoxSlotImageLayer :uint8 {
	BSIL_Base	UMETA(DisplayName = "Base"),
	BSIL_Frame	UMETA(DisplayName = "Frame"),
	BSIL_Pointed	UMETA(DisplayName = "ImagePointed"),
	BSIL_Selected	UMETA(DisplayName = "ImageSelected")
};


/**
 * SNaBoxSlot is a box for displaying items, shops, skills, etc.
 * It can be pointed, selected, dragged and dropped (not implemented).

 */
class NAWIDGETS_API SNaBoxSlot : public SCompoundWidget
{
public:
	
	SLATE_BEGIN_ARGS(SNaBoxSlot)
	{
		_Params = nullptr;
		_SubscriptFont = FTextBlockStyle::GetDefault().Font;
		_SuperscriptFont = FTextBlockStyle::GetDefault().Font;
		_bShowPointedWhenSelected = true;
	}

	SLATE_ATTRIBUTE(const FNaBoxSlotParams*, Params) /* Only for initialization. After initialization this pointer will be no longer valid. */
	SLATE_ATTRIBUTE(FSlateFontInfo, SubscriptFont)
	SLATE_ATTRIBUTE(FSlateFontInfo, SuperscriptFont)
	SLATE_ATTRIBUTE(bool, bShowPointedWhenSelected)	/* If true, when the slot is both selected and pointed, the pointed image will still be shown. */

	/* Called when pointed i.e. mouse hovered */
	SLATE_EVENT(FSimpleDelegate, OnPointed)	

	/* Called when unpointed i.e. mouse unhovered */
	SLATE_EVENT(FSimpleDelegate, OnUnpointed) 

	/* Called when selected. Conditions of selection is custom-defined by calling SetSelected(true). */
	SLATE_EVENT(FSimpleDelegate, OnSelected)

	/* Called when unselected. Conditions of unselection is custom-defined by calling SetSelected(false). */
	SLATE_EVENT(FSimpleDelegate, OnUnselected)
		
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

protected:

	/* Params */
	FNaBoxSlotParams Params;
	FSlateFontInfo SubscriptFont;
	FSlateFontInfo SuperscriptFont;

	/* Ptrs of brushes for each laer */
	TSharedPtr<FSlateBrush> BrushBase;
	TSharedPtr<FSlateBrush> BrushFrame;
	TSharedPtr<FSlateBrush> BrushPointed;
	TSharedPtr<FSlateBrush> BrushSelected;

	/* Ptrs of subwidgets */
	TSharedPtr<SButton> Button;
	TSharedPtr<SOverlay> Overlay;
	TSharedPtr<SLayeredImage> Images;
	TSharedPtr<STextBlock> Subscript;
	TSharedPtr<STextBlock> Superscript;
	
protected:

	bool bIsSelected = false;

	bool bIsPointed = false;

	bool bShowPointedWhenSelected;

	void SetShowPointedImage(bool Val);

	void SetShowSelectedImage(bool Val);

public:

	/* Get functions of subwidgets */

	TSharedPtr<SButton> GetButton() { return Button; };


public:

	FORCEINLINE bool IsSelected() { return bIsSelected; };
	FORCEINLINE bool IsPointed() { return bIsPointed; };

	/* Set selected state */
	void SetSelected(bool NewSelectedState);

	/* Set pointed state */
	void SetPointed(bool NewPointedState);

	/* Set image of a specified layer */
	void SetImage(ENaBoxSlotImageLayer Layer, UObject* NewImage);

	/* Reset all params */
	void Reset(const FNaBoxSlotParams & NewParams);

	/* Set text.
	* @Param bSetSuperscript If true, it will set superscript. Or it will set subscript.
	*/
	void SetText(bool bSetSuperscript, const FText & InText);

	/* Set font.
	* @Param bSetSuperscript If true, it will set superscript. Or it will set subscript.
	*/
	void SetFont(bool bSetSuperscript, const FSlateFontInfo & NewFont);
	
	FSlateFontInfo GetFont(bool bGetSuperscriptFont);

public:



public:

	/* Actions on pointed to this slot. Executed when calling SetPointed(true) */
	FSimpleDelegate OnPointed;
	void ExecOnPointed() { SetPointed(true); };

	/* Actions on stop pointing to this slot. Executed when calling SetPointed(false) */
	FSimpleDelegate OnUnpointed;
	void ExecOnUnpointed() { SetPointed(false); };

	/* Actions on selecting this slot. Executed when calling SetSelected(true) */
	FSimpleDelegate OnSelected;
	void ExecOnSelected() { SetSelected(true); };

	/* Actions on this slot unselected. Executed when calling SetSelected(false) */
	FSimpleDelegate OnUnselected;
	void ExecOnUnselected() { SetSelected(false); };


};
