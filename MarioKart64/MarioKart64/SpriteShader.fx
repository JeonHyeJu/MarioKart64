
struct VertexIn
{
	float4 Pos : POSITION;
	float4 Color : COLOR;
};

struct VertexOut
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

VertexOut main(VertexIn _vin)
{
    VertexOut vout;
    vout.Pos = _vin.Pos;
    vout.Color = _vin.Color;
    return vout;
}