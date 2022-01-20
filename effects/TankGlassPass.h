
		VertexShader = NULL;
		
		AlphaBlendEnable = true;
		AlphaTestEnable = true;
        AlphaFunc = greater;

		AlphaRef = 0;

		Lighting = true;
		SpecularEnable = false;
		CullMode = 1;
		ZFunc = LessEqual;
		ZWriteEnable=false;
ZWriteEnable=true;
		ZEnable=true;

        SrcBlend  = SrcAlpha;
        DestBlend = InvSrcAlpha;

        MaterialAmbient = (materialAmbient); 
        MaterialDiffuse = (materialDiffuse); 
//MaterialAmbient = (float4(0.0, 0.0, 0.0, 0.0));
//MaterialDiffuse = (float4(0.0, 0.0, 0.0, 0.2));
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
						
		ColorOp[1] = Disable;
		AlphaOp[1] = Disable;

		Texture[0] = (glassTexture);
		TextureTransformFlags[0] = Count2;
		TexCoordIndex[0] = 1;
		TextureTransform[0] = (glassTransform);
		Sampler[0] = (MapSampler);


