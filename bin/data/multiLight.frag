#version 410

struct DirectionalLight {
	vec3 direction;
	vec3 color;
};

struct PointLight {
	vec3 position;
	vec3 color;
	float radius;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	vec3 color;
	float cutoff;
};

#define NUM_DIR_LIGHTS 1
#define NUM_POINT_LIGHTS 2
#define NUM_SPOT_LIGHTS 2

uniform DirectionalLight directionalLights[NUM_DIR_LIGHTS];
uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform SpotLight spotLights[NUM_SPOT_LIGHTS];

uniform sampler2D diffuseTex;
uniform sampler2D specTex;
uniform sampler2D normTex;
uniform samplerCube envMap;
uniform vec3 cameraPos;
uniform vec3 ambientCol;

in vec3 fragNrm;
in vec3 fragWorldPos;
in vec2 fragUV;
in mat3 TBN;

out vec4 outCol;

float diffuse(vec3 lightDir, vec3 nrm) {
	float diffAmt = max(0.0, dot(nrm, lightDir));
	return diffAmt;
}

float specular(vec3 lightDir, vec3 viewDir, vec3 nrm, float shininess) {
	vec3 halfVec = normalize(viewDir + lightDir);
	float specAmt = max(0.0, dot(halfVec, nrm));
	return pow(specAmt, shininess);
}

void main() {
	vec3 nrm = texture(normTex, fragUV).rgb;
	nrm = normalize(nrm * 2.0 - 1.0);
	nrm = normalize(TBN * nrm);

	vec3 viewDir = normalize(cameraPos - fragWorldPos);

	vec3 diffuseColor = texture(diffuseTex, fragUV).rgb;
	float specMask = texture(specTex, fragUV).r;
	vec3 envReflections = texture(envMap, reflect(-viewDir, nrm)).rgb;

	vec3 finalColor = vec3(0.0);

	// 디렉셔널 light 계산
	for (int i = 0; i < NUM_DIR_LIGHTS; i++) {
		DirectionalLight light = directionalLights[i];
		vec3 sceneLight = mix(light.color, envReflections + light.color * 0.5, 0.5);

		float diffAmt = diffuse(light.direction, nrm);
		float specAmt = specular(light.direction, viewDir, nrm, 4.0) * specMask;

		vec3 envLighting = envReflections * specMask * diffAmt;
		vec3 specCol = specMask * sceneLight * specAmt;

		finalColor += diffuseColor * diffAmt * light.color;
		finalColor += specCol * sceneLight;
	}

	// 포인트 light 계산
	for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
		PointLight light = pointLights[i];
		vec3 sceneLight = mix(light.color, envReflections + light.color * 0.5, 0.5);
		vec3 toLight = light.position - fragWorldPos;
		vec3 lightDir = normalize(toLight);
		float distToLight = length(toLight);
		float falloff = 1.0 - (distToLight / light.radius);
		
		float diffAmt = diffuse(lightDir, nrm) * falloff;
		float specAmt = specular(lightDir, viewDir, nrm, 4.0) * specMask * falloff;

		vec3 envLighting = envReflections * specMask * diffAmt;
		vec3 specCol = specMask * sceneLight * specAmt;

		finalColor += diffAmt * sceneLight * diffuseColor;
		finalColor += specCol;
	}

	// 스포트 light 계산
	for (int i = 0; i < NUM_SPOT_LIGHTS; i++) {
		SpotLight light = spotLights[i];
		vec3 sceneLight = mix(light.color, envReflections + light.color * 0.5, 0.5);
		vec3 toLight = light.position - fragWorldPos;
		vec3 lightDir = normalize(toLight);
		float angle = dot(light.direction, -lightDir);
		float falloff = (angle > light.cutoff) ? 1.0 : 0.0;

		float diffAmt = diffuse(lightDir, nrm) * falloff;
		float specAmt = specular(lightDir, viewDir, nrm, 4.0) * specMask * falloff;

		vec3 envLighting = envReflections * specMask * diffAmt;
		vec3 specCol = specMask * sceneLight * specAmt;

		finalColor += diffAmt * sceneLight * diffuseColor;
		finalColor += specCol;
	}

	outCol = vec4(finalColor + ambientCol, 1.0);
}
