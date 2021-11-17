// Fill out your copyright notice in the Description page of Project Settings.


#include "MaterialConverter.h"
#include "Objects/RenderMaterial.h"

void UMaterialConverter::AssignPropertiesFromSpeckle(UMaterialInstanceDynamic* Material, const URenderMaterial* SpeckleMaterial)
{
	Material->SetScalarParameterValue("Opacity", SpeckleMaterial->Opacity);
	Material->SetScalarParameterValue("Metallic", SpeckleMaterial->Metalness);
	Material->SetScalarParameterValue("Roughness", SpeckleMaterial->Roughness);
	Material->SetVectorParameterValue("BaseColor", SpeckleMaterial->Diffuse);
	Material->SetVectorParameterValue("EmissiveColor", SpeckleMaterial->Emissive);
}
