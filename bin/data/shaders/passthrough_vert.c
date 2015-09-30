//pass-through vertex shader

#version 410

in vec4 position;
in vec2 texcoord; //seems to come from ofShader.cpp
//parameter passed from openFrameworks
uniform mat4 modelViewProjectionMatrix;

out vec2 vUv;


void main()
{   
	vUv = texcoord / vec2(640.0, 480.0);
    // send the vertices to the fragment shader
	gl_Position = modelViewProjectionMatrix * position;
}