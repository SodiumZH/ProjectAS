// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SNaBoxSlot.generated.h"

class SOverlay;
class SLayeredImage;
class STextBlock;

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

	/* Default */
	FNaBoxSlotParams() {};

	
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
		_Params = FNaBoxSlotParams();
	}

	SLATE_ATTRIBUTE(FNaBoxSlotParams, Params)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

public:

	/* Params */
	FNaBoxSlotParams Params;

	TSharedPtr<FSlateBrush> BrushBase;
	TSharedPtr<FSlateBrush> BrushFrame;
	TSharedPtr<FSlateBrush> BrushPointed;
	TSharedPtr<FSlateBrush> BrushSelected;

	TSharedPtr<SOverlay> Overlay;
	TSharedPtr<SLayeredImage> Images;
	TSharedPtr<STextBlock> Subscript;
	TSharedPtr<STextBlock> Superscript;

protected:

	bool bIsSelected = false;
	bool bIsPointed = false;
public:

	FORCEINLINE bool IsSelected() { return bIsSelected; };
	FORCEINLINE bool IsPointed() { return bIsPointed; };

	void SetSelected(bool NewSelectedState);
	void SetPointed(bool NewPointedState);

};
