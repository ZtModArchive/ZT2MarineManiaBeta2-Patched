
		VertexShader = NULL;

		AlphaBlendEnable = true;
//AlphaBlendEnable = false;
		AlphaTestEnable = true;
        AlphaFunc = greaterEqual;

		AlphaRef = 20;

		Lighting = true;
		SpecularEnable = false;
		CullMode = 1;
		ZFunc = LessEqual;
		ZWriteEnable=false;
//ZWriteEnable=true;
		ZEnable=true;

        SrcBlend  = SrcAlpha;
        DestBlend = InvSrcAlpha;

        MaterialAmbient = (materialAmbient); 
#ifdef FX_WALL_CW
        MaterialDiffuse = float4(1.f, 1.f, 1.f, 1.f - (minScum + (maxScum - minScum) * dirtinessCW));
#else // FX_WALL_CW
        MaterialDiffuse = float4(1.f, 1.f, 1.f, 1.f - (minScum + (maxScum - minScum) * dirtiness));
#endif // FX_WALL_CW
//MaterialAmbient = (float4(0.0, 0.0, 0.0, 0.0));
//MaterialDiffuse = (float4(0.1, 0.1, 0.1, 0.1));
        MaterialSpecular = (materialSpecular); 
        MaterialEmissive = (materialEmissive);
        MaterialPower = (materialPower);

		AmbientMaterialSource = (ambientMaterialSource);
		DiffuseMaterialSource = (diffuseMaterialSource);
		EmissiveMaterialSource = (emissiveMaterialSource);
		
		ColorArg1[0] = Texture;
		ColorArg2[0] = Diffuse;
		ColorOp[0] = Modulate;
//ColorOp[0] = SelectArg2;
		
		ColorArg1[1] = Texture;
		ColorArg2[1] = Current;
		ColorOp[1] = Modulate;
//ColorOp[1] = SelectArg1;
		
		ColorOp[2] = Disable;

		AlphaArg1[0] = Texture;
		AlphaArg2[0] = Diffuse;
		AlphaOp[0] = Subtract;
		
		AlphaArg1[1] = Texture;
		AlphaArg2[1] = Current;
		AlphaOp[1] = SelectArg2;
		
		AlphaOp[2] = Disable;

#ifdef FX_WALL_CW
		Texture[0] = (scumTextureCW);
#else // FX_WALL_CW
		Texture[0] = (scumTexture);
#endif // FX_WALL_CW
		TextureTransformFlags[0] = Count2;
		TexCoordIndex[0] = 0;
		TextureTransform[0] = (scumTransform);
		Sampler[0] = (MapSampler);

#ifdef FX_WALL_CW
		Texture[1] = (scumDetailCW);
#else // FX_WALL_CW
		Texture[1] = (scumDetail);
#endif // FX_WALL_CW
		TextureTransformFlags[1] = Count2;
		TexCoordIndex[1] = 1;
		TextureTransform[1] = (scumDetailTransform);
		Sampler[1] = (MapSampler);
