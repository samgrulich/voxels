#version 330 core

// Positions/Coordinates
layout (location = 0) in uint i_PackedVertex;

// Output normal vectors to the Fragment Shader
out vec3 v_Normal;
// Current position of our vertices 
// used to calculate direction of light
out vec3 v_VertexPosition;
// Block face
out float v_BlockFaceID;
// Fog intensity
out float v_FogIntensity;

// Fog variables
const float fogDensity = 0.001;
const float fogGradient = 4.0;

// Imports the camera matrix from the main function
uniform mat4 u_MVP;

// Uniform for chunk offset
uniform vec3 u_ChunkOffset;

vec2 texCoords[4] = vec2[4](
    vec2(0.0f, 0.0f), // Bottom left
    vec2(1.0f, 1.0f), // Top right
    vec2(1.0f, 0.0f), // Bottom right
    vec2(0.0f, 1.0f)  // Top left
);

vec3 backFaceNormals[4] = vec3[4](
	vec3(0.0f, 0.0f, -1.0f),
	vec3(0.0f, 0.0f, -1.0f),
	vec3(0.0f, 0.0f, -1.0f),
	vec3(0.0f, 0.0f, -1.0f)
);

vec3 frontFaceNormals[4] = vec3[4](
	vec3(0.0f, 0.0f, 1.0f),
	vec3(0.0f, 0.0f, 1.0f),
	vec3(0.0f, 0.0f, 1.0f),
	vec3(0.0f, 0.0f, 1.0f)
);

vec3 leftFaceNormals[4] = vec3[4](
	vec3(-1.0f, 0.0f, 0.0f),
	vec3(-1.0f, 0.0f, 0.0f),
	vec3( 0.0f, 0.0f, 1.0f),
	vec3(-1.0f, 0.0f, 0.0f)
);

vec3 rightFaceNormals[4] = vec3[4](
	vec3(0.0f, 0.0f,  1.0f),
	vec3(0.0f, 0.0f, -1.0f),
	vec3(0.0f, 0.0f, -1.0f),
	vec3(0.0f, 0.0f,  1.0f)
);

vec3 topFaceNormals[4] = vec3[4](
	vec3(0.0f, 0.0f, -1.0f),
	vec3(0.0f, 0.0f,  1.0f),
	vec3(0.0f, 0.0f, -1.0f),
	vec3(0.0f, 0.0f,  1.0f)
);

vec3 bottomFaceNormals[4] = vec3[4](
	vec3( 0.0f, 0.0f,  1.0f),
	vec3( 0.0f, 0.0f, -1.0f),
	vec3( 0.0f, 0.0f,  1.0f),
	vec3(-1.0f, 0.0f,  0.0f)
);


vec3 backFacePositions[4] = vec3[4](
	vec3(0.0f, 0.0f, 0.0f), // Bottom left
	vec3(1.0f, 1.0f, 0.0f), // Top Right
	vec3(1.0f, 0.0f, 0.0f), // Top Left
	vec3(0.0f, 1.0f, 0.0f)  // Bottom Right
);

vec3 frontFacePositions[4] = vec3[4](
	vec3(0.0f, 0.0f, 1.0f), // Bottom left
	vec3(1.0f, 1.0f, 1.0f), // Top Right
	vec3(0.0f, 1.0f, 1.0f), // Top Left
	vec3(1.0f, 0.0f, 1.0f)  // Bottom Right
);

vec3 leftFacePositions[4] = vec3[4](
	vec3(0.0f, 1.0f, 1.0f), // Top right
	vec3(0.0f, 0.0f, 0.0f), // Bottom left
	vec3(0.0f, 0.0f, 1.0f), // Top left
	vec3(0.0f, 1.0f, 0.0f)  // Bottom right
);

vec3 rightFacePositions[4] = vec3[4](
	vec3(1.0f, 1.0f, 1.0f), // Top right
	vec3(1.0f, 0.0f, 0.0f), // Bottom left
	vec3(1.0f, 1.0f, 0.0f), // Bottom right
	vec3(1.0f, 0.0f, 1.0f)  // Top left
);

