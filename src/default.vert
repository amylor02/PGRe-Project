#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 aNormal;
// Colors
layout (location = 2) in vec3 aColor;
// Texture Coordinates
layout (location = 3) in vec2 aTex;

uniform vec3 camPos;

// Outputs the current position for the Fragment Shader
out vec3 crntPos;
// Outputs the normal for the Fragment Shader
out vec3 Normal;
// Outputs the color for the Fragment Shader
out vec3 color;
// Outputs the texture coordinates to the Fragment Shader
out vec2 texCoord;
// Outputs the fragment position of the light
out vec4 fragPosLight;


out DATA
{
    vec3 Normal;
	vec3 color;
	vec2 texCoord;
    mat4 projection;
	mat4 modelo;
	vec3 lightPos;
	vec3 camPos;
	vec4 fragPosLight;
	vec3 crntPos;
} data_out;


// Imports the camera matrix from the main function
uniform mat4 camMatrix;
// Imports the model matrix from the main function
uniform mat4 model;
// Imports the light matrix
uniform mat4 lightProjection;
uniform vec3 lightPos;
void main()
{
	// calculates current position
	crntPos = vec3(model * vec4(aPos, 1.0f));
	// Assigns the normal from the Vertex Data to "Normal"
	Normal = aNormal;
	// Assigns the colors from the Vertex Data to "color"
	color = aColor;
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	texCoord = aTex;
	// Calculates the position of the light fragment for the fragment shader
	fragPosLight = lightProjection * vec4(crntPos, 1.0f);
	// Outputs the positions/coordinates of all vertices
	gl_Position = vec4(crntPos, 1.0);
	data_out.Normal = aNormal;
	data_out.color = aColor;
	data_out.texCoord = aTex;
	data_out.projection = camMatrix;
	data_out.modelo = model;
	data_out.lightPos = lightPos;
	data_out.camPos = camPos;
	data_out.fragPosLight = fragPosLight;
	data_out.crntPos = crntPos;
}