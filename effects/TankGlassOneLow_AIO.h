
////////////////
//placeholders
#ifndef Never
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
	float4		mDirtiness	: COLOR1;
	float		mFog		: FOG;
	float2		mUVScum		: TEXCOORD0;
	float2		mUVDetail	: TEXCOORD1;
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
				   float4 dirtiness : COLOR1,
                   float2 uvScum : TEXCOORD0,
                   float2 uvDetail : TEXCOORD1,
                   float3 uvRefl : TEXCOORD2,
				   uniform float4 glassColor,
				   uniform float4 reflection ) : COLOR0
{
	// Scum color is scum.rgb * detail.rgb
	// Scum alpha is just scum.a
	float4 scum = tex2D( sampleScum, uvScum );
	scum.a = saturate( scum.a * dirtiness.a );
	scum.rgb *= tex2D( sampleDetail, uvDetail );

	float4 total;
	total.rgb = scum.rgb * scum.a;

	// Modulate reflection map with reflection.rgb
	float4 refl = texCUBE( sampleRefl, uvRefl );
	refl.rgb *= reflection.rgb;
	// Add in the glass color (which is premultiplied with glass alpha)
	refl.rgb += fresnel.rgb * glassColor.rgb;
	// Alpha is glass alpha modulated by interpolated fresnel.
	refl.a = saturate( fresnel.a * glassColor.a );
	// Interpolate in the glass/reflection into the scum
	total.rgb = lerp( total.rgb, refl.rgb, refl.a );
	// Put in the inverted glass/refl/fresnel alpha
	total.a = (1 - scum.a) * (1 - refl.a);

    // Lookup mesh texture and modulate it with diffuse
    return total;
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

PsVertIn vs_waterAll(VsVertIn inVert, uniform float dirtiness)
{
	PsVertIn outVert;

	// Transform
	outVert.mFog = computeFog(inVert.mPosition.xyz);
	outVert.mPosition = mul(inVert.mPosition, kWorldToProj);

	// Evaluate fresnel term
	outVert.mFresnel = EvalFresnel(inVert.mNormal, kDefaultFresnelRange);
	outVert.mDirtiness = float4(dirtiness, dirtiness, dirtiness, dirtiness);

	// Transform texture coords for scum, reflect
	outVert.mUVScum = uvTransform(inVert.mUVScum, scumTransform);
	outVert.mUVDetail = uvTransform(inVert.mUVDetail, scumDetailTransform);

	outVert.mUVRefl = EvalReflCoord(inVert.mPosition.xyz, inVert.mNormal);

	return outVert;
}

// Lessons so far
//		Lose the glass texture and use a constant color instead
//		We only need one scum detail, not two
//		We can live without the reflection map on the wall between two tanks (TwoWater)
//		BUT
//		It's still looking improbable at best that we can do the two water version in 
//			a single pass. If we get the one water version down to a single pass, we
//			might get away with the two water as doing the one water twice.
