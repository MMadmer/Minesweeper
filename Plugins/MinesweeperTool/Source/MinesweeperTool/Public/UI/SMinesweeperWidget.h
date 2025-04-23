#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SEditableNumberText;

class SMinesweeperWidget final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMinesweeperWidget)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<SEditableNumberText> GridWidthTextBox;
	TSharedPtr<SEditableNumberText> GridHeightTextBox;
	TSharedPtr<SEditableNumberText> BombCountTextBox;

	TSharedPtr<SVerticalBox> GameContainer;
	TSharedPtr<STextBlock> ResultTextBlock;

	FReply OnStartGameClicked();

	void HandleGameFinished(const bool WasSuccessful);
};
