#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NaUtility.h"
#include "NaMobStateManager.generated.h"

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


/* Mob State Manager is a recorder of runtime-variable data of a mob. This component's tick is enabled.
E.g. HP, MP, atk, speed, can/can't move/jump, is/isn't dead...
Default state manager doesn't contain any data. Create subclass to add data.
It will update itself then the mob every tick. Data in this component should not be directly imported from save data, but calculated from Data Manager or be set in runtime.
Also, data in this component should be directly appliable. For example, attack stored in this component should be the real value (calculated from basic ablilty, equipment, buff, etc.) and can be directly used for damage calculation. 
Data directly imported or calculated from save data should be stored in Mob Data Manager (e.g. basic attack).
Only valid for mobs.
*/
UCLASS(Blueprintable, ClassGroup = (NaMobSystem), meta = (BlueprintSpawnableComponent))
class UNaMobStateManager : public UActorComponent {

	GENERATED_BODY()

public:
	UNaMobStateManager();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* State synchonization */

	/* If true, it will update properties every frame. If you're sure the properties will not change in a certain time period, set it false to improve performance.
	* Movement properties (speed, jump, accel) in this component needs to be applied by synchonizing to movement component. 
	* If synchoniation is off, setting these values will not change the real value until next sync. 
	* This value can be set in runtime and works immediately.
	* When this value is false, you can also call function "StateSync()"(C++) or "State Synchonize"(BP) to manually synchonize when needed.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Keywords = "state state data synchonize"), Category = "MobState|Synchonization")
	bool bDoTickSync = true;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "State Synchonize", Keywords = "state state data synchronize"), Category = "NaPack|MobSystem|MobState|Synchonization")
	virtual void StateSync();

	/* On mob state manager doing state synchonization. 
	* It will be executed every tick (when bDoTickSync == true) or when calling StateSync().
	*/
	UFUNCTION(BlueprintNativeEvent, DisplayName = "On State Synchonizing", Category = "NaPack|MobSystem|MobState|Synchonization")
	void OnStateSync();
	void OnStateSync_Implementation() {};
};


/* Default state component for mob's basic data, including movement, jump and dead/alive. */
UCLASS(ClassGroup = (NaMobSystem), Blueprintable)
class UNaMobBasicStateManager :public UNaMobStateManager {
	
	GENERATED_BODY()

public:

	virtual void StateSync() override;

	virtual void BeginPlay() override;

	/*********** Movement related ************/

protected:

	/* Mob movement type.
	* Walking: use walk speed scale.
	* Running: use run speed scale.
	* Special: use manually-set speed scale (by setting "Realtime Speed Scale").
	* No Movement: zero speed scale which means movement disabled.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MobState|Movement")
	ENaMobMovementType MovementType = ENaMobMovementType::MMT_Run;

public:

	UFUNCTION(BlueprintPure, Category = "NaPack|MobSystem|MobState|Movement")
	ENaMobMovementType GetMovementType() { return MovementType; };

	// Force set movement type.
	UFUNCTION(BlueprintCallable, Category = "NaPack|MobSystem|MobState|Movement")
	ENaMobMovementType SetMovementType(ENaMobMovementType NewType);
	
	// Enable/Disable movement. When enabling, it will refer to bRunWalkSetting to choose run or walk.
	UFUNCTION(BlueprintCallable, Category = "NaPack|MobSystem|MobState|Movement")
	void SetCanMove(bool CanMove);

	UFUNCTION(BlueprintNativeEvent, Category = "NaPack|MobSystem|MobState|Movement")
	void OnSetMovementType(ENaMobMovementType OldType, ENaMobMovementType NewType);
	void OnSetMovementType_Implementation(ENaMobMovementType OldType, ENaMobMovementType NewType) {};

	/* Run/Walk setting. True for run, false for walk. It determines which to choose when the mob changes from other movement states to run/walk. */
	UPROPERTY(BlueprintReadOnly, Category = "MobState|Movement")
	bool bRunWalkSetting = true;

	/* Set Run/Walk. This function is a setting: when it's running/walking, it will switch. In other cases, set new run/walk but doesn't immediately change movement type.
	* True for run, false for walk. 
	*/
	UFUNCTION(BlueprintCallable, Category = "NaPack|MobSystem|MobState|Movement")
	void SetRunWalk(bool NewRunWalk);

	/* Speed scale when using walk state. This value will be multiplied by basic walk speed. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobState|Movement")
	float WalkSpeedScale = 1.0;

	/* Speed scale when using run state. This value will be multiplied by basic walk speed. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobState|Movement")
	float RunSpeedScale = 2.0;

	/* Real speed scale applied on mob. This value will be multiplied by basic walk speed.
	* On walking, running and no-movement, setting this value will not work since it will be set back to the expected value every frame.
	* On special movement, this value will not be automatically updated and is needed to be set manually (once is OK, no need to be every-frame).
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobState|Movement")
	float RealtimeSpeedScale = 1.0;

	/* Max acceleration scale. This value will be multipied by basic acceletaion. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobState|Movement")
	float MaxAccel = 1.0;

	UFUNCTION(BlueprintPure, meta = (DefaultToSelf, Keywords = "enable movement enabled"), Category = "NaPack|MobSystem|MobState|Movement")
	bool CanMove() { return MovementType != ENaMobMovementType::MMT_NoMove; };

	/*********** Jump related ************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobState|Jump")
	ENaMobJumpType JumpType = ENaMobJumpType::MJT_Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobState|Jump")
	float DefaultJumpHeightScale = 1.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobState|Jump")
	float RealtimeJumpHeightScale = 1.0;

	UFUNCTION(BlueprintPure, meta = (DefaultToSelf, Keywords = "enable movement enabled"), Category = "NaPack|MobSystem|MobState|Movement")
	bool CanJump() { return JumpType != ENaMobJumpType::MJT_NoJump; };

	/* Stamina */
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobState|Stamina")
	bool bIsInvincible = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobState|Stamina")
	bool bIsDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobState|Stamina")
	int64 CurrentHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobState|Stamina")
	int64 MaxHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobState|Stamina")
	int64 CurrentMP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobState|Stamina")
	int64 MaxMP = 100;
};
 