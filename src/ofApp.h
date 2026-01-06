#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		ofMesh charMesh;
		ofMesh backgroundMesh;
		ofMesh cloudMesh;
		ofMesh sunMesh;

		ofShader shader;
		ofShader cloudShader;
		ofShader sunShader;
		ofShader spritesheetShader;

		ofImage alienImg;
		ofImage backgroundImg;
		ofImage cloudeImg;
		ofImage sunImg;

		bool walkRight;
		glm::vec3 charPos;
		glm::mat4 charTranslate;

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
		
};
