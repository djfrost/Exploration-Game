#version 120

uniform sampler2D diffuseMap;

varying vec2 oUV0;

void main ()
{
   gl_FragColor = texture2D(diffuseMap, oUV0);
}