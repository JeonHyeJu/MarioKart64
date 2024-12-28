Texture2D diffTexture;
SamplerState SampleType;

float4 main(float4 pos : SV_POSITION, float2 texcoord : TEXCOORD) : SV_TARGET
{
    // float4 textureColor = diffTexture.Sample(SampleType, texcoord);
    float4 textureColor = { 1.0f, 0.f, 0.f, 1.f };
    return textureColor;
}