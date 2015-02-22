#include "ofApp.h"

const int size = 50;

//--------------------------------------------------------------
void ofApp::setup(){
    expr.parsePN("/ sin 1 + 2 4.4");
    
    ofLogNotice() << expr.eval(1, 2, 3);
    
    expr.parsePN("sin x");
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();
    
    ofBackground(0, 0, 0);
    ofSetColor(ofColor::white, 127);
    for(int j = 0; j < size; j++) {
        for(int i = 0; i < size; i++) {
            float v = expr.eval(ofMap(i, 0, size, 0, M_PI * 2), ofMap(j, 0, size, 0, M_PI * 2));
            float x = ofMap(i - size / 2, 0, size / 2, 0, ofGetWidth());
            float y = ofMap(j - size / 2, 0, size / 2, 0, ofGetHeight());
            float z = ofMap(-v, -1, 1, -ofGetHeight() * 0.5f, ofGetHeight() * 0.5f);
            ofLine(x, y, 0.0f, x, y, z);
        }
    }
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 's') {
        expr.parsePN("sin x");
    } else if(key == 'c') {
        expr.parsePN("cos x");
    } else if(key == 'd') {
        expr.parsePN("+ sin x cos y");
    } else if(key == '_') {
        expr.parsePN("sin * x y");
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
