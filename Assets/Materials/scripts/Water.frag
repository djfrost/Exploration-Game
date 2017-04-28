
uniform vec4 SEA_WATER_COLOR;

// Tutorial items below
uniform mat4 o2v_projection_reflection; // composite Projection * ModelView
uniform sampler2D reflection_sampler;

varying vec3 interpolatedVertexObject;


void main ()
{
	vec4 vClipReflection = o2v_projection_reflection * vec4(interpolatedVertexObject.xy, 0.0 , 1.0);
	vec2 vDeviceReflection = vClipReflection.st / vClipReflection.q;
	vec2 vTextureReflection = vec2(0.5, 0.5) + 0.5 * vDeviceReflection;

	vec4 reflectionTextureColor = texture2D (reflection_sampler, vTextureReflection);

	// Framebuffer reflection can have alpha > 1
	reflectionTextureColor.a = 1.0;

	gl_FragColor = SEA_WATER_COLOR;
}
