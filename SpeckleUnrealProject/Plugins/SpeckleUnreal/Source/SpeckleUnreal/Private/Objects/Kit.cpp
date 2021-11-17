// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeckleUnrealManager.h"
#include "Objects/Base.h"
#include "Converters/Converter.h"

#include "Objects/Kit.h"

void UKit::ToNative(AActor* AOwner, ASpeckleUnrealManager* Manager, const TSharedPtr<FJsonObject> SpeckleObject, const TSharedPtr<FJsonObject> ParentObject)
	{
		if (!SpeckleObject->HasField("speckle_type"))
			return;
		if (SpeckleObject->GetStringField("speckle_type") == "reference" && SpeckleObject->HasField("referencedId")) {
			TSharedPtr<FJsonObject> ReferencedObj;
			if (Manager->SpeckleObjects.Contains(SpeckleObject->GetStringField("referencedId")))
				ToNative(AOwner, Manager, Manager->SpeckleObjects[SpeckleObject->GetStringField("referencedId")], ParentObject);
			return;
		}
		if (!SpeckleObject->HasField("id"))
			return; 


		AActor* Native = nullptr;

		const FString SpeckleType = SpeckleObject->GetStringField("speckle_type");
		if(kit.Converters.Contains(SpeckleType))
		{
			IConverter* Converter = Cast<IConverter>(kit.Converters[SpeckleType]);
			
			const FName t = "Unnamed Speckle Object";
			UBase* Base = static_cast<UBase*>(CreateDefaultSubobject(t, UBase::StaticClass(), Converter->GetConversionType()->StaticClass(), true, false));
			Base->Deserialize(SpeckleObject, Manager);
			
			Native = Converter->ToNative(Base);
		}
			
		if(IsValid(Native))
		{
			Native->AttachToActor(AOwner, FAttachmentTransformRules::KeepRelativeTransform);
			Native->SetOwner(AOwner);

			Manager->AddInprogressObject(Native);
		}
		else
		{
			Native = AOwner;
		}


		//Convert Children
		for (const auto& Kv : SpeckleObject->Values)
		{

			const TSharedPtr<FJsonObject>* SubObjectPtr;
			if (Kv.Value->TryGetObject(SubObjectPtr))
			{
				ToNative(Native, Manager, *SubObjectPtr, SpeckleObject);
				continue;
			}

			const TArray<TSharedPtr<FJsonValue>>* SubArrayPtr;
			if (Kv.Value->TryGetArray(SubArrayPtr))
			{
				for (const auto& ArrayElement : *SubArrayPtr)
				{
					const TSharedPtr<FJsonObject>* ArraySubObjPtr;
					if (!ArrayElement->TryGetObject(ArraySubObjPtr))
						continue;
					ToNative(Native, Manager, *ArraySubObjPtr, SpeckleObject);
				}
			}
		}
		
	}

void UKit::Clear()
{
	for(const auto Converter : kit.Converters)
	{
		IConverter* c = Cast<IConverter>(Converter.Value);
		if(c != nullptr)
			c->Clear();
	}
}
