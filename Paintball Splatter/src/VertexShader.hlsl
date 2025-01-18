cbuffer CBuf
{
    matrix transform;
};

float4 main(float3 pos : Position) : SV_Position
{
    float4 transformedPosition = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform);
    return float4(transformedPosition.x, transformedPosition.y, 0.0f, 1.0f);
}