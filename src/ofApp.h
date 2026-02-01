#pragma once

#include "ofMain.h"

struct CameraData {
	glm::vec3 pos;
	float fov;
};

struct DirectionalLight {
	glm::vec3 direction;
	glm::vec3 color;
	float intensity;
};

class ofApp : public ofBaseApp{

	public:
		ofMesh torusMesh;
		ofMesh shieldMesh;
		ofMesh planeMesh;
		ofShader diffuseShader;
		ofShader waterShader;
		CameraData cam;
		DirectionalLight dirLight;
		DirectionalLight waterLight;

		ofImage diffuseTex;
		ofImage specTex;
		ofImage normalTex;
		ofImage waterNrm;

		glm::mat4 rotationMatrix = glm::mat4(1.0f);
		BOOL isRIghtKeyPressed = FALSE;
		BOOL isLeftKeyPressed = FALSE;
		BOOL isUpKeyPressed = FALSE;
		BOOL isDownKeyPressed = FALSE;

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void drawWater(DirectionalLight& dirLight, glm::mat4& proj, glm::mat4& view);
		void drawShield(DirectionalLight& dirLight, glm::mat4& proj, glm::mat4& view);
		
};
