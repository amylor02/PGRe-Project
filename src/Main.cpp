/*
Note: Uses a stack of 64mb in VS to be able to render the terrain & water.
*/


#include"Mesh.h"
#include"Model.h"
#include<ctime>
#include <cstdlib>
#include <glm/gtx/string_cast.hpp>

const unsigned int width = 1280;
const unsigned int height = 720;

float getRandom(float x) {
	float r1 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
	float r2;

	if (r1 < 0.1) {
		r2= static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 4));
	}
	else if (r1 < 0.4) {
		r2 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2));
	}
	else {
		r2 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1));
	}
	 
	//r2 -= x / 2;
	return r2;
}


Mesh getPlane(float f, bool water, int vertices, Texture cube) {

	Texture texturesPlane[]
	{ 
		Texture("grass.jpg", "diffuse", 0, GL_RGB, GL_UNSIGNED_BYTE),
		//Texture("grassNM2.jpg", "normal", 1, GL_RGB, GL_UNSIGNED_BYTE),
		//Texture("grassSpec.jpg", "specular", 1, GL_RGB, GL_UNSIGNED_BYTE),
	};

	Texture texturesWater[]
	{
		Texture("wotah4.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("waterSpec3.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE),
		cube
	};

	int width, height, nChannels;
	unsigned char* data = stbi_load("hmap2.jpg",
		&width, &height, &nChannels,
		0);


	std::vector<unsigned char> xs;
	std::vector<unsigned char> ys;
	std::vector<unsigned char> zs;
	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			// retrieve texel for (i,j) tex coord
			unsigned char* texel = data + (j + width * i) * nChannels;
			xs.push_back(-height / 2.0f + i);
			zs.push_back(-width / 2.0f + j);
			// raw height at coordinate
			ys.push_back(texel[0]);
		}
	}


	float a = -20;
	float b = 20;
	const int dimension = 512;
	const int half = dimension / 2;
	const int tex_def = 32;
	Vertex verticesPlane[dimension * dimension];



	int k = 0, row = 0, column = 0;
	if(water==false){
	for (int i = half * (-1); i < half; i++) {
		for (int j = half; j > half * (-1); j--) {
			float r = getRandom(5);

			//verticesPlane[k++] = Vertex{ glm::vec3((float)i, 20.f + sin(2 * a * f) * cos(b) * cos(b) * 3 ,  (float)j), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2((float)row / ((dimension / tex_def) - 1), (float)column / ((dimension / tex_def) - 1)) };
			//verticesPlane[k] = Vertex{ glm::vec3((float)i, (-20 + sin(a*a*0.008) * sin(b*b*0.008) * 90)* f ,  (float)j), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2((float)row / ((dimension / tex_def) - 1), (float)column / ((dimension / tex_def) - 1)) };
			verticesPlane[k] = Vertex{ glm::vec3((float)i, (float)ys[k]/5 - 25.f,  (float)j), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2((float)row / ((dimension / tex_def) - 1), (float)column / ((dimension / tex_def) - 1)) };
			column++;
			k++;
			b -= 0.08f;
		}
		column = 0;
		row++;
		a += 0.08f;
		b = 20;

	}
	}

	else {
		for (int i = half * (-1); i < half; i++) {
			for (int j = half; j > half * (-1); j--) {
				float r = getRandom(5);

				//verticesPlane[k++] = Vertex{ glm::vec3((float)i, 20.f + sin(2 * a * f) * cos(b) * cos(b) * 3 ,  (float)j), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2((float)row / ((dimension / tex_def) - 1), (float)column / ((dimension / tex_def) - 1)) };
				verticesPlane[k] = Vertex{ glm::vec3((float)i, (-20 + sin(a*a*0.008) * sin(b*b*0.008) * 90)* f ,  (float)j), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2((float)row / ((dimension / tex_def) - 1), (float)column / ((dimension / tex_def) - 1)) };
				//verticesPlane[k] = Vertex{ glm::vec3(i, ys[k],  j), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2((float)row / ((dimension / tex_def) - 1), (float)column / ((dimension / tex_def) - 1)) };
				column++;
				k++;
				b -= 0.08f;
			}
			column = 0;
			row++;
			a += 0.08f;
			b = 20.f;

		}

	}

	k = 0;
	const int x = ((dimension - 1) * (dimension - 1));
	GLuint indicesPlane[6 * x];
	for (int i = 0; i < (dimension * dimension) - (dimension + 1); i++) {
		if ((i + 1) % dimension == 0) {
			i++;
			//std::cout << "i=" << i << "\n";
		}
		indicesPlane[k] = i; indicesPlane[k + 1] = i + dimension; indicesPlane[k + 2] = i + dimension + 1;
		indicesPlane[k + 3] = i + dimension + 1; indicesPlane[k + 4] = i + 1; indicesPlane[k + 5] = i;
		//std::cout << "last wrote to k=" << k+5 << "\n";
		k += 6;
		//std::cout << "k=" << k<<"\n";
	}

	k = 0;
	for (int k = 0; k < ((dimension - 1) * (dimension - 1)) * 6; k = k) {
		int a = indicesPlane[k++];
		int b = indicesPlane[k++];
		int c = indicesPlane[k++];

		glm::vec3 v1 = verticesPlane[a].position, v2 = verticesPlane[b].position, v3 = verticesPlane[c].position;


		glm::vec3 u = glm::vec3(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
		glm::vec3 v = glm::vec3(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);

		glm::vec3 normal;

		normal.x = (u.y * v.z) - (u.z * v.y);
		normal.y = (u.z * v.x) - (u.x * v.z);
		normal.z = (u.x * v.y) - (u.y * v.x);

		float length = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

		normal.x /= length;
		normal.y /= length;
		normal.z /= length;

		verticesPlane[a].normal = normal;
		verticesPlane[b].normal = normal;
		verticesPlane[c].normal = normal;

	}




	//Shader shaderPlane("default.vert", "default.frag");
	// Store mesh data in vectors for the mesh
	std::vector <Vertex> vertsPlane(verticesPlane, verticesPlane + sizeof(verticesPlane) / sizeof(Vertex));
	std::vector <GLuint> indPlane(indicesPlane, indicesPlane + sizeof(indicesPlane) / sizeof(GLuint));
	if (water == true) {
		std::vector <Texture> texPlane(texturesWater, texturesWater + sizeof(texturesWater) / sizeof(Texture));

		Mesh plane(vertsPlane, indPlane, texPlane);
		return plane;
	}
	else {
		std::vector <Texture> texPlane(texturesPlane, texturesPlane + sizeof(texturesPlane) / sizeof(Texture));

		Mesh plane(vertsPlane, indPlane, texPlane);
		return plane;
	}
}

