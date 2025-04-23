#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SUniformGridPanel;
class SButton;

DECLARE_DELEGATE_OneParam(FOnGameFinished, const bool /* bWasSuccessful */);

class SGameGridWidget final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGameGridWidget)
		{
		}

		SLATE_ARGUMENT(int32, GridWidth)
		SLATE_ARGUMENT(int32, GridHeight)
		SLATE_ARGUMENT(int32, BombCount)
		SLATE_EVENT(FOnGameFinished, OnGameFinished)
	SLATE_END_ARGS()

	SGameGridWidget();
	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<SUniformGridPanel> GridPanel;

	struct FCellData
	{
		TSharedPtr<SWidget> Widget;
		uint8 bWasRevealed : 1;
		uint8 bHasMine : 1;

		FCellData() : Widget(nullptr), bWasRevealed(false), bHasMine(false)
		{
		}
	};

	TArray<FCellData> Cells;
	int32 GridWidth = 3;
	int32 GridHeight = 3;

	uint8 bGameOver : 1;
	FOnGameFinished OnGameFinished;

	void GenerateMines(const int32 BombCount);
	int32 CountBombsAround(const int32 X, const int32 Y) const;
	void RevealCellVisual(const int32 X, const int32 Y);
	void RevealCellFloodFill(const int32 X, const int32 Y);

	FReply OnCellClicked(const int32 X, const int32 Y);
	int32 GetIndex(const int32 X, const int32 Y) const { return Y * GridWidth + X; }

	// Try to end the game with win condition
	void TryWinGame();

	// End the game with a result
	void EndGame(const bool WasSuccessful) const;
};
