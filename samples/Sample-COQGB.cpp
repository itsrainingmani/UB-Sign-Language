/******************************************************************************\
* Copyright (C) 2012-2014 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/

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


//Prints out "Sideways" and returns 1 if the hand is facing left. 
int isSideways(float *array) {

	if (array[2] < -40.0) {

std::cout << "Sideways" << std::endl;
		return 1;

	} else {
	
		return 0;

	}

}

//Prints out / returns 1 if hand is open.
int openOrClosed(Vector vector) {

	std::cout << vector.z << std::endl;

	if (vector.z >= 4.5) {	

std::cout << "HAND IS OPEN" << std::endl;
		return 1;

	} else {

		if (vector.z <= -2.5) {

std::cout << "HAND IS CLOSED" << std::endl;
			return 2;

		}

	}
	
	return 0;	

}

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

//Returns the float average of the desired inputs. Bone_start and bone_finish allow for some flexibility.
float averageDirection(float information[5][4][3], int finger, int bone_start, int bone_finish, int axis) {

	float sum = 0.0;
		
	for (int temp = bone_start; temp <= bone_finish; temp = temp + 1) {

		sum = sum + information[finger][temp][axis];

	}

	return sum/(bone_finish - bone_start + 1);

}

//Returns the char of a signed letter that is displayed. This is done stupidly.
//Also this is done with the LeapMotion mounted on the table, not as a webcam.
//TODO change this so that it works as a mounted webcam.
//TODO change this so it isn't dumb.
char whichLetter(float information[5][4][3]) {

	//C
	if (averageDirection(information, 0, 1, 3, 0) > 0.6 && 
		averageDirection(information, 0, 1, 3, 1) < 0) {

		std::cout << "The thumb is positioned like in C." << std::endl;

		if (averageDirection(information, 1, 1, 3, 0) > 0.65 &&
			averageDirection(information, 1, 1, 3, 1) < 0) {
			
			std::cout << "The index finger is positioned like in C." << std::endl;
			
			if (averageDirection(information, 2, 1, 3, 0) > 0.65 &&
				averageDirection(information, 2, 1, 3, 1) < 0) {
			
				std::cout << "The middle finger is positioned like in C." << std::endl;
			
				if (averageDirection(information, 3, 1, 3, 0) > 0.65 &&
					averageDirection(information, 3, 1, 3, 1) < 0) {
			
					std::cout << "The ring finger is positioned like in C." << std::endl;
	
					if (averageDirection(information, 4, 1, 3, 0) > 0.65 &&
						averageDirection(information, 4, 1, 3, 1) < 0) {
			
						std::cout << "The pinky finger is positioned like in C." << std::endl;
						return 'c';			

					}

				}

			}

		}

	}

	//O
	if (averageDirection(information, 0, 1, 3, 0) > 0.5 &&
		averageDirection(information, 0 ,1, 3, 1) < 0) {

		std::cout << "The thumb is positioned like in O." << std::endl;

		if(information[1][1][0] > 0.3 && averageDirection(information, 1, 2, 3, 0) < 0 &&
			averageDirection(information, 1, 1, 3, 1) > 0.25) { //doesnt check z axis
			
			std::cout << "The index finger is positioned like in O." << std::endl;
			
			if (information[2][1][0] > 0.3 && averageDirection(information, 2, 2, 3, 0) < 0 &&
				averageDirection(information, 2, 2, 3, 1) > 0.25) { //doesnt check z axis
			
				std::cout << "The middle finger is positioned like in O." << std::endl;
			
				if (information[3][1][0] > 0.3 && averageDirection(information, 3, 2, 3, 0) < 0.1 &&
					averageDirection(information, 3, 1, 3, 1) > 0.15) { //doesnt check z
			
					std::cout << "The ring finger is positioned like in O." << std::endl;
	
					if (information[4][1][0] > 0.25 && averageDirection(information, 4, 2, 3, 0) < 0.2 &&
						averageDirection(information, 4, 1, 3, 1) > 0.15) { //doesnt check z
			
						std::cout << "The pinky finger is positioned like in O." << std::endl;
						return 'o';			

					}

				}

			}

		}

	}

	//TODO Q
	if (information[0][1][0] > 0.15 &&
		averageDirection(information, 0, 1, 3, 1) > 0.4 &&
		averageDirection(information, 0, 1, 3, 2) > 0.3) {

		std::cout << "The thumb is positioned like in Q." << std::endl;

		if (averageDirection(information, 1, 1, 3, 0) > -0.3 && averageDirection(information, 1, 1, 3, 0) < 0.5 &&
			averageDirection(information, 1, 1, 3, 1) > 0.75) {
			
			std::cout << "The index finger is positioned like in Q." << std::endl;
			
			if (averageDirection(information, 2, 1, 3, 0) > -0.4 &&
				averageDirection(information, 2, 1, 3, 1) < 0.4 &&
				information[2][1][1] > 0.5 && information[2][3][1] < 0 &&
				averageDirection(information, 2, 1, 3, 2) < 0) {
	
				std::cout << "The middle finger is positioned like in Q." << std::endl;
			
				if (averageDirection(information, 3, 1, 3, 0) > -0.3 &&
					information[3][1][1] > 0.5 && information[3][3][1] < 0 &&
					averageDirection(information, 3, 1, 3, 2) < 0) {
					
					std::cout << "The ring finger is positioned like in Q." << std::endl;
	
					if (averageDirection(information, 4, 1, 3, 0) > -0.2 && 
						information[4][1][1] > 0.5 && information[4][3][1] < 0 &&
						averageDirection(information, 4, 1, 3, 2) < 0) {
				
						std::cout << "The pinky finger is positioned like in Q." << std::endl;
						return 'q';

					}

				}

			}

		}

	}

	//G
	if (averageDirection(information, 0, 1, 3, 0) > 0.4 && 
		averageDirection(information, 0, 1, 3, 1) < 0.2 &&
		averageDirection(information, 0, 1, 3, 2) > 0.2) {

		std::cout << "The thumb is positioned like in G." << std::endl;

		if (averageDirection(information, 1, 1, 3, 0) > 0.7 &&
			averageDirection(information, 1, 1, 3, 1) > -0.3 &&
			averageDirection(information, 1, 1, 3, 1) < 0.3) {
			
			std::cout << "The index finger is positioned like in G." << std::endl;
			
			if (information[2][1][0] > 0.5 && information[2][3][0] < -0.3 &&
				averageDirection(information, 2, 1, 3, 1) > -0.5 &&
				averageDirection(information, 2, 1, 3, 1) < 0.5 &&
				averageDirection(information, 2, 1, 3, 2) < -0.25) {
			
				std::cout << "The middle finger is positioned like in G." << std::endl;
			
				if (information[3][1][0] > 0.5 && information[3][3][0] < -0.3 &&
					averageDirection(information, 3, 1, 3, 1) > -0.5 &&
					averageDirection(information, 3, 1, 3, 1) < 0.5 &&
					averageDirection(information, 3, 1, 3, 2) < -0.25) {
			
					std::cout << "The ring finger is positioned like in G." << std::endl;
	
					if (information[4][1][0] > 0.5 && information[4][3][0] < -0.3 &&
						averageDirection(information, 4, 1, 3, 1) > -0.5 &&
						averageDirection(information, 4, 1, 3, 1) < 0.5 &&
						averageDirection(information, 4, 1, 3, 2) < -0.25) {
			
						std::cout << "The pinky finger is positioned like in G." << std::endl;
						return 'g';

					}

				}

			}

		}

	}

	//B
	if (information[0][1][0] > 0.15 && information[0][3][0] < -0.4 &&
		averageDirection(information, 0, 1, 3, 1) < 0 &&
		averageDirection(information, 0, 1, 3, 2) > 0.5) {

		std::cout << "The thumb is positioned like in B." << std::endl;

		if (averageDirection(information, 1, 1, 3, 0) > -0.15 &&
			averageDirection(information, 1, 1, 3, 0) < 0.15 &&
			averageDirection(information, 1, 1, 3, 1) < -0.5 &&
			averageDirection(information, 1, 1, 3, 2) > 0.5) {
		
			std::cout << "The index finger is positioned like in B." << std::endl;
			
			if (averageDirection(information, 1, 1, 3, 0) > -0.15 &&
				averageDirection(information, 1, 1, 3, 0) < 0.15 &&
				averageDirection(information, 1, 1, 3, 1) < -0.5 &&
				averageDirection(information, 1, 1, 3, 2) > 0.5) {
		
				std::cout << "The middle finger is positioned like in B." << std::endl;
			
				if (averageDirection(information, 1, 1, 3, 0) > -0.05 &&
					averageDirection(information, 1, 1, 3, 0) < 0.25 &&
					averageDirection(information, 1, 1, 3, 1) < -0.5 &&
					averageDirection(information, 1, 1, 3, 2) > 0.5) {
	
			
					std::cout << "The ring finger is positioned like in B." << std::endl;
	
					if (averageDirection(information, 1, 1, 3, 0) > 0 &&
						averageDirection(information, 1, 1, 3, 0) < 0.3 &&
						averageDirection(information, 1, 1, 3, 1) < -0.5 &&
						averageDirection(information, 1, 1, 3, 2) > 0.45) {
			
						std::cout << "The pinky finger is positioned like in B." << std::endl;
						return 'b';

					}

				}

			}

		}

	}

	return ' ';

}


void SampleListener::onFrame(const Controller& controller) {

	//Recieve information about the current frame.
	const Frame frame = controller.frame();

/*
//Print out frame info, including time, number of hands, and the ID of the frame.
std::cout << "Frame id: " << frame.id()
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

/*
//Print out the infor for the current hand, including type and position.
std::cout << std::string(2, ' ') << handType << ", id: " << hand.id()
	<< ", palm position: " << hand.palmPosition() << std::endl;
*/

		// Get the hand's normal vector and direction
		const Vector normal = hand.palmNormal();
		const Vector direction = hand.direction();

		//Create an array that houses the pitch, roll and yaw of the current hand.
		float angles[3];
	
		//Pitch
		angles[0] = direction.pitch() * RAD_TO_DEG;
		//Roll
		angles[1] = normal.roll() * RAD_TO_DEG;
		//Yaw
		angles[2] = direction.yaw() * RAD_TO_DEG;

/*
// Print out the pitch, roll, and yaw.
std::cout << "Pitch: " << angles[0] << " | Roll: " << angles[1]
	<< " | Yaw: " << angles[2] << std::endl;
*/

		//Setup for isSideways test
		//int sideways_check = isSideways(angles);

		//Setup for openOrClosed test
		//Vector open_or_closed_vector = Vector::zero();

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

/*
std::cout << std::string(6, ' ') <<  boneNames[boneType]
	<< " bone, start: " << bone.prevJoint()
	<< ", end: " << bone.nextJoint()
	<< ", direction: " << bone.direction() << std::endl;
*/

				Vector temp_vector = bone.direction();
				bone_information[finger_number][b][0] = temp_vector.x;
				bone_information[finger_number][b][1] = temp_vector.y;
				bone_information[finger_number][b][2] = temp_vector.z;

			}

			finger_number = finger_number + 1;

		}

//int open_or_closed_check = openOrClosed(open_or_closed_vector);
//open_or_closed_vector = Vector::zero();	
printBoneInformation(bone_information);		

char finally = whichLetter(bone_information);
std::cout << std::endl << "=== " << finally << " ===" << std::endl << std::endl;

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
