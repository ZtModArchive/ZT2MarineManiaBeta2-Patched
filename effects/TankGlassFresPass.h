
		VertexShader = NULL;
		
		AlphaBlendEnable = true;
		AlphaTestEnable = true;
        AlphaFunc = greater;

		AlphaRef = 0;

		Lighting = true;
		SpecularEnable = false;
		CullMode = 1;
		ZFunc = LessEqual;
		ZWriteEnable=true;
		ZEnable=true;

        SrcBlend  = SrcAlpha;
        DestBlend = InvSrcAlpha;

        MaterialAmbient = (materialAmbient); 
        MaterialDiffuse = (materialDiffuse); 
//MaterialDiffuse = (float4(0.0, 0.0, 0.0, 1.0));
        MaterialSpecular = (materialSpecular); 
        MaterialEmissive = (materialEmissive);
        MaterialPower = (materialPower);

		AmbientMaterialSource = (ambientMaterialSource);
		DiffuseMaterialSource = (diffuseMaterialSource);
		EmissiveMaterialSource = (emissiveMaterialSource);
		
		ColorArg1[0] = Texture;
		ColorArg2[0] = Diffuse;
		ColorOp[0] = Modulate;
		
		AlphaArg1[0] = Texture;
		AlphaArg2[0] = Diffuse;
		AlphaOp[0] = Modulate;
		
		ColorArg1[1] = Texture;
		ColorArg2[1] = Current;
		ColorOp[1] = SelectArg2;
//ColorOp[1] = SelectArg1;
//ColorOp[1] = Disable;
		
		AlphaArg1[1] = Texture | Complement;
		AlphaArg2[1] = Current;
		AlphaOp[1] = Modulate;
//AlphaOp[1] = SelectArg2;
//AlphaOp[1] = Disable;
				
		ColorOp[2] = Disable;
		AlphaOp[2] = Disable;

		Texture[0] = (glassTexture);
		TextureTransformFlags[0] = Count2;
		TexCoordIndex[0] = 1;
		TextureTransform[0] = (glassTransform);
		Sampler[0] = (MapSampler);

		Texture[1] = (lutTexture);
		TextureTransformFlags[1] = Count3;
		TexCoordIndex[1] = CameraSpaceNormal;
		TextureTransform[1] = (lutScaleOffset);
		Sampler[1] = (LutSampler);

