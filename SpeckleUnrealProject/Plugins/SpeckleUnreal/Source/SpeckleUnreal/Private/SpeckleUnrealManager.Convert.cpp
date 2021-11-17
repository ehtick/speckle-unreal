#include "SpeckleUnrealManager.h"






float ASpeckleUnrealManager::ParseScaleFactor(const FString& Units) const
{
	static const auto ParseUnits = [](const FString& LUnits) -> float
	{
		if (LUnits == "millimeters" || LUnits == "millimeter" || LUnits == "millimetres" || LUnits == "millimetre" || LUnits == "mm")
			return 0.1;
		if (LUnits == "centimeters" || LUnits == "centimeter" ||LUnits == "centimetres" || LUnits == "centimetre" || LUnits == "cm")
			return 1;
		if (LUnits == "meters" || LUnits == "meter" || LUnits == "metres" || LUnits == "metre" || LUnits == "m")
			return 100;
		if (LUnits == "kilometers" || LUnits == "kilometres" || LUnits == "km")
			return 100000;

		if (LUnits == "inches" || LUnits == "inch" || LUnits == "in")
			return 2.54;
		if (LUnits == "feet" || LUnits == "foot" || LUnits == "ft")
			return 30.48;
		if (LUnits == "yards" || LUnits == "yard"|| LUnits == "yd")
			return 91.44;
		if (LUnits == "miles" || LUnits == "mile" || LUnits == "mi")
			return 160934.4;

		return 1;
	};

	return ParseUnits(Units.ToLower()) * WorldToCentimeters;
}

TArray<TSharedPtr<FJsonValue>> ASpeckleUnrealManager::CombineChunks(const TArray<TSharedPtr<FJsonValue>>& ArrayField) const
{
	TArray<TSharedPtr<FJsonValue>> ObjectPoints;
		
	for(int32 i = 0; i < ArrayField.Num(); i++)
	{
		FString Index;
		if(ArrayField[i]->AsObject()->TryGetStringField("referencedId", Index))
		{
			const auto Chunk = SpeckleObjects[Index]->GetArrayField("data");
			ObjectPoints.Append(Chunk);
		}
		else
		{
			return ArrayField; //Array was never chunked to begin with
		}
	}
	return ObjectPoints;
}


// ASpeckleUnrealActor* ASpeckleUnrealManager::CreateBlockInstance(const TSharedPtr<FJsonObject> Obj)
// {
// 	//Transform
//     const FString Units = Obj->GetStringField("units");
// 	const float ScaleFactor = ParseScaleFactor(Units);
// 	
// 	const TArray<TSharedPtr<FJsonValue>>* TransformData;
// 	if(!Obj->TryGetArrayField("transform", TransformData)) return nullptr;
// 	
// 	
// 	FMatrix TransformMatrix;
// 	for(int32 Row = 0; Row < 4; Row++)
// 	for(int32 Col = 0; Col < 4; Col++)
// 	{
// 		TransformMatrix.M[Row][Col] = TransformData->operator[](Row * 4 + Col)->AsNumber();
// 	}
// 	TransformMatrix = TransformMatrix.GetTransposed();
// 	TransformMatrix.ScaleTranslation(FVector(ScaleFactor));
// 	
// 	//Block Instance
// 	const FString ObjectId = Obj->GetStringField("id"), SpeckleType = Obj->GetStringField("speckle_type");
//
// 	ASpeckleUnrealActor* BlockInstance = World->SpawnActor<ASpeckleUnrealActor>();
// 	BlockInstance->SetActorLabel(FString::Printf(TEXT("%s - %s"), *SpeckleType, *ObjectId));
//
// 	
// 	BlockInstance->SetActorTransform(FTransform(TransformMatrix));
// 	
// 	//Block Definition
// 	const TSharedPtr<FJsonObject>* BlockDefinitionReference;
// 	if(!Obj->TryGetObjectField("blockDefinition", BlockDefinitionReference)) return nullptr;
// 	
// 	const FString RefID = BlockDefinitionReference->operator->()->GetStringField("referencedId");
// 	
// 	const TSharedPtr<FJsonObject> BlockDefinition = SpeckleObjects[RefID];
// 	
// 	//For now just recreate mesh, eventually we should use instanced static mesh
// 	const auto Geometries = BlockDefinition->GetArrayField("geometry");
//
// 	for(const auto Child : Geometries)
// 	{
// 		const TSharedPtr<FJsonObject> MeshReference = Child->AsObject();
// 		const FString MeshID = MeshReference->GetStringField("referencedId");
// 		
// 		//It is important that ParentObject is the BlockInstance not the BlockDefinition to keep NativeIDs of meshes unique between Block Instances
// 		ImportObjectFromCache(BlockInstance, SpeckleObjects[MeshID], Obj);
// 	}
// 	
// 	
// 	return BlockInstance;
// }
//
//
//
