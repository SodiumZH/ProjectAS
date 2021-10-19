#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../NaGlobalHeader.h"
#include "NaMobDataManager.generated.h"

/* Mob Data Manager is intended to store data of a mob that are not needed to be tickly updated.
* For example, name, level, basic attack (intrinsic, unrelated with buffs).
* Data manager doesn't tick to save resource. Only when manually updated (e.g. changing equipment) it will change its state.
* Properties in this component should be cast to (processed with certain methods) the state manager's properties to make effect.
* For "static" properties (e.g. name, gender) the values can be directly called by other modules (e.g. UI). 
*/

UCLASS(BlueprintType)
class NAPACK_API UNaMobDataManager :public UActorComponent {

	GENERATED_BODY()

public:

	UNaMobDataManager();

	// Internal name in database/display to distinct mobs. Must be unique.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobData|Info")
	FName InternalName = TEXT("Mob");

	// Display name in the game. Not required to be unique.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobData|Info")
	FString DisplayName = TEXT("Mob");

	
};