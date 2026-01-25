#version 410

uniform vec3 lightDir;
uniform vec3 lightCol;
uniform vec3 cameraPos;
uniform vec3 ambientCol;

uniform sampler2D diffuseTex;
uniform sampler2D specTex;

in vec3 fragNrm;
in vec3 fragWorldPos;
in vec2 fragUV;

out vec4 outCol;

void main() {
	vec3 nrm = normalize(fragNrm);
	vec3 toCam = normalize(cameraPos - fragWorldPos);
	vec3 halfVec = normalize(lightDir + toCam);

	vec3 meshCol = texture(diffuseTex, fragUV).rgb;
	float diffAmt = max(0.0, dot(nrm, lightDir));
	vec3 diffCol = lightCol * meshCol * diffAmt;

	float specAmt = max(0.0, dot(halfVec, nrm));
	float specBright = pow(specAmt, 64);
	vec3 specCol = texture(specTex, fragUV).r * lightCol * specBright;

	vec3 ambient = ambientCol * meshCol;

	outCol = vec4(diffCol + specCol + ambient, 1.0);
}