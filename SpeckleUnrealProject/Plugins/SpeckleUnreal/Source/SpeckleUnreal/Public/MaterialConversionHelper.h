// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RenderMaterial.h"

/**
 * Helper functions for converting materials
 */
class FMaterialConversionHelper
{
	public:

		static void AssignPropertiesFromSpeckle(UMaterialInstanceDynamic* Material, const URenderMaterial* SpeckleMaterial);

		static URenderMaterial* ParseRenderMaterial(const TSharedPtr<FJsonObject> SpeckleObject);

};
