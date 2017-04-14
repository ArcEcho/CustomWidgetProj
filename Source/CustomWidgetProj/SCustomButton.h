// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Widgets/SCompoundWidget.h"

/**
 *
 */
class CUSTOMWIDGETPROJ_API SCustomButton : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SCustomButton)
        : _Label(TEXT("Default Value"))
        , _ButtonClicked()
    {}
    SLATE_ATTRIBUTE(FString, Label)
        SLATE_EVENT(FOnClicked, ButtonClicked)
        SLATE_END_ARGS()
public:
    void Construct(const FArguments&InArgs);
    TAttribute<FString> Label;
    FOnClicked ButtonClicked;
};
