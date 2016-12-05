# UB-Sign-Language
Sign language translation software for CSE 442

Group Members -
Chris Serafini, Tim McCormick, Manikandan Sundararajan, Aditya Bhavnani, Tim McClusky

Main Purpose - 
The purpose of this project is to develop software that can accurately translate sign language into English. 
The applications of such a system are endless. 
There are 70 million people worldwide who use sign langauge primarily for communication. 
It would enable such people to communicate effectively with those that don't.

### Requirements
**Hardware**
  - Leap Motion Controller
  
**Software**
  - Gesture Translator Code Base (Our code)
  - Leap Motion Code Base (See "Getting Started")
  
### Getting started
In order to get our system setup on your local machine, please use the following Git command to clone the repository onto your system - 

`$ git clone git@github.com:tsmanikandan/UB-Sign-Language.git`

Alternatively, you can download the repository as a .zip file and extract the files. The **master** branch contains deployment ready code.
Please check the requirements given below and update your system accordingly.

  1. Download the Leap Motion Controller software package from the Leap Motion website. Direct Linux Download Link: (https://warehouse.leapmotion.com/apps/4186/download) Website Link: (https://www.leapmotion.com/setup/linux)
  2. Extract the Leap Motion Controller software package to a preferred location.
  3. Install the Leap Motion Controller software. This can be done by either right clicking on the software file and installing it, or by running: sh $ sudo dpkg --install (extracted Leap Motion Controller software file)
  4. Download and install Qt using either the online installer or offline installer for your system from this link <https://www.qt.io/download-open-source/#section-2>
  5. Run Qt Creator and select File->Open File or Project.
  6. Navigate to the location where you extracted the repository, enter the **source** folder, select the gui.pro file and click **Open**.
  7. Select Build->Build Project "gui" to build the project.
  8. Select Build->Run to run the project
  
### Using The Visualizer
When you run the project, you will see our user interface. Click the **red** button to open/close the Leap Motion Diagnostic Visualizer. This Visualizer will let you see what the Leap motion sees when your hand is in view. You can use this to adjust your hand position/orientation for maximum sign recognition.

### General Use of the Gesture Translator
The application consists of 3 parts. The box at the top right shows what letter the application thinks you are signing. The box at the bottom left shows what letter you should be signing and the box at the top left is a reference image for the current letter. You can use the Visualizer in conjunction with the reference image to perfect your ASL!
