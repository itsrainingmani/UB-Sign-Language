/******************************************************************************\
* Copyright (C) 2012-2014 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/

/*
Author: Manikandan Tirumani Sundararajan
CSE 442 - Sign Language Translation
*/

#include <iostream>
#include <cstring>
#include "Leap.h"
#include <unistd.h>
#include "LeapMath.h"
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


//Prints out "Sideways" and returns 1 if the hand is facing left. 
int isSideways(float *array) {
	if (array[2] < -40.0) {
		// std::cout << "Sideways" << std::endl;
		return 1;
	} 
	else {		
		return 0;
	}
}

//Prints out all Finger and bone information
void printBoneInformation(float information[5][4][3]) {
	for (int finger = 0; finger < 5; finger = finger + 1) {
		std::cout << "Finger: ";
		switch (finger) {
			case 0:
			std::cout << "Thumb";
			break;

			case 1:
			std::cout << "Index";			
			break;

			case 2:
			std::cout << "Middle";
			break;

			case 3:
			std::cout << "Ring";
			break;

			case 4:
			std::cout << "Pinky";
			break;
		}

		std::cout << std::endl;
		for (int bone = 0; bone < 4; bone = bone + 1) {
			std::cout << "     ";
			switch (bone) {
				case 0:
				std::cout << "Metacarpal";
				break;

				case 1:
				std::cout << "Proximal";			
				break;

				case 2:
				std::cout << "Middle";
				break;

				case 3:
				std::cout << "Distal";
				break;
			}	
			std::cout << " (" << information[finger][bone][0] << ", "
				<< information[finger][bone][1] << ", "
				<< information[finger][bone][2] << ")"
				<< std::endl;
		}
	}
	std::cout << std::endl << std::endl;
}

//Prints out the average direction of all axes for every finger
void printDirection(float avgDir[5][3]){
	for (int finger = 0; finger < 5; finger++){
		std::cout<<"Finger "<<finger<<std::endl;
		for (int dir = 0; dir < 3; dir++){
			switch(dir){
				case 0:
				std::cout<<"X-Axis - "<<avgDir[finger][dir]<<std::endl;
				break;

				case 1:
				std::cout<<"Y-Axis - "<<avgDir[finger][dir]<<std::endl;
				break;

				case 2:
				std::cout<<"Z-Axis - "<<avgDir[finger][dir]<<std::endl;
				break;
			}
		}
	}
}

//Prints out the Roll, Pitch and Yaw for the hand
void printAngles(float angles[3]){
	for (int i = 0; i < 3; i++){
		switch(i){
			case 0:
			std::cout<<"Roll - "<<angles[i]<<std::endl;
			break;

			case 1:
			std::cout<<"Pitch - "<<angles[i]<<std::endl;
			break;

			case 2:
			std::cout<<"Yaw - "<<angles[i]<<std::endl;
			break;
		}
	}
}

//Returns the float average of the desired inputs. Bone_start and bone_finish allow for some flexibility.
float averageDirection(float information[5][4][3], int finger, int bone_start, int bone_finish, int axis) {
	float sum = 0.0;
	for (int temp = bone_start; temp <= bone_finish; temp = temp + 1) {
		sum = sum + information[finger][temp][axis];
	}
	return sum/(bone_finish - bone_start + 1);
}

//Calculates the average direction for all the fingers
void allAverageDirection(float information[5][4][3], float (&avgDir)[5][3]){
	// float avgDir[5][3];
	for (int i = 0; i < 5; i++){
		for (int j = 0; j < 3; j++){
			avgDir[i][j] = averageDirection(information, i, 1, 3, j);
		}
	}
}

//Calculates the distance between the distal phalanges for any 2 fingers
float distanceBetweenFingers(float information[5][4][3], int finger1, int finger2){
	float x1 = information[finger1][3][0];
	float y1 = information[finger1][3][2];
	float x2 = information[finger2][3][0];
	float y2 = information[finger2][3][2];

	float distancex = pow(x2-x1, 2);
	float distancey = pow(y2-y1, 2);

	return sqrt(distancex - distancey);
}

//Check for letter U
bool isLetterU(float information[5][4][3], float avgDir[5][3]){
	if (avgDir[0][2] > 0.3 && avgDir[1][2] > 0.9 && avgDir[2][2] > 0.9 && avgDir[3][2] < 0.0 && avgDir[4][2] < 0.0 && distanceBetweenFingers(information, 1, 2) < 0.1){
		return true;
	}
	return false;
}

//Check for letter V
bool isLetterV(float information[5][4][3], float avgDir[5][3]){
	if (avgDir[0][2] > 0.3 && avgDir[1][2] > 0.9 && avgDir[2][2] > 0.9 && avgDir[3][2] < 0.0 && avgDir[4][2] < 0.0 && distanceBetweenFingers(information, 1, 2) > 0.3 && (information[1][3][0] > information[2][3][0])){
		return true;
	}
	return false;
}

