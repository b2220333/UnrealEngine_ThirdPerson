// Copyright © 2018 wevet works All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemControllerExecuter.generated.h"

class ACharacterBase;
/**
*
*/
UINTERFACE(BlueprintType)
class WEVET_API UItemControllerExecuter : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class WEVET_API IItemControllerExecuter
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IItemControllerExecuter")
	void PrepareDestory();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IItemControllerExecuter")
	bool CanTaken();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IItemControllerExecuter")
	void Take(ACharacterBase* NewCharacter);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IItemControllerExecuter")
	void Release(ACharacterBase* NewCharacter);
};
