attribute vec4 position;

uniform mat4 worldViewProj;
uniform float currTime;
uniform float waveHeight;

varying vec3 interpolatedVertexObject;

void main()
{
	// Hello world
	float yOffset = sin((position.x+currTime)*10) + cos((position.y+currTime)*10);
	vec4 newPos = vec4(position.x, yOffset, position.z, position.w);
	///New pos translates certain vertices, possibly needs a subdivide modifier
	gl_Position = worldViewProj * newPos; // Need to* newPos;
	interpolatedVertexObject = vec3(newPos);
}
