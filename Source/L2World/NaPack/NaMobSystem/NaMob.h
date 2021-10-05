// Base class of all character-based mobs.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NaMobBase.h"
#include "../NaGlobalHeader.h"
#include "NaMob.generated.h"

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

UCLASS(BlueprintType)
class NAPACK_API ANaMob : public ACharacter, public INaMobBase
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

	UTimeControlComponent * TimeControl;

public:

	UFUNCTION(BlueprintPure, meta = (DefaultToSelf), Category = "NaPack|MobSystem")
	UTimeControlComponent * GetTimeControl() { return TimeControl; };


	UFUNCTION(BlueprintPure, Category = "NaPack|MobSystem")
	float GetTimeFromSpawn();

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

 

	/*==========================================================================*/
	/* Mob Data */

public:

	// Mob's general data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mob|Data")
	FNaMobGeneralData GeneralData = FNaMobGeneralData();
	
	// Tickly update interior variables from GeneralData
	virtual void Tick_DataSync();
	// Actions when tickly update the interior variables from General Data
	UFUNCTION(BlueprintImplementableEvent, Category = "NaPack|MobSystem")
	void OnDataSync();

	// Get if mob enables movement
	UFUNCTION(BlueprintPure, Category = "NaPack|MobSystem")
	static bool MobCanMove(ANaMob* Target) { return Target->GeneralData.Movement.bCanMove; };

	// Enable/disable mob movement
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf), Category = "NaPack|MobSystem")
	void SetMobEnableMovement(bool Value) { GeneralData.Movement.bCanMove = Value; };

	// Get if mob enables jump
	UFUNCTION(BlueprintPure, Category = "NaPack|MobSystem")
	static bool MobCanJump(ANaMob* Target) { return Target->GeneralData.Movement.bCanJump; };

	// Enable/disable mob jump
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf), Category = "NaPack|MobSystem")
	void SetMobEnableJump(bool Value) { GeneralData.Movement.bCanJump = Value; };

	UFUNCTION(BlueprintPure, Category = "NaPack|MobSystem")
	static FNaMobBasicInformation GetBasicInformation(ANaMob* Target) { return Target->GeneralData.BasicInfo; };

	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf), Category = "NaPack|MobSystem")
	void SetBasicInformation(const FNaMobBasicInformation& Value) { GeneralData.BasicInfo = Value; };

	UFUNCTION(BlueprintPure, Category = "NaPack|MobSystem")
	static FNaMobMovementData GetMovementData(ANaMob* Target) { return Target->GeneralData.Movement; };

	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf), Category = "NaPack|MobSystem")
	void SetMovementData(const FNaMobMovementData& Value) { GeneralData.Movement = Value; };

	UFUNCTION(BlueprintPure, Category = "NaPack|MobSystem")
	static FNaMobStamina GetStamina(ANaMob* Target) { return Target->GeneralData.Stamina; };

	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf), Category = "NaPack|MobSystem")
	void SetStamina(const FNaMobStamina& Value) { GeneralData.Stamina = Value; };

	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf), Category = "NaPack|MobSystem")
	static bool MobIsRunning(ANaMob* Target) { return Target->GeneralData.Movement.bIsRunning; };

	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf), Category = "NaPack|MobSystem")
	void SwitchWalkRun() { GeneralData.Movement.bIsRunning = !GeneralData.Movement.bIsRunning; };

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


	/* Skill */
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf), Category = "NaPack|MobSystem")
	class ANaMobSkill* UseSkill(
		TSubclassOf<class ANaMobSkill> SkillClass,
		const FTransform & InTransform,
		class USceneComponent* AttachToComponent = nullptr,
		FName SocketName = NAME_None,
		bool DoAttachment = true
	);

	
	//=======================================================================
	//	Animation

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "AnimationClass"), category = "Mob|Mesh|Animation")
	TSubclassOf<UAnimInstance> AnimClass;

	// Apply anim class settings to sk-meshes
	void UpdateAnimClass();
public:

	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf), Category = "NaPack|MobSystem")
	void SetAnimClass(TSubclassOf<UAnimInstance> NewClass);


	/* Anim state switch */

protected:

	TMap<FName, bool> AnimStateSwitch;

	TMap<FName, float> AnimSwitchCloseTime;

	void Tick_CloseAnimSwitch();

public:

	/** Get the animation switch value of a name. 
	Animation switch is a boolean specified by name for animation blueprint. In animation blueprint you can call switch value for state change.
	*/
	UFUNCTION(BlueprintPure, meta = (DefaultToSelf, Keywords = "animation switch anim switch animation state"), Category = "NaPack|MobSystem|AnimationSwitch")
	bool GetAnimStateSwitch(FName Key);

	/** Turn on an animation switch.
	* Animation switch is a boolean specified by name for animation blueprint. In animation blueprint you can call switch value for state change.
	* @Param Key Switch name.
	* @Param DeltaTime Time span that the switch keeps on. If this value is set non-positive, it will keep on until manually set off or overriden by next open action.
	*/
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf, Keywords = "animation switch anim switch animation state"), Category = "NaPack|MobSystem|AnimationSwitch")
	void OpenAnimStateSwitch(FName Key, float DeltaTime = 0.1);

	/** Turn off an animation switch.
	* Animation switch is a boolean specified by name for animation blueprint. In animation blueprint you can call switch value for state change.
	*/
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf, Keywords = "animation switch anim switch animation state"), Category = "NaPack|MobSystem|AnimationSwitch")
	void CloseAnimStateSwitch(FName Key);


	/* Weapon related */

protected:
	
	TMap<FName, ANaMobWeapon*> Weapons;

public:

	ANaMobWeapon* GetWeaponFromRegisterName(FName Name);

	/* Get register name of a weapon. Please note that empty name ("") is a null return. If empty name is returned, it means the weapon is not registered. */
	FName GetRegisterName(ANaMobWeapon* Weapon);

	// Register a weapon to a mob. Please note that the empty name ("") cannot be a register name.
	// WARNING: this action will NOT attach the weapon or setup any ownership!!!
	// WARNING: If the same name is existing, this action will replace the old weapon and print warning to log.
	// @Param ReplacementNoWarning If true, warning will not be printed when replacement happens. For cases when replacement is intended.
	void RegisterWeapon(FName Name, ANaMobWeapon* Weapon, bool ReplacementNoWarning = false);

	// Remove a weapon by name. WARNING: this action will NOT detach the weapon!!!
	void RemoveWeapon(FName Name);

	// Remove a weapon by ptr. WARNING: this action will NOT detach the weapon!!!
	// If the weapon is registered more than once, it will remove all.
	void RemoveWeapon(ANaMobWeapon* Weapon);
};
