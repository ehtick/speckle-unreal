// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Converter.h"
#include "SpeckleUnrealMesh.h"
#include "Components/ActorComponent.h"
#include "SpeckleUnrealManager.h"

#include "MeshConverter.generated.h"

class ASpeckleUnrealManager;
class URenderMaterial;

UCLASS(ClassGroup=(Speckle), meta=(BlueprintSpawnableComponent))
class SPECKLEUNREAL_API UMeshConverter : public UActorComponent, public IConverter
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Speckle|Mesh|Materials")
	FString SpeckleType;

	UPROPERTY()
	UWorld* World;
	
	UPROPERTY()
	ASpeckleUnrealManager* Manager;
		
public:
	// Sets default values for this component's properties
	UMeshConverter();

	virtual void OnRegister() override;

	/** Material to be applied to meshes when no RenderMaterial can be converted */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speckle|Mesh|Materials")
	UMaterialInterface* DefaultMeshMaterial;

	/** Material Parent for converted opaque materials*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speckle|Mesh|Materials")
	UMaterialInterface* BaseMeshOpaqueMaterial;

	/** Material Parent for converted materials with an opacity less than one */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speckle|Mesh|Materials")
	UMaterialInterface* BaseMeshTransparentMaterial;
	
	/** When generating meshes, materials in this TMap will be used instead of converted ones if the key matches the ID of the Object's RenderMaterial. (Takes priority over name matching)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speckle|Mesh|Materials|Overrides", DisplayName = "By Speckle ID")
	TMap<FString, UMaterialInterface*> MaterialOverridesById;

	/** When generating meshes, materials in this TSet will be used instead of converted ones if the material name matches the name of the Object's RenderMaterial. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speckle|Mesh|Materials|Overrides", DisplayName = "By Name")
	TSet<UMaterialInterface*> MaterialOverridesByName;

	/** Materials converted from stream RenderMaterial objects */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Speckle|Mesh|Materials")
	TMap<FString, UMaterialInterface*> ConvertedMaterials;

	/** Specify the Actor class this converter will create, must inherit from ASpeckleUnrealMesh*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speckle|Mesh")
	TSubclassOf<ASpeckleUnrealMesh> MeshActor {
		ASpeckleUnrealMesh::StaticClass()
	};
	
	virtual UObject* ConvertToNative(const TSharedPtr<FJsonObject> Object, const TSharedPtr<FJsonObject> ParentObj) override;
	virtual FString GetSpeckleType() const override { return SpeckleType; }

	UMaterialInterface* CreateMaterial(TSharedPtr<FJsonObject> RenderMaterialObject, bool AcceptMaterialOverride = true);
	UMaterialInterface* CreateMaterial(const URenderMaterial* SpeckleMaterial, bool AcceptMaterialOverride = true);
	ASpeckleUnrealMesh* CreateMesh(const TSharedPtr<FJsonObject> Obj, const TSharedPtr<FJsonObject> Parent = nullptr);

	virtual void DeleteObjects() override;
	virtual void BeginDestroy() override;
};
