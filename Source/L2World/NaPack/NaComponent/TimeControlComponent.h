

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimeControlComponent.generated.h"


DECLARE_DYNAMIC_DELEGATE(FTimePointEventSignature);
DECLARE_DYNAMIC_DELEGATE_OneParam(FTimeLoopEventSignature, float, TimeNow);

USTRUCT()
struct FTimePointEvent {
	
	GENERATED_USTRUCT_BODY()

public:

	double TimePointSeconds;	// Time from begin play

	FTimePointEventSignature EventDelegate;

	FTimePointEvent();

	FTimePointEvent(double Seconds, const FTimePointEventSignature & Event);

};

USTRUCT()
struct FTimeLoopEvent {
	
	GENERATED_USTRUCT_BODY()

public:

	double StartTime;	// Time from begin play

	double LoopTime;

	FTimeLoopEventSignature EventDelegate;

	double NextTime;	// Time from begin play

	FTimeLoopEvent();

	FTimeLoopEvent(double Start, double Loop, const FTimeLoopEventSignature & Event);

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class L2WORLD_API UTimeControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTimeControlComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:

	/* Time recording */

	double TimeBeginPlay = 0.0;

	// Absolute time
	double TimeLastTick = 0.0;

	// Absolute time
	double TimeThisTick = 0.0;

	// Get time from begin play.
	double GetTime();

	// Get time of last tick from begin play
	double GetTimeLastTick();

	void TimeInit();

	void Tick_UpdateTime();

	/* Events */

	TMap<FName, FTimePointEvent> PointEvents;

	TMap<FName, FTimeLoopEvent> LoopEvents;

	void Tick_PointEvents();
	
	void Tick_LoopEvents();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Get time seconds from begin play of this component */
	UFUNCTION(BlueprintPure, meta = (DefaultToSelf), Category="NaPack|Component")
	float GetTimeSecondsFromBeginPlay();

	/** Add time point event using time seconds from now. The event will be executed only once.
	* @Param ForceAdd If true, when the name is existing, the new event will override the old one. Or new event will not be added if the name is existing.
	*/
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf), Category = "NaPack|Component")
	void AddTimePointEvent(FName Name, float TimeSecondsFromNow, const FTimePointEventSignature & Event, bool bForceAdd = true);

	/* Add loop event using time seconds from now as the first execution time.
	* @Param ForceAdd If true, when the name is existing, the new event will override the old one. Or new event will not be added if the name is existing.
	*/
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf), Category = "NaPack|Component")
	void AddLoopEvent(FName Name, float TimeSecondsFromNow, float Period, const FTimeLoopEventSignature & Event, bool bForceAdd = true);

	UFUNCTION(BlueprintCallable, Category = "NaPack|Component")
	void RemoveTimePointEvent(FName Name, bool ExecuteBeforeRemove = false);

	UFUNCTION(BlueprintCallable, Category = "NaPack|Component")
	void RemoveLoopEvent(FName Name, bool ExecuteBeforeRemove = false);

};
