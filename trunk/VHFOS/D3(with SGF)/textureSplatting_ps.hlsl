sampler2D   DetailMap;
sampler2D   TextureOne;
sampler2D   TextureTwo;
sampler2D   TextureThree;
sampler2D   TextureFour;
sampler2D   LightMap;
sampler2D   ColorMap;

// Pixel shader output structure
struct PS_OUTPUT
{
    float4 Color : COLOR0;  // Pixel color   
};

struct PS_INPUT
{
   float3 Position               : POSITION;   // vertex position
   float3 Normal               : NORMAL;

   float4 Color               : COLOR;
   float2 DetailMapCoord         : TEXCOORD0;
   float2 TextureOneCoord         : TEXCOORD1;
   float2 TextureTwoCoord         : TEXCOORD2;
   float2 TextureThreeCoord      : TEXCOORD3;
   float2 TextureFourCoord         : TEXCOORD4;
   float2 LightMapCoord         : TEXCOORD5;
   float2 ColorMapCoord         : TEXCOORD6;
};

PS_OUTPUT main( PS_INPUT Input )
{
   PS_OUTPUT Output;

   float4 detailMapColor   = tex2D(DetailMap, Input.DetailMapCoord);
   
   float4 textOneColor      = tex2D(TextureOne,      Input.TextureOneCoord);
   float4 textTwoColor      = tex2D(TextureTwo,      Input.TextureTwoCoord);
   float4 textThreeColor   = tex2D(TextureThree,   Input.TextureThreeCoord);
   float4 textFourColor   = tex2D(TextureFour,   Input.TextureFourCoord);
   
   float4 lightMapColor   = tex2D(LightMap,      Input.LightMapCoord);
   float4 colorMapColor   = tex2D(ColorMap,      Input.ColorMapCoord);
   
   textOneColor   = textOneColor      * detailMapColor.x;
   textTwoColor   = textOneColor      + detailMapColor.y * (textTwoColor      - textOneColor);
   textThreeColor   = textTwoColor      + detailMapColor.z * (textThreeColor   - textTwoColor);
   textFourColor   = textThreeColor   + detailMapColor.w * (textFourColor      - textThreeColor);
   
   colorMapColor   = colorMapColor * 1.6;
   lightMapColor   = lightMapColor + 0.2;
   
   //Output.Color   = textFourColor * lightMapColor - colorMapColor;
   Output.Color   = textFourColor * lightMapColor * colorMapColor;

   return Output;
}