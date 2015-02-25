#include "ofApp.h"
#include "Parser.h"

const int size = 50;

//--------------------------------------------------------------
void ofApp::setup(){
    index = 0;
    trans = index;
    expr[index].parsePN("/ sin 1 + 2 4.4");
    
    expr[index].parsePN("sin x");
    
    receiver.setup(9005);
    
    expr[0].parse("sin(x + y) * cos(y) * cos(z) * (abs(x) + 1)");
    expr[1].parse("cos(x + y) * cos(y) * cos(z) * (abs(x) + 1)");
    current.set(ofRandomWidth() - ofGetWidth() * 0.5f,
                ofRandomHeight() - ofGetHeight() * 0.5f,
                ofRandomHeight() - ofGetHeight() * 0.5f);
    next.set(ofRandomWidth() - ofGetWidth() * 0.5f,
             ofRandomHeight() - ofGetHeight() * 0.5f,
             ofRandomHeight() - ofGetHeight() * 0.5f);
    
//    cam.setFov(3.0f);
//    cam.setupPerspective();
}

//--------------------------------------------------------------
void ofApp::update(){
    while (receiver.hasWaitingMessages()) {
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        if(m.getAddress() == "/expr") {
            index = 1 - index;
            string command = "";
            for(int i = 0; i < m.getNumArgs(); i++) {
                switch (m.getArgType(i)) {
                    case OFXOSC_TYPE_STRING:
                        command += m.getArgAsString(i);
                        command += " ";
                        break;
                    case OFXOSC_TYPE_FLOAT:
                        command += ofToString(m.getArgAsFloat(i));
                        command += " ";
                        break;
                    case OFXOSC_TYPE_INT32:
                        command += ofToString(m.getArgAsInt32(i));
                        command += " ";
                        break;
                    default:
                        break;
                }
            }
            expr[index].parsePN(command);
        }
    }
    if(ofGetFrameNum() % (60 * 2) == 0) {
        next.set(ofRandomWidth() - ofGetWidth() * 0.5f,
                 ofRandomHeight() - ofGetHeight() * 0.5f,
                 ofRandomHeight() - ofGetHeight() * 0.5f);
    }
    current = current * 0.8f + next * 0.1f;
    trans = trans * 0.9f + index * 0.1f;
    cam.setPosition(10 * current);
    cam.setTarget(ofVec3f(0, 0, 0));
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();
    
    ofBackground(0, 0, 0);
    ofSetColor(ofColor::white, 127);
    for(int j = 0; j < size; j++) {
        for(int i = 0; i < size; i++) {
            float v = expr[0].eval(ofMap(i, 0, size, 0, M_PI * 2), ofMap(j, 0, size, 0, M_PI * 2)) * (1.0f - trans)
                    + expr[1].eval(ofMap(i, 0, size, 0, M_PI * 2), ofMap(j, 0, size, 0, M_PI * 2)) * trans;
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
        index = 1 - index;
        expr[index].parsePN("sin x");
    } else if(key == 'c') {
        index = 1 - index;
        expr[index].parsePN("cos x");
    } else if(key == 'd') {
        index = 1 - index;
        expr[index].parsePN("+ sin x cos y");
    } else if(key == '_') {
        index = 1 - index;
        expr[index].parsePN("sin * x y");
    } else if(key == OF_KEY_LEFT ) {
        index = 1 - index;
    } else if(key == OF_KEY_RIGHT) {
        index = 1 - index;
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
