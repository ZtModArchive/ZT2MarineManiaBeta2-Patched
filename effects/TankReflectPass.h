
		VertexShader = NULL;
		
		// Clip/Raster state
		AlphaBlendEnable = true;
		AlphaTestEnable = false;
        AlphaFunc = greater;
        AlphaRef = 0;

        SrcBlend  = One;
        DestBlend = One;
//DestBlend = Zero;
		FogColor = 0;

		Lighting = true;
		SpecularEnable = false;
		CullMode = 1;
		ZFunc = LessEqual;
		ZWriteEnable=false;
//ZWriteEnable=false;
		ZEnable=true;
		
        MaterialAmbient = float4(0.f,0.f, 0.f, 0.f); 
        MaterialDiffuse = float4(0.f,0.f, 0.f, 0.f); 
//MaterialAmbient = (float4(0.0, 0.0, 0.0, 0.0));
//MaterialDiffuse = (float4(0.1, 0.1, 0.1, 0.1));
        MaterialEmissive = (materialSpecular);
        MaterialPower = (materialPower);
//MaterialEmissive = float4(0.f,0.f, 0.f, 0.f); 
		
		ColorArg1[0] = Texture;
		ColorArg2[0] = Diffuse;
		ColorOp[0] = Modulate;
//ColorOp[0] = SelectArg1;

		AlphaArg1[0] = Texture;
		AlphaArg2[0] = Diffuse;
		AlphaOp[0] = SelectArg1;
//AlphaOp[0] = SelectArg2;

		ColorArg1[1] = Texture | Complement;
		ColorArg2[1] = Current;
		ColorOp[1] = Modulate;
//ColorOp[1] = SelectArg1;
//ColorOp[1] = Disable;
		
		AlphaArg1[1] = Texture;
		AlphaArg2[1] = Current;
		AlphaOp[1] = SelectArg2;
//AlphaOp[1] = SelectArg1;
//AlphaOp[1] = Disable;
				
		ColorOp[2] = Disable;
		AlphaOp[2] = Disable;
		
		AmbientMaterialSource = (ambientMaterialSource);
		DiffuseMaterialSource = (diffuseMaterialSource);
		EmissiveMaterialSource = (emissiveMaterialSource);
		
		Texture[0] = (reflectTexture);
		TextureTransformFlags[0] = Count3;
		TexCoordIndex[0] = CameraSpaceReflectionVector;
		TextureTransform[0] = float4x4(kViewToWorld[0], kViewToWorld[1], kViewToWorld[2], float4(0.f, 0.f, 0.f, 1.f));
		Sampler[0] = (MapSampler);

		Texture[1] = (lutTexture);
		TextureTransformFlags[1] = Count3;
		TexCoordIndex[1] = CameraSpaceNormal;
		TextureTransform[1] = (lutScaleOffset);
		Sampler[1] = (LutSampler);

