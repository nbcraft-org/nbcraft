#ifdef _DIRECT3D9
// D3D9 does not support custom semantic names, which is very unfortunate
#define PS_FOG_COLOR TEXCOORD1
#define PS_FOG_INTENSITY TEXCOORD2
#define LIGHT TEXCOORD3
#define PS_OVERLAY_COLOR COLOR1
#endif

struct PS_Input
{
    float4 position : SV_Position;

    float3 light : LIGHT;
    float3 fogColor : PS_FOG_COLOR;
	float fogIntensity : PS_FOG_INTENSITY;

#ifndef COLOR_BASED
    float2 uv : TEXCOORD_0;
#endif

#ifdef USE_VERTEX_COLORS
	float4 color : COLOR;
#endif

#ifdef USE_OVERLAY
    float4 overlayColor : PS_OVERLAY_COLOR;
#endif

#ifdef GLINT
	// this will implode on D3D9, since idk what UV_1 actually maps to semantic-wise
	float2 layer1UV : UV_1;
	float2 layer2UV : UV_2;
#endif
};
