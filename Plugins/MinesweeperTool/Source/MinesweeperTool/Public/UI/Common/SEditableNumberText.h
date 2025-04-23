#pragma once

#include "Widgets/Input/SEditableTextBox.h"

/**
 * Editable text box that only accepts numeric input and allows an optional range.
 */
class SEditableNumberText final : public SEditableTextBox
{
public:
	SLATE_BEGIN_ARGS(SEditableNumberText)
			: _MinValue(TOptional<float>())
			  , _MaxValue(TOptional<float>())
		{
		}

		SLATE_ARGUMENT(TOptional<float>, MinValue)
		SLATE_ARGUMENT(TOptional<float>, MaxValue)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	float GetValue() const;

	virtual FReply OnKeyChar(const FGeometry& MyGeometry, const FCharacterEvent& InCharacterEvent) override;

private:
	TOptional<float> MinValue;
	TOptional<float> MaxValue;

	bool HandleVerifyTextChanged(const FText& NewText, FText& OutErrorMessage);
	void HandleTextCommitted(const FText& InText, ETextCommit::Type Arg);
};
