#version 330 core

layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>

out vec2 TexCoords;

vec4 texPos;

uniform mat4 projection; // all rows and columns are 0 for some reason even tho it's set in TextRenderer.cpp

mat4 projection2 = mat4(0.0025,		0.0,		0.0,	0.0,  // 1. column // thats a way around to problem above
						0.0,		-0.00333,	0.0,	0.0,  // 2. column
						0.0,		0.0,		-1.0,	0.0,  // 3. column
						-1.0,		1.0,		0.0,	1.0); // 4. column

void main()
{
	texPos = vertex;
	TexCoords = vertex.zw;
    gl_Position = projection2 * vec4(vertex.xy, 0.0, 1.0);    
} 