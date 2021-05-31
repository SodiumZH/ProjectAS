// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMobBase.h"
#include "LMob.generated.h"

UCLASS()
class L2WORLD_API ALMob : public ACharacter, public ILMobBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALMob();

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




	/////////////////////
	/** Skeletal Mesh **/
	/////////////////////


	/* Mesh */

public:

	// All applied skeletal mesh resource; use as array since there may be seperated meshes for a single object
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "Skeletal Meshes"), category = "Mob|Mesh")
	TArray<USkeletalMesh*> SkeletalMeshes;

	// Skeletal mesh component array. Each update of skeletal mesh should reset this array.
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Skeletal Mesh Components", Keywords = "skmesh"), category = "L2W|Mob")
	TArray<USkeletalMeshComponent*> GetSkeletalMeshComponents() { return SkeletalMeshComponents; };

	// Reset skeletal mesh components from skeletal mesh setting.
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Reset Skeletal Mesh Components", Keywords = "skmesh"), category = "L2W|Mob")
	void ResetSkMeshComponents(const TArray<USkeletalMesh*> & NewMeshes);

protected:
	
	TArray<USkeletalMeshComponent*> SkeletalMeshComponents;

	// Clear existing skeletal meshes.
	void ClearSkeletalMesh();

	// Update skeletal mesh from variable SkeletalMeshes
	void UpdateSkeletalMesh();


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
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Reset Enable Material Override", Keywords = "enable"), category = "L2W|Mob")
	void ResetEnableMatOverride(bool NewEnable);

	// Reset material overrides. If material override is disabled, it will do nothing.
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Reset Materials"), category = "L2W|Mob")
	void ResetMaterials(const TArray<UMaterialInterface*> & NewMats, const TArray<int> & NewMapping);


protected:

	// Default material
	UMaterial * DefaultMat = LoadObject<UMaterial>(nullptr, TEXT("/Engine/Content/EngineMaterials/WorldGridMaterial.WorldGridMaterial"));

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


	/* Animations */


};
