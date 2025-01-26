cbuffer FTransform : register(b0)
{
    float4 Scale;
    float4 Rotation;
    float4 Qut;
    float4 Location;

    float4 RelativeScale;
    float4 RelativeRotation;
    float4 RelativeQut;
    float4 RelativeLocation;

    float4 WorldScale;
    float4 WorldRotation;
    float4 WorldQuat;
    float4 WorldLocation;

    float4x4 ScaleMat;
    float4x4 RotationMat;
    float4x4 LocationMat;
    float4x4 RevolveMat;
    float4x4 ParentMat;
    float4x4 LocalWorld;
    float4x4 World;
    float4x4 View;
    float4x4 Projection;
    float4x4 WVP;
};

cbuffer FSpriteData : register(b1)
{
    float4 CuttingPos;
    float4 CuttingSize;
    float4 Pivot;
};

struct VOut
{
    float4 pos : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float2 orgUV : TEXCOORD1;
    float4 color : COLOR;
};

VOut main_VS(float4 pos : POSITION, float2 texcoord : TEXCOORD, float4 color : COLOR)
{
    VOut output;

    pos.x += (1.0f - Pivot.x) - 0.5f;
    pos.y += (1.0f - Pivot.y) - 0.5f;
    output.pos = mul(pos, WVP);
    
    output.orgUV = texcoord;
    output.texcoord = texcoord;
    output.texcoord.x = (texcoord.x * CuttingSize.x) + CuttingPos.x;
    output.texcoord.y = (texcoord.y * CuttingSize.y) + CuttingPos.y;
    
    output.color = color;

    return output;
}

Texture2D diffTexture;
SamplerState SampleType;

cbuffer FText
{
    float4 Color;
};

float4 GetMixedValue(float4 _color1, float4 _color2, float _per, float _max)
{
    // float last = 1 - _per;
    float last = _max - (_per * _max);
    return _color1 * last + _color2 * _per;
};

float4 main_PS(float4 pos : SV_POSITION, float2 texcoord : TEXCOORD0, float4 color : COLOR, float2 orgUV : TEXCOORD1) : SV_TARGET
{
    float4 textureColor = diffTexture.Sample(SampleType, texcoord);
    
    if (Color.a > 0 && textureColor.a)
    {
        float4 white = float4(1.f, 1.f, 1.f, 1.f);
        float4 color = GetMixedValue(white, Color, orgUV.y, textureColor.a);
        return color;
    }
    
    return textureColor;
}