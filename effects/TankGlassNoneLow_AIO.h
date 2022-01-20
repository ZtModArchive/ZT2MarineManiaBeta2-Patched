
////////////////
//placeholders
#ifndef Never
texture reflectTexture;
float4x4 reflectViewToUVW;
float4x4 kWorldToProj			: WorldToProj;
float4x4 kViewToWorld;
#endif //Never
////////////////


sampler sampleRefl = sampler_state
{
	texture = (reflectTexture);
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU	= WRAP;
	AddressV	= WRAP;
};

struct PsVertIn
{
	float4		mPosition	: POSITION;
	float4		mFresnel	: COLOR0;
	float		mFog		: FOG;
	float3		mUVRefl		: TEXCOORD2;
};

struct VsVertIn
{
	float4		mPosition	: POSITION;
	float3		mNormal		: NORMAL;
	float3		mUVScum		: TEXCOORD0;
	float2		mUVDetail	: TEXCOORD1;
};


float4 ps_waterAll( float4 fresnel : COLOR0,
                   float3 uvRefl : TEXCOORD2,
				   uniform float4 glassColor,
				   uniform float4 reflection ) : COLOR0
{
	// Modulate reflection map with reflection.rgb
	float4 refl = texCUBE( sampleRefl, uvRefl );
	refl.rgb *= reflection.rgb;
	// Add in the glass color (which is premultiplied with glass alpha)
	refl.rgb += fresnel.rgb * glassColor.rgb;
	// Alpha is glass alpha modulated by interpolated fresnel.
	refl.a = saturate( fresnel.a * glassColor.a );
	// premultiply in the alpha
	refl.rgb *= refl.a;

    // Lookup mesh texture and modulate it with diffuse
    return refl;
}

#include "texturetransform.h"
#include "shaderrig.h"
#include "fog.h"
#include "fresnel.h"


float3 EvalReflCoord(const float3 wPos, const float3 wNorm)
{
	float3 refl = wPos - kViewToWorld[3];
	return normalize(refl);
}

PsVertIn vs_waterAll(VsVertIn inVert)
{
	PsVertIn outVert;

	// Transform
	outVert.mFog = computeFog(inVert.mPosition.xyz);
	outVert.mPosition = mul(inVert.mPosition, kWorldToProj);

	// Evaluate fresnel term
	outVert.mFresnel = EvalFresnel(inVert.mNormal, kDefaultFresnelRange);

	outVert.mUVRefl = EvalReflCoord(inVert.mPosition.xyz, inVert.mNormal);

	return outVert;
}

