#pragma once

#include "ofMain.h"
#include "ofxEasyCubemap.h"

struct CameraData {
	glm::vec3 pos;
	float fov;
};

struct Light {
	virtual bool isPointLight() {
		return false;
	}
	virtual void apply(ofShader& shader) {};
};

struct DirectionalLight : public Light {
	glm::vec3 direction;
	glm::vec3 color;
	float intensity;
	virtual void apply(ofShader& shader) override {
		shader.setUniform3f("lightDir", -direction);
		shader.setUniform3f("lightCol", color * intensity);
	}
};

struct PointLight : public Light {
	glm::vec3 position;
	glm::vec3 color;
	float intensity;
	float radius;

	virtual bool isPointLight() override {
		return true;
	}
	virtual void apply(ofShader& shader) override {
		shader.setUniform3f("lightPos", position);
		shader.setUniform3f("lightCol", color * intensity);
		shader.setUniform1f("lightRadius", radius);
	}
};

struct SpotLight {
	glm::vec3 position;
	glm::vec3 direction;
	float cutOff;

	glm::vec3 color;
	float intensity;
};

class ofApp : public ofBaseApp{

	public:
		ofMesh torusMesh;
		ofMesh shieldMesh;
		ofMesh planeMesh;
		ofMesh cubeMesh;

		ofShader diffuseShader;
		ofShader waterShader;
		ofShader cubemapShader;
		ofShader skyboxShader;
		
		CameraData cam;
		DirectionalLight dirLight;
		DirectionalLight waterLight;
		PointLight pointLight;
		SpotLight spotLight;

		ofImage diffuseTex;
		ofImage specTex;
		ofImage normalTex;
		ofImage waterNrm;

		ofxEasyCubemap cubemap;

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

		void drawWater(glm::mat4& proj, glm::mat4& view);
		void drawShield(Light&, glm::mat4& proj, glm::mat4& view);
		void drawCube(glm::mat4& proj, glm::mat4& view);
		void drawSkybox(glm::mat4& proj, glm::mat4& view);
		
		DirectionalLight dirLights[1];
		PointLight pointLights[2];
		SpotLight spotLights[2];

		ofShader dirLightShieldShader;
		ofShader pointLightShieldShader;

		ofShader dirLightShaders[2];
		ofShader pointLightShaders[2];
};
