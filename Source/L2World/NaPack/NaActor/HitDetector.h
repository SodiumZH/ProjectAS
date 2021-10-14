#pragma once

#include "CoreMinimal.h"
#include "../NaGlobalHeader.h"
#include "../NaUtility/NaUtility.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HitDetector.generated.h"

class UBoxComponent;
struct FHitResult;

DECLARE_DYNAMIC_DELEGATE_OneParam(FHitDetectedSignature, const FHitResult &, HitResult);
DECLARE_DELEGATE_TwoParams(FTickHitDetectionSignature, float, TArray<FHitResult> &);

/* Hit Detector is an actor class which detects overlap hit with other actors.
* Designed for combats that need to detect hit of overlap objects e.g. weapon.
* Hit Detector Interface itself isn't encoded any function of detection itself, but only virtual hit detection report functions. 
* Hit detection should be implemented in inheriting classes.
*/
UCLASS(Blueprintable)
class NAPACK_API AHitDetectorInterface :public AActor {

	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	/* Hit notify */

protected:

	// Function directly called when hit detected. Should not be called anywhere else.
	void CalledOnHitDetected(const FHitResult & HitResult);

public: 

	// C++ actions when hit is detected
	virtual void OnHitDetected(const FHitResult & HitResult) {};
	

	// BP Actions when hit is detected
	UFUNCTION(BlueprintNativeEvent, DisplayName = "On Hit Detected", Category = "NaPack|Actor|HitDetector")
	void OnHitDetected_BP(const FHitResult & HitResult);
	void OnHitDetected_BP_Implementation(const FHitResult & HitResult) {};


	// Actions when hit is detected as delegate
	UPROPERTY(DisplayName = "On Hit Detected Delegate")
	FHitDetectedSignature OnHitDetected_Delegate;




	/* Hit options */

	// If false, it will stop tracing, returning no hit rusult. This value can be set in runtime.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitDetector")
	bool bOpened = true;

	// If true, the hit event will generate only once for an actor, unless the actor is manually retrieved.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitDetector")
	bool bHitOnlyOnce = true;

	// Whether the hit detection will trace complex collision.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitDetector")
	bool bTraceComplex = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitDetector")
	TEnumAsByte<ETraceTypeQuery> TraceChannel;


	/* Ignore list */

protected:

	TArray<AActor*> IgnoreActors;

public:

	// Add an actor to ignore list
	UFUNCTION(BlueprintCallable, Category = "NaPack|Actor|HitDetector")
	void AddIgnore(AActor* Ignore);

	// Add actors in array to ignore list
	UFUNCTION(BlueprintCallable, Category = "NaPack|Actor|HitDetector")
	void AddIgnoreMulti(const TArray<AActor*> Ignore);

	// Remove an actor from ignore list
	UFUNCTION(BlueprintCallable, Category = "NaPack|Actor|HitDetector")
	void ResumeIgnored(AActor* InActor);

	// Remove actors in array from ignore list
	UFUNCTION(BlueprintCallable, Category = "NaPack|Actor|HitDetector")
	void ResumeIgnoredMulti(const TArray<AActor*> InActorSet);


	/* Trace debug options */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitDetector|DebugOptions")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitDetector|DebugOptions")
	FLinearColor TraceColor = FLinearColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitDetector|DebugOptions")
	FLinearColor TraceHitColor = FLinearColor::Green;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitDetector|DebugOptions")
	float DrawTime = 5.0f;


	/* Hit detection */

	virtual void Tick(float dt) override;

	/* Method override to detect hit. Will be executed every tick unless closed (bOpened == false).
	* Note: DO NOT add the detected actors to ignore list!! This will be done automatically. If an actor is added to ignore list before or in this function, the OnHitDetected event will not generate for it.
	* Note: DO NOT execute it in Tick manually! Or it will be executed twice. 
	* Note: this function will not be executed when closed (bOpened == false). If something needs to be updated when closed, implement in Tick function instead.
	* @Param DeltaTime Tick delta time.
	* @Param HitResult Hit result array generated from tick detection. It will automatically generate OnHitDetected event with each element.
	*/
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Tick Hit Detecting Method", Keywords = "hit detect hit check"), Category = "NaPack|Actor|HitDetector")
	void Tick_DetectHit(float DeltaTime, TArray<FHitResult> & HitResult);
	virtual void Tick_DetectHit_Implementation(float DeltaTime, TArray<FHitResult> & HitResult) {};

	// Hit result array. It will be updated every frame.
	TArray<FHitResult> HitResultTemp;

};

UCLASS(BlueprintType)
class NAPACK_API ABoxHitDetector :public AHitDetectorInterface {

	GENERATED_BODY()

public:

	ABoxHitDetector();

	virtual void OnConstruction(const FTransform& trans) override;

	virtual void Tick(float dt) override;

protected:

	virtual void BeginPlay() override;

	/* Components */


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* SceneRoot;

	// Box component as template. Tracing will use its transform and shape, but the component itself doesn't apply collision.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* TemplateBox;

	/* Hit detection */

	FVector LastLocation;
	
	bool bIsLastLocationInitialized = false;

public:

	// Main detection function by box sweeping
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf, DisplayName = "Default Hit Detecting Method (Box Hit Detector)", Keywords = "hit detect hit check hit"))
	void Tick_DetectHit_BoxHitDetector(float TickDeltaTime, TArray<FHitResult> & HitResult);

};