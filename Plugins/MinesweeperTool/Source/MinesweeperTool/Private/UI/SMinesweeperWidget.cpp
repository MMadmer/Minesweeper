#include "UI/SMinesweeperWidget.h"

#include "UI/SGameGridWidget.h"
#include "UI/Common/SEditableNumberText.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"

void SMinesweeperWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SVerticalBox)

		// Columns
		+ SVerticalBox::Slot().AutoHeight().Padding(4)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("Grid Width: ")))
			]
			+ SHorizontalBox::Slot().FillWidth(1)
			[
				SAssignNew(GridWidthTextBox, SEditableNumberText)
				.MinValue(3)
				.MaxValue(99)
			]
		]

		// Rows
		+ SVerticalBox::Slot().AutoHeight().Padding(4)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("Grid Height: ")))
			]
			+ SHorizontalBox::Slot().FillWidth(1)
			[
				SAssignNew(GridHeightTextBox, SEditableNumberText)
				.MinValue(3)
				.MaxValue(99)
			]
		]

		// Mines
		+ SVerticalBox::Slot().AutoHeight().Padding(4)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("Bombs: ")))
			]
			+ SHorizontalBox::Slot().FillWidth(1)
			[
				SAssignNew(BombCountTextBox, SEditableNumberText)
				.MinValue(1)
			]
		]

		// New game button
		+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(4)
		[
			SNew(SButton)
			.Text(FText::FromString(TEXT("Start New Game")))
			.OnClicked(this, &SMinesweeperWidget::OnStartGameClicked)
		]

		// Game widget
		+ SVerticalBox::Slot().FillHeight(1).Padding(4)
		[
			SAssignNew(GameContainer, SVerticalBox)
		]

	];
}

FReply SMinesweeperWidget::OnStartGameClicked()
{
	const int32 Width = GridWidthTextBox->GetValue();
	const int32 Height = GridHeightTextBox->GetValue();
	const int32 Bombs = BombCountTextBox->GetValue();

	if (GameContainer.IsValid())
	{
		GameContainer->ClearChildren();

		// Make a game result text
		GameContainer->AddSlot()
		             .AutoHeight()
		             .HAlign(HAlign_Center)
		             .Padding(4)
		[
			SAssignNew(ResultTextBlock, STextBlock)
			.Text(FText::GetEmpty())
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", 18))
		];

		// Make a new game widget
		GameContainer->AddSlot()
		             .FillHeight(1)
		[
			SNew(SGameGridWidget)
			.GridWidth(Width)
			.GridHeight(Height)
			.BombCount(Bombs)
			.OnGameFinished(FOnGameFinished::CreateSP(this, &SMinesweeperWidget::HandleGameFinished))
		];
	}

	return FReply::Handled();
}

void SMinesweeperWidget::HandleGameFinished(const bool WasSuccessful)
{
	if (ResultTextBlock.IsValid())
	{
		ResultTextBlock->SetText(FText::FromString(WasSuccessful ? TEXT("You Won!") : TEXT("Game Over!")));
	}
}
