#include "leapgui.h"
#include <QApplication>
#include <QLabel>
#include <string.h>
#include "Leap.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <cctype>

class SampleListener : public QObject, public Leap::Listener {
    QLabel *imglbl;
    QLabel *chrlbl;
public:
    virtual void onInit(const Leap::Controller&);
    virtual void onConnect(const Leap::Controller& controller);
    virtual void onDisconnect(const Leap::Controller&);
    virtual void onExit(const Leap::Controller&);
    virtual void onFrame(const Leap::Controller& controller);
    virtual void onFocusGained(const Leap::Controller&);
    virtual void onFocusLost(const Leap::Controller&);
    virtual void onDeviceChange(const Leap::Controller&);
    virtual void onServiceConnect(const Leap::Controller&);
    virtual void onServiceDisconnect(const Leap::Controller&);
//    virtual void onImages(const Leap::Controller& controller);

    void setLabels(QLabel* i, QLabel* c){
        imglbl = i;
        chrlbl = c;
    }
};

//QApplication a();
//LeapGui lg;
//QLabel *imglbl = lg.findChild<QLabel *>("imgLabel");
//QLabel *chrlbl = lg.findChild<QLabel *>("charLabel");

//QLabel *imglbl = new QLabel();
//QLabel *chrlbl = new QLabel();

const std::string fingerNames[] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
const std::string boneNames[] = {"Metacarpal", "Proximal", "Middle", "Distal"};
const std::string stateNames[] = {"STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END"};

void SampleListener::onInit(const Leap::Controller&) {
  std::cout << "Initialized" << std::endl;
}

void SampleListener::onConnect(const Leap::Controller& controller) {
  std::cout << "Connected" << std::endl;
  controller.enableGesture(Leap::Gesture::TYPE_CIRCLE);
  controller.enableGesture(Leap::Gesture::TYPE_KEY_TAP);
  controller.enableGesture(Leap::Gesture::TYPE_SCREEN_TAP);
  controller.enableGesture(Leap::Gesture::TYPE_SWIPE);
}

