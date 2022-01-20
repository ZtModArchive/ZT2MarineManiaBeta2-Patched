
#define LessEqual 4
#define Never 1


bool fogEnable					: FogEnable =	true;
dword cullMode					: CullMode = 2;

bool AlphaSort = true;

texture reflectTexture			: TextureReflect;
texture glassTexture			: TextureGlass;
texture scumTexture				: TextureScumCCW;
texture scumDetail				: DetailScumCCW;
texture sideTexture				: TextureSide;

float4x4 kModelToWorld			: ModelToWorld;
float4x4 kWorldToProj			: WorldToProj;
float4x4 kWorldViewProj			: ModelToProj;
float4x4 kViewToWorld			: ViewToWorld;


float4 materialAmbient			: MaterialAmbient =			{ 1.0f, 1.0f, 1.0f, 1.0f };
float4 materialDiffuse			: MaterialDiffuse =			{ 1.0f, 1.0f, 1.0f, 1.0f };
float4 materialEmissive			: MaterialEmissive =		{ 0.0f, 0.0f, 0.0f, 0.0f };
float4 materialSpecular			: MaterialSpecular =		{ 0.0f, 0.0f, 0.0f, 0.0f };
float materialPower				: MaterialPower =			1.0f;


float4x4 glassTransform : GlassTransform = float4x4( 1.0, 0.0, 0.0, 0.0, 
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

float4x4 reflectViewToUVW				= float4x4( 1.0, 0.0, 0.0, 0.0, 
													0.0, 1.0, 0.0, 0.0, 
													0.0, 0.0, 1.0, 0.0, 
													0.0, 0.0, 0.0, 1.0  );

float4x4 invertTransform				= float4x4( -1.0, 0.0, 0.0, 0.0, 
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

// All our LUT specifics
sampler LutSampler = sampler_state
{
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = POINT;

	AddressU	= CLAMP;
	AddressV	= CLAMP;
};

texture lutTexture				: TextureLUT;
dword lutTexCoordIndex			: TexCoordIndexLUT = 1;

float4x4 vecLocalToWorld		: VecToWorld;
float4x4 vecWorldToView			: VecWorldToView;

float4x4 lutScaleOffset : LUTScaleOffset = float4x4( 0.0, 0.0, 0.0, 0.0, 
													0.0, 1.0, 0.0, 0.0, 
													0.5, 0.0, 1.0, 0.0, 
													0.5, 0.0, 0.0, 1.0  );

// End LUT specifics

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FIXED FUNCTION TECHNIQUE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MF_SIDE_PASS
#define MF_SCUM_PASS
//#define MF_GLASS_PASS
#define MF_GLASS_FRES_PASS
#define MF_REFLECT_PASS
//#define MF_SOLID_PASS
//#define MF_WIREFRAME_PASS

float minScum					: MinScum = 0.f;
float maxScum					: MaxScum = 1.f;
float dirtiness					: DirtinessCCW = 0.5f;

#include "waterside.h"
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

DWORD	kCameraContained		: CameraContainedCCW = false;

#include "TankGlassOne_AIO.h"

PixelShader comp_ps_waterAll = compile ps_1_1 ps_waterAll(
											materialAmbient,	// Glass Color
											kWaterColor,		// Side Color
											materialSpecular	// Reflection color.rgb
											);

technique T2
<
	float Quality = 1.1; bool NoRender = false;
>
{
	pass AllPass
	{
		VertexShader = compile vs_1_1 vs_waterAll(dirtiness,
					 kPosX, kPosY, 
					 kDepthOffset,
					 kAmplitude,
					 kFrequency,
					 kPhase,
					 kRampOffset,
					 kRampScale,
					 kK);
					 
		PixelShader = (comp_ps_waterAll);

		Texture[1] = (scumTexture);
		Texture[2] = (scumDetail);
	
		AlphaBlendEnable = true;
//AlphaBlendEnable = false;
		AlphaTestEnable = false;
        AlphaFunc = greater;

		AlphaRef = 0;
		
		Lighting = true;
		SpecularEnable = false;
		CullMode = 1;
		
		ZFunc = LessEqual;
		ZWriteEnable=true;
		ZEnable=true;

        SrcBlend  = One;
        DestBlend = InvSrcAlpha;
	}

#ifdef MF_WIREFRAME_PASS
	pass WireFramePass
	{
	#include "TankDbgWirePass.h"
	CullMode = 2;
	}
#endif // MF_WIREFRAME_PASS

}

#if 1
technique T1
<
	float Quality=1.0; bool NoRender=false;
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
	CullMode = (kCameraContained ? 1 : 2);
	}
#endif // MF_SIDE_PASS

#ifdef MF_SCUM_PASS
	pass ScumPass
	{
	#include "TankScumPass.h"
	CullMode = (kCameraContained ? 1 : 2);
	}	
#endif // MF_SCUM_PASS

#ifdef MF_GLASS_FRES_PASS
	pass GlassFresPass
	{
	#include "TankGlassFresPass.h"
	}
#endif // MF_GLASS_FRES_PASS

#ifdef MF_GLASS_PASS
	pass GlassPass
	{
	#include "TankGlassPass.h"
	}
#endif // MF_GLASS_PASS


#ifdef MF_REFLECT_PASS
    pass ReflectPass
    {
    #include "TankReflectPass.h"
    }
#endif // MF_REFLECT_PASS

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
	CullMode = 3;
	}
#endif // MF_WIREFRAME_PASS
}

#endif

technique T0
<
	float Quality=0.0;
>
{

	pass GlassFresPass
	{	
	#include "TankGlassFresPass.h"
	}

	pass ScumPass
	{
	#include "TankScumPass.h"
	}
}
