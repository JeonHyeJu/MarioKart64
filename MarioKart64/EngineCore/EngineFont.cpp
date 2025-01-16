#include "PreCompile.h"
#include "EngineFont.h"

#pragma comment(lib, "FW1FontWrapper.lib")

IFW1Factory* FontFactory = nullptr;

class UFontFactoryCreator
{
public:
	UFontFactoryCreator()
	{
		FW1CreateFactory(FW1_VERSION, &FontFactory);
	}
	~UFontFactoryCreator()
	{
		if (nullptr != FontFactory)
		{
			FontFactory->Release();
			FontFactory = nullptr;
		}
	}
};

UFontFactoryCreator Inst;

UEngineFont::UEngineFont()
{
}

UEngineFont::~UEngineFont()
{
}

std::shared_ptr<UEngineFont> UEngineFont::Load(std::string_view _Name, std::string_view _Path)
{
	std::string UpperName = ToUpperName(_Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("이미 로드한 텍스처를 도 로드하려고 했습니다." + UpperName);
		return nullptr;
	}

	std::shared_ptr<UEngineFont> NewRes = std::make_shared<UEngineFont>();
	PushRes<UEngineFont>(NewRes, _Name, _Path);
	NewRes->ResLoad(_Path);
	return NewRes;
}

void UEngineFont::ResLoad(std::string_view _Path)
{
	std::wstring WPath = UEngineString::AnsiToUnicode(_Path);

	D3D11_BLEND_DESC blendDesc = { 0, };
	for (int i = 0; i < 4; ++i)
	{
		blendDesc.RenderTarget[i].BlendEnable = true;
		blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
	}

	if (FontFactory->CreateFontWrapper(UEngineCore::GetDevice().GetDevice(), WPath.c_str(), blendDesc, &Font))
	{
		MSGASSERT("폰트 생성 실패");
		return;
	}
	
}

void UEngineFont::FontDraw(const std::string& _Text, float _FontScale, const FVector& _Pos, const UColor& _Color /*= FVector::BLACK*/, FW1_TEXT_FLAG Flag /*= FW1_LEFT*/)
{
	std::wstring Text = UEngineString::AnsiToUnicode(_Text);

	Font->DrawString(UEngineCore::GetDevice().GetContext(), Text.c_str(), _FontScale, _Pos.X, _Pos.Y, _Color.Color, Flag);

	UEngineCore::GetDevice().GetContext()->GSSetShader(nullptr, nullptr, 0);
}