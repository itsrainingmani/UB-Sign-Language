/******************************************************************************\
* Copyright (C) 2012-2014 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/
//TODO TEST OUT ISL
#include <iostream>
#include <cstring>
#include "Leap.h"
#include <unistd.h>

using namespace Leap;

class SampleListener : public Listener {
  public:
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
    virtual void onFocusGained(const Controller&);
    virtual void onFocusLost(const Controller&);
    virtual void onDeviceChange(const Controller&);
    virtual void onServiceConnect(const Controller&);
    virtual void onServiceDisconnect(const Controller&);

  private:
};

const std::string fingerNames[] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
const std::string boneNames[] = {"Metacarpal", "Proximal", "Middle", "Distal"};
const std::string stateNames[] = {"STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END"};

void SampleListener::onInit(const Controller& controller) {
  std::cout << "Initialized" << std::endl;
}

void SampleListener::onConnect(const Controller& controller) {
  std::cout << "Connected" << std::endl;
  controller.enableGesture(Gesture::TYPE_CIRCLE);
  controller.enableGesture(Gesture::TYPE_KEY_TAP);
  controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
  controller.enableGesture(Gesture::TYPE_SWIPE);
}

void SampleListener::onDisconnect(const Controller& controller) {
  // Note: not dispatched when running in a debugger.
  std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
  std::cout << "Exited" << std::endl;
}

void SampleListener::onFrame(const Controller& controller) {
  // Get the most recent frame and report some basic information
 usleep(500000); // Sleep for a half second
 const Frame frame = controller.frame();
/*  std::cout << "Frame id: " << frame.id()
            << ", timestamp: " << frame.timestamp()
            << ", hands: " << frame.hands().count()
            << ", extended fingers: " << frame.fingers().extended().count()
            << ", tools: " << frame.tools().count()
            << ", gestures: " << frame.gestures().count() << std::endl;
*/
  HandList hands = frame.hands();
  for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
    // Get the first hand
    const Hand hand = *hl;
    std::string handType = hand.isLeft() ? "Left hand" : "Right hand";
//    std::cout << std::string(2, ' ') << handType << ", id: " << hand.id()
//              << ", palm position: " << hand.palmPosition() << std::endl;
    // Get the hand's normal vector and direction
    const Vector normal = hand.palmNormal();
    const Vector direction = hand.direction();

    // Calculate the hand's pitch, roll, and yaw angles
//    std::cout << std::string(2, ' ') <<  "pitch: " << direction.pitch() * RAD_TO_DEG << " degrees, "
//              << "roll: " << normal.roll() * RAD_TO_DEG << " degrees, "
//              << "yaw: " << direction.yaw() * RAD_TO_DEG << " degrees" << std::endl;

    // Get fingers
	Vector test = Vector::zero();
	Vector check = Vector::zero();
	float dir [5];
	int count = 0;
    const FingerList fingers = hand.fingers();
    for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
      const Finger finger = *fl;

      // Get finger bones
      for (int b = 2; b < 3; ++b) {
        Bone::Type boneType = static_cast<Bone::Type>(b);
        Bone bone = finger.bone(boneType);
//        std::cout << std::string(6, ' ') <<  boneNames[boneType]
//                  << " bone, start: " << bone.prevJoint()
//                  << ", end: " << bone.nextJoint()
//                  << ", direction: " << bone.direction() << std::endl;
	test += bone.direction();
	check = bone.direction();
	dir[count] = check.z;
	}
	check = Vector::zero();
	count ++;
    }
//	std::cout << test.z << std::endl; //TODO FIND OUT HOW TO ROTATE VALES TO MAKE HAND UP PROPERLY
	testmethod(test);
	if(isL(dir)){
		std::cout << "IT IS L" << std::endl;
	}
	if(isP(dir)){
		std::cout << "IT IS P" << std::endl;
	}
	if(isK(dir)){
		std::cout << "IT IS K" << std::endl;
	}
	if(isD(dir)){
		std::cout << "IT IS D" << std::endl;
	}
	test = Vector::zero();
  }
}
void testmethod(Vector test){

	if(test.z >=4.5){
		std::cout << "HAND IS OPEN" << std::endl;
	}
	if(test.z <= -2.5){
		std::cout << "HAND IS CLOSED" << std::endl;
	}

}
bool isL(float dir[5]){ 
	bool check = false;
	for(int i = 0; i < 5; i++){
		std::cout << dir[i] << " ";
	}
	std::cout << " " << std::endl;
	int add = dir[2] + dir[3] + dir[4];
	if(dir[0]  >= .6 && dir[0] <= .8 && dir[1] >= .9 && add <= -1.5){
		check = true;
	}
	return check;
}
bool isP(float dir [5]){
	bool check = false;
	return check;
}

bool isK(float dir [5]){
	bool check = false;
	int add =dir[0] + dir[1];
	int add2 = dir[3] + dir[4];
	if(add >= 1.25 && dir[2] >= -.1 && dir[2] <= .1 && add2 <= -1.5){
		check = true;
	}
	return check;
}

bool isD(float dir [5]){
	bool check = false;
	if(dir[0] >=0 && dir[0] <= .25 && dir[1] >= .9 && dir[2] <= 0 && dir[2] >= -.25 && dir[3] <= 0 && dir[3] >= -.25 && dir[4] <= 0 && dir[4] >= -.25){
		check = true;
	}
	return check;
}

bool isR(float dir [5]){
	return false; //TODO LOOK TO SWAP THIS LETTER FOR ANOTHER ONE
}

void SampleListener::onFocusGained(const Controller& controller) {
  std::cout << "Focus Gained" << std::endl;
}

void SampleListener::onFocusLost(const Controller& controller) {
  std::cout << "Focus Lost" << std::endl;
}

void SampleListener::onDeviceChange(const Controller& controller) {
  std::cout << "Device Changed" << std::endl;
  const DeviceList devices = controller.devices();

  for (int i = 0; i < devices.count(); ++i) {
    std::cout << "id: " << devices[i].toString() << std::endl;
    std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
  }
}

void SampleListener::onServiceConnect(const Controller& controller) {
  std::cout << "Service Connected" << std::endl;
}

void SampleListener::onServiceDisconnect(const Controller& controller) {
  std::cout << "Service Disconnected" << std::endl;
}

int main(int argc, char** argv) {
  // Create a sample listener and controller
  SampleListener listener;
  Controller controller;

  // Have the sample listener receive events from the controller
  controller.addListener(listener);

  if (argc > 1 && strcmp(argv[1], "--bg") == 0)
    controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);

  // Keep this process running until Enter is pressed
  std::cout << "Press Enter to quit..." << std::endl;
    std::cin.get();

  // Remove the sample listener when done
  controller.removeListener(listener);

  return 0;
}