void SampleListener::onDisconnect(const Leap::Controller&) {
  // Note: not dispatched when running in a debugger.
  std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Leap::Controller&) {
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
int openOrClosed(Leap::Vector vector) {
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
            std::cout << "Bone";
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

//Returns the float average of the desired inputs. Leap::Bone_start and bone_finish allow for some flexibility.
float averageDirection(float information[5][4][3], int finger, int bone_start, int bone_finish, int axis) {
    float sum = 0.0;
    for (int temp = bone_start; temp <= bone_finish; temp = temp + 1) {
        sum = sum + information[finger][temp][axis];
    }
    return sum/(bone_finish - bone_start + 1);
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

bool in_bounds(float information[5][4][3], int finger, int bone, int axis, float target) {
 
    float min, max;
    float range = 0.30;
    
    if ((target+range) > 1.0) {
        
        max = 1.0;
        
    } else {
     
        max = target + range;
        
    }
    
    if ((target - range) < -1.0) {
        
        min = -1.0;
        
    } else {
     
        min = target - range;
        
    }
    
    return (information[finger][bone][axis] > min && information[finger][bone][axis] < max);
    
}

bool check_bone(float information[5][4][3], int finger, int bone, float bonex, float boney, float bonez) {

	bool check1 = false;
	bool check2 = false;
	bool check3 = false;

	if (in_bounds(information, finger, bone, 0, bonex)) {

		check1 = true;

	}

	if (in_bounds(information, finger, bone, 1, boney)) {

		check2 = true;

	}

	if (in_bounds(information, finger, bone, 2, bonez)) {

		check3 = true;

	}

	return (check1 && check2 && check3);

} 

bool check_finger(float information[5][4][3], int finger, float bone1x, float bone2x, float bone3x, float bone1y, float bone2y, float bone3y, float bone1z, float bone2z, float bone3z) {

	bool check1 = false;
	bool check2 = false;
	bool check3 = false;

	if (bone_check(information, finger, 1, bone1x, bone1y, bone1z)) {

		check1 = true;
	
	}

	if (bone_check(information, finger, 2, bone2x, bone2y, bone2z)) {

		check2 = true;

	}

	if (bone_check(information, finger, 3, bone3x, bone3y, bone3z)) {

		check3 = true;

	}

	return (check1 && check2 && check3);

}


//TODO Change all to upper case
//Returns the char of a signed letter that is displayed. This is done stupidly.
//Also this is done with the LeapMotion mounted on the table, not as a webcam.
char whichLetter(float information[5][4][3]) {

	//A
	if (check_finger(information, 0, 0.49, 0.53, -0.04, -0.80, -0.78, -0.84, 0.36, 0.34, 0.53) {
		if (check_finger(information, 1, 0.01, -0.08, -0.07, 0.00, 0.99, 0.84, 0.99, 0.06, -0.53) {
			if (check_finger(information, 2, 0.18, 0.01, -0.09, 0.00, 0.99, 0.82, 0.98, 0.04, -0.55) {
				if (check_finger(information, 3, 0.21, 0.19, 0.03, -0.10, 0.97, 0.88, 0.97, 0.13, -0.48) {
					if(check_finger(information, 4, 0.30, 0.52, 0.25, -0.20, 0.85, 0.83, 0.93, 0.08, -0.50) {

					return 'A';
				
					}
				}
			}
		}
	}


	//B
	if (check_finger(information, 0, 0.52, -0.17, -0.86, -0.33, -0.15, 0.18, 0.79, 0.98, 0.48) {
		if (check_finger(information, 1, 0.01, 0.00, -0.01, -0.75, -0.71, -0.67, 0.66, 0.71, 0.75) {
			if (check_finger(information, 2, -0.04, -0.04, -0.04, -0.72, -0.68, -0.64, 0.69, 0.73, 0.77) {
				if (check_finger(information, 3, -0.08, -0.07, -0.06, -0.70, -0.65, -0.60, 0.71, 0.76, 0.80) {
					if(check_finger(information, 4, -0.07, -0.05, -0.03, -0.73, -0.68, -0.62, 0.68, 0.74, 0.79) {

					return 'B';
				
					}
				}
			}
		}
	}


	//C
	if (check_finger(information, 0, 0.90, 0.90, 0.91, -0.41, -0.42, -0.41, 0.09, 0.15, 0.00) {
		if (check_finger(information, 1, 0.76, 0.78, 0.80, -0.64, -0.60, -0.58, 0.15, 0.16, 0.17) {
			if (check_finger(information, 2, 0.76, 0.78, 0.79, -0.60, -0.58, -0.56, 0.24, 0.24, 0.24) {
				if (check_finger(information, 3, 0.80, 0.82, 0.83, -0.60, -0.57, -0.55, 0.06, 0.06, 0.06) {
					if(check_finger(information, 4, 0.76, 0.78, 0.80, -0.65, -0.62, 0.60, -0.02, -0.03, -0.03) {

					return 'C';
				
					}
				}
			}
		}
	}


	//D
	if (check_finger(information, 0, 0.49, -0.06, -0.71, -0.55, -0.46, -0.11, 0.67, 0.89, 0.70) {
		if (check_finger(information, 1, 0.10, 0.07, 0.01, -0.88, -0.36, 0.28, 0.46, 0.93, 0.96) {
			if (check_finger(information, 2, 0.14, -0.08, -0.15, 0.71, 0.80, 0.33, 0.69, -0.61, -0.94) {
				if (check_finger(information, 3, 0.19, 0.08, -0.04, 0.67, 0.80, 0.33, 0.71, -0.60, -0.94) {
					if(check_finger(information, 4, 0.31, 0.37, 0.15, 0.58,  0.75, 0.34, 0.76, -0.55, -0.93) {

					return 'D';
				
					}
				}
			}
		}
	}


	//E
	if (check_finger(information, 0, 0.28, -0.37, -0.71, -0.36, -0.21, -0.07, 0.89, 0.90, 0.70) {
		if (check_finger(information, 1, 0.06, -0.04, -0.08, -0.69, 0.58, 0.98, 0.72, 0.82, -0.19) {
			if (check_finger(information, 2, 0.04, 0.12, 0.02, -0.69, 0.60, 0.97, 0.72, 0.79, -0.25) {
				if (check_finger(information, 3, 0.15, 0.22, 0.05, -0.57, 0.57, 0.99, 0.80, 0.78, -0.02) {
					if(check_finger(information, 4, 0.19, 0.38, 0.18, -0.66, 0.44, 0.97, 0.72, 0.81, 0.11) {

					return 'E';
				
					}
				}
			}
		}
	}


	//F
	if (check_finger(information, 0, 0.53, -0.13, -0.09, -0.28, -0.15, -0.16, 0.79, 0.98, 0.98) {
		if (check_finger(information, 1, 0.15, -0.27, -0.37, 0.59, 0.94, 0.60, 0.80, -0.20, -0.70) {
			if (check_finger(information, 2, -0.01, -0.01, 0.00, -0.71, -0.68, -0.66, 0.71, 0.73, 0.76) {
				if (check_finger(information, 3, 0.10, 0.11, 0.12, -0.73, -0.71, -0.69, 0.67, 0.70, 0.71) {
					if(check_finger(information, 4, 0.07, 0.09, 0.11, -0.84, -0.82, -0.79, 0.53, 0.57, 0.60) {

					return 'F';
				
					}
				}
			}
		}
	}


	//G
	if (check_finger(information, 0, 0.85, 0.93, 0.88, -0.44, -0.14, -0.36, 0.29, 0.35, 0.31) {
		if (check_finger(information, 1, 0.98, 0.99, 0.98, -0.11, -0.02, 0.03, 0.19, -0.05, -0.20) {
			if (check_finger(information, 2, 0.84, -0.31, -0.77, -0.01, 0.21, 0.22, -0.54, -0.92, -0.60) {
				if (check_finger(information, 3, 0.82, -0.34, -0.79, -0.07, 0.05, 0.08, -0.56, -0.94, -0.61) {
					if(check_finger(information, 4, 0.86, -0.27, -0.73, -0.21, -0.14, -0.02, -0.47, -0.95, -0.68) {

					return 'G';
				
					}
				}
			}
		}
	}


	//H
	if (check_finger(information, 0, 0.92, 0.92, 0.33, -0.04, -0.08, 0.94, 0.38, 0.38, -0.04) {
		if (check_finger(information, 1, 0.99, 0.99, 0.99, -0.03, -0.02, -0.02, -0.06, -0.08, -0.10) {
			if (check_finger(information, 2, 0.99, 0.98, 0.98, -0.05, -0.05, -0.05, -0.12, -0.15, -0.17) {
				if (check_finger(information, 3, 0.66, -0.72, -0.99, -0.08, -0.01, 0.04, -0.75, -0.70, -0.13) {
					if(check_finger(information, 4, 0.71, -0.66, -0.98, -0.21, -0.30, -0.12, -0.67, -0.69, -0.18) {

					return 'H';
				
					}
				}
			}
		}
	}


	//I
	if (check_finger(information, 0, 0.48, 0.31, -0.69, -0.30, -0.29, -0.10, 0.83, 0.90, 0.72) {
		if (check_finger(information, 1, 0.05, -0.17, -0.24, 0.65, 0.98, 0.95, 0.77, 0.12, -0.19) {
			if (check_finger(information, 2, 0.19, 0.03, -0.05, 0.67, 0.99, 0.97, 0.71, 0.07, -0.23) {
				if (check_finger(information, 3, 0.25, 0.16, 0.09, 0.65, 0.98, 0.96, 0.71, 0.05, -0.27) {
					if(check_finger(information, 4, -0.03, 0.17, 0.30, -0.75, 0.36, 0.04, 0.66, 0.92, 0.95) {

					return 'I';
				
					}
				}
			}
		}
	}


	//TODO J somehow...

	//K
	if (check_finger(information, 0, 0.43, -0.20, -0.37, -0.52, -0.29, -0.19, 0.74, 0.94, 0.91) {
		if (check_finger(information, 1, 0.06, 0.08, 0.10, -0.74, -0.58, -0.41, 0.67, 0.81, 0.91) {
			if (check_finger(information, 2, 0.22, 0.24, 0.24, 0.24, 0.33, 0.38, 0.95, 0.92, 0.90) {
				if (check_finger(information, 3, 0.33, 0.22, 0.06, 0.68, 0.86, 0.56, 0.65, -0.46, -0.83) {
					if(check_finger(information, 4, 0.44, 0.43, 0.23, 0.57, 0.80, 0.57, 0.69, -0.41, -0.79) {

					return 'K';
				
					}
				}
			}
		}
	}


	//L
	if (check_finger(information, 0, 0.73, 0.75, 0.90, -0.44, -0.42, -0.30, 0.53, 0.50, 0.30) {
		if (check_finger(information, 1, 0.11, 0.06, 0.02, -0.78, -0.64, -0.50, 0.61, 0.77, 0.87) {
			if (check_finger(information, 2, -0.02, -0.06, -0.06, 0.52, 0.99, 0.86, 0.86, -0.06, -0.49) {
				if (check_finger(information, 3, 0.03, 0.11, 0.11, 0.50, 0.99, 0.86, 0.86, -0.06, -0.49) {
					if(check_finger(information, 4, 0.15, 0.39, 0.37, 0.40, 0.92, 0.84, 0.90, 0.04, -0.41) {

					return 'L';
				
					}
				}
			}
		}
	}


	//M
	if (check_finger(information, 0, 0.19, 0.00, -0.53, -0.18, -0.12, 0.06, 0.96, 0.99, 0.85) {
		if (check_finger(information, 1, 0.05, 0.05, 0.05, -0.35, -0.27, -0.21, 0.94, 0.96, 0.97) {
			if (check_finger(information, 2, 0.04, 0.06, 0.07, -0.32, -0.24, -0.18, 0.95, 0.97, 0.98) {
				if (check_finger(information, 3, 0.11, 0.13, 0.15, -0.30, -0.21, -0.13, 0.95, 0.97, 0.98) {
					if(check_finger(information, 4, 0.31, 0.40, 0.30, 0.42, 0.91, 0.86, 0.85, 0.08, -0.42) {

					return 'M';
				
					}
				}
			}
		}
	}


	//N
	if (check_finger(information, 0, 0.32, -0.33, -0.58, -0.39, -0.17, -0.04, 0.86, 0.93, 0.81) {
		if (check_finger(information, 1, -0.02, -0.02, -0.01, -0.04, 0.02, 0.06, 0.99, 0.99, 0.99) {
			if (check_finger(information, 2, 0.01, 0.03, 0.03, 0.00, 0.05, 0.09, 0.99, 0.99, 0.99) {
				if (check_finger(information, 3, 0.24, 0.27, 0.23, 0.70, 0.96, 0.91, 0.67, -0.02, -0.33) {
					if(check_finger(information, 4, 0.35, 0.53, 0.52, 0.63, 0.85, 0.80, 0.69, 0.01, -0.30) {

					return 'N';
				
					}
				}
			}
		}
	}


	//O
	if (check_finger(information, 0, 0.55, 0.59, 0.32, -0.80, -0.66, -0.16, -0.24, 0.46, 0.93) {
		if (check_finger(information, 1, 0.97, -0.07, -0.63, 0.18, 0.99, 0.77, 0.15, 0.03, -0.07) {
			if (check_finger(information, 2, 0.98, -0.07, -0.64, 0.17, 0.98, 0.75, 0.01, -0.18, -0.17) {
				if (check_finger(information, 3, 0.99, 0.01, -0.59, 0.09, 0.95, 0.77, -0.03, -0.31, -0.25) {
					if(check_finger(information, 4, 0.99, 0.08, -0.53, -0.04, 0.79, 0.71, -0.09, -0.60, -0.47) {

					return 'O';
				
					}
				}
			}
		}
	}


	//P
	if (check_finger(information, 0, 0.62, -0.02, -0.15, 0.14, 0.43, 0.46, 0.77, 0.90, 0.87) {
		if (check_finger(information, 1, 0.12, 0.10, 0.09, 0.21, 0.31, 0.40, 0.97, 0.94, 0.91) {
			if (check_finger(information, 2, 0.13, 0.09, 0.07, 0.97, 0.99, 0.95, 0.19, -0.13, -0.30) {
				if (check_finger(information, 3, 0.14, 0.05, -0.03, 0.99, 0.22, -0.33, -0.09, -0.97, -0.95) {
					if(check_finger(information, 4, 0.24, 0.31, 0.19, 0.97, 0.25, -0.29, -0.01, -0.92, -0.94) {

					return 'P';
				
					}
				}
			}
		}
	}


	//Q
	if (check_finger(information, 0, 0.22, 0.14, -0.41, 0.89, 0.91, 0.87, 0.40, 0.40, 0.28) {
		if (check_finger(information, 1, 0.06, 0.03, 0.02, 0.99, 0.93, 0.85, -0.06, -0.36, -0.52) {
			if (check_finger(information, 2, 0.18, 0.07, -0.02, 0.89, -0.05, -0.54, -0.42, -0.99, -0.84) {
				if (check_finger(information, 3, 0.21, 0.25, 0.15, 0.89, -0.04, -0.54, -0.39, -0.97, -0.83) {
					if(check_finger(information, 4, 0.29, 0.50,  0.38, 0.91, 0.00, -0.50, -0.29, -0.87, -0.78) {

					return 'Q';
				
					}
				}
			}
		}
	}


	//R
	if (check_finger(information, 0, 0.65, 0.39, 0.66, -0.49, -0.48, -0.49, 0.58, 0.79, 0.56) {
		if (check_finger(information, 1, -0.03, -0.06, -0.08, 0.13, 0.32, 0.44, 0.99, 0.95, 0.89) {
			if (check_finger(information, 2, 0.21, 0.19, 0.18, 0.08, 0.25, 0.36, 0.97, 0.95, 0.92) {
				if (check_finger(information, 3, 0.09, 0.03, -0.01, 0.88, 0.78, 0.40, 0.46, -0.63, -0.92) {
					if(check_finger(information, 4, 0.22, 0.28, 0.19, 0.82, 0.78, 0.43, 0.54, -0.56, -0.88) {

					return 'R';
				
					}
				}
			}
		}
	}


	//S
	if (check_finger(information, 0, 0.23, -0.15, -0.72, -0.54, -0.49, -0.25, 0.81, 0.86, 0.65) {
		if (check_finger(information, 1, 0.03, -0.20, -0.18, 0.38, 0.94, 0.54, 0.93, -0.30, -0.82) {
			if (check_finger(information, 2, 0.19, -0.09, -0.19, 0.38, 0.95, 0.55, 0.90, -0.30, 0.81) {
				if (check_finger(information, 3, 0.23, 0.03, -0.11, 0.33, 0.96, 0.59, 0.91, -0.27, -0.80) {
					if(check_finger(information, 4, 0.31, 0.38, 0.12, 0.26, 0.88, 0.57, 0.91, -0.29, -0.81) {

					return 'S';
				
					}
				}
			}
		}
	}


	//T
	if (check_finger(information, 0, 0.04, -0.21, -0.21, -0.31, -0.18, -0.18, 0.95, 0.96, 0.96) {
		if (check_finger(information, 1, 0.24, -0.09, 0.22, 0.58, 0.84, 0.31, 0.78, -0.53, -0.92) {
			if (check_finger(information, 2, 0.38, 0.05, -0.20, 0.52, 0.86, 0.36, 0.77, -0.51, -0.91) {
				if (check_finger(information, 3, 0.43, 0.23, 0.02, 0.63, 0.85, 0.57, 0.65, -0.46, -0.83) {
					if(check_finger(information, 4, 0.54, 0.44, 0.18, 0.50, 0.79, 0.56, 0.68, -0.43, -0.81) {

					return 'T';
				
					}
				}
			}
		}
	}


	//U
	if (check_finger(information, 0, 0.28, -0.42, -0.88, -0.44, -0.31, -0.06, 0.86, 0.85, 0.49) {
		if (check_finger(information, 1, -0.06, -0.07, -0.08, -0.72, -0.69, -0.67, 0.69, 0.72, 0.74) {
			if (check_finger(information, 2, 0.05, 0.05, 0.05, -0.67, -0.63, -0.60, 0.74, 0.77, 0.80) {
				if (check_finger(information, 3, 0.05, 0.19, 0.18, 0.64, 0.96, 0.74, 0.77, -0.23, -0.64) {
					if(check_finger(information, 4, 0.17, 0.45, 0.41, 0.57, 0.88, 0.69, 0.80, -0.17, -0.59) {

					return 'U';
				
					}
				}
			}
		}
	}


	//V
	if (check_finger(information, 0, 0.22, -0.39, -0.97, -0.39, -0.23, 0.26, 0.89, 0.89, -0.03) {
		if (check_finger(information, 1, 0.23, 0.23, 0.23, -0.89, -0.86, -0.83, 0.40, 0.45, 0.50) {
			if (check_finger(information, 2, -0.35, -0.34, -0.34, -0.84, -0.83, -0.80, 0.41, 0.45, 0.48) {
				if (check_finger(information, 3, 0.19, 0.34, 0.32, 0.49, 0.93, 0.91, 0.85, 0.12, -0.26) {
					if(check_finger(information, 4, 0.30, 0.54, 0.52, 0.40, 0.83, 0.83, 0.86, 0.15, -0.22) {

					return 'V';
				
					}
				}
			}
		}
	}


	//W
	if (check_finger(information, 0, 0.09, -0.25, -0.61, 0.18, 0.32, 0.42, 0.98, 0.91, 0.67) {
		if (check_finger(information, 1, 0.34, 0.36, 0.38, -0.53, -0.47, -0.42, 0.77, 0.80, 0.83) {
			if (check_finger(information, 2, -0.01, 0.00, 0.00, -0.59, -0.57, -0.55, 0.81, 0.82, 0.84) {
				if (check_finger(information, 3, -0.32, -0.30, -0.28, -0.38, -0.30, -0.22, 0.87, 0.91, 0.93) {
					if(check_finger(information, 4, 0.19, 0.47, 0.48, 0.57, 0.88, 0.76, 0.80, 0.07, -0.42) {

					return 'W';
				
					}
				}
			}
		}
	}


	//X
	if (check_finger(information, 0, 0.30, -0.36, -0.92, 0.17, 0.32, 0.30, 0.94, 0.87, 0.24) {
		if (check_finger(information, 1, 0.09, -0.02, -0.10, -0.21, 0.78, 0.39, 0.21, -0.67, -0.90) {
			if (check_finger(information, 2, 0.06, 0.17, 0.17, 0.98, 0.73, 0.39, 0.21, -0.66, -0.90) {
				if (check_finger(information, 3, 0.13, 0.31, 0.31, 0.98, 0.67, 0.34, 0.17, -0.67, -0.89) {
					if(check_finger(information, 4, 0.22, 0.55, 0.54, 0.93, 0.65, 0.34, 0.30, -0.52, -0.77) {

					return 'X';
				
					}
				}
			}
		}
	}


	//Y
	if (check_finger(information, 0, 0.58, 0.52, 0.63, -0.68, -0.70, -0.66, 0.45, 0.49, 0.41) {
		if (check_finger(information, 1, -0.08, 0.04, 0.35, 0.98, 0.72, 0.93, -0.17, -0.69) {
			if (check_finger(information, 2, 0.07, 0.21, 0.16, 0.35, 0.96, 0.70, 0.93, -0.18, -0.70) {
				if (check_finger(information, 3, 0.09, 0.41, 0.34, 0.27, 0.91, 0.69, 0.96, -0.09, -0.64) {
					if(check_finger(information, 4, -0.70, -0.62, -0.55, -0.45, -0.38, -0.33, 0.56, 0.69, 0.77) {

					return 'Y';
				
					}
				}
			}
		}
	}

	//TODO Z somehow...

    return ' ';

}

void SampleListener::onFrame(const Leap::Controller& controller) {

    //Recieve information about the current frame.
    const Leap::Frame frame = controller.frame();

    Leap::ImageList images = frame.images();
    Leap::Image image = images[1];

    const unsigned char* image_buffer = image.data();
    int width = image.width();
    int height = image.height();
//    int bpp = image.bytesPerPixel();
    QImage img((uchar *) image_buffer, width, height, QImage::Format_RGB888);
    QPixmap pxmap = QPixmap::fromImage(img);
    imglbl->setPixmap(pxmap);

//Print out frame info, including time, number of hands, and the ID of the frame.
//std::cout << "Leap::Frame id: " << frame.id()
//    << ", timestamp: " << frame.timestamp()
//    << ", hands: " << frame.hands().count()
//    << ", extended fingers: " << frame.fingers().extended().count()
//    << ", tools: " << frame.tools().count()
//    << ", gestures: " << frame.gestures().count() << std::endl;


    Leap::HandList hands = frame.hands();

    for (Leap::HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {

        // Get the first hand
        const Leap::Hand hand = *hl;
        std::string handType = hand.isLeft() ? "Left hand" : "Right hand";

/*
//Print out the infor for the current hand, including type and position.
std::cout << std::string(2, ' ') << handType << ", id: " << hand.id()
    << ", palm position: " << hand.palmPosition() << std::endl;
*/

        // Get the hand's normal vector and direction
        const Leap::Vector normal = hand.palmNormal();
        const Leap::Vector direction = hand.direction();

        //Create an array that houses the pitch, roll and yaw of the current hand.
        float angles[3];

        //Pitch
        angles[0] = direction.pitch() * Leap::RAD_TO_DEG;
        //Roll
        angles[1] = normal.roll() * Leap::RAD_TO_DEG;
        //Yaw
        angles[2] = direction.yaw() * Leap::RAD_TO_DEG;

/*
// Print out the pitch, roll, and yaw.
std::cout << "Pitch: " << angles[0] << " | Roll: " << angles[1]
    << " | Yaw: " << angles[2] << std::endl;
*/

        //Setup for isSideways test
        //int sideways_check = isSideways(angles);

        //Setup for openOrClosed test
        //Leap::Vector open_or_closed_vector = Leap::Vector::zero();

        //Keeps track of one bones of each finger.
        //A triple array, with the first element being the finger (0 thumb, 4 pinky),
        //the second element being the bone of that finger, (0 metacarpel, 3 distal),
        //and the third element being the axis of that bones (x, y, z).
        float bone_information[5][4][3];

        //Get finger information
        const Leap::FingerList fingers = hand.fingers();

        int finger_number = 0;

        for (Leap::FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {

            const Leap::Finger finger = *fl;

            // Get finger bones
            for (int b = 0; b < 4; ++b) {

                Leap::Bone::Type boneType = static_cast<Leap::Bone::Type>(b);
                Leap::Bone bone = finger.bone(boneType);

/*
std::cout << std::string(6, ' ') <<  boneNames[boneType]
    << " bone, start: " << bone.prevJoint()
    << ", end: " << bone.nextJoint()
    << ", direction: " << bone.direction() << std::endl;
*/

                Leap::Vector temp_vector = bone.direction();
                bone_information[finger_number][b][0] = temp_vector.x;
                bone_information[finger_number][b][1] = temp_vector.y;
                bone_information[finger_number][b][2] = temp_vector.z;

            }
            finger_number = finger_number + 1;
        }

    //int open_or_closed_check = openOrClosed(open_or_closed_vector);
    //open_or_closed_vector = Leap::Vector::zero();
    //printBoneInformation(bone_information);

    char finally = toupper(whichLetter(bone_information));
    chrlbl->setText(QString(finally));
//    std::cout << std::endl << "The presented letter: " << finally << std::endl << std::endl;
    }

    //Delay a quarter of a second so the output is readable.
    //This delay should be removed entirely once the output doesn't need to be read in real time.
    usleep(500000);
}

void SampleListener::onFocusGained(const Leap::Controller&) {
  std::cout << "Focus Gained" << std::endl;
}

void SampleListener::onFocusLost(const Leap::Controller&) {
  std::cout << "Focus Lost" << std::endl;
}

void SampleListener::onDeviceChange(const Leap::Controller& controller) {
  std::cout << "Leap::Device Changed" << std::endl;
  const Leap::DeviceList devices = controller.devices();

  for (int i = 0; i < devices.count(); ++i) {
    std::cout << "id: " << devices[i].toString() << std::endl;
    std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
  }
}

void SampleListener::onServiceConnect(const Leap::Controller&) {
  std::cout << "Service Connected" << std::endl;
}

void SampleListener::onServiceDisconnect(const Leap::Controller&) {
  std::cout << "Service Disconnected" << std::endl;
}

int main(int argc, char * argv[]){
    QApplication a(argc, argv);
    LeapGui lg;
    QLabel *imglbl = lg.findChild<QLabel *>("imgLabel");
    QLabel *chrlbl = lg.findChild<QLabel *>("charLabel");

    SampleListener listener;
    listener.setLabels(imglbl, chrlbl);
    Leap::Controller controller;
    controller.addListener(listener);

//    QLabel *charLabel = new QLabel();
//    QLabel *imgLabel= new QLabel();

//    imgLabel->setMinimumSize(400, 400);
//    charLabel->setMinimumSize(200, 200);

//    charLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
//    imgLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

//    charLabel->setText("Char");
//    QFont font = charLabel->font();
//    font.setPointSize(72);
//    font.setBold(true);
//    charLabel->setFont(font);
//    charLabel->setAlignment(Qt::AlignCenter);

//    imgLabel->show();
//    charLabel->show();

//    charLabel->connect(&listener, SIGNAL(objectNameChanged(QString)),
//                          SLOT(setText(QString)));

    lg.show();

    int rc = a.exec();
    controller.removeListener(listener);
    return rc;
}
