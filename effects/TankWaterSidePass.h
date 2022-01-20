

		AlphaBlendEnable = true;
//AlphaBlendEnable = false;
		AlphaTestEnable = false;
        AlphaFunc = greater;

		AlphaRef = 0;

		Lighting = true;
		SpecularEnable = false;
#ifdef FX_WALL_CW
		CullMode = 3;
#else // FX_WALL_CW
		CullMode = 2;
#endif // FX_WALL_CW
		
		ZFunc = LessEqual;
		ZWriteEnable=false;
		ZEnable=true;

        SrcBlend  = SrcAlpha;
        DestBlend = InvSrcAlpha;

        MaterialAmbient = float4(0.f, 0.f, 0.f, 1.f); 
        MaterialDiffuse = (kWaterColor);
//MaterialDiffuse = float4(0.f, 0.f, 0.f, 1.f);
        MaterialSpecular = float4(0.f, 0.f, 0.f, 1.f); 
        MaterialEmissive = (kWaterColor);
//MaterialEmissive = float4(1.f, 0.f, 0.f, 0.f);
        MaterialPower = 1.f;

		AmbientMaterialSource = Material;
		DiffuseMaterialSource = Material;
		EmissiveMaterialSource = Material;
		
		ColorArg1[0] = Diffuse;
		ColorArg2[0] = Diffuse;
		ColorOp[0] = SelectArg2;
		
		AlphaArg1[0] = Diffuse;
		AlphaArg2[0] = Diffuse;
		AlphaOp[0] = SelectArg2;
						
		ColorOp[1] = Disable;
		AlphaOp[1] = Disable;

		// We need to set these explicitly, because the device monitor
		// doesn't work between passes.
		TextureTransformFlags[0] = Disable;
		TextureTransformFlags[1] = Disable;
		TextureTransformFlags[2] = Disable;
		TextureTransformFlags[3] = Disable;
		TextureTransformFlags[4] = Disable;
		TextureTransformFlags[5] = Disable;
		TextureTransformFlags[6] = Disable;
		TextureTransformFlags[7] = Disable;
		
		TexCoordIndex[0] = 0;
		TexCoordIndex[1] = 1;
		TexCoordIndex[2] = 2;
		TexCoordIndex[3] = 3;
		TexCoordIndex[4] = 4;
		TexCoordIndex[5] = 5;
		TexCoordIndex[6] = 6;
		TexCoordIndex[7] = 7;
