#if 0
Shader "Test 001" 
{
    
	// test
	Properties
	{
		Color4f color=(10.15, 0.5, 1., 1100);   // abs
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


int a;
int b;
float4 a_color;

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VOut vs_main(float4 position : POSITION, float4 color : COLOR)
{
    VOut output;
	output.position = position;
	output.color = color;
	return output;
}


float4 ps_main(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
    return color;
}