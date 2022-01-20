
////////////////
//placeholders
#ifndef Never
#if 0
float4 kPosX;
float4 kPosY;
float4 kDists;
float4 kAmplitude;
float4 kFrequency;
float4 kPhase;
float4 kRampOffset;
float4 kRampScale;
float4 kK;
float4 kDepthOffset;
#endif // 0
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


sampler sampleSide = sampler_state
{
	texture = (sideTexture);
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;
	MipMapLODBias = -15;

	AddressU	= CLAMP;
	AddressV	= CLAMP;
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
	float2		mUVSide		: TEXCOORD0;
	float2		mUVScum		: TEXCOORD1;
	float2		mUVDetail	: TEXCOORD2;
	float3		mUVRefl		: TEXCOORD3;
};

struct VsVertIn
{
	float4		mPosition	: POSITION;
	float3		mNormal		: NORMAL;
	float3		mUVScum		: TEXCOORD0;
	float2		mUVDetail	: TEXCOORD1;
};

/*
// Pixel shader
//{
PixelShader ps_TestAsm =
			asm
			{
				ps_1_2

def c0, 0.6, 1.0, 0.9, 0.75; // r,g,b,a glass color
def c1, 0.5, 0.0, 0.0, 0.5; // r,g,b, a; on color for side (alpha premultiplied)
def c2, 0.f, 0.f, 0.f, 0.25; // x,x,x, a; dirtiness
def c3, 0.4, 0.6, 0.8, 0.0; // r,g,b, x; material specular (for refl map)

tex t0; // side texture
tex t1; // scum texture
tex t2; // scum detail texture
tex t3; // reflection map
	// Side
	// W coord of last UV coord goes from 0 at base to 0.5 at top,
	// and uses cnd r0, uv.w, oncolor, offcolor
	// DEF c1 is constant side color premultiplied rgb*a
	// DEF t0 is side texture
	// INST mul r0, t0, c1;
	// r0.rgb is sideColor.rgb
	// r0.a is sideTex.a * sideColor.a
mul_sat r0.a, t0, c1;
	// ;; alpha = r0.a

	// Scum
	// DEF c2.a is dirtiness constant
	// DEF t0 is scum texture
	// DEF t1 is scum detail
	// Scum.a = t1.a * c2.a
	// INST mul t1.a, t1.a, c2.a;
mul_sat t1.a, t1.a, c2.a;
	// DEF scum.rgb is scumtexture.rgb * scumdetail.rgb
mul	t1.rgb, t1, t2;
	// color = side.rgb*side.a * (1-scum.a) + scum.rgb * scum.a
	// that's a lrp from side.rgb to scum.rgb by scum.a
	// INST lrp r0.rgb, scum.aaa, scum.rgb, side.rgb
lrp r0.rgb, t1.a, t1, c1;
	// We want to accumulate (1-a0)*(1-a1)..., not a0*a1.
	// We'll start the inversion here.
	// +INST mul r0.a, 1-r0.a, 1-scum.a
+mul r0.a, 1-r0.a, 1-t1.a;

	// Glass and reflect
	// Glass color = (glass * glass.a + reflect) * fresnel
	// DEF t3 is reflection texture
	// DEF v0.a is fresnel term
	// DEF c3.rgb is material specular (reflection map strength)
	// INST mul t3.rgb, t3.rgb, c3.rgb
mul t3.rgb, t3, c3;
	// DEF r1.a is glass alpha * fresnel
	// +INST mul r1.a, c0.a, v0.a
+mul_sat r1.a, c0.a, v0.a;
	// premultiply glass.rgb * glass.a and add reflection map
	// to get glassReflFres.rgb
	// INST r1.rgb, c0, t3;
add r1.rgb, c0, t3;
	// color = sidescum * (1-glassReflFres.a) + glassReflFres.rgb * glassReflFres.a;
	// INST lrp r0.rgb, r1.a, r1.rgb, r0.rgb;
lrp r0.rgb, r1.a, r1, r0;
	// alpha is sidescum.a * (1-glassFres.a)
	// +INST mul r0.a, r0.a, 1-r1.a;
+mul r0.a, r0.a, 1-r1.a;
//}

};
*/

