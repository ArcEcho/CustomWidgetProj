// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomWidgetProj.h"
#include "CustomImageWidget.h"  
#include "Slate/SlateBrushAsset.h" 
#include "Materials/MaterialInterface.h"
#include "Engine/Texture2D.h"
#include "Engine/Texture2DDynamic.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "SCustomImage.h"

//LOCTEXT
#define LOCTEXT_NAMESPACE "UMG"

UCustomImageWidget::UCustomImageWidget(const FObjectInitializer& ObjectInitializer)
    :Super(ObjectInitializer)
    , ColorAndOpacity(FLinearColor::White)
    , Duration(0.05)
    , ColumnCount(1)
    , RowCount(1)
    , CutFrameCount(0)
{

}

void UCustomImageWidget::SetDuration(float InDuration)
{
    Duration = InDuration;
    if (MyImage.IsValid())
    {
        MyImage->SetDuration(InDuration);
    }
}

void UCustomImageWidget::PlayAnimation(bool ShouldLoop)
{
    if (!MyImage.IsValid())
    {
        return;
    }

    MyImage->PlayAnimation(ShouldLoop);
}

void UCustomImageWidget::PauseAnimation()
{
    if (!MyImage.IsValid())
    {
        return;
    }

    MyImage->PauseAnimation();
}

void UCustomImageWidget::ResumeAnimation()
{
    if (!MyImage.IsValid())
    {
        return;
    }

    MyImage->ResumeAnimation();
}

void UCustomImageWidget::StopAnimation(bool ShouldnotifyAnimationEndEvent)
{
    if (!MyImage.IsValid())
    {
        return;
    }

    MyImage->StopAnimation(ShouldnotifyAnimationEndEvent);
}

bool UCustomImageWidget::IsAnimationPlaying()
{
    if (!MyImage.IsValid())
    {
        return false;
    }

    return  MyImage->IsAnimationPlaying();
}

bool UCustomImageWidget::IsAnimationPaused()
{
    if (!MyImage.IsValid())
    {
        return false;
    }

    return  MyImage->IsAnimationPaused();
}

void UCustomImageWidget::SetColorAndOpacity(FLinearColor InColorAndOpacity)
{
    ColorAndOpacity = InColorAndOpacity;

    if (MyImage.IsValid())
    {
        MyImage->SetColorAndOpacity(ColorAndOpacity);
    }
}

void UCustomImageWidget::SetOpacity(float InOpactiy)
{
    ColorAndOpacity.A = InOpactiy;

    if (MyImage.IsValid())
    {
        MyImage->SetColorAndOpacity(ColorAndOpacity);
    }
}

const FSlateBrush* UCustomImageWidget::ConvertImage(TAttribute<FSlateBrush> InImageAsset)  const
{
    UCustomImageWidget* Mutablethis = const_cast<UCustomImageWidget*>(this);
    Mutablethis->Brush = InImageAsset.Get();

    return &Brush;
}

FReply UCustomImageWidget::HandleMouseButtonDown(const FGeometry& Geometery, const FPointerEvent& MouseEvent)
{
    if (OnMouseButtonDownEvent.IsBound())
    {
        return OnMouseButtonDownEvent.Execute(Geometery, MouseEvent).NativeReply;
    }

    return FReply::Unhandled();
}

void UCustomImageWidget::HandleAnimationEnd()
{
    OnAnimationEnd.Broadcast();
}

void UCustomImageWidget::SetBrush(const FSlateBrush& InBrush)
{
    Brush = InBrush;

    if (MyImage.IsValid())
    {
        MyImage->SetImage(&Brush);
    }
}

void UCustomImageWidget::SetBrushFromAsset(USlateBrushAsset* Asset)
{
    Brush = Asset ? Asset->Brush : FSlateBrush();

    if (MyImage.IsValid())
    {
        MyImage->SetImage(&Brush);
    }
}

void UCustomImageWidget::SetBrushFromTexture(UTexture2D* Texture, bool bMatchSize /*= false*/)
{
    Brush.SetResourceObject(Texture);

    if (Texture)
    {
        Texture->bIgnoreStreamingMipBias = true;
    }

    if (bMatchSize && Texture)
    {
        Brush.ImageSize.X = Texture->GetSizeX();
        Brush.ImageSize.Y = Texture->GetSizeY();
    }

    if (MyImage.IsValid())
    {
        MyImage->SetImage(&Brush);
    }
}

void UCustomImageWidget::SetBrushFromTextureDynamic(UTexture2DDynamic* Texture, bool bMatchSize /*= false*/)
{
    Brush.SetResourceObject(Texture);

    if (bMatchSize && Texture)
    {
        Brush.ImageSize.X = Texture->SizeX;
        Brush.ImageSize.Y = Texture->SizeY;
    }

    if (MyImage.IsValid())
    {
        MyImage->SetImage(&Brush);
    }
}

void UCustomImageWidget::SetBrushFromMaterial(UMaterialInterface* Material)
{
    Brush.SetResourceObject(Material);

    if (MyImage.IsValid())
    {
        MyImage->SetImage(&Brush);
    }
}


UMaterialInstanceDynamic* UCustomImageWidget::GetDynamicMaterial()
{
    UMaterialInterface* Material = nullptr;

    UObject* Resource = Brush.GetResourceObject();
    Material = Cast<UMaterialInterface>(Resource);

    if (Material)
    {
        UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(Material);

        if (!DynamicMaterial)
        {
            DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
            Brush.SetResourceObject(DynamicMaterial);

            if (MyImage.IsValid())
            {
                MyImage->SetImage(&Brush);
            }
        }

        return DynamicMaterial;
    }

    return nullptr;
}


TSharedRef<SWidget> UCustomImageWidget::RebuildWidget()
{
    MyImage = SNew(SCustomImage);

    return MyImage.ToSharedRef();
}


void UCustomImageWidget::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    TAttribute<FSlateColor> ColorAndOpacityBinding = OPTIONAL_BINDING(FSlateColor, ColorAndOpacity);

    TAttribute<const FSlateBrush*> ImageBinding = OPTIONAL_BINDING_CONVERT(FSlateBrush, Brush, const FSlateBrush*, ConvertImage);

    if (MyImage.IsValid())
    {
        MyImage->SetImage(ImageBinding);
        MyImage->SetColorAndOpacity(ColorAndOpacityBinding);
        MyImage->SetOnMouseButtonDown(BIND_UOBJECT_DELEGATE(FPointerEventHandler, HandleMouseButtonDown));
        MyImage->SetOnAnimationEnd(BIND_UOBJECT_DELEGATE(FAnimationEndDelegate, HandleAnimationEnd));


        MyImage->SetDuration(Duration);
        MyImage->SetColumnCount(ColumnCount);
        MyImage->SetRowCount(RowCount);
        MyImage->SetCutFrameCount(CutFrameCount);
        MyImage->ResetAnimation();
    }
}

void UCustomImageWidget::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);

    MyImage.Reset();
}

#if WITH_EDITOR



const FText UCustomImageWidget::GetPaletteCategory()
{
    return LOCTEXT("Common", "Custom Widget");
}
#endif

#undef LOCTEXT_NAMESPACE