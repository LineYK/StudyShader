#version 410

uniform vec3 lightDir;
uniform vec3 lightCol;
uniform vec3 cameraPos;
uniform vec3 ambientCol;

uniform sampler2D diffuseTex;
uniform sampler2D specTex;
uniform sampler2D normTex;

in vec3 fragNrm;
in vec3 fragWorldPos;
in vec2 fragUV;
in vec2 fragUV2;
in mat3 TBN;

out vec4 outCol;

void main() {
	vec3 nrm = texture(normTex, fragUV).rgb;
	nrm = (nrm * 2.0 - 1.0);
	vec3 nrm2 = texture(normTex, fragUV2).rgb;
	nrm2 = (nrm2 * 2.0 - 1.0);
	nrm = normalize(TBN * (nrm + nrm2));

	vec3 toCam = normalize(cameraPos - fragWorldPos);
	vec3 halfVec = normalize(lightDir + toCam);

	float diffAmt = max(0.0, dot(nrm, lightDir));
	vec3 diffCol = lightCol * vec3(0.3, 0.3, 0.4) * diffAmt;

	float specAmt = max(0.0, dot(halfVec, nrm));
	float specBright = pow(specAmt, 512);
	vec3 specCol = lightCol * specBright;

	outCol = vec4(diffCol + specCol + ambientCol, 1.0);
}