// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "NaItemEffect.h"
#include "LItemEffectBase.generated.h"

/**
 * Base of game item effects, providing MobSystem interface
 */
UCLASS()
class PROJECTAS_API UItemEffectBase : public UNaItemEffect
{
	GENERATED_BODY()

public:

	//	virtual ENaItemContainerUsageResult ItemEffect_Implementation(int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition) override;

};
