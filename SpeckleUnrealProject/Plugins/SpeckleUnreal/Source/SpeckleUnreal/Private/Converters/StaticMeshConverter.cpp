// Fill out your copyright notice in the Description page of Project Settings.


#include "Converters/StaticMeshConverter.h"

#include "Converters/MeshConverter.h"

#include "Objects/Mesh.h"
#include "SpeckleUnrealMesh.h"
#include "StaticMeshAttributes.h"
#include "StaticMeshDescription.h"
#include "StaticMeshOperations.h"
#include "AssetRegistry/AssetRegistryModule.h"



ASpeckleUnrealActor* UStaticMeshConverter::ToNative_Implementation(const UBase* Base)
{
	const UMesh* SMesh = Cast<const UMesh>(Base);
	return MeshToNative(SMesh);
}

ASpeckleUnrealMesh* UStaticMeshConverter::MeshToNative_Implementation(const UMesh* SMesh)
{
	return nullptr;
	// FString StreamID = "Speckle Stream"; //TODO set speckle stream
	// //Create Mesh Asset
	// FString PackagePath = FPaths::Combine(TEXT("/Game/Speckle/Geometry"), StreamID, SMesh->Id);
	// UPackage* Package = CreatePackage(*PackagePath);
	//
	// //TODO check if mesh already exists;
	//
	// UStaticMesh* UMesh = NewObject<UStaticMesh>(Package, FName(SMesh->Id), RF_Public);
	//
	// UMesh->InitResources();
	// UMesh->SetLightingGuid();
	//
	// UStaticMeshDescription* StaticMeshDescription = UStaticMesh::CreateStaticMeshDescription(Package);
	// FMeshDescription& BaseMeshDescription = StaticMeshDescription->GetMeshDescription();
	//
	// //Build Settings
	// FStaticMeshSourceModel& SrcModel = UMesh->AddSourceModel();
	// SrcModel.BuildSettings.bRecomputeNormals = false;
	// SrcModel.BuildSettings.bRecomputeTangents = false;
	// SrcModel.BuildSettings.bRemoveDegenerates = false;
	// SrcModel.BuildSettings.bUseHighPrecisionTangentBasis = false;
	// SrcModel.BuildSettings.bUseFullPrecisionUVs = false;
	// SrcModel.BuildSettings.bGenerateLightmapUVs = true;
	// SrcModel.BuildSettings.SrcLightmapIndex = 0;
	// SrcModel.BuildSettings.DstLightmapIndex = 1;
	//
	// UStaticMesh::FBuildMeshDescriptionsParams MeshParams;
	// MeshParams.bBuildSimpleCollision = BuildSimpleCollision;
	// MeshParams.bCommitMeshDescription = true;
	// MeshParams.bMarkPackageDirty = true;
	// MeshParams.bUseHashAsGuid = false;
	//
	// //Set Mesh Data
	//
	// const FName MaterialSlotName = UMesh->AddMaterial(Material);;
	// BaseMeshDescription.PolygonGroupAttributes().RegisterAttribute<FName>(MeshAttribute::PolygonGroup::ImportedMaterialSlotName, 1, MaterialSlotName,  EMeshAttributeFlags::Transient);
	// {
	// 	const size_t NumberOfVertices = Vertices.Num();
	// 	StaticMeshDescription->ReserveNewVertices(NumberOfVertices);
	// 	
	// 	TMap<int32, FVertexID> VertexMap;
	// 	VertexMap.Reserve(NumberOfVertices);
	// 	
	// 	{
	// 		int32 i = 0;
	// 		for(const FVector VertexPosition : Vertices)
	// 		{
	// 			const FVertexID VertID = StaticMeshDescription->CreateVertex();
	// 			StaticMeshDescription->SetVertexPosition(VertID, VertexPosition);
	// 			VertexMap.Add(i, VertID);
	// 			i++;
	// 		}
	// 	}
	//
	//
	// 	//Convert Faces
	// 	const FPolygonGroupID PolygonGroupID = StaticMeshDescription->CreatePolygonGroup();
	//
	// 	//StaticMeshDescription->SetPolygonGroupMaterialSlotName(PolygonGroupID, MaterialSlotName);
	// 	
	// 	StaticMeshDescription->VertexInstanceAttributes().RegisterAttribute<FVector2D>(MeshAttribute::VertexInstance::TextureCoordinate, 2, FVector2D::ZeroVector, EMeshAttributeFlags::None);
	//
	//
	// 	StaticMeshDescription->ReserveNewTriangles(SMesh->Faces.Num() * 3); //Reserve space assuming faces will all be triangles
	// 	StaticMeshDescription->ReserveNewPolygons(SMesh->Faces.Num() * 3);
	// 	StaticMeshDescription->ReserveNewVertexInstances(SMesh->Vertices.Num()); //Reserve space assuming faces will all be triangles
	//
	// 	//TODO this whole section needs re-jigging.
	// 	for(int32 i = 0; i < SMesh->Faces.Num(); i ++)
	// 	{
	// 		TArray<TTuple<int32,int32>> ParsedPolygon = SMesh->Faces[i];
	// 		TArray<FVertexInstanceID> VertexInstances;
	// 		VertexInstances.Reserve(ParsedPolygon.Num());
	//
	// 		
	// 		TSet<FVertexID> Verts;
	// 		Verts.Reserve(ParsedPolygon.Num());
	// 		
	// 		for(const auto v : ParsedPolygon)
	// 		{
	// 			FVertexID Vert = VertexMap[v.Key];
	// 			bool AlreadyInSet;
	// 			Verts.Add(Vert, &AlreadyInSet);
	//
	// 			if(AlreadyInSet)
	// 			{
	// 				UE_LOG(LogTemp, Warning, TEXT("Invalid Polygon while creating mesh - vertex appears more than once in a face, duplicates will be ignored"));
	// 				continue;
	// 			}
	//
	// 			FVertexInstanceID VertexInstance = StaticMeshDescription->CreateVertexInstance(Vert);
	// 			
	// 			VertexInstances.Add(VertexInstance);
	// 			
	// 			StaticMeshDescription->SetVertexInstanceUV(VertexInstance, TextureCoordinates[v.Value]);
	// 		};
	//
	//
	// 		if(VertexInstances.Num() < 3)
	// 		{
	// 			UE_LOG(LogTemp, Warning, TEXT("Invalid Polygon while creating mesh - face has fewer than 3 verts, this face will be ignored"));
	// 			continue;
	// 		}
	// 		
	// 		TArray<FEdgeID> Edges;
	// 		Edges.Reserve(ParsedPolygon.Num());
	// 		
	// 		const FPolygonID PolygonID = StaticMeshDescription->CreatePolygon(PolygonGroupID, VertexInstances, Edges);
	//
	// 		for (const FEdgeID EdgeID : Edges)
	// 		{
	// 			StaticMeshDescription->GetEdgeHardnesses()[EdgeID] = true;
	// 		}
	// 		
	// 		StaticMeshDescription->ComputePolygonTriangulation(PolygonID);
	// 	}
	// 	
	// 	
	// 	BaseMeshDescription.PolygonAttributes().RegisterAttribute<FVector>(MeshAttribute::Polygon::Normal, 1, FVector::ZeroVector, EMeshAttributeFlags::Transient);
	// 	BaseMeshDescription.PolygonAttributes().RegisterAttribute<FVector>(MeshAttribute::Polygon::Tangent, 1, FVector::ZeroVector, EMeshAttributeFlags::Transient);
	// 	BaseMeshDescription.PolygonAttributes().RegisterAttribute<FVector>(MeshAttribute::Polygon::Binormal, 1, FVector::ZeroVector, EMeshAttributeFlags::Transient);
	// 	BaseMeshDescription.PolygonAttributes().RegisterAttribute<FVector>(MeshAttribute::Polygon::Center, 1, FVector::ZeroVector, EMeshAttributeFlags::Transient);
	// 	
	// 	FStaticMeshOperations::ComputePolygonTangentsAndNormals(BaseMeshDescription);
	// 	FStaticMeshOperations::ComputeTangentsAndNormals(BaseMeshDescription, EComputeNTBsFlags::Normals | EComputeNTBsFlags::Tangents);
	// }
	//
	//
	//
	// //Mesh->PreEditChange(nullptr);
	// 		
	// UMesh->LightMapCoordinateIndex = SrcModel.BuildSettings.DstLightmapIndex;
	// UMesh->BuildFromMeshDescriptions(TArray<const FMeshDescription*>{&BaseMeshDescription}, MeshParams);
	// 	
	// if(UseFullBuild) UMesh->Build(true); //This makes conversion time much slower, but is needed for generating lightmap UVs
	// //Mesh->PostEditChange(); //This doesn't seem to be required
	//
	// FAssetRegistryModule::AssetCreated(UMesh);
}
