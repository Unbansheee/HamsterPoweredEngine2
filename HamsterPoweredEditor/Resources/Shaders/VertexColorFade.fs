#version 460 core

#include "Resources/Shaders/Common.glsl"

	void main()
	{
		FinalColor = vec4(FragColor, 1.0f) * abs(sin(CurrentTime));
	}


