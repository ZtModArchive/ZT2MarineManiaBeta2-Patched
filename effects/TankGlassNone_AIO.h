
////////////////
//placeholders
#ifndef Never
texture sideTexture;
texture scumTexture;
texture scumDetail;
texture reflectTexture;
float4x4 scumTransform;
float4x4 scumDetailTransform;
float4x4 reflectViewToUVW;
float4x4 kWorldToProj			: WorldToProj;
float4x4 kViewToWorld;
#endif //Never
////////////////


sampler sampleScum = sampler_state
{
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU	= WRAP;
	AddressV	= WRAP;
};

sampler sampleDetail = sampler_state
{
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU	= WRAP;
	AddressV	= WRAP;
};

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
	float3		mUVRefl		: TEXCOORD0;
};

struct VsVertIn
{
	float4		mPosition	: POSITION;
	float3		mNormal		: NORMAL;
};

float4 ps_NoneAll( float4 fresnel : COLOR0,
                   float3 uvRefl : TEXCOORD0,
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

	return refl;
}

#include "watersurface.h"
#include "texturetransform.h"
#include "shaderrig.h"


float4 EvalFresnel(const float3 normal)
{
	float4 fresnel;
	
	fresnel.w = abs(dot(normal, kViewToWorld[2]));

	const float2 kFresAt = float2(1.f, 0.25f);

	fresnel.w = lerp(kFresAt.x, kFresAt.y, fresnel.w);

	fresnel.xyz = kLightRig[kOffsetToAmbient];

	return saturate(fresnel);
}

float3 EvalReflCoord(const float3 wPos, const float3 wNorm)
{
	float3 refl = wPos - kViewToWorld[3];
	return normalize(refl);
}

#include "texturetransform.h"
#include "shaderrig.h"
#include "fog.h"

PsVertIn vs_NoneAll(VsVertIn inVert)
{
	PsVertIn outVert;

	// Transform
	outVert.mFog = computeFog(inVert.mPosition.xyz);
	outVert.mPosition = mul(inVert.mPosition, kWorldToProj);

	// Evaluate fresnel term
	outVert.mFresnel = EvalFresnel(inVert.mNormal);

	outVert.mUVRefl = EvalReflCoord(inVert.mPosition.xyz, inVert.mNormal);

	return outVert;
}

