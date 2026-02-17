#include "ofApp.h"

#define USE_NIGHT_SKYBOX 1

using namespace glm;

void calcTangents(ofMesh& mesh)
{
	std::vector<vec4> tangents;
	tangents.resize(mesh.getNumVertices());

	uint indexCount = mesh.getNumIndices();

	const vec3* vertices = mesh.getVerticesPointer();
	const vec2* uvs = mesh.getTexCoordsPointer();
	const uint* indices = mesh.getIndexPointer();

	for (uint i = 0; i < indexCount - 2; i += 3)
	{
		const vec3& v0 = vertices[indices[i]];
		const vec3& v1 = vertices[indices[i + 1]];
		const vec3& v2 = vertices[indices[i + 2]];
		const vec2& uv0 = uvs[indices[i]];
		const vec2& uv1 = uvs[indices[i + 1]];
		const vec2& uv2 = uvs[indices[i + 2]];

		vec3 edge1 = v1 - v0;
		vec3 edge2 = v2 - v0;
		vec2 dUV1 = uv1 - uv0;
		vec2 dUV2 = uv2 - uv0;

		float f = 1.0f / (dUV1.x * dUV2.y - dUV2.x * dUV1.y);

		vec4 tan;
		tan.x = f * (dUV2.y * edge1.x - dUV1.y * edge2.x);
		tan.y = f * (dUV2.y * edge1.y - dUV1.y * edge2.y);
		tan.z = f * (dUV2.y * edge1.z - dUV1.y * edge2.z);
		tan.w = 0;
		tan = normalize(tan);

		tangents[indices[i]] += (tan);
		tangents[indices[i + 1]] += (tan);
		tangents[indices[i + 2]] += (tan);
	}

	int numColors = mesh.getNumColors();

	for (int i = 0; i < tangents.size(); ++i)
	{
		vec3 t = normalize(tangents[i]);
		if (i >= numColors)
		{
			mesh.addColor(ofFloatColor(t.x, t.y, t.z, 0.0));
		}
		else
		{
			mesh.setColor(i, ofFloatColor(t.x, t.y, t.z, 0.0));
		}
	}
}


vec3 getLightDirection(DirectionalLight& l) {
	return normalize(l.direction * -1.0f);
}

glm::vec3 getLightColor(DirectionalLight& l)
{
	return l.color * l.intensity;
}

glm::vec3 getLightColor(PointLight& l)
{
	return l.color * l.intensity;
}

glm::vec3 getLightColor(SpotLight& l)
{
	return l.color * l.intensity;
}

void ofApp::drawWater(mat4& proj, mat4& view) {
	static float t = 0.0f;
	t += ofGetLastFrameTime();

	vec3 right = vec3(1, 0, 0);
	mat4 rotatetion = rotate(radians(-90.0f), right);
	mat4 model = rotatetion * scale(vec3(5.0f, 4.0f, 4.0f));
	mat4 mvp = proj * view * model;
	mat3 normalMatrix = transpose(inverse(mat3(model)));

	ofShader& shader = waterShader;
	shader.begin();
	shader.setUniformMatrix4f("model", model);
	shader.setUniformMatrix4f("mvp", mvp);
	shader.setUniformMatrix3f("normal", normalMatrix);
	shader.setUniform3f("meshSpecCol", vec3(1.0, 1.0, 1.0));
	shader.setUniformTexture("normTex", waterNrm, 0);
	shader.setUniformTexture("envMap", cubemap.getTexture(), 1);
	shader.setUniform1f("time", t);
	shader.setUniform3f("ambientCol", vec3(0.0, 0.0, 0.0));
	shader.setUniform3f("cameraPos", cam.pos);

	shader.setUniform3f("directionalLights[0].direction", getLightDirection(dirLights[0]));
	shader.setUniform3f("directionalLights[0].color", getLightColor(dirLights[0]));

	shader.setUniform3f("pointLights[0].position", pointLights[0].position);
	shader.setUniform3f("pointLights[0].color", getLightColor(pointLights[0]));
	shader.setUniform1f("pointLights[0].radius", pointLights[0].radius);

	shader.setUniform3f("pointLights[1].position", pointLights[1].position);
	shader.setUniform3f("pointLights[1].color", getLightColor(pointLights[1]));
	shader.setUniform1f("pointLights[1].radius", pointLights[1].radius);

	shader.setUniform3f("spotLights[0].position", spotLights[0].position);
	shader.setUniform3f("spotLights[0].direction", spotLights[0].direction);
	shader.setUniform3f("spotLights[0].color", getLightColor(spotLights[0]));
	shader.setUniform1f("spotLights[0].cutOff", spotLights[0].cutOff);

	shader.setUniform3f("spotLights[1].position", spotLights[1].position);
	shader.setUniform3f("spotLights[1].direction", spotLights[1].direction);
	shader.setUniform3f("spotLights[1].color", getLightColor(spotLights[1]));
	shader.setUniform1f("spotLights[1].cutOff", spotLights[1].cutOff);

	planeMesh.draw();
	shader.end();
}

