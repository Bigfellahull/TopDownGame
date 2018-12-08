cbuffer Parameters : register(b0)
{
	row_major float4x4 MatrixTransform;
	row_major float4x4 ScrollTransform;
};

void SpriteVertexShader(inout float4 color : COLOR0,
	inout float2 texCoord : TEXCOORD0,
	inout float4 position : SV_Position)
{
	position = mul(position, MatrixTransform);

	texCoord = mul(float4(texCoord.xy, 0, 1), ScrollTransform).xy;
}