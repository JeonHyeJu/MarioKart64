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

struct VOut
{
    float4 pos : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

VOut main(float4 pos : POSITION, float2 texcoord : TEXCOORD)
{
    VOut output;

    output.pos = mul(pos, World);
    output.pos = mul(output.pos, View);
    output.pos = mul(output.pos, Projection);
    output.texcoord = texcoord;

    return output;
}