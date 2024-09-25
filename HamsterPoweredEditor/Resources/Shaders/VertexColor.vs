
	#version 460 core

	layout (location = 0) in vec3 Position;
	layout (location = 1) in vec3 Color;
	layout (location = 2) in vec2 TexCoord;
	layout (location = 3) in vec3 Normal;
	layout (location = 4) in int TexIndex;

	out vec3 FragColor;
	out vec2 v_TexCoord;
	out vec3 v_Normal;
	out vec3 v_Position;
	out vec3 v_TransformedNormal;
	out int v_TexIndex;
	
	uniform mat4 u_ViewProjection;
	uniform mat4 u_Transform;

	void main()
	{
		mat4 MVP = u_ViewProjection * u_Transform;
		
		gl_Position = MVP * vec4(Position, 1.0f);
		FragColor = Color;
		v_TexCoord = TexCoord;
		v_Normal = Normal;
		v_TexIndex = TexIndex;

		v_TransformedNormal = mat3(transpose(inverse(u_Transform))) * Normal;
		
		v_Position = vec3(u_Transform * vec4(Position, 1.0f));
	}


