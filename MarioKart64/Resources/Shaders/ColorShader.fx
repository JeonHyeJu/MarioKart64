struct EngineVertex
{
    float4 POSITION : POSITION;
    float4 UV : TEXCOORD;
    float4 COLOR : COLOR;
};

struct VertexShaderOutPut
{
    float4 SVPOSITION : SV_POSITION;
    float4 UV : TEXCOORD;
    float4 COLOR : COLOR;
};

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

VertexShaderOutPut MY_VS(EngineVertex _Vertex)
{
	VertexShaderOutPut OutPut;
	OutPut.SVPOSITION = mul(_Vertex.POSITION, WVP);
	OutPut.UV = _Vertex.UV;
	OutPut.COLOR = _Vertex.COLOR;
	return OutPut;
}

cbuffer FColor : register(b0)
{
	float4 Albedo;
};

float4 GetMixedValue(float4 _color1, float4 _color2, float _per)
{
    float last = 1 - _per;
    return _color1 * last + _color2 * _per;
};

float4 MY_PS(VertexShaderOutPut _Vertex) : SV_Target0
{
    float4 white = float4(1.f, 1.f, 1.f, 1.f);
    float4 color = GetMixedValue(Albedo, white, _Vertex.UV.y);
    return color;
};
