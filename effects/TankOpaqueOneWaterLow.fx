
#define LessEqual 4
#define Never 1



bool fogEnable					: FogEnable =	true;
dword cullMode					: CullMode = 2;

bool AlphaSort = false;

texture baseTexture				: Texture0;
DWORD baseTexCoordIndex			: TexCoordIndex0 = 1;
DWORD baseTexTransFlags			: TextureTransformFlags0 = 2;

texture scumTexture				: TextureScumCCW;
texture scumDetail				: DetailScumCCW;

float4x4 kModelToWorld			: ModelToWorld;
float4x4 kWorldToProj			: WorldToProj;
float4x4 kWorldViewProj			: ModelToProj;
float4x4 kViewToWorld			: ViewToWorld;


float4 materialAmbient			: MaterialAmbient =			{ 1.0f, 1.0f, 1.0f, 1.0f };
float4 materialDiffuse			: MaterialDiffuse =			{ 1.0f, 1.0f, 1.0f, 1.0f };
float4 materialEmissive			: MaterialEmissive =		{ 0.0f, 0.0f, 0.0f, 0.0f };
float4 materialSpecular			: MaterialSpecular =		{ 0.0f, 0.0f, 0.0f, 0.0f };
float materialPower				: MaterialPower =			1.0f;


float4x4 baseTransform : TextureTransform0 = float4x4( 1.0, 0.0, 0.0, 0.0, 
													0.0, 1.0, 0.0, 0.0, 
													0.0, 0.0, 1.0, 0.0, 
													0.0, 0.0, 0.0, 1.0  );

float4x4 scumTransform : ScumTransform = float4x4( 1.0, 0.0, 0.0, 0.0, 
													0.0, 1.0, 0.0, 0.0, 
													0.0, 0.0, 1.0, 0.0, 
													0.0, 0.0, 0.0, 1.0  );

float4x4 scumDetailTransform : ScumDetailTransform = float4x4( 1.0, 0.0, 0.0, 0.0, 
													0.0, 1.0, 0.0, 0.0, 
													0.0, 0.0, 1.0, 0.0, 
													0.0, 0.0, 0.0, 1.0  );


dword diffuseMaterialSource			: DiffuseMaterialSource =    0;
dword ambientMaterialSource			: AmbientMaterialSource =    0;
dword emissiveMaterialSource		: EmissiveMaterialSource =   0;

sampler MapSampler = sampler_state
{
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	
	AddressU	= Wrap;
	AddressV	= Wrap;
};


// End LUT specifics

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FIXED FUNCTION TECHNIQUE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MF_BASE_PASS
#define MF_SCUM_PASS
//#define MF_SOLID_PASS
//#define MF_WIREFRAME_PASS

float minScum					: MinScum = 0.f;
float maxScum					: MaxScum = 1.f;
float dirtiness					: DirtinessCCW = 0.5f;

DWORD	kCameraContained		: CameraContainedCCW = false;

technique T1
<
	float Quality=1.0;
>
{

#ifdef MF_BASE_PASS
	pass BasePass
	{
	#include "TankBasePass.h"
	}
#endif // MF_BASE_PASS

#ifdef MF_SCUM_PASS
	pass ScumPass
	{
	#include "TankScumPass.h"
	CullMode = 3;
	}	
#endif // MF_SCUM_PASS

#ifdef MF_SOLID_PASS
	pass SolidPass
	{
	#include "TankDbgSolidPass.h"
	}
#endif // MF_SOLID_PASS

#ifdef MF_WIREFRAME_PASS
	pass WireFramePass
	{
	#include "TankDbgWirePass.h"
	}
#endif // MF_WIREFRAME_PASS
}

technique T0
<
	float Quality=0.0;
>
{

#ifdef MF_BASE_PASS
	pass BasePass
	{
	#include "TankBasePass.h"
	}
#endif // MF_BASE_PASS

#ifdef MF_SCUM_ON_LOW
#ifdef MF_SCUM_PASS
	pass ScumPass
	{
	#include "TankScumPass.h"
	CullMode = 3;
	}	
#endif // MF_SCUM_PASS
#endif // MF_SCUM_ON_LOW
}