void ofApp::drawShield(mat4& proj, mat4& view) {

	mat4 model = translate(vec3(0.0, 0.75f, 0.0f)) * rotationMatrix;
	mat4 mvp = proj * view * model;
	mat3 normalMatrix = transpose(inverse(mat3(model)));

	ofShader& shader = diffuseShader;
	shader.begin();
	shader.setUniformMatrix4f("model", model);
	shader.setUniformMatrix4f("mvp", mvp);
	shader.setUniformMatrix3f("normal", normalMatrix);
	shader.setUniform3f("meshSpecCol", vec3(1.0, 1.0, 1.0));
	shader.setUniformTexture("diffuseTex", diffuseTex, 0);
	shader.setUniformTexture("specTex", specTex, 1);
	shader.setUniformTexture("normTex", normalTex, 2);
	shader.setUniformTexture("envMap", cubemap.getTexture(), 3);
	shader.setUniform3f("cameraPos", cam.pos);
	shader.setUniform3f("ambientCol", vec3(0.0, 0.0, 0.0));

	shader.setUniform3f("directionalLights[0].direction", getLightDirection(dirLights[0]));
	shader.setUniform3f("directionalLights[0].color", getLightColor(dirLights[0]));

	shader.setUniform3f("pointLights[0].position", pointLights[0].position);
	shader.setUniform3f("pointLights[0].color", getLightColor(pointLights[0]));
	shader.setUniform1f("pointLights[0].radius", pointLights[0].radius);

	shader.setUniform3f("pointLights[1].position", pointLights[1].position);
	shader.setUniform3f("pointLights[1].color", getLightColor(pointLights[1]));
	shader.setUniform1f("pointLights[1].radius", pointLights[1].radius);

	shader.setUniform3f("spotLights[0].position", spotLights[0].position);
	shader.setUniform3f("spotLights[0].direction", spotLights[0].direction);
	shader.setUniform3f("spotLights[0].color", getLightColor(spotLights[0]));
	shader.setUniform1f("spotLights[0].cutOff", spotLights[0].cutOff);

	shader.setUniform3f("spotLights[1].position", spotLights[1].position);
	shader.setUniform3f("spotLights[1].direction", spotLights[1].direction);
	shader.setUniform3f("spotLights[1].color", getLightColor(spotLights[1]));
	shader.setUniform1f("spotLights[1].cutOff", spotLights[1].cutOff);

	shieldMesh.draw();
	shader.end();
}

void ofApp::drawCube(glm::mat4& proj, glm::mat4& view)
{
	static float rotAngle = 0.01f;
	rotAngle += 0.1f;

	mat4 r = rotate(radians(rotAngle), vec3(0, 1, 0));
	mat4 s = scale(vec3(0.4, 0.4, 0.4));
	mat4 model = translate(vec3(0, 0.75f, 0)) * r * s;
	mat4 mvp = proj * view * model;

	ofShader& shader = cubemapShader;

	shader.begin();
	shader.setUniformMatrix4f("mvp", mvp);
	shader.setUniformTexture("envMap", cubemap.getTexture(), 0);
	shader.setUniform3f("cameraPos", cam.pos);
	cubeMesh.draw();
	shader.end();
}

void ofApp::drawSkybox(glm::mat4& proj, glm::mat4& view)
{
	mat4 model = translate(cam.pos);
	mat4 mvp = proj * view * model;

	ofShader& shader = skyboxShader;
	glDepthFunc(GL_LEQUAL);
	shader.begin();
	shader.setUniformMatrix4f("mvp", mvp);
	shader.setUniformTexture("envMap", cubemap.getTexture(), 0);
	cubeMesh.draw();
	shader.end();
	glDepthFunc(GL_LESS);
}

