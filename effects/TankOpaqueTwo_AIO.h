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
float4 materialDiffuse			: MaterialDiffuse =			{ 1.0f, 1.0f, 1.0f, 1.0f };
float4 materialEmissive			: MaterialEmissive =		{ 0.0f, 0.0f, 0.0f, 0.0f };
#endif //Never


sampler sampleBase = sampler_state
{
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU	= WRAP;
	AddressV	= WRAP;
};

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

struct PsVertIn
{
	float4		mPosition	: POSITION;
	float4		mDiffuseDirty	: COLOR0;
	float		mFog		: FOG;
	float2		mUVBase		: TEXCOORD0;
	float2		mUVScum		: TEXCOORD1;
	float2		mUVDetail	: TEXCOORD2;
};

struct VsVertIn
{
	float4		mPosition	: POSITION;
	float3		mNormal		: NORMAL;
	float3		mUVScum		: TEXCOORD0;
	float2		mUVDetail	: TEXCOORD1;
};

float4 ps_OpaqueTwo( float4 diffuseDirty : COLOR0,
                   float2 uvBase : TEXCOORD0,
                   float2 uvScum : TEXCOORD1,
                   float2 uvDetail : TEXCOORD2
						) : COLOR0
{
	float4 base = tex2D( sampleBase, uvBase );

	// Blend the scum onto it.

	float4 scum = tex2D( sampleScum, uvScum );
	scum.a = saturate( scum.a * diffuseDirty.a );
//scum.a = 1.f;
//float4 scum;
//scum.rgb = tex2D( sampleScum, uvScum );
//scum.a = dirtiness.a;
	scum.rgb *= tex2D( sampleDetail, uvDetail );
//scum = tex2D( sampleDetail, uvDetail );
	base.rgb = lerp( base.rgb, scum.rgb, scum.a );
//side.rgb = lerp( sideColor.rgb, scum.aaa, scum.a );
	base.rgb *= diffuseDirty;

    return base;
}

#include "texturetransform.h"
#include "shaderrig.h"
#include "fog.h"

PsVertIn vs_OpaqueTwo(VsVertIn inVert, uniform float dirtiness, uniform float dirtinessCW)
{
	PsVertIn outVert;

	outVert.mFog = computeFog(inVert.mPosition.xyz);
	outVert.mPosition = mul(inVert.mPosition, kWorldToProj);

	outVert.mUVBase = uvTransform(inVert.mUVDetail, baseTransform);
	outVert.mUVScum = uvTransform(inVert.mUVScum, scumTransform);
	outVert.mUVDetail = uvTransform(inVert.mUVDetail, scumDetailTransform);

	outVert.mDiffuseDirty.rgb = lightFromRig(inVert.mNormal, materialDiffuse, materialEmissive);
	outVert.mDiffuseDirty.a = max(dirtiness, dirtinessCW);


	return outVert;
}
