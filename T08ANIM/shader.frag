// version
#version 330

// output color
layout(location = 0) out vec4 OutColor;

uniform float Time;
uniform int PartNo;

// input data (from vertex shader)
in vec4 DrawColor;
in vec3 DrawPos;
in vec3 DrawNormal;

void main( void )
{
  vec4 C = DrawColor;
  
  /*
  if (DrawPos.z > 33.5 + 35 * sin(5 * Time))
    //discard;
    OutColor = vec4(1, 0, 0, 1);//DrawColor
  else
  */
  if (PartNo == 0)
	{
	 C = vec4(1, 0, 0, 1);
	} 
  if (PartNo == 1)
	{
	 C = vec4(0, 1, 0, 1); 
	}
  if (PartNo == 2)
	{
	 C = vec4(0, 0, 1, 1);
	}
  vec3 L = normalize(vec3(sin(Time), 1, cos(Time)));

  float nl = max(dot(L, normalize(DrawNormal)), 0);
  OutColor = C * nl;


}
