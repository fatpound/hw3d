struct VSOut
{
    float3 color : COLOR;
    float4 pos : SV_Position;
};

cbuffer CBuf
{
    matrix transform;
};

VSOut main(float2 pos : POSITION, float3 col : COLOR) // now we dont need to type the semantic here because thats already in the struct
{
    VSOut vso;
    
    vso.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform);
    vso.color = col;
    
    return vso;
}