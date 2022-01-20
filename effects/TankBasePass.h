
		VertexShader = NULL;
		
		AlphaBlendEnable = false;
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
        DestBlend = Zero;

        MaterialAmbient = (materialAmbient); 
        MaterialDiffuse = (materialDiffuse); 
//MaterialAmbient = (float4(0.0, 0.0, 0.0, 0.0));
//MaterialDiffuse = (float4(0.0, 0.0, 0.0, 0.2));
        MaterialSpecular = (materialSpecular); 
        MaterialEmissive = (materialEmissive);
        MaterialPower = (materialPower);

		AmbientMaterialSource = Material;
		DiffuseMaterialSource = Material;
		EmissiveMaterialSource = Material;
		
		ColorArg1[0] = Texture;
		ColorArg2[0] = Diffuse;
		ColorOp[0] = Modulate;
		
		AlphaArg1[0] = Texture;
		AlphaArg2[0] = Diffuse;
		AlphaOp[0] = Modulate;
						
		ColorOp[1] = Disable;
		AlphaOp[1] = Disable;

		Texture[0] = (baseTexture);
		TextureTransformFlags[0] = (baseTexTransFlags);
		TexCoordIndex[0] = (baseTexCoordIndex);
		TextureTransform[0] = (baseTransform);
		Sampler[0] = (MapSampler);
