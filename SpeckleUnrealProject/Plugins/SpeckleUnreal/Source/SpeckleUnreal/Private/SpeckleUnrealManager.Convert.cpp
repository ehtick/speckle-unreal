#include "SpeckleUnrealManager.h"

#include "Converter/Converter.h"


bool ASpeckleUnrealManager::TryGetExistingNative(const FString &ObjectSpeckleId, UObject*& OutObject)
{
	if (InProgressObjects.Contains(ObjectSpeckleId))
	{
		OutObject = InProgressObjects[ObjectSpeckleId];
		return true;
	}
	
	if(!CreatedObjects.Contains(ObjectSpeckleId)) //TODO There is a risk that created objects might have been modified, so we might not want to re-use them
		return false;
	
	OutObject = CreatedObjects[ObjectSpeckleId];
	return IsValid(OutObject);
}


void ASpeckleUnrealManager::ImportObjectFromCache(const TSharedPtr<FJsonObject> SpeckleObject, const TSharedPtr<FJsonObject> ParentObject)
{
	
	if (!SpeckleObject->HasField("speckle_type")) return;
	
	if (SpeckleObject->GetStringField("speckle_type") == "reference" && SpeckleObject->HasField("referencedId"))
	{
		TSharedPtr<FJsonObject> ReferencedObj;
		if (SpeckleObjects.Contains(SpeckleObject->GetStringField("referencedId")))
			ImportObjectFromCache(SpeckleObjects[SpeckleObject->GetStringField("referencedId")], ParentObject);		
		return;
	}
	
	if (!SpeckleObject->HasField("id")) return;


	
	const FString ObjectId = SpeckleObject->GetStringField("id");
	const FString SpeckleType = SpeckleObject->GetStringField("speckle_type");

	//Get or Create Native
	UObject* Native;
	
	if(//!TryGetExistingNative(ObjectId, Native)
		//&&
	Converters.Contains(SpeckleType))
	{
		Native = Converters[SpeckleType]->ConvertToNative(SpeckleObject, ParentObject);
	//}
	
	//if(IsValid(Native))
	//{
		InProgressObjects.Add(ObjectId, Native);
	}


	//Object has no native conversion, go through each of its properties and try convert that
	
	for (const auto& Kv : SpeckleObject->Values)
	{
		const TSharedPtr<FJsonObject>* SubObjectPtr;
		if (Kv.Value->TryGetObject(SubObjectPtr))
		{
			ImportObjectFromCache(*SubObjectPtr, SpeckleObject);
			continue;
		}

		const TArray<TSharedPtr<FJsonValue>>* SubArrayPtr;
		if (Kv.Value->TryGetArray(SubArrayPtr))
		{
			for (auto& ArrayElement : *SubArrayPtr)
			{
				const TSharedPtr<FJsonObject>* ArraySubObjPtr;
				if (!ArrayElement->TryGetObject(ArraySubObjPtr))
					continue;
				ImportObjectFromCache(*ArraySubObjPtr, SpeckleObject);
			}
		}
	}
}





TArray<TSharedPtr<FJsonValue>> ASpeckleUnrealManager::CombineChunks(const TArray<TSharedPtr<FJsonValue>>* ArrayField) const
{
	TArray<TSharedPtr<FJsonValue>> ObjectPoints;
	for(int i = 0; i < ArrayField->Num(); i++)
	{
		const FString Index = ArrayField->operator[](i)->AsObject()->GetStringField("referencedId");
		const auto Chunk = SpeckleObjects[Index]->GetArrayField("data");
		ObjectPoints.Append(Chunk);
	}
	return ObjectPoints;
}

float ASpeckleUnrealManager::ParseScaleFactor(const FString& Units)
{
	// unreal engine units are in cm by default but the conversion is editable by users so
	// this needs to be accounted for later.
	const FString LUnits = Units.ToLower();
	
	if (LUnits == "kilometers" || LUnits == "kilometres" || LUnits == "km")
		return 100000;
	if (LUnits == "meters" || LUnits == "meter" || LUnits == "metres" || LUnits == "metre" || LUnits == "m")
		return 100;
	if (LUnits == "centimeters" || LUnits == "centimeter" ||LUnits == "centimetres" || LUnits == "centimetre" || LUnits == "cm")
		return 1;
	if (LUnits == "millimeters" || LUnits == "millimeter" || LUnits == "millimetres" || LUnits == "millimetre" || LUnits == "mm")
		return 0.1;

	if (LUnits == "miles" || LUnits == "mile" || LUnits == "mi")
		return 160934.4;
	if (LUnits == "yards" || LUnits == "yard"|| LUnits == "yd")
		return 91.44;
	if (LUnits == "feet" || LUnits == "foot" || LUnits == "ft")
		return 30.48;
	if (LUnits == "inches" || LUnits == "inch" || LUnits == "in")
		return 2.54;
	
	return 1;
}



