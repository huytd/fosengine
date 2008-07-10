
float4x4 WorldViewProj;  // World * View * Projection transformation

struct VS_INPUT
{
   float4 Position   : POSITION;   // vertex position
   
   float2 DetailMapCoord         : TEXCOORD0;
   float2 TextureOneCoord         : TEXCOORD1;
   float2 TextureTwoCoord         : TEXCOORD2;
   float2 TextureThreeCoord      : TEXCOORD3;
   float2 TextureFourCoord         : TEXCOORD4;
   float2 LightMapCoord         : TEXCOORD5;
   float2 ColorMapCoord         : TEXCOORD6;
   
};

// Vertex shader output structure
struct VS_OUTPUT
{
   float4 Position   : POSITION;   // vertex position
   
   float2 DetailMapCoord         : TEXCOORD0;
   float2 TextureOneCoord         : TEXCOORD1;
   float2 TextureTwoCoord         : TEXCOORD2;
   float2 TextureThreeCoord      : TEXCOORD3;
   float2 TextureFourCoord         : TEXCOORD4;
   float2 LightMapCoord         : TEXCOORD5;
   float2 ColorMapCoord         : TEXCOORD6;
};


VS_OUTPUT main( VS_INPUT Input )
{
   VS_OUTPUT Output;

   // transform position to clip space
   Output.Position = mul(Input.Position, WorldViewProj);

   Output.DetailMapCoord      = Input.DetailMapCoord;
   Output.TextureOneCoord      = Input.TextureOneCoord;
   Output.TextureTwoCoord      = Input.TextureTwoCoord;
   Output.TextureThreeCoord   = Input.TextureThreeCoord;
   Output.TextureFourCoord      = Input.TextureFourCoord;
   Output.LightMapCoord      = Input.LightMapCoord;
   Output.ColorMapCoord      = Input.ColorMapCoord;
   
   return Output;
}