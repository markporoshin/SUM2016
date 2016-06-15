// version
#version 330

// output color
layout(location = 0) out vec4 OutColor;

uniform float Time;

// input data (from vertex shader)
in vec4 DrawColor;
in vec3 DrawPos;
in vec3 DrawNormal;

void main( void )
{
  /*
  if (DrawPos.z > 33.5 + 35 * sin(5 * Time))
    //discard;
    OutColor = vec4(1, 0, 0, 1);//DrawColor
  else
  */

  vec3 L = normalize(vec3(1, 1, 1));

  float nl = max(dot(L, normalize(DrawNormal)), 0);


   OutColor = DrawColor * nl;
}
