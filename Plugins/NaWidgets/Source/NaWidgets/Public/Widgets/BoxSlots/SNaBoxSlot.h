// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "Fonts/SlateFontInfo.h"
#include "Widgets/SCompoundWidget.h"
#include "SNaBoxSlot.generated.h"

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

	/* Events */

	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override;

	
	DECLARE_DELEGATE(FNaBoxSlotSelectionEvent);

	/* Actions on pointed to this slot */
	FNaBoxSlotSelectionEvent OnPointed;

	/* Actions on stop pointing to this slot */
	FNaBoxSlotSelectionEvent OnUnpointed;

	/* Actions on selecting this slot. */
	FNaBoxSlotSelectionEvent OnSelected;

	/* Actions on this slot unselected. */
	FNaBoxSlotSelectionEvent OnUnselected;
	
	DECLARE_DELEGATE_TwoParams(FNaBoxSlotMouseEvent, const FGeometry&, const FPointerEvent&);

	FNaBoxSlotMouseEvent EventMouseButtonDown;

	FNaBoxSlotMouseEvent EventMouseButtonUp;

	FNaBoxSlotMouseEvent EventMouseMove;

	FNaBoxSlotMouseEvent EventMouseButtonDoubleClick;

};
