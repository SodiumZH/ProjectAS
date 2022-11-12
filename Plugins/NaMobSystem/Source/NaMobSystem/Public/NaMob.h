// Base class of all character-based mobs.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NaMobBase.h"
#include "NaMob.generated.h"

#define NAPACK_MOB_SYSTEM
#define MOB_DEFAULT_HEIGHT 180.f
#define MOB_DEFAULT_DIAMETER 60.f


/**

Content

General Functions	R34
Mesh Settings		R55
	Meshes			R59
	Materials		R86
Mesh-collision Transform Correction		R139
	Capsule			R141
	Mesh			R187
Data				R227
Basic Action		R282
Animation			R321
	Animation State Switch	

*/

class ANaMobWeapon;
class UTimeControlComponent;
class UNaMobSkillManager;
class ANaMobSkill;
class UNaMobWeaponManager;
class UNaMobStateManager;
class UNaMobDataManager;
class UNaMobBasicStateManager;

UCLASS(BlueprintType)
class NAMOBSYSTEM_API ANaMob : public ACharacter, public INaMobBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANaMob();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Construction script
	virtual void OnConstruction(const FTransform & trans) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	/* Components */

protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UTimeControlComponent* TimeControl;
	UTimeControlComponent* TimeControl_0;	// For unknown nullization

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UNaMobSkillManager* SkillManager;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UNaMobWeaponManager* WeaponManager;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UNaMobBasicStateManager* BasicStateManager;
	UNaMobBasicStateManager* BasicStateManager_0;	// For unknown nullization

public:

	UTimeControlComponent * GetTimeControl() { return TimeControl; };
	UNaMobSkillManager* GetSkillManager() { return SkillManager; };
	UNaMobWeaponManager* GetWeaponManager() { return WeaponManager; };
	UNaMobBasicStateManager* GetBasicStateManager() { return BasicStateManager; };

	// Get all actors related to this mob, including itself, skills, skill collisions, weapons
	void GetAllRelatives(TArray<AActor*>& Out);


	/* Types */

	/* Check if there's any type conflict in the mob. 
	* Return true if everything is right. Return false if type conflict exists, and also print error information to log.
	* A type conflict is a case in which mob type components (e.g. player component, enemy component) are not correctly configured, mostly when there's multiple in one mob.
	*/
	bool CheckTypeConflict();

	/* Assert when type conflict exists in a mob. Return true if everything is right.
	* Warning: This function may cause CRASH! For the no-crash version, use CheckTypeConflict().
	*/
	bool AssertNoTypeConflict();

	// Return if this mob is a player mob i.e. contains a player component.
	bool IsPlayerMob();

	// Return the mob's player component. If there isn't one, return null.
	class UNaMobPlayerComponent* GetPlayerComponent();

	// Return if this mob is an enemy mob i.e. contains an enemy component.
	bool IsEnemyMob();

	// Return the mob's enemy component. If there isn't one, return null.
	class UNaMobEnemyComponent* GetEnemyComponent();


	/*==========================================================================*/
	////////////
	/** Mesh **/
	////////////


	/* Mesh */

public:

	// All applied skeletal mesh resource; use as array since there may be seperated meshes for a single object.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "Skeletal Meshes"), category = "Mob|Mesh")
	TArray<USkeletalMesh*> SkeletalMeshes;

	// Skeletal mesh component array. Each update of skeletal mesh should reset this array.
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Skeletal Mesh Components", Keywords = "skmesh"), category = "NaPack|MobSystem")
	TArray<USkeletalMeshComponent*> GetSkeletalMeshComponents() { return SkeletalMeshComponents; };

	// Reset skeletal mesh components from skeletal mesh setting.
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Reset Skeletal Mesh Components", Keywords = "skmesh"), category = "NaPack|MobSystem")
	void ResetSkMeshComponents(const TArray<USkeletalMesh*> & NewMeshes);

protected:
	
	TArray<USkeletalMeshComponent*> SkeletalMeshComponents;

	// Clear existing skeletal meshes.
	void ClearSkeletalMesh();

	// Update skeletal mesh from variable SkeletalMeshes
	void UpdateSkeletalMesh();

	/*==========================================================================*/
	/* Materials */

