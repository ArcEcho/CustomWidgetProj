// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomWidgetProj.h"
#include "SCustomImage.h"
#include "SlateOptMacros.h"
#include "DrawElements.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SCustomImage::Construct(const FArguments& InArgs)
{

    Image = InArgs._Image;
    ColorAndOpacity = InArgs._ColorAndOpacity;
    Duration = InArgs._Duration.Get();
    ColumnCount = InArgs._ColumnCount.Get();
    RowCount = InArgs._RowCount.Get();
    CutFrameCount = InArgs._CutFrameCount.Get();
    OnAnimationEnd = InArgs._OnAnimationEnd;
    OnMouseButtonDownHandler = InArgs._OnMouseButtonDown;

    bCanTick = false;
    bShouldLoop = false;
    bIsPaused = false;
    bIsFirstLoop = true;
    bShouldNotifyAtEnd = false;

    AccumulatedFrameTime = 0.0f;
    CachedFrameIndex = 0;

    MyBrush = FSlateBrush();

    /*
    ChildSlot
    [
        // Populate the widget
    ];
    */
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

int32 SCustomImage::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    //const FSlateBrush* ImageBrush = Image.Get();
    const FSlateBrush* ImageBrush = &MyBrush;

    if ((ImageBrush != nullptr) && (ImageBrush->DrawAs != ESlateBrushDrawType::NoDrawType))
    {
        const bool bIsEnabled = ShouldBeEnabled(bParentEnabled);
        const uint32 DrawEffects = bIsEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;

        const FLinearColor FinalColorAndOpacity(InWidgetStyle.GetColorAndOpacityTint() * ColorAndOpacity.Get().GetColor(InWidgetStyle) * ImageBrush->GetTint(InWidgetStyle));

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
            /*AllottedGeometry.ToPaintGeometry()*/ PaintGeometry,
            ImageBrush,
            MyClippingRect,
            DrawEffects,
            FinalColorAndOpacity
        );
    }

    return LayerId;
}

void SCustomImage::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
    AccumulatedFrameTime += InDeltaTime;

    if (AccumulatedFrameTime < Duration)
    {
        return;
    }

    AccumulatedFrameTime -= Duration;

    CachedFrameIndex++;

    if (bIsFirstLoop && CachedFrameIndex >= RowCount * ColumnCount - CutFrameCount)
    {
        bIsFirstLoop = false;
    }

    CachedFrameIndex %= (RowCount * ColumnCount - CutFrameCount);

    //Determine whether to stop playing animation by bShouldLoop
    if (CachedFrameIndex == 1 && !bIsFirstLoop  && !bShouldLoop)
    {
        StopAnimation(true);
    }
}


void SCustomImage::SetDuration(float InDuration)
{
    Duration = InDuration;
}

void SCustomImage::SetColumnCount(int32 InColumnCount)
{
    ColumnCount = InColumnCount;
}

void SCustomImage::SetRowCount(int32 InRowCount)
{
    RowCount = InRowCount;
}

void SCustomImage::SetCutFrameCount(int32 InCutFrameCount)
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

void SCustomImage::ResetAnimation()
{
    AccumulatedFrameTime = 0.0f;
    CachedFrameIndex = 0;
    bIsFirstLoop = true;
}


void SCustomImage::PlayAnimation(bool InShouldLoop /*= false*/, bool InShouldNotifyAtEnd /* = false */)
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

bool SCustomImage::IsAnimationPlaying()
{
    return bCanTick;
}

void SCustomImage::PauseAnimation()
{
    if (!IsAnimationPlaying() || bIsPaused)
    {
        return;
    }

    bIsPaused = true;
    bCanTick = false;
}

bool SCustomImage::IsAnimationPaused()
{
    return bIsPaused;
}

void SCustomImage::ResumeAnimation()
{
    if (!bIsPaused)
    {
        return;
    }

    bCanTick = true;
    bIsPaused = false;
}

void SCustomImage::StopAnimation(bool ShouldNotifyEndEvent /* = false */)
{
     if (!IsAnimationPlaying())
     {
         return;
     }

     ResetAnimation();
     bCanTick = false;

     if (ShouldNotifyEndEvent)
     {
         NotifyEnd();
     }
}

void SCustomImage::SetOnAnimationEnd(FAnimationEndDelegate EventHandler)
{
    OnAnimationEnd = EventHandler;
}

void SCustomImage::SetTex(UTexture2D* InTex)
{
    if (InTex != nullptr)
    {
        InTex->bIgnoreStreamingMipBias = true;
        MyBrush.SetResourceObject(InTex);
    }
}

void SCustomImage::NotifyEnd()
{

    if (OnAnimationEnd.IsBound())
    {
        OnAnimationEnd.Execute();
    }

}


