
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
texture scumTextureCW			: TextureScumCW;
texture scumDetailCW			: DetailScumCW;


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
#define MF_SCUM_PASS_CW
//#define MF_SIDE_PASS
//#define MF_SIDE_PASS_CW
//#define MF_SOLID_PASS
//#define MF_WIREFRAME_PASS

float minScum					: MinScum = 0.f;
float maxScum					: MaxScum = 1.f;
float dirtiness					: DirtinessCCW = 0.5f;
float dirtinessCW				: DirtinessCW = 0.5f;

//#include "waterside.h"
// #include "watersideparams.h"
float4 kFrequency				: FrequencyCCW;
float4 kPhase					: PhaseCCW;
float4 kAmplitude				: AmplitudeCCW;
float4 kPosX					: PosXCCW;
float4 kPosY					: PosYCCW;
float4 kDepthOffset				: DepthOffsetCCW; // water level is w component
float4 kDepthScale				: DepthScaleCCW;
float4 kRampOffset				: RampOffsetCCW;
float4 kRampScale				: RampScaleCCW;
float4 kK						: KCCW;
float4 kWaterColor				: WaterColorCCW = float4(0.2f, 0.45f, 0.6f, 0.5f);

float4 kFrequencyCW				: FrequencyCW;
float4 kPhaseCW					: PhaseCW;
float4 kAmplitudeCW				: AmplitudeCW;
float4 kPosXCW					: PosXCW;
float4 kPosYCW					: PosYCW;
float4 kDepthOffsetCW			: DepthOffsetCW; // water level is w component
float4 kDepthScaleCW			: DepthScaleCW;
float4 kRampOffsetCW			: RampOffsetCW;
float4 kRampScaleCW				: RampScaleCW;
float4 kKCW						: KCW;
float4 kWaterColorCW			: WaterColorCW = float4(0.2f, 0.45f, 0.6f, 0.5f);

DWORD	kCameraContained		: CameraContainedCCW = false;
DWORD	kCameraContainedCW		: CameraContainedCW = false;

#include "TankOpaqueTwo_AIO.h"

technique TAll
<
	float Quality=1.1;
>
{
	pass AllPass
	{
		VertexShader = compile vs_1_1 vs_OpaqueTwo(dirtiness, dirtinessCW);
		
		PixelShader = compile ps_1_1 ps_OpaqueTwo();

		Texture[0] = (baseTexture);
		Texture[1] = (scumTexture);
		Texture[2] = (scumDetail);
	
		AlphaBlendEnable = false;
		AlphaTestEnable = false;

		Lighting = true;
		CullMode = 1;
		
		ZFunc = LessEqual;
		ZWriteEnable=true;
		ZEnable=true;
	}
}

#if 0 // T1
technique T1
<
	float Quality=1.0;
>
{

#ifdef MF_SIDE_PASS
	pass SidePass
	{
		VertexShader = compile vs_1_1 vs_waterSide(kWorldToProj,
													kWaterColor,
													kFrequency,
													kPhase,
													kAmplitude,
													kPosX,
													kPosY,
													kDepthOffset,
													kRampOffset,
													kRampScale,
													kK);

	#include "TankWaterSidePass.h"
	CullMode = 3;
	}
#endif // MF_SIDE_PASS

#ifdef MF_SIDE_PASS_CW
#define FX_WALL_CW
	pass SidePass
	{
		VertexShader = compile vs_1_1 vs_waterSide(kWorldToProj,
													kWaterColorCW,
													kFrequencyCW,
													kPhaseCW,
													kAmplitudeCW,
													kPosXCW,
													kPosYCW,
													kDepthOffsetCW,
													kRampOffsetCW,
													kRampScaleCW,
													kKCW);

	#include "TankWaterSidePass.h"
	CullMode = 2;
	}
#undef FX_WALL_CW
#endif // MF_SIDE_PASS_CW

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

#ifdef MF_SCUM_PASS_CW
#define FX_WALL_CW
	pass ScumPass
	{
	#include "TankScumPass.h"
	CullMode = 2;
	}	
#undef FX_WALL_CW
#endif // MF_SCUM_PASS_CW

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

#endif // T1

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

#ifdef MF_SCUM_PASS
	pass ScumPass
	{
	#include "TankScumPass.h"
	CullMode = 3;
	}	
#endif // MF_SCUM_PASS

#ifdef MF_SCUM_PASS_CW
#define FX_WALL_CW
	pass ScumPass
	{
	#include "TankScumPass.h"
	CullMode = 2;
	}	
#undef FX_WALL_CW
#endif // MF_SCUM_PASS_CW
}
