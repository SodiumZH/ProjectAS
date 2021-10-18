#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../NaGlobalHeader.h"
#include "../../NaUtility/NaUtility.h"
#include "NaMobStatusManager.generated.h"

class ANaMob;

UENUM(BlueprintType)
enum class ENaMobMovementType:uint8 {
	MMT_Walk		UMETA(DisplayName = "Walking"), // Normally walking, using walking speed
	MMT_Run			UMETA(DisplayName = "Running"), // Normally running, using running speed
	MMT_Special		UMETA(DisplayName = "Special"), // Special state of movement, e.g. using skill which enables movement, and use manually specified speed by setting "Realtime Speed Scale".
	MMT_NoMove		UMETA(DisplayName = "NoMovement") // Disable movement
};

UENUM(BlueprintType)
enum class ENaMobJumpType :uint8 {
	MJT_Default		UMETA(DisplayName = "Default"), // Using default jump height
	MJT_Special		UMETA(DisplayName = "Special"), // Use manually-specified jump height, by setting "Realtime Jump Height Scale".
	MJT_NoJump		UMETA(DisplayName = "NoJump")	// Disable jump
};


/* Mob Status Manager is a recorder of runtime-variable data of a mob. 
* E.g. HP, MP, atk, speed, can/can't move/jump, is/isn't dead...
* It will update itself then the mob every tick. Data in this component should not be directly imported from save data, but calculated from Data Manager or be set in runtime.
* Also, data in this component should be directly appliable. For example, attack stored in this component should be the real value (calculated from basic ablilty, equipment, buff, etc.) and can be directly used for damage calculation. 
* Data directly imported or calculated from save data should be stored in Mob Data Manager (e.g. basic attack).
* Only valid for mobs.
*/
UCLASS(BlueprintType)
class NAPACK_API UNaMobStatusManager : public UActorComponent {

	GENERATED_BODY()

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*********** Movement related ************/

	/* Mob movement type.
	* Walking: use walk speed scale.
	* Running: use run speed scale.
	* Special: use manually-set speed scale (by setting "Realtime Speed Scale").
	* No Movement: zero speed scale which means movement disabled.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobStatus|Movement")
	ENaMobMovementType MovementType = ENaMobMovementType::MMT_Run;

	/* Speed scale when using walk status. This value will be multiplied by basic walk speed. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobStatus|Movement")
	float WalkSpeedScale = 1.0;

	/* Speed scale when using run status. This value will be multiplied by basic walk speed. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobStatus|Movement")
	float RunSpeedScale = 2.0;

	/* Real speed scale applied on mob. This value will be multiplied by basic walk speed.
	* On walking, running and no-movement, setting this value will not work since it will be set back to the expected value every frame.
	* On special movement, this value will not be automatically updated and is needed to be set manually (once is OK, no need to be every-frame).
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobStatus|Movement")
	float RealtimeSpeedScale = 1.0;

	/* Max acceleration scale. This value will be multipied by basic acceletaion. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobStatus|Movement")
	float MaxAccel = 1.0;

	UFUNCTION(BlueprintPure, meta = (DefaultToSelf, Keywords = "enable movement enabled"), Category = "NaPack|MobSystem|MobStatus|Movement")
	bool CanMove() { return MovementType != ENaMobMovementType::MMT_NoMove; };


	/*********** Jump related ************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobStatus|Jump")
	ENaMobJumpType JumpType = ENaMobJumpType::MJT_Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobStatus|Jump")
	float DefaultJumpHeightScale = 1.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobStatus|Jump")
	float RealtimeJumpHeightScale = 1.0;

	UFUNCTION(BlueprintPure, meta = (DefaultToSelf, Keywords = "enable movement enabled"), Category = "NaPack|MobSystem|MobStatus|Movement")
	bool CanJump() { return JumpType != ENaMobJumpType::MJT_NoJump; };


	/* Stamina related */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobStatus|Stamina")
	bool bIsInvincible = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobStatus|Stamina")
	bool bIsDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobStatus|Stamina")
	int64 CurrentHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobStatus|Stamina")
	int64 MaxHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobStatus|Stamina")
	int64 CurrentMP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobStatus|Stamina")
	int64 MaxMP = 100;

	/* Status synchonization */

	/* If true, it will update properties every frame. If you're sure the properties will not change in a certain time period, set it false to improve performance.
	* Movement properties (speed, jump, accel) in this component needs to be applied by synchonizing to movement component. 
	* If synchoniation is off, setting these values will not change the real value until next sync. 
	* This value can be set in runtime and works immediately.
	* When this value is false, you can also call function "StatusSync()"(C++) or "Status Synchonize"(BP) to manually synchonize when needed.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Keywords = "status state data synchonize"), Category = "MobStatus|Synchonization")
	bool bDoTickSync = true;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Status Synchonize", Keywords = "state status data synchronize"), Category = "NaPack|MobSystem|MobStatus|Synchonization")
	virtual void StatusSync();

	/* On mob status manager doing status synchonization. 
	* It will be executed every tick (when bDoTickSync == true) or when calling StatusSync().
	*/
	UFUNCTION(BlueprintNativeEvent, DisplayName = "On Status Synchonized", Category = "NaPack|MobSystem|MobStatus|Synchonization")
	void OnStatusSync();
	void OnStatusSync_Implementation() {};
};