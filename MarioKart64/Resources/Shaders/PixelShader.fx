Texture2D diffTexture;
SamplerState SampleType;

cbuffer FColor : register(b1)
{
    float4 Albedo;
};

float4 main_PS(float4 pos : SV_POSITION, float2 texcoord : TEXCOORD) : SV_TARGET
{
    float4 textureColor = diffTexture.Sample(SampleType, texcoord);
    return textureColor + Albedo;
}