vec3 topFacePositions[4] = vec3[4](
	vec3(0.0f, 1.0f, 0.0f),
	vec3(1.0f, 1.0f, 1.0f),
	vec3(1.0f, 1.0f, 0.0f),
	vec3(0.0f, 1.0f, 1.0f)
);

vec3 bottomFacePositions[4] = vec3[4](
	vec3(0.0f, 0.0f, 1.0f),
	vec3(1.0f, 0.0f, 0.0f),
	vec3(1.0f, 0.0f, 1.0f),
	vec3(0.0f, 0.0f, 0.0f)
);

// This is the order that vertices must be rendered
// because of the winding order required for backface culling
const int indices[6] = int[6]( 0, 1, 2, 1, 0, 3 );

void main()
{
	// Unpack vertex data
	uint x        = (i_PackedVertex) 	  & 63u;   // 6 bits, x position in chunk
	uint y        = (i_PackedVertex >> 6)  & 63u;  // 6 bits, y position in chunk
	uint z        = (i_PackedVertex >> 12) & 63u;  // 6 bits, z position in chunk
	uint faceID   = (i_PackedVertex >> 18) & 7u;   // 3 bits, what face in the cube this is
	uint vertexID = (i_PackedVertex >> 21) & 7u;   // 3 bits, which of 6 face vertices is this
	uint texID    = (i_PackedVertex >> 24) & 31u;  // 5 bits, which texture to use

	// Adjust the offset for this chunk by the block size
	vec3 pos = vec3(x, y, z);				

	// This is the final position for the vertex in world coordinates
	v_VertexPosition = pos + u_ChunkOffset;

	// Set our normal vectors and adjust vertex position
	switch(faceID) {
		case 0u: // 0 is Index for Back face
			v_Normal = backFaceNormals[indices[vertexID]];
			v_VertexPosition += backFacePositions[indices[vertexID]];
			// v_TexCoord = texCoords[indices[aVertexID]] * blockSize;
			break;
		case 1u: // 1 is Index for Front face
			v_Normal = frontFaceNormals[indices[vertexID]];
			v_VertexPosition += frontFacePositions[indices[vertexID]];
			// TexCoord = texCoords[indices[aVertexID]] * blockSize;
			break;
		case 2u: // 2 is Index for Left face
			v_Normal = leftFaceNormals[indices[vertexID]];
			v_VertexPosition += leftFacePositions[indices[vertexID]];
			// TexCoord = texCoords[indices[aVertexID]] * blockSize;
			break;
		case 3u: // 3 is Index for Right face
			v_Normal = rightFaceNormals[indices[vertexID]];
			v_VertexPosition += rightFacePositions[indices[vertexID]];
			// TexCoord = texCoords[indices[aVertexID]] * blockSize;
			break;
		case 4u: // 4 is Index for Top face
			v_Normal = topFaceNormals[indices[vertexID]];
			v_VertexPosition += topFacePositions[indices[vertexID]];
			// TexCoord = texCoords[indices[aVertexID]] * blockSize;
			break;
		case 5u: // 5 is Index for Bottom face
			v_Normal = bottomFaceNormals[indices[vertexID]];
			v_VertexPosition += bottomFacePositions[indices[vertexID]];
			// TexCoord = texCoords[indices[aVertexID]] * blockSize;
			break;
	}

	// Outputs the positions/coordinates of all vertices
	gl_Position = u_MVP * vec4(v_VertexPosition, 1.0f);
	// Set our normal vectors from vertex data, adjust it by vertex position
	v_Normal += v_VertexPosition; 
	// Set our texture ID from vertex data
	// TexID = aTexID; 
	// Block face
	v_BlockFaceID = faceID;

	// Calculate Fog
	// vec4 positionRelativeToCamera = viewMatrix * vec4(VertexPosition, 1.0f);
	// float distance = length(positionRelativeToCamera.xyz);
	// FogIntensity = exp(-pow((distance*fogDensity), fogGradient));
	// FogIntensity = clamp(FogIntensity, 0.0, 1.0); // Clamp fog visibility between 0 and 1
    v_FogIntensity = 0.0f;
}
