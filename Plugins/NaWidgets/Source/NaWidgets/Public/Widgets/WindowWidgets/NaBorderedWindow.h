// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Widgets/WindowWidgets/SNaBorderedWindow.h"
#include "NaBorderedWindow.generated.h"

/**
 * UMG wrapper for SNaBorderedWindow.
 * Exposes the 9-part bordered window widget for use in UMG and Blueprints.
 */
UCLASS()
class NAWIDGETS_API UNaBorderedWindow : public UWidget
{
	GENERATED_BODY()

public:
	/** Image and size configuration for all 9 parts. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FNaBorderedWindowParams Params;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sizing")
	FVector2D MinBodySize = FVector2D(50.f, 50.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sizing")
	FVector2D MaxBodySize = FVector2D(1000.f, 1000.f);

	/** Optional widget placed over the center body area. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
	UWidget* Content = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Bordered Window")
	void SetBodySize(FVector2D NewSize);

	UFUNCTION(BlueprintCallable, Category = "Bordered Window")
	void SetBorderWidths(float Top, float Bottom, float Left, float Right);

	UFUNCTION(BlueprintCallable, Category = "Bordered Window")
	FVector2D GetBodySize() const;

	TSharedPtr<SNaBorderedWindow> BorderedWindow;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void SynchronizeProperties() override;
	virtual const FText GetPaletteCategory() override;
};
