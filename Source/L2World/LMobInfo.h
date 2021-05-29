#pragma once

#include "CoreMinimal.h"
#include "LMobInfo.generated.h"


USTRUCT(BlueprintType)
struct FLMobInfo {

	GENERATED_USTRUCT_BODY()

public:

	FLMobInfo() {};

	// Current health
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 CurrentHP = 100;

	// Current magic
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 CurrentMP = 100;

	// Max health
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 MaxHP = 100;

	// Max magic
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 MaxMP = 100;

	// Attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 Atk = 1;

	// Defense
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 Def = 1;

};