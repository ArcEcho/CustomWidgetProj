// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "SlateCore.h"

DECLARE_DELEGATE(FAnimationEndDelegate)

/**
 *
 */
class CUSTOMWIDGETPROJ_API SCustomImage : public SImage
{
public:
    SLATE_BEGIN_ARGS(SCustomImage)
        :_Image(FCoreStyle::Get().GetDefaultBrush())
        , _ColorAndOpacity(FLinearColor(0, 0, 1, 0.02333))
        , _Duration(0.05)
        , _ColumnCount(1)
        , _RowCount(1)
        , _CutFrameCount(0)
        , _OnMouseButtonDown()
    {}
    SLATE_ATTRIBUTE(const FSlateBrush*, Image)
        SLATE_ATTRIBUTE(FSlateColor, ColorAndOpacity)
        SLATE_ATTRIBUTE(float, Duration)
        SLATE_ATTRIBUTE(int32, ColumnCount)
        SLATE_ATTRIBUTE(int32, RowCount)
        SLATE_ATTRIBUTE(int32, CutFrameCount)
        SLATE_EVENT(FAnimationEndDelegate, OnAnimationEnd)
        SLATE_EVENT(FPointerEventHandler, OnMouseButtonDown)
        SLATE_END_ARGS()

        /** Constructs this widget with InArgs */
        void Construct(const FArguments& InArgs);

public:
    virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect&  MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

    virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

    void SetDuration(float InDuration);

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
    void SetTex(UTexture2D* InTex);

protected:

    void NotifyEnd();

    float AccumulatedFrameTime;
    int32 CachedFrameIndex;

    FSlateBrush MyBrush;

    float Duration;
    int32 ColumnCount;
    int32 RowCount;
    int32 CutFrameCount;

    bool  bShouldLoop;

    bool bIsFirstLoop;
    bool bShouldNotifyAtEnd;

    bool bIsPaused;

    FAnimationEndDelegate OnAnimationEnd;
};
