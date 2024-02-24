#include "ofApp.h"

#define BALLRADIUS 15
#define BRICKNO 32
#define BRICKLINE 8 
#define BRICKWIDTH 64
#define BRICKHEIGHT 40
#define RACQUETPOS 50

int bricks[BRICKLINE][BRICKNO];
int xPos, yPos;
int xDir, yDir;
int padX, padWidth;
int ballSpeed = 20;


#include "ofApp.h"

struct bulletType {
	int x;
	int y;
	int visible;
};

struct bulletType bullet[100];

void initBullet();
void updateBullet();
void addBullet(int x, int y);
void drawBullet();




// 총알 배열 변수 초기화
void initBullet() {
	int i;

	for (i = 0; i<100; i++) {
		bullet[i].x = 0;
		bullet[i].y = 0;
		bullet[i].visible = 0;
	}
}

// 총알의 좌표 변경 및 벽이나 벽돌과의 충돌 감지
void updateBullet() {
	int i, x, y;

	for (i = 0; i<100; i++) {
		if (bullet[i].visible == 1) {
			bullet[i].y -= 10;
			x = bullet[i].x;
			y = bullet[i].y;
			if (bricks[y / BRICKHEIGHT][x / BRICKWIDTH] == 1) {
				bricks[y / BRICKHEIGHT][x / BRICKWIDTH] = 0;
				bullet[i].visible = 0;
			}
			if (bullet[i].y < 0) {
				bullet[i].visible = 0;
			}
		}
	}
}

// 키보드가 눌리면 총알 발사
void addBullet(int x, int y) {
	int i;

	for (i = 0; i<100; i++) { // 모든 총알에 대하여…
		if (bullet[i].visible == 0) {
			bullet[i].x = x;
			bullet[i].y = y;
			bullet[i].visible = 1;
			break;
		}
	}
}


// 모든 총알 중에서 활성화된 총알을 그린다.
void drawBullet() {
	int i;

	for (i = 0; i<100; i++) {
		if (bullet[i].visible) ofCircle(bullet[i].x, bullet[i].y, 3);
	}
}

int mapRead(char *inputFile, int buffer[][BRICKNO]) {
	FILE *fp = NULL;
	int c;
	int i, j;

	fopen_s(&fp, inputFile, "r");

	if (fp == NULL) {
		printf("파일 열기 실패");
		return 1;
	}

	for (i = 0; i < BRICKLINE; i++) {
		for (j = 0; j < BRICKNO; j++) {
			if ((c = fgetc(fp)) == EOF) return 0;
			else buffer[i][j] = c - 48;
		}
		fgetc(fp);
	}
	fclose(fp);

	return 0;
}


void ofApp::setup() {
	int i, j;

	ofSetWindowTitle("2D Brick Break Out Game");
	ofSetWindowShape(1024, 768);
	ofSetFrameRate(40);
	ofBackground(ofColor::white);
	ofSetColor(ofColor::black);
	ofSetLineWidth(1);
	ofFill();

	xPos = ofGetWidth() / 2;
	yPos = ofGetHeight() / 2;
	xDir = yDir = ballSpeed;
	padX = 30;
	padWidth = 200;

	for (j = 0; j < BRICKLINE; j++) {
		for (i = 0; i < BRICKNO; i++) {
			bricks[j][i] = 1;
		}
	}

	if (mapRead("map.txt", bricks) == 1) {
		printf("File read error !\n");
		exit();
	}

	initBullet();
}


//--------------------------------------------------------------
void ofApp::update() {
	// ball
	xPos += xDir;
	yPos += yDir;

	//racquet
	padX = ofGetMouseX();

	//collision check
	if (yPos < BRICKHEIGHT*BRICKLINE + BALLRADIUS) {
		if (bricks[yPos / BRICKHEIGHT][xPos / BRICKWIDTH] == 1) {
			bricks[yPos / BRICKHEIGHT][xPos / BRICKWIDTH] = 0;
			yDir *= -1;
		}
	}

	// ball bounce
	if (xPos < BALLRADIUS || xPos > ofGetWidth() - BALLRADIUS) xDir *= -1;

	if (yPos < BALLRADIUS) yDir *= -1;

	if (yPos > ofGetHeight() - RACQUETPOS - BALLRADIUS) {
		if (xPos >= padX && xPos <= padX + padWidth)
			yDir *= -1;
		else if (yPos > ofGetHeight() - BALLRADIUS)
			yDir *= -1;
	}
	updateBullet();
}

//--------------------------------------------------------------
void ofApp::draw() {
	int i, j;

	// brick
	for (j = 0; j < BRICKLINE; j++) {
		for (i = 0; i < BRICKNO; i++) {
			if (bricks[j][i] == 1) {
				ofSetColor(ofColor::black);
			}
			else {
				ofSetColor(ofColor::white);
			}
			ofRect(i*BRICKWIDTH, j*BRICKHEIGHT, BRICKWIDTH, BRICKHEIGHT);
		}
	}
	ofSetColor(ofColor::black);

	// ball
	ofCircle(xPos, yPos, BALLRADIUS);

	// racquet
	ofLine(padX, ofGetHeight() - RACQUETPOS, padX + padWidth, ofGetHeight() - RACQUETPOS);

	drawBullet();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	addBullet(padX + padWidth / 2, ofGetHeight() - RACQUETPOS);
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
