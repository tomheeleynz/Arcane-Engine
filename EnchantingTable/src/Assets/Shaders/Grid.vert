#version 450

layout (location = 0) in vec3 aPos;

layout (set = 0, binding = 0) uniform MVP {
	mat4 proj;
	mat4 view;
} mvp;

layout (location = 0) out vec3 nearPoint;
layout (location = 1) out vec3 farPoint;
layout (location = 2) out mat4 fragView;
layout (location = 7) out mat4 fragProj;

vec3 UnProjectPoint(float x, float y, float z, mat4 view, mat4 projection) {
	mat4 viewInv = inverse(view);
	mat4 projInv = inverse(projection);
	vec4 unprojectedPoint = viewInv * projInv * vec4(x, y, z, 1.0);
	return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
	nearPoint = UnProjectPoint(aPos.x, aPos.y, 0.0, mvp.view, mvp.proj).xyz;
	farPoint = UnProjectPoint(aPos.x, aPos.y, 1.0, mvp.view, mvp.proj).xyz;
	fragView = mvp.view;
	fragProj = mvp.proj;
	gl_Position = vec4(aPos, 1.0);
}

