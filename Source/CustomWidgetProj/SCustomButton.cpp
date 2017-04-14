// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomWidgetProj.h"
#include "SCustomButton.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SCustomButton::Construct(const FArguments& InArgs)
{
    Label = InArgs._Label;
    ButtonClicked = InArgs._ButtonClicked;
    ChildSlot
        .VAlign(VAlign_Center)
        .HAlign(HAlign_Center)
        [
            SNew(SButton)
            .OnClicked(ButtonClicked)
            .Content()
            [
                SNew(STextBlock)
                .Text_Lambda([this] {return FText::FromString(Label.Get()); })
            ]
        ];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
