Texture2D diffTexture;
SamplerState SampleType;

cbuffer FDebug
{
    float4 Index;
};

float4 main_PS(float4 pos : SV_POSITION, float2 texcoord : TEXCOORD, float4 color : COLOR) : SV_TARGET
{
    //if (Index.w > 0 && abs(color.x - Index.x) < 1e-1)
    //{
        //return float4(0.f, 1.f, 0.f, 1.f);
    //}
    
    float4 textureColor = diffTexture.Sample(SampleType, texcoord);
    return textureColor;
}