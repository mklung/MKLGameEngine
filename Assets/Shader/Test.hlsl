Shader "Test  02" 
{
    
	// test
	Properties
	{
		Color4f color=(1,1,1,1);   // abs
		String name = "";
	}
	Pass
	{
		// Queue "Transparent"
		// Cull None
		
		VsFunc vs_main;
		PsFunc ps_main;
	
	}
	
	Pass
	{
		VsFunc vs_main2;
		PsFunc ps_main2;
	}
	
}

