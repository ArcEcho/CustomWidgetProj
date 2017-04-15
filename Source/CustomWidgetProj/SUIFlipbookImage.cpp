#include "CustomWidgetProj.h"

// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomWidgetProj.h"
#include "SUIFlipbookImage.h"
#include "SlateOptMacros.h"
#include "DrawElements.h"  
#include "Engine/Texture2D.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SUIFlipbookImage::Construct(const FArguments& InArgs)
{
    ColorAndOpacity = InArgs._ColorAndOpacity;
    Duration = InArgs._Duration.Get();
    LoopInterval = InArgs._LoopInterval.Get();
    ColumnCount = InArgs._ColumnCount.Get();
    RowCount = InArgs._RowCount.Get();
    CutFrameCount = InArgs._CutFrameCount.Get();
    OnAnimationEndHandler = InArgs._OnAnimationEnd;

    bCanTick = false;
    bShouldLoop = false;
    bIsPaused = false;
    bIsFirstLoop = true;
    bShouldNotifyAtEnd = false;

    AccumulatedFrameTime = 0.0f;
    AccumulatedLoopIntervalTime = 0.0f;
    CachedFrameIndex = 0;

    ImageBrush = FSlateBrush(*FCoreStyle::Get().GetDefaultBrush());
    ImageBrush.Tiling = ESlateBrushTileType::NoTile;  //This is very important
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION


int32 SUIFlipbookImage::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{

    const bool bIsEnabled = ShouldBeEnabled(bParentEnabled);
    const uint32 DrawEffects = bIsEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;

    const FLinearColor FinalColorAndOpacity(InWidgetStyle.GetColorAndOpacityTint() * ColorAndOpacity.Get().GetColor(InWidgetStyle) * ImageBrush.GetTint(InWidgetStyle));

    //Calculate offset and desired size
    int32 CurrentRowIndex = CachedFrameIndex / ColumnCount;
    int32 CurrentColounIndex = CachedFrameIndex % ColumnCount;

    auto AdjustedSize = FVector2D(
        AllottedGeometry.GetLocalSize().X * ColumnCount,
        AllottedGeometry.GetLocalSize().Y * RowCount
    );

    auto AdjustedOffset = FVector2D(
        -CurrentColounIndex * AllottedGeometry.GetLocalSize().X,
        -CurrentRowIndex *AllottedGeometry.GetLocalSize().Y
    );

    auto PaintGeometry = AllottedGeometry.ToPaintGeometry(
        AdjustedOffset,
        AdjustedSize,
        1
    );

    FSlateDrawElement::MakeBox(
        OutDrawElements,
        LayerId,
        //AllottedGeometry.ToPaintGeometry(),
        PaintGeometry,
        &ImageBrush,
        MyClippingRect,
        ESlateDrawEffect::None,
        FinalColorAndOpacity
    );


    return LayerId;
}

FVector2D SUIFlipbookImage::ComputeDesiredSize(float) const
{
    return ImageBrush.ImageSize;
}

void SUIFlipbookImage::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
    if (bIsInLoopInterval)
    {
        AccumulatedLoopIntervalTime += InDeltaTime;

        if (AccumulatedLoopIntervalTime < LoopInterval)
        {
            return;
        }

        bIsInLoopInterval = false;
        AccumulatedFrameTime += AccumulatedLoopIntervalTime - LoopInterval;
        AccumulatedLoopIntervalTime = 0.0f;
    }
    else
    {
        AccumulatedFrameTime += InDeltaTime;
    }

    if (AccumulatedFrameTime < Duration)
    {
        return;
    }

    //This will cause a faster play effect after the game window is suspended. 
    AccumulatedFrameTime -= Duration;

    CachedFrameIndex++;

    //When CachedFrameIndex equals to  (RowCount * ColumnCount - CutFrameCount), this means a loop was completed.
    if (CachedFrameIndex == RowCount * ColumnCount - CutFrameCount)
    {
        bIsInLoopInterval = true;

        //Determine whether to stop playing animation by bShouldLoop
        if (bIsFirstLoop && !bShouldLoop)
        {
            StopAnimation(bShouldNotifyAtEnd);
            return;
        }

        if (bIsFirstLoop)
        {
            bIsFirstLoop = false;
        }
    }

    CachedFrameIndex %= (RowCount * ColumnCount - CutFrameCount);
}