//Check for letter W
bool isLetterW(float information[5][4][3], float avgDir[5][3]){
	if (avgDir[0][2] > 0.2 && avgDir[1][2] > 0.9 && avgDir[2][2] > 0.9 && avgDir[3][2] > 0.9 && avgDir[4][2] < 0.0){
		return true;
	}
	return false;
}

//Check for letter R
bool isLetterR(float information[5][4][3], float avgDir[5][3]){
	if (avgDir[0][2] > 0.3 && avgDir[1][2] > 0.9 && avgDir[2][2] > 0.9 && avgDir[3][2] < 0.0 && avgDir[4][2] < 0.0 && distanceBetweenFingers(information, 1, 2) > 0.2 && (information[1][3][0] < information[2][3][0])){
		return true;
	}
	return false;
}

//Check for letter H
//This so inaccurate. Holy Shit I need to do something about it.
//Never mind. It works now. Need to hold the hand right in the middle of the leap though
bool isLetterH(float information[5][4][3], float avgDir[5][3], float angles[3]){
	if (isSideways(angles)){
		if (avgDir[0][0] > 0.0 && avgDir[1][0] > 0.0 && avgDir[2][0] > 0.0 && avgDir[3][0] < 0.0 && avgDir[4][0] < 0.0){
			if (avgDir[0][1] > 0.0 && avgDir[1][1] > 0.0 && avgDir[2][1] > 0.0 && avgDir[3][1] < 0.0 && avgDir[4][1] < 0.0){
				if (avgDir[0][2] < 0.0 && avgDir[1][2] < 0.0 && avgDir[2][2] < 0.0 && avgDir[3][2] < 0.0 && avgDir[4][2] < 0.0){
					return true;
				}
			}	
		}
	}
	return false;
}

//Prints the bone information for a specific finger
void printSpecificFingerInfo(float information[5][4][3], int finger){
	for (int bone = 0; bone < 4; bone = bone + 1) {
			// std::cout << "     ";
			switch (bone) {
				case 0:
				std::cout << "Metacarpal";
				break;

				case 1:
				std::cout << "Proximal";			
				break;

				case 2:
				std::cout << "Middle";
				break;

				case 3:
				std::cout << "Distal";
				break;
			}	
			std::cout << " (" << information[finger][bone][0] << ", "
				<< information[finger][bone][1] << ", "
				<< information[finger][bone][2] << ")"
				<< std::endl;
	}
}

void SampleListener::onFrame(const Controller& controller) {

	//Recieve information about the current frame.
	const Frame frame = controller.frame();

	HandList hands = frame.hands();
	
	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
	
		// Get the first hand
		const Hand hand = *hl;
		std::string handType = hand.isLeft() ? "Left hand" : "Right hand";

		// Get the hand's normal vector and direction
		const Vector normal = hand.palmNormal();
		const Vector direction = hand.direction();

		//Create an array that houses the pitch, roll and yaw of the current hand.
		float angles[3];
	
		//Pitch
		angles[1] = direction.pitch() * RAD_TO_DEG;
		//Roll
		angles[0] = normal.roll() * RAD_TO_DEG;
		//Yaw
		angles[2] = direction.yaw() * RAD_TO_DEG;

		//Keeps track of one bones of each finger. 
		//A triple array, with the first element being the finger (0 thumb, 4 pinky),
		//the second element being the bone of that finger, (0 metacarpel, 3 distal),
		//and the third element being the axis of that bones (x, y, z).
		float bone_information[5][4][3];
		
		//Get finger information
		const FingerList fingers = hand.fingers();

		int finger_number = 0;

		for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {

			const Finger finger = *fl;
	
			// Get finger bones
			for (int b = 0; b < 4; ++b) {

				Bone::Type boneType = static_cast<Bone::Type>(b);
				Bone bone = finger.bone(boneType);

				Vector temp_vector = bone.direction();
				bone_information[finger_number][b][0] = temp_vector.x;
				bone_information[finger_number][b][1] = temp_vector.y;
				bone_information[finger_number][b][2] = temp_vector.z;

			}
			finger_number = finger_number + 1;
		}

		float dir[5][3];
		allAverageDirection(bone_information, dir);
		
		if (isLetterU(bone_information, dir))
			std::cout<<"The Letter is U"<<std::endl;
		else if (isLetterV(bone_information, dir))
			std::cout<<"The Letter is V"<<std::endl;
		else if (isLetterW(bone_information, dir))
			std::cout<<"The Letter is W"<<std::endl;
		else if (isLetterR(bone_information, dir))
			std::cout<<"The Letter is R"<<std::endl;
		else if (isLetterH(bone_information, dir, angles))
			std::cout<<"The Letter is H"<<std::endl;

		std::cout << std::endl;

	}
		//Delay a quarter of a second so the output is readable.
		//This delay should be removed entirely once the output doesn't need to be read in real time.
	usleep(500000);

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