// Vertices coordinates
Vertex vertices[] =
{ //               COORDINATES           /            COLORS          /           NORMALS         /       texcords         //
	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(4.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};

// Vertices coordinates
Vertex vertices2[] =
{ //               COORDINATES           /            COLORS          /           TexCoord         /       NORMALS         //
	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
};

// Indices for vertices order
GLuint indices2[] =
{
	0, 1, 2,
	0, 2, 3
};


Vertex lightVertices[] =
{ //     COORDINATES     //
	Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

float skyboxVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//        7--------6
	 1.0f, -1.0f,  1.0f,//       /|       /|
	 1.0f, -1.0f, -1.0f,//      4--------5 |
	-1.0f, -1.0f, -1.0f,//      | |      | |
	-1.0f,  1.0f,  1.0f,//      | 3------|-2
	 1.0f,  1.0f,  1.0f,//      |/       |/
	 1.0f,  1.0f, -1.0f,//      0--------1
	-1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
};


int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 8);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Alexis Mylordos PGRe Project", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);





	Texture textures[]
	{
		Texture("bricks2.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("bricksNormal2.png", "normal", 1, GL_RGBA, GL_UNSIGNED_BYTE)
	};



	Texture textures2[]
	{
		Texture("bricks2.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		//Texture("brixSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE),
		Texture("bricksNormal2.png", "normal", 1,GL_RGBA, GL_UNSIGNED_BYTE)

	};







	glfwSwapInterval(1);


	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "normMap.frag", "normMap.geom");
	//Shader shaderProgram2("default.vert", "normMap.frag", "normMap.geom");
	Shader shaderProgram2("default.vert", "normMap.frag", "normMap.geom");
	// Store mesh data in vectors for the mesh
	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));

	std::vector <Vertex> verts2(vertices2, vertices2 + sizeof(vertices2) / sizeof(Vertex));
	std::vector <GLuint> ind2(indices2, indices2 + sizeof(indices2) / sizeof(GLuint));
	std::vector <Texture> tex2(textures2, textures2 + sizeof(textures2) / sizeof(Texture));
	// Create floor mesh
	Mesh floor(verts, ind, tex);
	Mesh floor2(verts2, ind2, tex2);


	// Shader for light cube
	Shader lightShader("light.vert", "light.frag");
	// Store mesh data in vectors for the mesh
	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	// Crate light mesh
	Mesh light(lightVerts, lightInd, tex);





	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 60.0f, -30.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 objectPos = glm::vec3(1.0f, 0.0f, 4.8f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);

	glm::vec3 objectPos2 = glm::vec3(0.0f, 14.0f, 0.5f);
	glm::mat4 objectModel2 = glm::mat4(1.0f);
	objectModel2 = glm::translate(objectModel2, objectPos2);

	lightModel = glm::translate(lightModel, glm::vec3(0.f, 0.f, 0.0f));
	lightModel = glm::rotate(lightModel, glm::radians(1.f), glm::vec3(1.0f, 0.0f, 0.0f));
	//lightModel = glm::translate(lightModel, glm::vec3(0.f, -5.f, 0.0f));

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	shaderProgram2.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram2.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel2));
	glUniform4f(glGetUniformLocation(shaderProgram2.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram2.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	//glUniform1i(glGetUniformLocation(shaderProgram2.ID, "normal0"), 2);

	Shader shaderProgram3("model.vert", "default.frag");

	// Take care of all the light related things

	shaderProgram3.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram3.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram3.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


	Model model("models/airplane/scene.gltf");


	/*SKYBOX*/
	Shader skyboxShader("skybox.vert", "skybox.frag");
	skyboxShader.Activate();
	glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);

	// Create VAO, VBO, and EBO for the skybox
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



	std::string facesCubemap[6] =
	{
		"skybox/right.jpg",
		"skybox/left.jpg",
		"skybox/top.jpg",
		"skybox/bottom.jpg",
		"skybox/front.jpg",
		"skybox/back.jpg"
	};
	//for reflections
	std::string facesCubemap2[6] =
	{
		"skybox/right2.jpg",
		"skybox/left2.jpg",
		"skybox/top2.jpg",
		"skybox/bottom.jpg",
		"skybox/back2.jpg",
		"skybox/front2.jpg"
	};

	// Creates the cubemap texture object
	unsigned int cubemapTexture;
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// These are very important to prevent seams
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Cycles through all the textures and attaches them to the cubemap object
	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
			stbi_image_free(data);
		}
	}

	Texture sky(facesCubemap2);

	/*SKYBOX END*/

	/*WATER*/

	//Texture mol("bricksNormal2.png", "cubemap", 1, GL_RGBA, GL_UNSIGNED_BYTE);

	Shader shaderWater("planeShader.vert", "water.frag");
	Mesh water = getPlane(0.0f, true, 128, sky);
	glm::vec3 waterPos = glm::vec3(0.0f, -0.5f, 0.0f);
	glm::mat4 waterModel = glm::mat4(1.0f);
	waterModel = glm::translate(waterModel, waterPos);
	waterModel = glm::scale(waterModel, glm::vec3(1.5f, 1.5f, 1.5f));
	shaderWater.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderWater.ID, "model"), 1, GL_FALSE, glm::value_ptr(waterModel));
	glUniform4f(glGetUniformLocation(shaderWater.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderWater.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	/*WATER END*/

	/*TERRAIN*/
	Shader shaderTerrain("terrain.vert","terrain.frag");
	Mesh terrain = getPlane(1.0f, false, 256, sky);
	glm::vec3 terrainPos = glm::vec3(0.0f, 0.0f, 0.0f);
	//glm::vec3 terrainPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 terrainModel = glm::mat4(1.0f);
	terrainModel = glm::scale(terrainModel, glm::vec3(1.5f,1.5f,1.5f));
	terrainModel = glm::translate(terrainModel, terrainPos);
	//terrainModel = glm::scale(terrainModel, glm::vec3(0.1f, 0.1f, 0.1f));
	shaderTerrain.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderTerrain.ID, "model"), 1, GL_FALSE, glm::value_ptr(terrainModel));
	glUniform4f(glGetUniformLocation(shaderTerrain.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderTerrain.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform1f(glGetUniformLocation(shaderTerrain.ID, "random"),getRandom(2));

	/*TERRAIN END*/

	////////////////////////////////////////////

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	glBlendFunc(GL_ONE, GL_SRC_ALPHA);

	//glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
	//glBlendEquation(GL_FUNC_ADD);
	//glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	/*SHADOW MAP*/
	// Framebuffer for Shadow Map
	Shader shadowMapProgram("shadowMap.vert", "shadowMap.frag");

	unsigned int shadowMapFBO;
	glGenFramebuffers(1, &shadowMapFBO);

	// Texture for Shadow Map FBO
	unsigned int shadowMapWidth = 4096, shadowMapHeight = 4096;
	unsigned int shadowMap;
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	// Prevents darkness outside the frustrum
	float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
	// Needed since we don't touch the color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// Matrices needed for the light's perspective
	glm::mat4 orthgonalProjection = glm::ortho(-12.0f, 12.0f, -12.0f,12.0f, 0.1f, 90.0f);
	//glm::mat4 lightView = glm::lookAt(7.0f * lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightView = glm::lookAt(1.f * lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightProjection = orthgonalProjection * lightView;

	shadowMapProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shadowMapProgram.ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));
	/*SHADOW MAP END*/

	glm::mat4 identity = glm::mat4(1.f);
	glm::mat4 modelAirplane = glm::mat4(1.0f);
	modelAirplane = glm::translate(modelAirplane, glm::vec3(0.0, 3.0, 0.0));


	// Main while loop
	float i = 0.f, j = 0.f, amt = 0.1f, amt2 = 0.1f, planeVar = 1.f, rad = 0.f;
	int cnt = 0;

	while (!glfwWindowShouldClose(window))
	{

		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Calculate model animations

		if (cnt < 100) {
			i += 0.1f;
			cnt++;
		}

		else if (cnt < 200) {
			i -= 0.1f;
		}
		else
		{
			cnt = 0;
		}
		//plank animation
		objectModel = glm::rotate(objectModel, glm::radians((float)sin(i) * 2), glm::vec3(0.0, 0.0, 1.0));
		objectModel2 = glm::rotate(objectModel2, glm::radians((float)sin(i) * 1.5f), glm::vec3(1.0, 0.0, 1.0));


		//Put every airplane animation in one matrix to send to the shadow map shader
		glm::mat4 airplaneTransform = glm::mat4(1.f);
		airplaneTransform = glm::scale(airplaneTransform, glm::vec3(0.6f, 0.6f, 0.6f));
		airplaneTransform = glm::rotate(airplaneTransform, glm::radians((float)sin(i) * 2), glm::vec3(0.0f, 0.0f, 1.0f));
		airplaneTransform = glm::rotate(airplaneTransform, glm::radians((float)sin(i / 2) * 5), glm::vec3(1.0f, 0.0f, 0.0f));
		airplaneTransform = glm::rotate(airplaneTransform, glm::radians((float)rad), glm::vec3(0.0f, 1.0f, 0.0f));
		airplaneTransform = glm::translate(airplaneTransform, glm::vec3(2.0f, 0.0f, 0.0f));
		airplaneTransform = glm::rotate(airplaneTransform, glm::radians((float)110.f), glm::vec3(0.0f, 1.0f, 0.0f));

		airplaneTransform = glm::translate(airplaneTransform, glm::vec3(0.0f, 4.0f, 0.0f));


		rad += 1;
		//reset every cirlce
		if (rad >= 360) { rad = 0; }

		glEnable(GL_DEPTH_TEST);

		// Preparations for the Shadow Map
		glViewport(0, 0, shadowMapWidth, shadowMapHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);




		shadowMapProgram.Activate();
		//glUniformMatrix4fv(glGetUniformLocation(shadowMapProgram.ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));
		// Draw scene for shadow map
		//glUniformMatrix4fv(glGetUniformLocation(shadowMapProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(airplaneTransform));
		glFrontFace(GL_CCW);
		glUniformMatrix4fv(glGetUniformLocation(shadowMapProgram.ID, "rot"), 1, GL_FALSE, glm::value_ptr(airplaneTransform));
		model.Draw(shadowMapProgram, camera);
		//glUniformMatrix4fv(glGetUniformLocation(shadowMapProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
		//floor.Draw(shadowMapProgram, camera);
		glUniformMatrix4fv(glGetUniformLocation(shadowMapProgram.ID, "rot"), 1, GL_FALSE, glm::value_ptr(objectModel2));
		floor2.Draw(shadowMapProgram, camera);
		glFrontFace(GL_CW);
		glUniformMatrix4fv(glGetUniformLocation(shadowMapProgram.ID, "rot"), 1, GL_FALSE, glm::value_ptr(terrainModel));
		terrain.Draw(shadowMapProgram,camera);
		//glUniformMatrix4fv(glGetUniformLocation(shadowMapProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
		//light.Draw(shadowMapProgram, camera);
		//plane.Draw(shadowMapProgram, camera);
		// Switch back to the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Switch back to the default viewport
		glViewport(0, 0, width, height);


		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 1000.0f);



		lightShader.Activate();
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
		glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

		shaderProgram.Activate();
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
		glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		//shadowmap
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));
		// Bind the Shadow Map
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, shadowMap);
		glUniform1i(glGetUniformLocation(shaderProgram.ID, "shadowMap"), 2);
		//


		shaderProgram2.Activate();
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram2.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel2));
		glUniform4f(glGetUniformLocation(shaderProgram2.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		glUniform3f(glGetUniformLocation(shaderProgram2.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		//glUniform1i(glGetUniformLocation(shaderProgram2.ID, "normal0"), 2);
		//shadowmap
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram2.ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));
		// Bind the Shadow Map
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, shadowMap);
		glUniform1i(glGetUniformLocation(shaderProgram2.ID, "shadowMap"), 2);
		//

		shaderProgram3.Activate();
		glUniform4f(glGetUniformLocation(shaderProgram3.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		glUniform3f(glGetUniformLocation(shaderProgram3.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		//shadowmap
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram3.ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));
		// Bind the Shadow Map
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, shadowMap);
		glUniform1i(glGetUniformLocation(shaderProgram3.ID, "shadowMap"), 2);
		//
		// Draws different meshes

		//floor.Draw(shaderProgram, camera);
		glFrontFace(GL_CCW);
		light.Draw(lightShader, camera);
		floor2.Draw(shaderProgram2, camera);
		
		model.Draw(shaderProgram3, camera, airplaneTransform, identity, identity);

		// Since the cubemap will always have a depth of 1.0, we need that equal sign so it doesn't get discarded
		glDepthFunc(GL_LEQUAL);

		skyboxShader.Activate();
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		// We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
		// The last row and column affect the translation of the skybox (which we don't want to affect)
		view = glm::mat4(glm::mat3(glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up)));
		projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// Draws the cubemap as the last object so we can save a bit of performance by discarding all fragments
		// where an object is present (a depth of 1.0f will always fail against any object's depth value)
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glFrontFace(GL_CCW);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Switch back to the normal depth function
		glDepthFunc(GL_LESS);
		glFrontFace(GL_CW);

		//Water
		shaderWater.Activate();
		glUniformMatrix4fv(glGetUniformLocation(shaderWater.ID, "model"), 1, GL_FALSE, glm::value_ptr(waterModel));
		glUniform4f(glGetUniformLocation(shaderWater.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		glUniform3f(glGetUniformLocation(shaderWater.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform1f(glGetUniformLocation(shaderWater.ID, "translateY"), planeVar += 0.02f);
		//shadowmap
		glUniformMatrix4fv(glGetUniformLocation(shaderWater.ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));
		// Bind the Shadow Map
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, shadowMap);
		glUniform1i(glGetUniformLocation(shaderWater.ID, "shadowMap"), 2);

		//Terrain
		shaderTerrain.Activate();
		//shadowmap
		glUniformMatrix4fv(glGetUniformLocation(shaderTerrain.ID, "model"), 1,GL_FALSE, glm::value_ptr(terrainModel));
		glUniformMatrix4fv(glGetUniformLocation(shaderTerrain.ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));
		glUniform4f(glGetUniformLocation(shaderTerrain.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		glUniform3f(glGetUniformLocation(shaderTerrain.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		// Bind the Shadow Map
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, shadowMap);
		glUniform1i(glGetUniformLocation(shaderTerrain.ID, "shadowMap"), 2);

	
		terrain.Draw(shaderTerrain, camera);
		//glEnable(GL_BLEND);
		water.Draw(shaderWater, camera);
		//glDisable(GL_BLEND);


		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	shaderProgram.Delete();
	shaderProgram2.Delete();
	shadowMapProgram.Delete();
	lightShader.Delete();
	skyboxShader.Delete();
	shaderWater.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}