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

// "In Bounds" - checks to see if INFO is within the range of TARGET, using
// RANGE to determine how accurate the hand position needs to be. Used in "whichLetter"
// to reduce typing and make logic less intimidating.
bool ib(float info, float target) {
 
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
    
    return (info > min && info < max);
    
}


//Returns the char of a signed letter that is displayed. This is done stupidly.
//Also this is done with the LeapMotion mounted on the table, not as a webcam.
//TODO change this so that it works as a mounted webcam.
//TODO change this so it isn't dumb.
char whichLetter(float information[5][4][3]) {

    //A
    if (information[0][2][2] >= 0.89 && information[0][2][2] < 1.0 && information[1][2][2] < -0.85
        && information[1][2][2] >= -1.0 && information[4][2][2] < -0.75 && information[4][2][2] > -1.0) {
        return 'a';
    }

    //B
    if (information[0][1][0] > 0.15 && information[0][3][0] < -0.4 &&
        averageDirection(information, 0, 1, 3, 1) < 0 &&
        averageDirection(information, 0, 1, 3, 2) > 0.5) {

//std::cout << "The thumb is positioned like in B." << std::endl;

        if (averageDirection(information, 1, 1, 3, 0) > -0.15 &&
            averageDirection(information, 1, 1, 3, 0) < 0.15 &&
            averageDirection(information, 1, 1, 3, 1) < -0.5 &&
            averageDirection(information, 1, 1, 3, 2) > 0.5) {

//std::cout << "The index finger is positioned like in B." << std::endl;

            if (averageDirection(information, 1, 1, 3, 0) > -0.15 &&
                averageDirection(information, 1, 1, 3, 0) < 0.15 &&
                averageDirection(information, 1, 1, 3, 1) < -0.5 &&
                averageDirection(information, 1, 1, 3, 2) > 0.5) {

//std::cout << "The middle finger is positioned like in B." << std::endl;

                if (averageDirection(information, 1, 1, 3, 0) > -0.05 &&
                    averageDirection(information, 1, 1, 3, 0) < 0.25 &&
                    averageDirection(information, 1, 1, 3, 1) < -0.5 &&
                    averageDirection(information, 1, 1, 3, 2) > 0.5) {


//std::cout << "The ring finger is positioned like in B." << std::endl;

                    if (averageDirection(information, 1, 1, 3, 0) > 0 &&
                        averageDirection(information, 1, 1, 3, 0) < 0.3 &&
                        averageDirection(information, 1, 1, 3, 1) < -0.5 &&
                        averageDirection(information, 1, 1, 3, 2) > 0.45) {

//std::cout << "The pinky finger is positioned like in B." << std::endl;
                        return 'b';

                    }
                }
            }
        }
    }

    //C
    if (averageDirection(information, 0, 1, 3, 0) > 0.6 &&
        averageDirection(information, 0, 1, 3, 1) < 0) {

//std::cout << "The thumb is positioned like in C." << std::endl;

        if (averageDirection(information, 1, 1, 3, 0) > 0.65 &&
            averageDirection(information, 1, 1, 3, 1) < 0) {

//std::cout << "The index finger is positioned like in C." << std::endl;

            if (averageDirection(information, 2, 1, 3, 0) > 0.65 &&
                averageDirection(information, 2, 1, 3, 1) < 0) {

//std::cout << "The middle finger is positioned like in C." << std::endl;

                if (averageDirection(information, 3, 1, 3, 0) > 0.65 &&
                    averageDirection(information, 3, 1, 3, 1) < 0) {

//std::cout << "The ring finger is positioned like in C." << std::endl;

                    if (averageDirection(information, 4, 1, 3, 0) > 0.65 &&
                        averageDirection(information, 4, 1, 3, 1) < 0) {

//std::cout << "The pinky finger is positioned like in C." << std::endl;
                        return 'c';

                    }
                }
            }
        }
    }

    //D
    if (information[1][2][2] >= 0.9 &&
        information[0][2][2]+information[2][2][2]+information[3][2][2]+information[4][2][2] <= 1 &&
            information[0][2][2]+information[2][2][2]+information[3][2][2]+information[4][2][2] >= -0.5) {
        return 'd';
    }

    //E
    if (information[0][2][2] >= 0.64 && information[0][2][2] < 0.8 && information[1][2][2] <= -0.2 &&
        information[1][2][2] > -0.42 && information[4][2][2] < 0.2 && information[4][2][2] > -0.2) {
        return 'e';
    }


    //F
    if (information[0][2][2]+information[1][2][2] >= -0.2 && information[0][2][2]+information[1][2][2] <= 0.3 &&
        information[2][2][2]+information[3][2][2]+information[4][2][2] >= 2.6) {
        return 'f';
    }

    //G
    if (averageDirection(information, 0, 1, 3, 0) > 0.4 &&
        averageDirection(information, 0, 1, 3, 1) < 0.2 &&
        averageDirection(information, 0, 1, 3, 2) > 0.2) {

//std::cout << "The thumb is positioned like in G." << std::endl;

        if (averageDirection(information, 1, 1, 3, 0) > 0.7 &&
            averageDirection(information, 1, 1, 3, 1) > -0.3 &&
            averageDirection(information, 1, 1, 3, 1) < 0.3) {

//std::cout << "The index finger is positioned like in G." << std::endl;

            if (information[2][1][0] > 0.5 && information[2][3][0] < -0.3 &&
                averageDirection(information, 2, 1, 3, 1) > -0.5 &&
                averageDirection(information, 2, 1, 3, 1) < 0.5 &&
                averageDirection(information, 2, 1, 3, 2) < -0.25) {

//std::cout << "The middle finger is positioned like in G." << std::endl;

                if (information[3][1][0] > 0.5 && information[3][3][0] < -0.3 &&
                    averageDirection(information, 3, 1, 3, 1) > -0.5 &&
                    averageDirection(information, 3, 1, 3, 1) < 0.5 &&
                    averageDirection(information, 3, 1, 3, 2) < -0.25) {

//std::cout << "The ring finger is positioned like in G." << std::endl;

                    if (information[4][1][0] > 0.5 && information[4][3][0] < -0.3 &&
                        averageDirection(information, 4, 1, 3, 1) > -0.5 &&
                        averageDirection(information, 4, 1, 3, 1) < 0.5 &&
                        averageDirection(information, 4, 1, 3, 2) < -0.25) {

//std::cout << "The pinky finger is positioned like in G." << std::endl;
                        return 'g';

                    }
                }
            }
        }
    }

    //H
    if (averageDirection(information, 0, 1, 3, 0) > 0 && averageDirection(information, 1, 1, 3, 0) > 0 &&
        averageDirection(information, 2, 1, 3, 0) > 0 && averageDirection(information, 3, 1, 3, 0) < 0 &&
        averageDirection(information, 4, 1, 3, 0) < 0 &&
            averageDirection(information, 0, 1, 3, 1) > 0 && averageDirection(information, 1, 1, 3, 1) > 0 &&
            averageDirection(information, 2, 1, 3, 1) > 0 && averageDirection(information, 3, 1, 3, 1) < 0 &&
            averageDirection(information, 4, 1, 3, 1) < 0 &&
                averageDirection(information, 0, 1, 3, 2) < 0 && averageDirection(information, 1, 1, 3, 2) < 0 &&
                averageDirection(information, 2, 1, 3, 2) < 0 && averageDirection(information, 3, 1, 3, 2) < 0 &&
                averageDirection(information, 4, 1, 3, 2) < 0) {

        return 'h';
    }

    //I
    if (information[1][2][1]+information[2][2][1]+information[3][2][1] >= 2.7 &&
        information[4][2][1] <= -0.6 && information[0][2][1] < 0.15) {
        return 'i';
    }

    //TODO J | Need to figure out motion...

    //K
    if (information[2][2][2] <= 0.2 && information[2][2][2] >= -0.2 &&
        information[0][2][2] >= 0.6 && information[1][2][2] >= 0.6 &&
            information[3][2][2] <= -0.6 && information[4][2][2] <= -0.6) {
        return 'k';
    }

    //L
    if (information[0][2][2] >= 0.6 && information[0][2][2] <= 0.8 &&
        information[1][2][2] >= 0.9 && information[2][2][2] <= -0.5 &&
            information[3][2][2] <= -0.5 && information[4][2][2] <= -0.5) {
        return 'l';
    }

    //M
    if (information[1][2][2] >= 0.25 && information[2][2][2] >= 0.25 &&
        information[3][2][2] >= 0.25 && information[4][2][1] > 0.7 &&
            information[3][2][1] < 0.4) {
        return 'm';
    }

    //N
    if (information[1][2][2]+information[2][2][2]+information[3][2][2] >= 1.2 &&
        information[0][2][2] >= 0.6 && information[0][2][0] < 0 &&
            information[3][2][1] > 0.4 && information[4][2][1] > -0.5) {
        return 'n';
    }

    //O
    if (averageDirection(information, 0, 1, 3, 0) > 0.5 &&
        averageDirection(information, 0 ,1, 3, 1) < 0) {

        //std::cout << "The thumb is positioned like in O." << std::endl;

        if(information[1][1][0] > 0.3 && averageDirection(information, 1, 2, 3, 0) < 0 &&
            averageDirection(information, 1, 1, 3, 1) > 0.25) { //doesnt check z axis

            //std::cout << "The index finger is positioned like in O." << std::endl;

            if (information[2][1][0] > 0.3 && averageDirection(information, 2, 2, 3, 0) < 0 &&
                averageDirection(information, 2, 2, 3, 1) > 0.25) { //doesnt check z axis

                //std::cout << "The middle finger is positioned like in O." << std::endl;

                if (information[3][1][0] > 0.3 && averageDirection(information, 3, 2, 3, 0) < 0.1 &&
                    averageDirection(information, 3, 1, 3, 1) > 0.15) { //doesnt check z

                    //std::cout << "The ring finger is positioned like in O." << std::endl;

                    if (information[4][1][0] > 0.25 && averageDirection(information, 4, 2, 3, 0) < 0.2 &&
                        averageDirection(information, 4, 1, 3, 1) > 0.15) { //doesnt check z

                        //std::cout << "The pinky finger is positioned like in O." << std::endl;
                        return 'o';
                    }
                }
            }
        }
    }

    //P
    if (information[2][2][1] <= 0.2 && information[2][2][1] >= -0.2 &&
        information[1][2][1] >= 0.6 && information[0][2][1] >= 0.6 &&
            information[3][2][1] <= -0.6 && information[4][2][1] <= -0.6) {
        return 'p';
    }


    //Q
    if (information[0][1][0] > 0.15 &&
        averageDirection(information, 0, 1, 3, 1) > 0.4 &&
        averageDirection(information, 0, 1, 3, 2) > 0.3) {

//std::cout << "The thumb is positioned like in Q." << std::endl;

        if (averageDirection(information, 1, 1, 3, 0) > -0.3 && averageDirection(information, 1, 1, 3, 0) < 0.5 &&
            averageDirection(information, 1, 1, 3, 1) > 0.75) {

//std::cout << "The index finger is positioned like in Q." << std::endl;

            if (averageDirection(information, 2, 1, 3, 0) > -0.4 &&
                averageDirection(information, 2, 1, 3, 1) < 0.4 &&
                information[2][1][1] > 0.5 && information[2][3][1] < 0 &&
                averageDirection(information, 2, 1, 3, 2) < 0) {

//std::cout << "The middle finger is positioned like in Q." << std::endl;

                if (averageDirection(information, 3, 1, 3, 0) > -0.3 &&
                    information[3][1][1] > 0.5 && information[3][3][1] < 0 &&
                    averageDirection(information, 3, 1, 3, 2) < 0) {

//std::cout << "The ring finger is positioned like in Q." << std::endl;

                    if (averageDirection(information, 4, 1, 3, 0) > -0.2 &&
                        information[4][1][1] > 0.5 && information[4][3][1] < 0 &&
                        averageDirection(information, 4, 1, 3, 2) < 0) {

//std::cout << "The pinky finger is positioned like in Q." << std::endl;
                        return 'q';
                    }
                }
            }
        }
    }

    //R
    if (averageDirection(information, 0, 1, 3, 2) > 0.3 && averageDirection(information, 1, 1, 3, 2) > 0.9 &&
        averageDirection(information, 2, 1, 3, 2) > 0.9 && averageDirection(information, 3, 1, 3, 2) < 0 &&
            averageDirection(information, 4, 1, 3, 2) < 0 && distanceBetweenFingers(information, 1, 2) > 0.2 &&
                (information[1][3][0] < information[2][3][0])) {
        return 'r';
    }

    //S
    if (information[0][2][2] > 0.65 && information[0][2][2] < 0.89 && information[1][2][2] < -0.85 &&
        information[1][2][2] > -1.0 && information[4][2][2] < -0.75 && information[4][2][2] > -1.0) {
        return 's';
    }

    //T
    if (information[0][2][2] >= 0.8 && information[0][2][2] < 1.0 && information[1][2][2] < -0.5 &&
        information[1][2][2] > -0.75 && information[4][2][2] < -0.48 && information[4][2][2] > -0.73) {
        return 't';
    }

    //U
    if (averageDirection(information, 0, 1, 3, 2) > 0.3 && averageDirection(information, 1, 1, 3, 2) > 0.9 &&
        averageDirection(information, 2, 1, 3, 2) > 0.9 && averageDirection(information, 3, 1, 3, 2) < 0 &&
            averageDirection(information, 4, 1, 3, 2) < 0 && distanceBetweenFingers(information, 1, 2) < 0.1) {
        return 'u';
    }

    //V
    if (averageDirection(information, 0, 1, 3, 2) > 0.3 && averageDirection(information, 1, 1, 3, 2) > 0.9 &&
        averageDirection(information, 2, 1, 3, 2) > 0.9 && averageDirection(information, 3, 1, 3, 2) < 0 &&
            averageDirection(information, 4, 1, 3, 2) < 0 && distanceBetweenFingers(information, 1, 2) > 0.3 &&
                (information[1][3][0] > information[2][3][0])) {
        return 'v';
    }

    //W
    if (averageDirection(information, 0, 1, 3, 2) > 0.2 && averageDirection(information, 1, 1, 3, 2) > 0.9 &&
        averageDirection(information, 2, 1, 3, 2) > 0.9 && averageDirection(information, 3, 1, 3, 2) > 0.9 &&
            averageDirection(information, 4, 1, 3, 2) < 0) {
        return 'w';
    }

    //X
    if (information[0][2][2] >= 0.65 && information[0][2][2] <= 0.9 && information[1][2][2] >= 0.14 &&
        information[1][2][2] < 0.42 && information[4][2][2] < -0.59 && information[4][2][2] > -0.82) {
        return 'x';
    }

    //Y
    if (information[1][2][1]+information[2][2][1]+information[3][2][1] >= 1.6 &&
        information[4][2][0] <= 0.1 && information[0][2][0] > 0.2) {
        return 'y';
    }

    //TODO Z | Need to do motion...

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
