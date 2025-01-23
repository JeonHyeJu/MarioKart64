#include "Light.hlsli"
#include "Transform.hlsli"

struct EngineVertex
{
	float4 POSITION : POSITION;
	float4 TEXCOORD : TEXCOORD;
	float4 COLOR : COLOR;
	float4 NORMAL : NORMAL;
	float4 BINORMAL : BINORMAL;
	float4 TANGENT : TANGENT;
};

struct VertexShaderOutPut
{
	float4 SVPOSITION : SV_POSITION; 
	float4 TEXCOORD : TEXCOORD; 
	float4 COLOR : COLOR;
	
	float4 VIEWPOS : POSITION;
	float4 NORMAL : NORMAL; 
	float4 BINORMAL : BINORMAL; 
	float4 TANGENT : TANGENT; 
};

VertexShaderOutPut MeshLight_VS(EngineVertex _Vertex /*, int _DataIndex*/)
{
	VertexShaderOutPut OutPut;
	
	OutPut.SVPOSITION = mul(_Vertex.POSITION, WVP);
	OutPut.TEXCOORD = _Vertex.TEXCOORD;
	OutPut.COLOR = _Vertex.COLOR;
	
	OutPut.VIEWPOS = mul(_Vertex.POSITION, World * View);

	_Vertex.NORMAL.w = 0.0f;
	OutPut.NORMAL = mul(_Vertex.NORMAL, World * View);
	
	_Vertex.BINORMAL.w = 0.0f;
	OutPut.BINORMAL = mul(_Vertex.BINORMAL, World * View);
	
	_Vertex.TANGENT.w = 0.0f;
	OutPut.TANGENT = mul(_Vertex.TANGENT, World * View);
	
	return OutPut;
}

cbuffer MeshColor : register(b0)
{
	float4 SettingColor;
};

float4 MeshLight_PS(VertexShaderOutPut _Vertex) : SV_Target0
{
	float4 ResultColor;
	ResultColor = SettingColor;
	
	float4 DiffuseColor;
	float4 SpacularLight;
	float4 AmbiantLight = (0.1f, 0.1f, 0.1f, 0.1f);
	
	for (int i = 0; i < LightCount; ++i)
	{
		DiffuseColor += CalDiffusLight(_Vertex.NORMAL, LightArr[i]);
		SpacularLight += CalSpacularLight(_Vertex.VIEWPOS, _Vertex.NORMAL, LightArr[i]);
		//AmbiantLight += LightArr[0].AmbientLight;
	}
	
	ResultColor.xyz *= (DiffuseColor.xyz + SpacularLight.xyz + AmbiantLight.xyz);
	ResultColor.a = 1.0f;
	
	return ResultColor;
};

