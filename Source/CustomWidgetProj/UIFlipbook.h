// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Widget.h"
#include "UIFlipbook.generated.h"

class SUIFlipbookImage;
class UTexture2D;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnimationEndEvent);

/**
 *
 */
UCLASS()
class CUSTOMWIDGETPROJ_API UUIFlipbook : public UWidget
{
    GENERATED_BODY()

public:
    UUIFlipbook(const FObjectInitializer& ObjectInitializer);

    //Custom section

    /** 0.05s(20FPS) is enough for UI Animation, so clamp min duration at 0.05. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIFlipbook", meta = (ClampMin = "0.05", ClampMax = "100"))
        float Duration;

    /** Time between each animation loop */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIFlipbook", meta = (ClampMin = "0"))
        float LoopInterval;

    /** Number of frames in the column*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIFlipbook", meta = (ClampMin = "1", ClampMax = "100"))
        int32 ColumnCount;

    /** Number of frames in the row*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIFlipbook", meta = (ClampMin = "1", ClampMax = "100"))
        int32 RowCount;

    /** The flipbook may contain some empty frames, here is the opportunity to block them.  */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIFlipbook", meta = (ClampMin = "0"))
        int32 CutFrameCount;

    /** True if you want the flipbook auto play loop,but this will not nofity animation end event at each loop end. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UIFlipbook", meta = (sRGB = "true"))
        bool bAutoPlayLoop;

    /**A texture is just enough, so here just surpport texture instead of slate brush */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UIFlipbook", meta = (sRGB = "true"))
        UTexture2D* FlipbookImage;

    /** When an animation loop is end, this event will notify depends on your parameter in PlayAnimation(). */
    UPROPERTY(BlueprintAssignable, Category = "UIFlipbook|Events")
        FOnAnimationEndEvent OnAnimationEnd;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance", meta = (sRGB = "true"))
        FLinearColor ColorAndOpacity;

    /** A bindable delegate for the ColorAndOpacity. */
    UPROPERTY()
        FGetLinearColor ColorAndOpacityDelegate;          //This should be exist if you use a binding of the  ColorAndOpacity in SynchronizeProperies().

    UFUNCTION(BlueprintCallable, Category = "UIFlipbook")
        void PlayAnimation(bool ShouldLoop, bool InShouldNotifyAtEnd = false);

    UFUNCTION(BlueprintCallable, Category = "UIFlipbook")
        void PauseAnimation();

    UFUNCTION(BlueprintCallable, Category = "UIFlipbook")
        void ResumeAnimation();

    UFUNCTION(BlueprintCallable, Category = "UIFlipbook")
        void StopAnimation(bool ShouldnotifyAnimationEndEvent);

    UFUNCTION(BlueprintCallable, Category = "UIFlipbook")
        bool IsAnimationPlaying();

    UFUNCTION(BlueprintCallable, Category = "UIFlipbook")
        bool IsAnimationPaused();

    virtual void SynchronizeProperties() override;

    virtual void ReleaseSlateResources(bool bReleaseChildren) override;

#if WITH_EDITOR
    virtual const FText GetPaletteCategory() override;
#endif

protected:
    TSharedPtr<SUIFlipbookImage> MyImage;

    virtual TSharedRef<SWidget> RebuildWidget() override;

    void HandleAnimationEnd();
};
