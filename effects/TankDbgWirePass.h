
		VertexShader = NULL;
		
		AlphaBlendEnable = false;
		AlphaTestEnable = false;
        AlphaFunc = greater;

		AlphaRef = 0;

		Lighting = true;
		SpecularEnable = false;
		CullMode = 2;
		ZFunc = LessEqual;
		ZWriteEnable=false;
		ZEnable=true;

        SrcBlend  = One;
        DestBlend = Zero;

        MaterialAmbient = float4(1.f, 0.f, 0.f, 1.f); 
        MaterialDiffuse = float4(1.f, 0.f, 0.f, 1.f); 
        MaterialSpecular = float4(1.f, 0.f, 0.f, 1.f); 
        MaterialEmissive = float4(1.f, 0.f, 0.f, 1.f);
        MaterialPower = 1.f;

		AmbientMaterialSource = Material;
		DiffuseMaterialSource = Material;
		EmissiveMaterialSource = Material;
		
		ColorArg1[0] = Texture;
		ColorArg2[0] = Diffuse;
		ColorOp[0] = SelectArg2;
		
		AlphaArg1[0] = Texture;
		AlphaArg2[0] = Diffuse;
		AlphaOp[0] = SelectArg2;
						
		ColorOp[1] = Disable;
		AlphaOp[1] = Disable;

		FillMode = WireFrame;

