#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NaMobControllerInterface.h"
#include "../../NaGlobalHeader.h"
#include "NaMobPlayerController.generated.h"

UCLASS(Blueprintable)
class NAPACK_API ANaMobPlayerController :public APlayerController, public INaMobControllerInterface {

	GENERATED_BODY()

public:

};