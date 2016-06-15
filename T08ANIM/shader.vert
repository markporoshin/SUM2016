// version
#version 330

// shader attributes layout
layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;

// global variables
uniform mat4 MatrWorld;
uniform mat4 MatrView;
uniform mat4 MatrProj;

// output data
out vec4 DrawColor;
out vec3 DrawPos;
out vec3 DrawNormal;

void main( void )
{
  gl_Position = MatrProj * MatrView * MatrWorld * vec4(InPosition, 1);

  DrawColor = InColor; /* * (InTexCoord.x + InTexCoord.y);*/ //vec4(InNormal * 2, 1);
  DrawPos = InPosition;
  DrawNormal = mat3(inverse(transpose(MatrWorld))) * InNormal;
}
