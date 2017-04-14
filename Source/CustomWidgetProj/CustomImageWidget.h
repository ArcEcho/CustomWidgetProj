// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Widget.h"
#include "CustomImageWidget.generated.h"

class SCustomImage;
class UMaterialInstanceDynamic;
class UMaterialInterface;
class USlateBrushAsset;
class UTexture2D;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnimationEndEvent);

/**
*
*/
UCLASS()
class CUSTOMWIDGETPROJ_API UCustomImageWidget : public UWidget
{
    GENERATED_BODY()

public:
    UCustomImageWidget(const FObjectInitializer& ObjectInitializer);

    //Custom section

    /** 0.05s(20FPS) is enough for UI Animation, so clamp min duration at 0.05. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Image", meta = (ClampMin = "0.05", ClampMax = "100"))
        float Duration;

    UFUNCTION(BlueprintCallable, Category = "Custom Image")
        void SetDuration(float InDuration);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Image", meta = (ClampMin = "1", ClampMax = "100"))
        int32 ColumnCount;
   
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Image", meta = (ClampMin = "1", ClampMax = "100"))
        int32 RowCount;

    /** The flipbook may contain some empty frames, here is the opportunity to block them.  */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Image", meta = (ClampMin = "0"))
        int32 CutFrameCount;

    UFUNCTION(BlueprintCallable, Category = "Custom Image")
        void PlayAnimation(bool ShouldLoop);

    UFUNCTION(BlueprintCallable, Category = "Custom Image")
        void PauseAnimation();

    UFUNCTION(BlueprintCallable, Category = "Custom Image")
        void ResumeAnimation();


    UFUNCTION(BlueprintCallable, Category = "Custom Image")
        void StopAnimation(bool ShouldnotifyAnimationEndEvent);

    UFUNCTION(BlueprintCallable, Category = "Custom Image")
        bool IsAnimationPlaying();

    UFUNCTION(BlueprintCallable, Category = "Custom Image")
        bool IsAnimationPaused();

    UPROPERTY(BlueprintAssignable, Category = "Custom Image|Events")
        FOnAnimationEndEvent OnAnimationEnd;

    //The following section is the as UImage,and it is unnecessary if you make this widget derived from UImage.

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
        FSlateBrush Brush;

    UPROPERTY()
        FGetSlateBrush BrushDelegate;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance", meta = (sRGB = "true"))
        FLinearColor ColorAndOpacity;

    UPROPERTY()
        FGetLinearColor ColorAndOpacityDelegate;

    UPROPERTY(EditAnywhere, Category = "Events", meta = (IsBindableEvent = "True"))
        FOnPointerEvent OnMouseButtonDownEvent;

    UFUNCTION(BlueprintCallable, Category = "Appearance")
        void SetColorAndOpacity(FLinearColor InColorAndOpacity);

    UFUNCTION(BlueprintCallable, Category = "Appearance")
        void SetOpacity(float InOpactiy);

    UFUNCTION(BlueprintCallable, Category = "Appearance")
        void SetBrush(const FSlateBrush& InBrush);

    UFUNCTION(BlueprintCallable, Category = "Appearance")
        void SetBrushFromAsset(USlateBrushAsset* Asset);

    UFUNCTION(BlueprintCallable, Category = "Appearance")
        void SetBrushFromTexture(UTexture2D* Texture, bool bMatchSize = false);

    UFUNCTION(BlueprintCallable, Category = "Appearance")
        void SetBrushFromTextureDynamic(UTexture2DDynamic* Texture, bool bMatchSize = false);

    UFUNCTION(BlueprintCallable, Category = "Appearance")
        void SetBrushFromMaterial(UMaterialInterface* Material);

    UFUNCTION(BlueprintCallable, Category = "Appearance")
        UMaterialInstanceDynamic* GetDynamicMaterial();

    virtual void SynchronizeProperties() override;

    virtual void ReleaseSlateResources(bool bReleaseChildren) override;


#if WITH_EDITOR
    virtual const FText GetPaletteCategory() override;
#endif

protected:
    TSharedPtr<SCustomImage> MyImage;

    virtual TSharedRef<SWidget> RebuildWidget() override;

    const FSlateBrush* ConvertImage(TAttribute<FSlateBrush> InImageAsset) const;

    FReply HandleMouseButtonDown(const FGeometry& Geometey, const FPointerEvent& MouseEvent);

    void HandleAnimationEnd();
};