public:

	// If true, materials will use materials below instead of originally set materials on skeletal meshes.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "Enable Material Override"), category = "Mob|Mesh|Material")
	bool bEnableMaterialOverride = false;

	// All materials. Each material can appear only once since they will be mapped.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "Materials"), category = "Mob|Mesh|Material")
	TArray<UMaterialInterface*> Materials;

	// Mesh-Material map, i.e. define which materials a sk-mesh of a specific index should apply.
	// The order is: mesh0-mat0, mesh0-mat1,..., mesh0-matn, mesh1-mat0,....
	// If there are repeated elements, use negative values to show how many times it should repeat.
	// For example, if you want [1,1,1,1,2], you can input [1,-3,2] in which -3 represents 3 repeats of the last element of -3 i.e. 1.
	// Negative values on the beginning will be treated as repeats of 0. 
	// If this array is not long enough, the lost elements will repeat the last element.
	// E.g. [1] with 3 total material sockets will be treated as [1,1,1]. Empty array is regarded as [0].
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Material Index Map"), category = "Mob|Mesh|Material")
	TArray<int> MatMapping;

	// Reset boolean "enable material override" and update materials if needed.
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Reset Enable Material Override", Keywords = "enable"), category = "NaPack|MobSystem")
	void ResetEnableMatOverride(bool NewEnable);

	// Reset material overrides. If material override is disabled, it will do nothing.
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Reset Materials"), category = "NaPack|MobSystem")
	void ResetMaterials(const TArray<UMaterialInterface*> & NewMats, const TArray<int> & NewMapping);


protected:

	// Default material
	UMaterial* DefaultMat;

	// Record original materials when updating meshes. When mat override is disabled they will be applied.
	TArray<UMaterialInterface*> OriginalMats;

	// Decode matmapping data for applying it on resetting. Meshes are taken from variable "SkeletalMeshComponents", so set it correctly before calling this.
	TArray<int> DecodeMatMapping(const TArray<int>& input);

	// Apply materials and DECODED matmapping to existing meshes. It will IGNORE whether override enabled.
	// Warning: this func is unsafe. Any wrong input will cause error.
	void ApplyMaterials_Unsafe(const TArray<UMaterialInterface*> & Mat, const TArray<int> & DecodedMap);

	// Update materials from variables "Materials" and "MatMapping". If override is disabled it will do nothing.
	void UpdateMaterials();

	// Reset materials to original materials
	void InitializeMaterials();

	/*==========================================================================*/
	/* Mesh-collision Correction */

	// Capsule //

public:

	// Capsule height scale rel to default
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "Capsule Height Scale"), category = "Mob|Capsule")
	float CapsuleHeightScale = 1.f;

	// Capsule diameter scale rel to default
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "Capsule Diameter Scale"), category = "Mob|Capsule")
	float CapsuleDiameterScale = 1.f;

	// If true, XYZ of mesh scale keeps equal.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Lock Mesh Scale", Keywords = "scale size"), category = "Mob|Mesh")
	bool bLockMeshScale = true;

	// Only when Lock Mesh Scale is true, this value overrides scale in Mesh Offset.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Locked Mesh Scale Value XYZ", Keywords="scale size"), category = "Mob|Mesh")
	float LockedScale3D = 1.f;

	// If true, XY of mesh scale keeps equal. Ignored if Lock Mesh Scale is true.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Lock Mesh Scale XY", Keywords = "scale size"), category = "Mob|Mesh")
	bool bLockMeshScaleXY = false;
	
	// Only when Lock Mesh Scale XY is applied, this value overrides scale XY in Mesh Offset.
	// Set Z value in variable Mesh Offset.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Locked Mesh Scale Value XY", Keywords = "scale size"), category = "Mob|Mesh")
	float LockedScaleXY = 1.f;

	// Mesh offset relative to capsule. Adjust this to align mesh's feet to capsule's bottom. 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "Mesh Offset", Keywords = "transforms"), category = "Mob|Mesh")
	FTransform MeshOffset = FTransform();

	UFUNCTION(BlueprintCallable, meta=(DisplayName = "Reset Capsule Scale", Keywords = "set reset capsule scale size", DefaultToSelf), category = "NaPack|MobSystem")
	void SetCapsuleScale(float HeightScale = 1.f, float DiameterScale = 1.f);

protected:

	// Refresh capsule size. Do this after setting capsule scale to apply it.
	void UpdateCapsule();

	// Reset capsule size
	void ResetCapsuleSize(float HeightScale, float DiameterScale);



	// Mesh //

