#pragma once
#include "SceneComponent.h"
#include "EngineSprite.h"
#include "RenderUnit.h"

class URenderer : public USceneComponent
{
	friend class UEngineCamera;

public:
	ENGINEAPI URenderer();
	ENGINEAPI ~URenderer();

	URenderer(const URenderer& _Other) = delete;
	URenderer(URenderer&& _Other) noexcept = delete;
	URenderer& operator=(const URenderer& _Other) = delete;
	URenderer& operator=(URenderer&& _Other) noexcept = delete;

	ENGINEAPI void SetOrder(int _Order) override;
 
	ENGINEAPI void SetSprite(std::string_view _Value);
	ENGINEAPI void SetSprite(UEngineSprite* _Sprite);

	ENGINEAPI void SetSpriteData(size_t _Index);

protected:
	ENGINEAPI void BeginPlay() override;
	ENGINEAPI virtual void Render(UEngineCamera* _Camera, float _DeltaTime);

	FSpriteData SpriteData;

	class UEngineSprite* Sprite = nullptr;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerState = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> TransformConstBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> SpriteConstBuffer = nullptr;
	void ShaderResInit();
	void ShaderResSetting();

	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayOut = nullptr;
	void InputAssembler1Init();
	void InputAssembler1Setting();
	void InputAssembler1LayOut();

	Microsoft::WRL::ComPtr<ID3DBlob> VSShaderCodeBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> VSErrorCodeBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader = nullptr;
	void VertexShaderInit();
	void VertexShaderSetting();

	Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer = nullptr;

	D3D11_PRIMITIVE_TOPOLOGY Topology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	void InputAssembler2Init();
	void InputAssembler2Setting();

	D3D11_VIEWPORT ViewPortInfo;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerState = nullptr;
	void RasterizerInit();
	void RasterizerSetting();

	Microsoft::WRL::ComPtr<ID3DBlob> PSShaderCodeBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> PSErrorCodeBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader = nullptr;
	void PixelShaderInit();
	void PixelShaderSetting();

	void OutPutMergeSetting();

	std::vector<URenderUnit> Units;
};

