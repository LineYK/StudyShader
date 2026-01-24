#version 410

uniform vec3 lightDir;
uniform vec3 lightCol;
uniform vec3 meshCol;
uniform vec3 meshSpecCol;
uniform vec3 cameraPos;
uniform vec3 ambientCol;

in vec3 fragNrm;
in vec3 fragWorldPos;

out vec4 outCol;

void main() {
	vec3 nrm = normalize(fragNrm);
	vec3 toCam = normalize(cameraPos - fragWorldPos);
	vec3 halfVec = normalize(lightDir + toCam);

	float diffAmt = max(0.0, dot(nrm, lightDir));
	vec3 diffCol = lightCol * meshCol * diffAmt;

	float specAmt = max(0.0, dot(halfVec, nrm));
	float specBright = pow(specAmt, 64);
	vec3 specCol = lightCol * meshSpecCol * specBright;

	vec3 ambient = ambientCol * meshCol;

	outCol = vec4(diffCol + specCol + ambient, 1.0);
}