//--------------------------------------------------------------
void ofApp::setup(){
	ofDisableArbTex();
	ofEnableDepthTest();

	shieldMesh.load("shield.ply");
	planeMesh.load("plane.ply");
	calcTangents(shieldMesh);
	calcTangents(planeMesh);

	diffuseTex.load("shield_diffuse.png");
	specTex.load("shield_spec.png");
	normalTex.load("shield_normal.png");
	waterNrm.load("water_nrm.png");
	waterNrm.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);

	diffuseShader.load("mesh.vert", "multiLight.frag");
	waterShader.load("water.vert", "multiLightWater.frag");

	cubeMesh.load("cube.ply");
	cubemapShader.load("cubemap.vert", "cubemap.frag");

#if USE_NIGHT_SKYBOX
	cubemap.load("night_front.jpg", "night_back.jpg",
		"night_right.jpg", "night_left.jpg",
		"night_top.jpg", "night_bottom.jpg");

#else
	cubemap.load("cube_front.jpg", "cube_back.jpg",
		"cube_right.jpg", "cube_left.jpg",
		"cube_top.jpg", "cube_bottom.jpg");
#endif


	skyboxShader.load("skybox.vert", "skybox.frag");

	cam.pos = vec3(0, 0.75f, 1);
	cam.fov = radians(90.0f);
}

//--------------------------------------------------------------
void ofApp::update(){
	if (isRIghtKeyPressed) {
		rotationMatrix = rotate(rotationMatrix, radians(1.0f), vec3(0, 1, 0));
	}

	if (isLeftKeyPressed) {
		rotationMatrix = rotate(rotationMatrix, radians(-1.0f), vec3(0, 1, 0));
	}

	if (isUpKeyPressed) {
		rotationMatrix = rotate(rotationMatrix, radians(1.0f), vec3(1, 0, 0));
	}

	if (isDownKeyPressed) {
		rotationMatrix = rotate(rotationMatrix, radians(-1.0f), vec3(1, 0, 0));
	}

}

//--------------------------------------------------------------
void ofApp::draw(){
	float aspect = 1024.0f / 768.0f;

	mat4 view = inverse(translate(cam.pos));
	mat4 proj = perspective(cam.fov, aspect, 0.1f, 10.0f);

	dirLights[0].color = vec3(1, 1, 0);
	dirLights[0].intensity = 1.0f;
	dirLights[0].direction = vec3(1, -1, -1);

	pointLights[0].color = vec3(1, 0, 0);
	pointLights[0].radius = 1.0f;
	pointLights[0].position = vec3(-0.5, 0.5, 0.25);
	pointLights[0].intensity = 1.0;

	pointLights[1].color = vec3(0, 1, 0);
	pointLights[1].radius = 1.0f;
	pointLights[1].position = vec3(0.5, 0.5, 0.25);
	pointLights[1].intensity = 1.0;

	spotLights[0].color = vec3(0, 0, 1);
	spotLights[0].position = cam.pos + vec3(0.0, 0.5, 0);
	spotLights[0].intensity = 1.0;
	spotLights[0].direction = vec3(0, 0, -1);
	spotLights[0].cutOff = glm::cos(glm::radians(35.0f));

	spotLights[1].color = vec3(0, 1, 1);
	spotLights[1].position = cam.pos + vec3(0.0, -0.5, 0);
	spotLights[1].intensity = 1.5;
	spotLights[1].direction = vec3(0, 0, -1);
	spotLights[1].cutOff = glm::cos(glm::radians(15.0f));

	drawShield(proj, view);
	drawWater(proj, view);
	drawSkybox(proj, view);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ofKey::OF_KEY_RIGHT) {
		isRIghtKeyPressed = TRUE;
	}
	else if (key == ofKey::OF_KEY_LEFT)
	{
		isLeftKeyPressed = TRUE;
	}
	else if (key == ofKey::OF_KEY_UP)
	{
		isUpKeyPressed = TRUE;
	}
	else if (key == ofKey::OF_KEY_DOWN)
	{
		isDownKeyPressed = TRUE;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == ofKey::OF_KEY_RIGHT) {
		isRIghtKeyPressed = FALSE;
	}
	else if (key == ofKey::OF_KEY_LEFT)
	{
		isLeftKeyPressed = FALSE;
	}
	else if (key == ofKey::OF_KEY_UP)
	{
		isUpKeyPressed = FALSE;
	}
	else if (key == ofKey::OF_KEY_DOWN)
	{
		isDownKeyPressed = FALSE;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
