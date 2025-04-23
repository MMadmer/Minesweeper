#include "UI/SGameGridWidget.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Images/SImage.h"

SGameGridWidget::SGameGridWidget()
{
	bGameOver = false;
}

void SGameGridWidget::Construct(const FArguments& InArgs)
{
	bGameOver = false;

	GridWidth = InArgs._GridWidth;
	GridHeight = InArgs._GridHeight;
	OnGameFinished = InArgs._OnGameFinished;

	Cells.SetNum(GridWidth * GridHeight);
	GenerateMines(InArgs._BombCount);

	ChildSlot
	[
		SAssignNew(GridPanel, SUniformGridPanel)
	];

	for (int32 Y = 0; Y < GridHeight; ++Y)
	{
		for (int32 X = 0; X < GridWidth; ++X)
		{
			const int32 Index = GetIndex(X, Y);

			TSharedPtr<SButton> CellButton;

			TSharedRef<SWidget> ButtonWidget =
				SAssignNew(CellButton, SButton)
				.OnClicked_Lambda([this, X, Y] { return OnCellClicked(X, Y); });

			Cells[Index].Widget = ButtonWidget;

			GridPanel->AddSlot(X, Y)
			[
				ButtonWidget
			];
		}
	}
}

void SGameGridWidget::GenerateMines(const int32 BombCount)
{
	TSet<int32> UniqueIndices;

	const int32 Total = Cells.Num();
	while (UniqueIndices.Num() < BombCount)
	{
		UniqueIndices.Add(FMath::RandRange(0, Total - 1));
	}

	for (const int32 Index : UniqueIndices)
	{
		Cells[Index].bHasMine = true;
	}
}

int32 SGameGridWidget::CountBombsAround(const int32 X, const int32 Y) const
{
	int32 BombsAround = 0;
	for (int32 DX = -1; DX <= 1; ++DX)
	{
		for (int32 DY = -1; DY <= 1; ++DY)
		{
			if (DX == 0 && DY == 0) continue;

			const int32 NX = X + DX;
			const int32 NY = Y + DY;

			if (NX >= 0 && NX < GridWidth && NY >= 0 && NY < GridHeight)
			{
				if (Cells[GetIndex(NX, NY)].bHasMine)
				{
					BombsAround++;
				}
			}
		}
	}

	return BombsAround;
}

void SGameGridWidget::RevealCellVisual(const int32 X, const int32 Y)
{
	const int32 Index = GetIndex(X, Y);
	if (!Cells.IsValidIndex(Index) || !Cells[Index].bWasRevealed) return;

	TSharedPtr<SWidget> CellWidget;

	if (Cells[Index].bHasMine)
	{
		CellWidget = SNew(STextBlock)
			.Text(FText::FromString(TEXT("*")));
	}
	else
	{
		const int32 BombsAround = CountBombsAround(X, Y);

		CellWidget = SNew(STextBlock)
			.Text(FText::AsNumber(BombsAround));
	}

	GridPanel->AddSlot(X, Y)
	         .HAlign(HAlign_Center)
	         .VAlign(VAlign_Center)
	[
		CellWidget.ToSharedRef()
	];
}

void SGameGridWidget::RevealCellFloodFill(const int32 X, const int32 Y)
{
	if (X < 0 || X >= GridWidth || Y < 0 || Y >= GridHeight) return;

	const int32 Index = GetIndex(X, Y);
	if (Cells[Index].bWasRevealed || Cells[Index].bHasMine) return;

	Cells[Index].bWasRevealed = true;

	RevealCellVisual(X, Y);

	if (CountBombsAround(X, Y) == 0)
	{
		for (int32 DX = -1; DX <= 1; ++DX)
		{
			for (int32 DY = -1; DY <= 1; ++DY)
			{
				if (DX == 0 && DY == 0) continue;
				RevealCellFloodFill(X + DX, Y + DY);
			}
		}
	}
}

FReply SGameGridWidget::OnCellClicked(const int32 X, const int32 Y)
{
	const int32 Index = GetIndex(X, Y);

	if (bGameOver || Cells[Index].bWasRevealed)
	{
		return FReply::Handled();
	}

	if (Cells[Index].bHasMine)
	{
		// Open all cells
		for (int32 i = 0; i < Cells.Num(); ++i)
		{
			if (!Cells[i].bWasRevealed)
			{
				Cells[i].bWasRevealed = true;

				const int32 RevealX = i % GridWidth;
				const int32 RevealY = i / GridWidth;

				RevealCellVisual(RevealX, RevealY);
			}
		}

		bGameOver = true;
		EndGame(false);
		return FReply::Handled();
	}

	RevealCellFloodFill(X, Y);
	TryWinGame();

	return FReply::Handled();
}

void SGameGridWidget::TryWinGame()
{
	if (bGameOver) return;

	for (const FCellData& Cell : Cells)
	{
		// Continue playing
		if (!Cell.bWasRevealed && !Cell.bHasMine) return;
	}

	// Win
	for (int32 i = 0; i < Cells.Num(); ++i)
	{
		if (!Cells[i].bWasRevealed)
		{
			Cells[i].bWasRevealed = true;
			const int32 RevealX = i % GridWidth;
			const int32 RevealY = i / GridWidth;
			RevealCellVisual(RevealX, RevealY);
		}
	}

	bGameOver = true;
	EndGame(true);
}

void SGameGridWidget::EndGame(const bool WasSuccessful) const
{
	if (OnGameFinished.IsBound())
	{
		OnGameFinished.Execute(WasSuccessful);
	}
}
