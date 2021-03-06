// Copyright 2018 wevet works All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "WevetExtension.h"
#include "SaveGame/WevetGameInstance.h"
#include "SingletonFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class WEVET_API USingletonFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	template<class ActorType>
	static FORCEINLINE ActorType* GetSingletonInstance(ActorType* Instance)
	{
		if (Instance == nullptr) 
		{
			// Instance pointer not initialized

			// HACK: Using GameInstance as world context object...
			//       since it should always exist
			UWorld* const World = UWevetGameInstance::GetInstance()->GetWorld();

			if (World)
			{
				// Look for already existing instances
				TArray<AActor*> FoundActors;
				UGameplayStatics::GetAllActorsOfClass(World, ActorType::StaticClass(), FoundActors);

				if (FoundActors.Num() > 0)
				{
					Instance = Cast<ActorType>(FoundActors[0]);
				}
				else
				{
					FName BPClassName = FName(*(FString::Printf(TEXT("BP_%s"), *(ActorType::StaticClass()->GetName()))));
					FString BlueprintPath(FString::Printf(TEXT("%s.%s_C"), *BPClassName.ToString(), *BPClassName.ToString()));
					FString Path("/Game/Game/Blueprints/Singletons/");
					Path.Append(BlueprintPath);
					TSubclassOf <ActorType> Template = TSoftClassPtr<ActorType>(FSoftObjectPath(*Path)).LoadSynchronous();

					if (Template != nullptr)
					{
						Instance = World->SpawnActor<ActorType>(Template);
					}
					else
					{
						Instance = World->SpawnActor<ActorType>();
					}
#if WITH_EDITOR
					if (AActor * InstanceActor = Cast<AActor>(Instance))
					{
						InstanceActor->SetFolderPath("Singleton");
					}
#endif
				}
			}
		}
		return Instance;
	}

	template <typename BPClass>
	static FORCEINLINE BPClass* LoadBlueprintAsset(const FName& Directory, const FName& Name)
	{
		if (Directory == NAME_None) 
		{
			return nullptr;
		}

		FString ClassName = Name.ToString().Append(TEXT("_C"));
		TArray<UObject*> Assets;
		if (EngineUtils::FindOrLoadAssetsByPath(*(Directory.ToString()), Assets, EngineUtils::EAssetToLoad::ATL_Class)) 
		{
			for (int i = 0, n = Assets.Num(); i < n; i++) 
			{
				UObject* Asset = Assets[i];

				if (Asset == nullptr) 
				{
					continue;
				}
				BPClass* Class = Cast<BPClass>(Asset);
				if (Class == nullptr || !Asset->GetName().Equals(ClassName)) 
				{
					continue;
				}
				return Class;
			}
		}
		return nullptr;
	}

	template <typename BPClass>
	static FORCEINLINE BPClass* LoadGameBlueprintAsset(const FName& Directory, const FName& Name)
	{
		FString FixedGameDirectory = FString::Printf(TEXT("/Game/Blueprints/%s"), *(Directory.ToString()));
		return LoadBlueprintAsset<BPClass>(
			FName(*(FixedGameDirectory)),
			Name
			);
	}

};