void SUIFlipbookImage::SetDuration(float InDuration)
{
    Duration = InDuration;
}

void SUIFlipbookImage::SetLoopInterval(float InLoopInterval)
{
    LoopInterval = InLoopInterval;
}

void SUIFlipbookImage::SetColumnCount(int32 InColumnCount)
{
    ColumnCount = InColumnCount;
}

void SUIFlipbookImage::SetRowCount(int32 InRowCount)
{
    RowCount = InRowCount;
}

void SUIFlipbookImage::SetCutFrameCount(int32 InCutFrameCount)
{
    if (InCutFrameCount >= ColumnCount * RowCount)
    {
        CutFrameCount = 0;
    }
    else
    {
        CutFrameCount = InCutFrameCount;
    }
}

void SUIFlipbookImage::ResetAnimation()
{
    AccumulatedFrameTime = 0.0f;
    CachedFrameIndex = 0;
    bIsFirstLoop = true;

    AccumulatedLoopIntervalTime = 0.0f;
    bIsInLoopInterval = false;
}


void SUIFlipbookImage::PlayAnimation(bool InShouldLoop /*= false*/, bool InShouldNotifyAtEnd /* = false */)
{
    if (IsAnimationPlaying())
    {
        return;
    }
    else
    {
        ResetAnimation();
        bCanTick = true;
        bShouldLoop = InShouldLoop;
        bShouldNotifyAtEnd = InShouldNotifyAtEnd;
    }
}

bool SUIFlipbookImage::IsAnimationPlaying()
{
    return bCanTick;
}

void SUIFlipbookImage::PauseAnimation()
{
    if (!IsAnimationPlaying() || bIsPaused)
    {
        return;
    }

    bIsPaused = true;
    bCanTick = false;
}

bool SUIFlipbookImage::IsAnimationPaused()
{
    return bIsPaused;
}

void SUIFlipbookImage::ResumeAnimation()
{
    if (!bIsPaused)
    {
        return;
    }

    bCanTick = true;
    bIsPaused = false;
}

void SUIFlipbookImage::StopAnimation(bool ShouldNotifyEndEvent /* = false */)
{
    if (!IsAnimationPlaying())
    {
        return;
    }

    ResetAnimation();
    bCanTick = false;

    if (ShouldNotifyEndEvent)
    {
        NotifyAnimationEnd();
    }
}

void SUIFlipbookImage::SetOnAnimationEnd(FAnimationEndDelegate EventHandler)
{
    OnAnimationEndHandler = EventHandler;
}

void SUIFlipbookImage::SetImageRes(UTexture2D* InImageRes)
{
    ImageBrush.SetResourceObject(InImageRes);

    if (InImageRes) // Since this texture is used as UI, don't allow it affected by budget.
    {
        InImageRes->bIgnoreStreamingMipBias = true;
    }

    Invalidate(EInvalidateWidget::Layout);
}

void SUIFlipbookImage::SetColorAndOpacity(const TAttribute<FSlateColor>& InColorAndOpacity)
{
    if (!ColorAndOpacity.IdenticalTo(InColorAndOpacity))
    {
        ColorAndOpacity = InColorAndOpacity;
        Invalidate(EInvalidateWidget::Layout);
    }
}

void SUIFlipbookImage::SetColorAndOpacity(FLinearColor InColorAndOpacity)
{
    if (ColorAndOpacity.IsBound() || ColorAndOpacity.Get() != InColorAndOpacity)
    {
        ColorAndOpacity = InColorAndOpacity;
        Invalidate(EInvalidateWidget::Layout);
    }
}

void SUIFlipbookImage::NotifyAnimationEnd()
{
    if (OnAnimationEndHandler.IsBound())
    {
        OnAnimationEndHandler.Execute();
    }
}


