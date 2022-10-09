#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aCol;

out vec4 vertexColor; // specify a color output to the fragment shader

void main()
{
  vertexColor = vec4(aCol,1.0);
  gl_Position = vec4(aPos, 1.0f); // see how we directly give a vec3 to vec4's constructor
}