float4 ps_waterAll( float4 fresnel : COLOR0,
				   float4 dirtiness : COLOR1,
                   float2 uvSide : TEXCOORD0,
                   float2 uvScum : TEXCOORD1,
                   float2 uvDetail : TEXCOORD2,
                   float3 uvRefl : TEXCOORD3,
				   uniform float4 glassColor,
				   uniform float4 sideColor,
				   uniform float4 reflection ) : COLOR0
{
	float4 side = tex2D( sampleSide, uvSide );
	// We don't actually need the side.rgb, side color is just sideColor.rgb
	// But we do need the product of side.a * sideColor.a where:
	//	side.a is texture to draw line between below and above water surface
	//	sideColor.a is max opacity of the water.
	side.a = saturate(side.a * sideColor.a);

	// Scum color is scum.rgb * detail.rgb
	// Scum alpha is just scum.a
	float4 scum = tex2D( sampleScum, uvScum );
	scum.a = saturate( scum.a * dirtiness.a );
//scum.a = 1.f;
//float4 scum;
//scum.rgb = tex2D( sampleScum, uvScum );
//scum.a = dirtiness.a;
	scum.rgb *= tex2D( sampleDetail, uvDetail );
//scum = tex2D( sampleDetail, uvDetail );
	side.rgb = lerp( sideColor.rgb, scum.rgb, scum.a );
//side.rgb = lerp( sideColor.rgb, scum.aaa, scum.a );
	// We need to get these inverted (because it's premultiplied),
	// so this is a good (free) time to invert both.
	side.a = (1 - side.a) * (1 - scum.a);

	// Modulate reflection map with reflection.rgb
	float4 refl = texCUBE( sampleRefl, uvRefl );
	refl.rgb *= reflection.rgb;
	// Add in the glass color (which is premultiplied with glass alpha)
	refl.rgb += fresnel.rgb * glassColor.rgb;
	// Alpha is glass alpha modulated by interpolated fresnel.
	refl.a = saturate( fresnel.a * glassColor.a );
	// Interpolate in the glass/reflection into the side/scum
	side.rgb = lerp( side.rgb, refl.rgb, refl.a );
	// Put in the inverted glass/refl/fresnel alpha
//	side.a *= (1-refl.a);
	side.a = 1 - side.a * (1 - refl.a);

    // Lookup mesh texture and modulate it with diffuse
    return side;
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

float2 EvalSideCoord(const float4 wPos, const float bottom, 
					 const float4 kPosX, const float4 kPosY, 
					 const float4 kDepthOffset,
					 const float4 kAmplitude,
					 const float4 kFrequency,
					 const float4 kPhase,
					 const float4 kRampOffset,
					 const float4 kRampScale,
					 const float4 kK)
{
	float4 kDirX = wPos.xxxx - kPosX;
	float4 kDirY = wPos.yyyy - kPosY;
	float4 kDirSq = kDirX * kDirX + kDirY * kDirY;
	float4 kInvDir = rsqrt(kDirSq);
	kDirX *= kInvDir;
	kDirY *= kInvDir;
	float4 kDists = 1.f / kInvDir;

	float4 sines;
	float4 cosines;
	CalcSinCos(kDists, 
		kAmplitude, 
		kFrequency, 
		kPhase, 
		kRampOffset, kRampScale,
		1.f, 
		sines, cosines);
		
	float4 kDirXK = kDirX * kK;
	float4 kDirYK = kDirY * kK;

	float waterHeight = CalcFinalPosition(wPos, sines, cosines, kDepthOffset.w, kDirXK, kDirYK).z;


	waterHeight -= bottom;

	waterHeight = waterHeight > 1.e-2f ? 0.5f * (wPos.z - bottom) / waterHeight : 1.f;

//return float2(waterHeight, minHeight);
	return float2(0.f, waterHeight);
//return float2(0.f, 1.f);
}

float3 EvalReflCoord(const float3 wPos, const float3 wNorm)
{
	float3 refl = wPos - kViewToWorld[3];
	return normalize(refl);
}

PsVertIn vs_waterAll(VsVertIn inVert, uniform float dirtiness,
					 uniform const float4 kPosX, uniform const float4 kPosY, 
					 uniform const float4 kDepthOffset,
					 uniform const float4 kAmplitude,
					 uniform const float4 kFrequency,
					 uniform const float4 kPhase,
					 uniform const float4 kRampOffset,
					 uniform const float4 kRampScale,
					 uniform const float4 kK)
{
	PsVertIn outVert;

	// Transform
	CalcScreenPosAndFog(kWorldToProj, inVert.mPosition, outVert.mPosition, outVert.mFog.x);

	// Evaluate fresnel term
	outVert.mFresnel = EvalFresnel(inVert.mNormal);
	outVert.mDirtiness = float4(dirtiness, dirtiness, dirtiness, dirtiness);

	// Evaluate water height uvcoord
	outVert.mUVSide = EvalSideCoord(inVert.mPosition, inVert.mUVScum.z,
									 kPosX, kPosY, 
									 kDepthOffset,
									 kAmplitude,
									 kFrequency,
									 kPhase,
									 kRampOffset,
									 kRampScale,
									 kK);

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
