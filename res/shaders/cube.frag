#version 330 core

// Outputs colors in RGBA
out vec4 f_Color;

// Import normal vectors
in vec3 v_Normal;
// Import the vertex's position
in vec3 v_VertexPosition; 
// Block face
in float v_BlockFaceID;
// Fog
in float v_FogIntensity;

vec3 skyColour = vec3(0.54f, 0.81f, 0.94f);

void main()
{
	// Normalize the normal vector
	vec3 normal = normalize(v_Normal);
    f_Color = vec4(1.0f, 0.0f, 1.0f, 1.0f);



	// Depending on the face lighting will be different
	// float faceLight = 0.0f;

	// switch (int(BlockFaceID)) {
	// 	case 0: // 0 for Back face
	// 		faceLight = 0.3f * 1.0f;
	// 		break;
	// 	case 1: // 1 for Front face
	// 		faceLight = 0.3f * 1.0f;
	// 		break;
	// 	case 2: // 2 for Left face
	// 		faceLight = 0.2f * 1.0f;
	// 		break;
	// 	case 3: // 3 for Right face
	// 		faceLight = 0.2f * 1.0f;
	// 		break;
	// 	case 4: // 4 for Top face
	// 		faceLight = 0.4f * 1.0f;
	// 		break;
	// 	case 5: // 5 for Bottom face
	// 		faceLight = 0.1f * 1.0f;
	// 		break;
	// }

	// Our final color with phong lightings
	// For some reason faceLight causes visual distortion on windows so getting rid of that
	// vec4 FinalColor = (0.6f + (0.05*BlockFaceID));// + faceLight + AmbientOcclusionIntensity);
	// FinalColor = mix(vec4(skyColour, 1.0), FinalColor, FogIntensity);

	// Add transparency if needed
	// if(int(TexID) == 5) // water
	// {
	// 	FragColor = vec4(FinalColor.rgb, 0.4);
	// }
	// else if(int(TexID) == 4 || int(TexID) == 8) // Leaves or Ice
	// {
	// 	FragColor = vec4(FinalColor.rgb, 0.7);
	// }
	// else
	// {
		// FragColor = vec4(FinalColor.rgb, 1.0);
	// }
}
