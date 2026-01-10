#include "ofApp.h"

using namespace glm;

void buildMesh(ofMesh& mesh, float w, float h, glm::vec3 pos) {
	float verts[] = {
		-w + pos.x, -h + pos.y, pos.z,
		-w + pos.x,  h + pos.y, pos.z,
		 w + pos.x,  h + pos.y, pos.z,
		 w + pos.x, -h + pos.y, pos.z
	};

	float uvs[] = {
		0, 0,
		0, 1,
		1, 1,
		1, 0
	};

	for (int i = 0; i < 4; i++) {
		int idx = i * 3;
		int uvIdx = i * 2;
		mesh.addVertex(glm::vec3(verts[idx], verts[idx + 1], verts[idx + 2]));
		mesh.addTexCoord(glm::vec2(uvs[uvIdx], uvs[uvIdx + 1]));
	}

	ofIndexType indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	mesh.addIndices(indices, 6);
}

mat4 buildMatrix(glm::vec3 trans, float rot, glm::vec3 scale) {
	mat4 translation = glm::translate(trans);
	mat4 rotation = glm::rotate(rot, glm::vec3(0, 0, 1));
	mat4 scaler = glm::scale(scale);
	return translation * rotation * scaler;
}

mat4 buildViewMatrix(CameraData cam) {
	return inverse(buildMatrix(cam.position, cam.rotation, glm::vec3(1, 1, 1)));
}

//--------------------------------------------------------------
void ofApp::setup(){
	charTranslate = mat4();

	ofDisableArbTex();
	ofEnableDepthTest();

	buildMesh(charMesh, 0.1, 0.2, glm::vec3(0, -0.25, 0));
	buildMesh(backgroundMesh, 1.0, 1.0, glm::vec3(0, 0, 0));
	buildMesh(cloudMesh, 0.25, 0.16, glm::vec3(0.0, 0, 0.0)); 
	buildMesh(sunMesh, 1.0, 1.0, glm::vec3(0.0, 0.0, 0.0));

	alienImg.load("walk_sheet.png");
	backgroundImg.load("forest.png");
	cloudeImg.load("cloud.png");
	sunImg.load("sun.png");

	shader.load("passthrough.vert", "alphaTest.frag");
	cloudShader.load("passthrough.vert", "cloud.frag");
	sunShader.load("passthrough.vert", "cloud.frag");
	spritesheetShader.load("spritesheet.vert", "alphaTest.frag");

}

//--------------------------------------------------------------
void ofApp::update(){
	if (walkRight) {
		float speed = 0.4 * ofGetLastFrameTime();
		charPos = vec3(speed, 0, 0);
		charTranslate = charTranslate * translate(charPos);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	cam.position = vec3(0, 0, 0);
	mat4 view = buildViewMatrix(cam);
	mat4 proj = ortho(-1.33f, 1.33f, -1.0f, 1.0f, 0.0f, 10.0f);

	static float frame = 0;
	frame = (frame > 10) ? 0.0 : frame += 0.2;
	glm::vec2 spriteSize = glm::vec2(0.28, 0.19);
	glm::vec2 spriteOffset = glm::vec2((int) frame % 3, (int) frame/3);

	ofDisableBlendMode();
	ofEnableDepthTest();

	spritesheetShader.begin();
	spritesheetShader.setUniformTexture("tex", alienImg, 0);
	spritesheetShader.setUniformMatrix4f("proj", proj);
	spritesheetShader.setUniformMatrix4f("view", view);
	spritesheetShader.setUniform2f("size", spriteSize);
	spritesheetShader.setUniform2f("offset", spriteOffset);
	spritesheetShader.setUniformMatrix4f("model", charTranslate);
	charMesh.draw();
	spritesheetShader.end();


	shader.begin();
	shader.setUniformTexture("tex", backgroundImg, 0);
	shader.setUniformMatrix4f("proj", proj);
	shader.setUniformMatrix4f("view", view);
	shader.setUniformMatrix4f("model", translate(vec3(0,0,-0.05)));
	backgroundMesh.draw();
	shader.end();

	ofDisableDepthTest();
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ALPHA);

	static float rotation = 1.0f;
	rotation += 1.0f * ofGetLastFrameTime();

	mat4 translationA = translate(vec3(-0.55, 0.0, 0.0));
	mat4 scaleA = scale(vec3(1.5, 1.0, 1.0));
	mat4 transformA = translationA * scaleA;

	mat4 ourRotation = rotate(rotation, vec3(0, 0, 1));
	mat4 newMatrix = transformA * ourRotation * inverse(transformA);
	mat4 finalMatrixA = newMatrix * transformA;
	mat4 newMatrix2 = transformA * ourRotation;
	mat4 transformB = buildMatrix(glm::vec3(0.4, 0.2, 0.0), 1.0f, glm::vec3(1.0, 1.0, 1.0));

	mat4 S = scale(vec3(1.5, 1.0, 1.0));
	mat4 T = translate(vec3(-0.55, 0.0, 0.0));
	mat4 R = rotate(rotation, vec3(0, 0, 1));

	// 이동만 되돌림
	mat4 final =
		T *
		ourRotation *
		inverse(T) *
		T *
		R *
		S;

	cloudShader.begin();
	cloudShader.setUniformTexture("tex", cloudeImg, 0);
	cloudShader.setUniformMatrix4f("proj", proj);
	cloudShader.setUniformMatrix4f("model", final);
	cloudShader.setUniformMatrix4f("view", view); 
	cloudMesh.draw();

	cloudShader.setUniformMatrix4f("model", transformB);
	cloudMesh.draw();

	cloudShader.end();
	
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	sunShader.begin();
	sunShader.setUniformTexture("tex", sunImg, 0);
	sunShader.setUniformMatrix4f("proj", proj);
	sunShader.setUniformMatrix4f("view", view);
	sunShader.setUniformMatrix4f("model", mat4());
	sunMesh.draw();
	sunShader.end();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ofKey::OF_KEY_RIGHT) {
		walkRight = true;
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == ofKey::OF_KEY_RIGHT) {
		walkRight = false;
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
