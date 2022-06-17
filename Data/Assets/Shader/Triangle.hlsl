#if 0
Shader "Test 001" 
{
   
	// test
	Properties
	{
		_Color ("Color", Color4f) = (0.5, 0.1, 1);
		_Test2 ("Test2", Vec4f) = (0, 0, 0, 1);
		_Test ("Test", Float) = 0.5;
	}
	Pass
	{
		// Queue "Transparent"
		// Cull None
		
		VsFunc vs_main;
		PsFunc ps_main;
	
	}
	
}
#endif

float4 testColor;

struct VOut
{
	
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VOut vs_main(float4 position : POSITION, float4 color : COLOR)
{

    VOut output;
	output.position = position;
	output.color = color * testColor;
	return output;
}


float4 ps_main(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
    return color * testColor;
}