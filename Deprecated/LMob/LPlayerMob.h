#pragma once

#include "CoreMinimal.h"
#include "../NaPack/NaMobSystem/NaPlayerMob.h"
#include "LMobData.h"
#include "LMobBase.h"
#include "LPlayerMob.generated.h"

UCLASS(BlueprintType)
class L2WORLD_API ALPlayerMob : public ANaPlayerMob, public ILMobBase
{

	GENERATED_BODY()

public:

	ALPlayerMob(){};

};