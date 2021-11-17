// Fill out your copyright notice in the Description page of Project Settings.

#include "Converters/MeshConverter.h"

#include "Objects/RenderMaterial.h"

UMaterialInterface* UMeshConverter::GetMaterial(const URenderMaterial* SpeckleMaterial, const bool AcceptMaterialOverride)
{
	const auto MaterialID = SpeckleMaterial->Id;
	
	if(AcceptMaterialOverride)
	{
		//Override by id
		if(MaterialOverridesById.Contains(MaterialID))
		{
			return MaterialOverridesById[MaterialID];
		}
		//Override by name
		const FString Name = SpeckleMaterial->Name;
		for (UMaterialInterface* Mat : MaterialOverridesByName)
		{
			if(Mat->GetName() == Name) return Mat;
		}
	}


	if(ConvertedMaterials.Contains(MaterialID))
	{
		return ConvertedMaterials[MaterialID];
	}
	
	return CreateMaterial(SpeckleMaterial);
}
