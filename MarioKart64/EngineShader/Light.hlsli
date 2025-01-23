struct FLightData
{
	float4 LightPos;
	float4 LightDir;
	float4 LightRevDir;
	float4 LightColor;
	float4 AmbientLight;
	float4 ViewLightPos;
	float4 ViewLightDir;
	float4 ViewLightRevDir;
	float4 CameraPosition;
};

cbuffer FLightDatas : register(b11)
{
	int LightCount;
	FLightData LightArr[256];
};

float4 CalSpacularLight(float4 _ViewPos, float4 _ViewNormal, FLightData _Data)
{
	float ResultLight = 0.0f;

	float3 N = normalize(_ViewNormal.xyz);
	float3 L = normalize(_Data.ViewLightRevDir.xyz);
	
	float3 Reflection = normalize(2.0f * N.xyz * dot(L.xyz, N.xyz) - L.xyz);
	
	float3 Eye = normalize(_Data.CameraPosition.xyz - _ViewPos.xyz);

	ResultLight = max(0.0f, dot(Reflection.xyz, Eye.xyz));
	
	ResultLight = pow(ResultLight, 60.0f);
	return ResultLight;
}

float4 CalDiffusLight(float4 _ViewNormal, FLightData _Data)
{
	float4 N = normalize(_ViewNormal);
	float4 L = normalize(_Data.ViewLightRevDir);
	float ResultLight;
	ResultLight = dot(N, L);

	ResultLight = max(0.0f, ResultLight);
	
	return ResultLight * 0.5f;

}

