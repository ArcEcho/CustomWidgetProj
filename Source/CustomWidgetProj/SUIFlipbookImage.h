// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Attribute.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Styling/SlateColor.h"
#include "Styling/CoreStyle.h"
#include "Widgets/SLeafWidget.h"
#include "SlateCore.h" 

class FPaintArgs;
class FSlateWindowElementList;
class UTexture2D;

DECLARE_DELEGATE(FAnimationEndDelegate)

/**
 *  Implements a widget that displays an image with a desired width and height and can draw flipbook from texture.
 */
class CUSTOMWIDGETPROJ_API SUIFlipbookImage : public SLeafWidget
{
public:
    SLATE_BEGIN_ARGS(SUIFlipbookImage)
        : _ColorAndOpacity(FLinearColor::White)
        , _Duration(0.05f)
        , _LoopInterval(0.0f)
        , _ColumnCount(1)
        , _RowCount(1)
        , _CutFrameCount(0)
    {}

    /** Color and opacity */
    SLATE_ATTRIBUTE(FSlateColor, ColorAndOpacity)

        /** Time between each frame */
        SLATE_ATTRIBUTE(float, Duration)

        /** Time between each loop */
        SLATE_ATTRIBUTE(float, LoopInterval)
        SLATE_ATTRIBUTE(int32, ColumnCount)
        SLATE_ATTRIBUTE(int32, RowCount)

        /** Number of the blocking frames */
        SLATE_ATTRIBUTE(int32, CutFrameCount)
        SLATE_EVENT(FAnimationEndDelegate, OnAnimationEnd)

        SLATE_END_ARGS()

        /** Constructor */
        SUIFlipbookImage()
    {
        bCanTick = false;
        bCanSupportFocus = false;
    }


    /** Constructs this widget with InArgs */
    void Construct(const FArguments& InArgs);

public:
    virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect&  MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

    virtual FVector2D ComputeDesiredSize(float) const override;
    
    virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

    void SetDuration(float InDuration);

    void SetLoopInterval(float InLoopInterval);

    void SetColumnCount(int32 InColumnCount);

    void SetRowCount(int32 InRowCount);

    void SetCutFrameCount(int32 InCutFrameCount);

    void PlayAnimation(bool InShouldLoop = false, bool InShouldNotifyAtEnd = false);

    bool IsAnimationPlaying();

    void PauseAnimation();

    bool IsAnimationPaused();

    void ResumeAnimation();

    void ResetAnimation();

    void StopAnimation(bool InShouldNotifyAtEnd = false);

    void SetOnAnimationEnd(FAnimationEndDelegate EventHandler);

    void SetImageRes(UTexture2D* InImageRes);

    /** See the ColorAndOpacity attribute */
    void SetColorAndOpacity(const TAttribute<FSlateColor>& InColorAndOpacity);

    /** See the ColorAndOpacity attribute */
    void SetColorAndOpacity(FLinearColor InColorAndOpacity);

protected:

    void NotifyAnimationEnd();

    void UpdateImageBrushUVRegion();

    float AccumulatedFrameTime;
    float AccumulatedLoopIntervalTime;
    int32 CachedFrameIndex;

    bool bIsInLoopInterval;
    bool  bShouldLoop;

    bool bIsFirstLoop;
    bool bShouldNotifyAtEnd;

    bool bIsPaused;

    FSlateBrush ImageBrush;

    /** Color and opacity scale for this image */
    TAttribute<FSlateColor> ColorAndOpacity;

    float Duration;
    float LoopInterval;
    int32 ColumnCount;
    int32 RowCount;
    int32 CutFrameCount;

    FAnimationEndDelegate OnAnimationEndHandler;

};
