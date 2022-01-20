
#define Never 1

bool fogEnable					: FogEnable =	true;
dword cullMode					: CullMode = 2;

bool AlphaSort = true;

texture reflectTexture : TextureReflect;
texture glassTexture : TextureGlass;

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

#include "TankGlassNone_AIO.h"

technique TAll
<
	float Quality=1.1;
>
{
	pass AllPass
	{
		VertexShader = compile vs_1_1 vs_NoneAll();
		
		PixelShader = compile ps_1_1 ps_NoneAll(materialAmbient, materialSpecular);

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
        DestBlend = SrcAlpha;
	}
}


//#define MF_GLASS_PASS
#define MF_GLASS_FRES_PASS
#define MF_REFLECT_PASS
//#define MF_SOLID_PASS
//#define MF_WIREFRAME_PASS

#if 1 // T1
technique T1
<
	float Quality=1.0;
>
{

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
	}

#endif // MF_WIREFRAME_PASS
}
#endif // T1

technique T0
<
	float Quality=0.0;
>
{
	pass GlassFresPass
	{
	#include "TankGlassFresPass.h"
	}

}