public:

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Reset Mesh Offset", Keywords = "set reset mesh offset transform location rotation position scale size"), category = "NaPack|MobSystem")
	void SetMeshOffset(FTransform InTrans);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Reset Mesh Offset (In LRS)", Keywords = "set reset mesh offset transform location rotation position scale size"), category = "NaPack|MobSystem")
	void SetMeshOffset_LRS(FVector Location = FVector(0, 0, 0), FRotator Rotation = FRotator(0, 0, 0), FVector Scale = FVector(1, 1, 1));

	UFUNCTION(BlueprintCallable, DisplayName = "Reset Mesh Location", category = "NaPack|MobSystem")
	void SetMeshLocation(FVector InLoc = FVector(0, 0, 0));

	UFUNCTION(BlueprintCallable, DisplayName = "Reset Mesh Rotation", category = "NaPack|MobSystem")
	void SetMeshRotation(FRotator InRot = FRotator(0, 0, 0));
	
	UFUNCTION(BlueprintCallable, DisplayName = "Reset Mesh Scale", category = "NaPack|MobSystem")
	void SetMeshScale(FVector InScale = FVector(0, 0, 0));


protected:

	// Calculate actual transform to apply
	FTransform GetMeshTransApplied();

	// Refresh mesh transform. Call this after setting mesh transfrom to apply it. 
	void UpdateMeshTransform();

	// Reset mesh size
	void ResetMeshTransform(FTransform Trans);

	// Initialization of capsule and mesh on construction
	void InitCapsuleMeshSize();

 

public:

	/*==========================================================================*/
	/* Basic actions */

	// Kill a mob
	UFUNCTION(BlueprintCallable, Category = "NaPack|MobSystem")
	virtual void MobDie();
	UFUNCTION(BlueprintImplementableEvent, Category = "NaPack|MobSystem")
	void OnMobDying();

	// Resume a mob, set HP & MP max with no other effects
	UFUNCTION(BlueprintCallable, Category = "NaPack|MobSystem")
	void DefaultMobResume();

	// Resume a mob, with custom settings
	UFUNCTION(BlueprintCallable, Category = "NaPack|MobSystem")
	void CustomMobResume(int64 NewHP = 1);

	// Actions on any type of resuming
	UFUNCTION(BlueprintImplementableEvent, Category = "NaPack|MobSystem")
	void OnMobResuming();

	// Actions only on custom resuming
	UFUNCTION(BlueprintImplementableEvent, Category = "NaPack|MobSystem")
	void OnCustomMobResuming(int64 NewHP);

	// Take damage
	UFUNCTION(BlueprintCallable, Category = "NaPack|MobSystem")
	virtual void MobTakeDamage(int64 Damage);
	// Actions when taking damage
	UFUNCTION(BlueprintImplementableEvent, Category = "NaPack|MobSystem")
	void OnMobTakingDamage(int64 Damage);
	
	//=======================================================================
	//	Animation

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "AnimationClass"), category = "Mob|Mesh|Animation")
	TSubclassOf<class UAnimInstance> AnimClass;

	// Apply anim class settings to sk-meshes
	void UpdateAnimClass();
public:

	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf), Category = "NaPack|MobSystem")
	void SetAnimClass(TSubclassOf<UAnimInstance> NewClass);


	/* Anim state switch */

protected:

	TMap<FName, bool> AnimStateSwitch;

	TMap<FName, double> AnimSwitchCloseTime;

	void Tick_CloseAnimSwitch();

public:

	/** Get the animation switch value of a name.
	Animation switch is a boolean specified by name for animation blueprint. In animation blueprint you can call switch value for state change.
	*/
	bool GetAnimStateSwitch(FName Key);

	/** Turn on an animation switch.
	* Animation switch is a boolean specified by name for animation blueprint. In animation blueprint you can call switch value for state change.
	* @Param Key Switch name.
	* @Param DeltaTime Time span that the switch keeps on. If this value is set non-positive, it will keep on until manually set off or overriden by next open action.
	*/
	void OpenAnimStateSwitch(FName Key, float DeltaTime = 0.1);

	/** Turn off an animation switch.
	* Animation switch is a boolean specified by name for animation blueprint. In animation blueprint you can call switch value for state change.
	*/
	void CloseAnimStateSwitch(FName Key);



};
