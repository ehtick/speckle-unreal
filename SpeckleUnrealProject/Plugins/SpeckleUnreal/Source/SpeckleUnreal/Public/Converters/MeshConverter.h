// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Converter.h"
#include "UObject/Object.h"
#include "MeshConverter.generated.h"

class ASpeckleUnrealMesh;
class UMesh;
/**
 * 
 */
UCLASS(Abstract)
class SPECKLEUNREAL_API UMeshConverter : public UObject, public IConverter
{
	GENERATED_BODY()

protected:
	UMeshConverter()
	{
		static ConstructorHelpers::FObjectFinder<UMaterial> SpeckleMaterial(TEXT("Material'/SpeckleUnreal/SpeckleMaterial.SpeckleMaterial'"));
		static ConstructorHelpers::FObjectFinder<UMaterial> SpeckleGlassMaterial(TEXT("Material'/SpeckleUnreal/SpeckleGlassMaterial.SpeckleGlassMaterial'"));

		DefaultMeshMaterial = SpeckleMaterial.Object;
		BaseMeshOpaqueMaterial = SpeckleMaterial.Object;
		BaseMeshTransparentMaterial = SpeckleGlassMaterial.Object;
	}

public:
	/** Material to be applied to meshes when no RenderMaterial can be converted */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speckle|Materials")
	UMaterialInterface* DefaultMeshMaterial;

	/** Material Parent for converted opaque materials*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speckle|Materials")
	UMaterialInterface* BaseMeshOpaqueMaterial;

	/** Material Parent for converted materials with an opacity less than one */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speckle|Materials")
	UMaterialInterface* BaseMeshTransparentMaterial;
	
	/** When generating meshes, materials in this TMap will be used instead of converted ones if the key matches the ID of the Object's RenderMaterial. (Takes priority over name matching)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speckle|Materials|Overrides", DisplayName = "By Speckle ID")
	TMap<FString, UMaterialInterface*> MaterialOverridesById;

	/** When generating meshes, materials in this TSet will be used instead of converted ones if the material name matches the name of the Object's RenderMaterial. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speckle|Materials|Overrides", DisplayName = "By Name")
	TSet<UMaterialInterface*> MaterialOverridesByName;

	/** Materials converted from stream RenderMaterial objects */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Speckle|Materials")
	TMap<FString, UMaterialInterface*> ConvertedMaterials;

	UFUNCTION(BlueprintCallable)
	virtual UMaterialInterface* GetMaterial(const class URenderMaterial* SpeckleMaterial, const bool AcceptMaterialOverride = true);

	virtual TSubclassOf<UBase> GetConversionType_Implementation() override PURE_VIRTUAL(UMeshConverter::GetConversionType, return UBase::StaticClass(););

protected:
	UFUNCTION(BlueprintCallable)
	virtual UMaterialInterface* CreateMaterial(const class URenderMaterial* SpeckleMaterial) PURE_VIRTUAL(UMeshConverter::CreateMaterial, return nullptr; );
};
