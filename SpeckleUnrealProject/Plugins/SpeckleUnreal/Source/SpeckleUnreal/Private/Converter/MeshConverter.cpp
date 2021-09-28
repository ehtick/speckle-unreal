// Fill out your copyright notice in the Description page of Project Settings.


#include "Converter/MeshConverter.h"

#include "MaterialConversionHelper.h"

UMeshConverter::UMeshConverter()
{
	static ConstructorHelpers::FObjectFinder<UMaterial> SpeckleMaterial(TEXT("Material'/SpeckleUnreal/SpeckleMaterial.SpeckleMaterial'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> SpeckleGlassMaterial(TEXT("Material'/SpeckleUnreal/SpeckleGlassMaterial.SpeckleGlassMaterial'"));
	
	DefaultMeshMaterial = SpeckleMaterial.Object;
	BaseMeshOpaqueMaterial = SpeckleMaterial.Object;
	BaseMeshTransparentMaterial = SpeckleGlassMaterial.Object;
	
	//bWantsInitializeComponent = true;
}


void UMeshConverter::OnRegister()
{
	Super::OnRegister();
	
	Manager = static_cast<ASpeckleUnrealManager*>(this->GetOwner());
	ConvertedMaterials.Empty();
	World = GetWorld();
	Manager->AddConverter(this);
	
}



UObject* UMeshConverter::ConvertToNative(const TSharedPtr<FJsonObject> Object, const TSharedPtr<FJsonObject> ParentObj)
{
	return CreateMesh(Object, ParentObj);
}

UMaterialInterface* UMeshConverter::CreateMaterial(TSharedPtr<FJsonObject> RenderMaterialObject, const bool AcceptMaterialOverride)
{
	if (RenderMaterialObject->GetStringField("speckle_type") == "reference")
		RenderMaterialObject = Manager->GetSpeckleObjects()->operator[](RenderMaterialObject->GetStringField("referencedId"));

	//Parse to a URenderMaterial
	const URenderMaterial* SpeckleMaterial = FMaterialConversionHelper::ParseRenderMaterial(RenderMaterialObject);

	return CreateMaterial(SpeckleMaterial, AcceptMaterialOverride);
}

UMaterialInterface* UMeshConverter::CreateMaterial(const URenderMaterial* SpeckleMaterial, const bool AcceptMaterialOverride)
{
	const auto MaterialID = SpeckleMaterial->ObjectID;

	
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
	
	//Create Convert Material Instance
	UMaterialInterface* ExplicitMaterial;
	if(SpeckleMaterial->Opacity >= 1)
		ExplicitMaterial = BaseMeshOpaqueMaterial;
	else
		ExplicitMaterial = BaseMeshTransparentMaterial;
		
	UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(ExplicitMaterial, this, FName(SpeckleMaterial->Name));
	FMaterialConversionHelper::AssignPropertiesFromSpeckle(DynMaterial, SpeckleMaterial);

	ConvertedMaterials.Add(MaterialID, DynMaterial);
	
	return DynMaterial;
}

ASpeckleUnrealMesh* UMeshConverter::CreateMesh(const TSharedPtr<FJsonObject> Obj, const TSharedPtr<FJsonObject> Parent)
{
	const FString ObjId = Obj->GetStringField("id");
	UE_LOG(LogTemp, Log, TEXT("Creating mesh for object %s"), *ObjId);

	const FString Units = Obj->GetStringField("units");
	// unreal engine units are in cm by default but the conversion is editable by users so
	// this needs to be accounted for later.
	
	const float ScaleFactor = ASpeckleUnrealManager::ParseScaleFactor(Units);

	// The following line can be used to debug large objects
	// ScaleFactor = ScaleFactor * 0.1;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Manager;
	ASpeckleUnrealMesh* MeshInstance = World->SpawnActor<ASpeckleUnrealMesh>(MeshActor, SpawnParams);
	
	// attaches each speckleMesh under this actor (SpeckleManager)
	MeshInstance->AttachToActor(Manager, FAttachmentTransformRules::KeepRelativeTransform);
	MeshInstance->SetOwner(Manager);

	MeshInstance->SetActorLabel(FString::Printf(TEXT("%s - %s"), *ASpeckleUnrealMesh::StaticClass()->GetName(), *ObjId));


	//Parse Vertices
	TArray<FVector> ParsedVertices;
	{
		TArray<TSharedPtr<FJsonValue>> ObjectVertices = Manager->CombineChunks(&Obj->GetArrayField("vertices"));
		const auto NumberOfVertices = ObjectVertices.Num() / 3;
	
		ParsedVertices.SetNum(NumberOfVertices);

		for (size_t i = 0, j = 0; i < NumberOfVertices; i++, j += 3)
		{
			ParsedVertices[i] = FVector
			(
				ObjectVertices[j].Get()->AsNumber() * -1,
				ObjectVertices[j + 1].Get()->AsNumber(),
				ObjectVertices[j + 2].Get()->AsNumber()
			) * ScaleFactor;
		
		}
	}



	//Parse Triangles
	TArray<int32> ParsedTriangles;
	{
		TArray<TSharedPtr<FJsonValue>> ObjectFaces = Manager->CombineChunks(&Obj->GetArrayField("faces"));
		//convert mesh faces into triangle array regardless of whether or not they are quads

		int32 j = 0;
		while (j < ObjectFaces.Num())
		{
			if (ObjectFaces[j].Get()->AsNumber() == 0)
			{
				//Triangles
				ParsedTriangles.Add(ObjectFaces[j + 1].Get()->AsNumber());
				ParsedTriangles.Add(ObjectFaces[j + 2].Get()->AsNumber());
				ParsedTriangles.Add(ObjectFaces[j + 3].Get()->AsNumber());
				j += 4;
			}
			else
			{
				//Quads to triangles
				ParsedTriangles.Add(ObjectFaces[j + 1].Get()->AsNumber());
				ParsedTriangles.Add(ObjectFaces[j + 2].Get()->AsNumber());
				ParsedTriangles.Add(ObjectFaces[j + 3].Get()->AsNumber());

				ParsedTriangles.Add(ObjectFaces[j + 1].Get()->AsNumber());
				ParsedTriangles.Add(ObjectFaces[j + 3].Get()->AsNumber());
				ParsedTriangles.Add(ObjectFaces[j + 4].Get()->AsNumber());

				j += 5;
			}
		}
	}


	// Material priority (low to high): DefaultMeshMaterial, Material set on parent, Converted RenderMaterial set on mesh, MaterialOverridesByName match, MaterialOverridesById match
	UMaterialInterface* Material;

	if (Obj->HasField("renderMaterial"))
	{
		Material = CreateMaterial(Obj->GetObjectField("renderMaterial"));
	}
	else if (Parent && Parent->HasField("renderMaterial"))
	{
		Material = CreateMaterial(Parent->GetObjectField("renderMaterial"));
	}
	else
		Material = DefaultMeshMaterial;

	MeshInstance->SetMesh(ParsedVertices, ParsedTriangles, Material, FLinearColor::White);

	//UE_LOG(LogTemp, Warning, TEXT("Added %d vertices and %d triangles"), ParsedVertices.Num(), ParsedTriangles.Num());

	return MeshInstance;
}

void UMeshConverter::DeleteObjects()
{
	ConvertedMaterials.Empty();
}

void UMeshConverter::BeginDestroy()
{
	DeleteObjects();
	Super::BeginDestroy();
}
