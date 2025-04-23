#include "UI/Common/SEditableNumberText.h"
#include "Misc/Char.h"
#include "Misc/DefaultValueHelper.h"

void SEditableNumberText::Construct(const FArguments& InArgs)
{
	MinValue = InArgs._MinValue;
	MaxValue = InArgs._MaxValue;

	SEditableTextBox::Construct(
		SEditableTextBox::FArguments()
		.OnVerifyTextChanged(this, &SEditableNumberText::HandleVerifyTextChanged)
		.OnTextCommitted(this, &SEditableNumberText::HandleTextCommitted)
	);

	SetText(FText::AsNumber(InArgs._MinValue.Get(0.0f)));
}

float SEditableNumberText::GetValue() const
{
	float ParsedValue = 0.0f;
	FDefaultValueHelper::ParseFloat(GetText().ToString(), ParsedValue);

	if (MinValue.IsSet()) ParsedValue = FMath::Max(ParsedValue, MinValue.GetValue());
	if (MaxValue.IsSet()) ParsedValue = FMath::Min(ParsedValue, MaxValue.GetValue());

	return ParsedValue;
}

FReply SEditableNumberText::OnKeyChar(const FGeometry& MyGeometry, const FCharacterEvent& InCharacterEvent)
{
	const TCHAR Char = InCharacterEvent.GetCharacter();

	// Pass if digit
	if (FChar::IsDigit(Char))
	{
		return SEditableTextBox::OnKeyChar(MyGeometry, InCharacterEvent);
	}

	// Only one '.' or ','
	if (Char == TEXT('.') || Char == TEXT(','))
	{
		const FString CurrentText = GetText().ToString();
		if (!CurrentText.Contains(TEXT(".")) && !CurrentText.Contains(TEXT(",")))
		{
			return SEditableTextBox::OnKeyChar(MyGeometry, InCharacterEvent);
		}
	}

	return FReply::Handled();
}

bool SEditableNumberText::HandleVerifyTextChanged(const FText& NewText, FText& OutErrorMessage)
{
	const FString Input = NewText.ToString();

	// Check if not a number
	float ParsedValue = 0.0f;
	if (!FDefaultValueHelper::ParseFloat(Input, ParsedValue))
	{
		OutErrorMessage = FText::FromString(TEXT("Input must be a valid number"));
		return false;
	}

	// -------------------------------------------------------------------------------------------------------
	// Range check
	// -------------------------------------------------------------------------------------------------------

	if (MinValue.IsSet() && ParsedValue < MinValue.GetValue())
	{
		OutErrorMessage = FText::FromString(FString::Printf(TEXT("Must be ≥ %.2f"), MinValue.GetValue()));
		return false;
	}

	if (MaxValue.IsSet() && ParsedValue > MaxValue.GetValue())
	{
		OutErrorMessage = FText::FromString(FString::Printf(TEXT("Must be ≤ %.2f"), MaxValue.GetValue()));
		return false;
	}

	// -------------------------------------------------------------------------------------------------------

	return true;
}

void SEditableNumberText::HandleTextCommitted(const FText& InText, ETextCommit::Type Arg)
{
	const FString Input = InText.ToString();
	float Parsed = 0.0f;

	// Check if non-digit
	if (!FDefaultValueHelper::ParseFloat(Input, Parsed))
	{
		SetText(FText::GetEmpty());
		return;
	}

	// Check if not in range
	if ((MinValue.IsSet() && Parsed < MinValue.GetValue()) ||
		(MaxValue.IsSet() && Parsed > MaxValue.GetValue()))
	{
		SetText(FText::GetEmpty());
		return;
	}

	SetText(FText::AsNumber(Parsed));
}
