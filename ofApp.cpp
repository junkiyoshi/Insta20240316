#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::update() {

	this->face.clear();

	float phi_deg_step = 0.1;
	float theta_deg_step = 0.1;

	float R = 230;
	float r = R * 0.3;

	float threshold_1 = 0.5 - 0.06;
	float threshold_2 = 0.5 + 0.06;

	for (float phi_deg = 0; phi_deg < 360; phi_deg += phi_deg_step) {

		ofColor face_color;
		face_color.setHsb(ofMap(phi_deg, 0, 360, 0, 255), 255, 255);

		for (float theta_deg = 0; theta_deg < 360; theta_deg += theta_deg_step) {

			auto noise_location = this->make_point(R, r, theta_deg, phi_deg) * 0.008;
			auto noise_value = ofNoise(noise_location.x, noise_location.y + ofGetFrameNum() * 0.05, noise_location.z);
			if (noise_value <= threshold_1 || noise_value >= threshold_2) { continue; }

			float alpha = ofMap(abs(noise_value - 0.5), 0, 0.06, 255, 0);

			auto index = this->face.getNumVertices();
			vector<glm::vec3> vertices;

			vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg - theta_deg_step * 0.5, phi_deg - phi_deg_step * 0.5)));
			vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg + theta_deg_step * 0.5, phi_deg - phi_deg_step * 0.5)));
			vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg - theta_deg_step * 0.5, phi_deg + phi_deg_step * 0.5)));
			vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg + theta_deg_step * 0.5, phi_deg + phi_deg_step * 0.5)));

			this->face.addVertices(vertices);

			this->face.addIndex(index + 0); this->face.addIndex(index + 1); this->face.addIndex(index + 3);
			this->face.addIndex(index + 0); this->face.addIndex(index + 3); this->face.addIndex(index + 2);

			for (auto& vertex : vertices) {

				this->face.addColor(ofColor(face_color, alpha));
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 1.44);

	this->face.draw();

	/*
	int start = 1;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_point(float R, float r, float u, float v, float scale) {

	// 数学デッサン教室 描いて楽しむ数学たち　P.31

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u) * scale) * cos(v);
	auto y = (R + r * cos(u) * scale) * sin(v);
	auto z = r * sin(u) * scale;

	return glm::vec3(x, y, z);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}