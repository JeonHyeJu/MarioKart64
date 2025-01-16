#pragma once
#include "EngineResources.h"
#include "ThirdParty/FW1FontWrapper/Inc/FW1FontWrapper.h"

class UEngineFont : public UEngineResources
{
public:
	UEngineFont();
	~UEngineFont();

	UEngineFont(const UEngineFont& _Other) = delete;
	UEngineFont(UEngineFont&& _Other) noexcept = delete;
	UEngineFont& operator=(const UEngineFont& _Other) = delete;
	UEngineFont& operator=(UEngineFont&& _Other) noexcept = delete;

	ENGINEAPI static std::shared_ptr<UEngineFont> Load(std::string_view _Name, std::string_view _Path);
	void FontDraw(const std::string& _Text, float _FontScale, const FVector& _Pos, const UColor& _Color = UColor::BLACK, FW1_TEXT_FLAG Flag = FW1_LEFT);

protected:

private:
	Microsoft::WRL::ComPtr<IFW1FontWrapper> Font;
	void ResLoad(std::string_view _Path);
};

