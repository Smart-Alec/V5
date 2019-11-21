#include <iostream>
#include <fstream>
#include <string>
#include <vex.h>

vex::brain Brain;
vex::controller Controller;
vex::competition Competition;
vex::motor RightDrive = vex::motor(vex::PORT20, true);
vex::motor LeftDrive = vex::motor(vex::PORT11);
vex::motor RightSpinner = vex::motor(vex::PORT19);
vex::motor LeftSpinner = vex::motor(vex::PORT14);
vex::motor Storage = vex::motor(vex::PORT16);
vex::motor LeftFlipper = vex::motor(vex::PORT18);
vex::motor RightFlipper = vex::motor(vex::PORT13, true);

char atoc(int analog){ // analog to char
  // analog can be of range (-128..127)
  if(analog > 100){
    char byte = 100;
    return byte;
  }else if(analog < -100){
    char byte = -100;
    return byte;
  }else{
    char byte = analog;
    return byte;
  }
}

int ctoa(char byte){ // char to analog
  int analog = byte;
  //bugfix?
  if(analog > 100){
    return (analog-256);
  }else if(analog <= 100){
    return analog;
  }else{
    return 0; //uhoh
  }
}

bool get_bit(char analog, int index){ // gets selected bit from byte
  if(((analog >> index) & 1) == 1){
    return true;
  }else if(((analog >> index) & 1) == 0){
    return false;
  }else{
    return false; // that's hacky
  }
}

int btoi(bool button){ // bool to int
  if(button)
    return 1;
  else
    return 0;
}

char set_bits(int bits[8]){ // sets bits in a byte
  char byte[1];
  for(int i = 0; i < 8; i++){
    if(bits[i] == 1){ // set
      byte[0] |= 1 << i;
    }else if(bits[i] == 0){ // unset
      byte[0] &= ~(1 << i);
    }
  }
  return byte[0]; // Don't remove indexing; It's super hacky but the compiler is broken and won't recognize char byte;
}

void control(int axis_1, int axis_2, int axis_3, int axis_4, bool l2, bool r2, bool l1, bool r1, bool right, bool up, bool left, bool down, bool x, bool y, bool a, bool b){
  //add "driver control" function here
  LeftDrive.spin(vex::directionType::fwd, (axis_3+axis_1), vex::percentUnits::pct);
  RightDrive.spin(vex::directionType::fwd, (axis_3-axis_1), vex::percentUnits::pct);
  if(r1){
    //intake
    RightSpinner.spin(vex::directionType::fwd, 100, vex::percentUnits::pct);
    LeftSpinner.spin(vex::directionType::fwd, 100, vex::percentUnits::pct);
  }else if(l1){
    //outtake
    RightSpinner.spin(vex::directionType::rev, 100, vex::percentUnits::pct);
    LeftSpinner.spin(vex::directionType::rev, 100, vex::percentUnits::pct);
  }else{
    RightSpinner.stop(vex::brakeType::hold);
    LeftSpinner.stop(vex::brakeType::hold);
  }
  //tilt storage
  if(up){
    Storage.spin(vex::directionType::fwd, 5, vex::percentUnits::pct);
  }else if(down){
    Storage.spin(vex::directionType::rev, 5, vex::percentUnits::pct);
  }else{
    Storage.stop(vex::brakeType::hold);
  }

  if(r2){
    LeftFlipper.spin(vex::directionType::fwd, 100, vex::percentUnits::pct);
    RightFlipper.spin(vex::directionType::fwd, 100, vex::percentUnits::pct);
  }else if(l2){
    LeftFlipper.spin(vex::directionType::rev, 100, vex::percentUnits::pct);
    RightFlipper.spin(vex::directionType::rev, 100, vex::percentUnits::pct);
  }else{
    LeftFlipper.stop(vex::brakeType::hold);
    RightFlipper.stop(vex::brakeType::hold);
  }
}

void execute_autonomous(std::string filename){
  std::ifstream file(filename, std::ios::in | std::ios::binary);
  while(true){
    char temp[8];
    file.read(temp, 8);
    if(ctoa(temp[7]) == 0)
      break; // ReadX == 0 is EOF
    control(ctoa(temp[0]), ctoa(temp[1]), ctoa(temp[2]), ctoa(temp[3]), get_bit(temp[4], 1), get_bit(temp[4], 3), get_bit(temp[4], 5), get_bit(temp[4], 7), get_bit(temp[5], 1), get_bit(temp[5], 3), get_bit(temp[5], 5), get_bit(temp[5], 7), get_bit(temp[6], 1), get_bit(temp[6], 3), get_bit(temp[6], 5), get_bit(temp[6], 7));
    vex::task::sleep(ctoa(temp[7]));
  }
  file.close();
}

void record_autonomous(std::string filename, int read_x){
  std::ofstream file(filename, std::ios::out | std::ios::binary);
  while(true){
    control(Controller.Axis1.value(), Controller.Axis2.value(), Controller.Axis3.value(), Controller.Axis4.value(), Controller.ButtonL2.pressing(), Controller.ButtonR2.pressing(), Controller.ButtonL1.pressing(), Controller.ButtonR1.pressing(), Controller.ButtonRight.pressing(), Controller.ButtonUp.pressing(), Controller.ButtonLeft.pressing(), Controller.ButtonDown.pressing(), Controller.ButtonX.pressing(), Controller.ButtonY.pressing(), Controller.ButtonA.pressing(), Controller.ButtonB.pressing());
    if(Controller.ButtonB.pressing()){
      break;
    } // finish capture
    char temp[8];
    temp[0] = atoc(Controller.Axis1.value());
    temp[1] = atoc(Controller.Axis2.value());
    temp[2] = atoc(Controller.Axis3.value());
    temp[3] = atoc(Controller.Axis4.value());
    int temp_top[8] = {0, btoi(Controller.ButtonL2.pressing()), 0, btoi(Controller.ButtonR2.pressing()), 0, btoi(Controller.ButtonL1.pressing()), 0, btoi(Controller.ButtonR1.pressing())};
    temp[4] = set_bits(temp_top);
    int temp_direction[8] = {0, btoi(Controller.ButtonRight.pressing()), 0, btoi(Controller.ButtonUp.pressing()), 0, btoi(Controller.ButtonLeft.pressing()), 0, btoi(Controller.ButtonDown.pressing())};
    temp[5] = set_bits(temp_direction);
    int temp_letters[8] = {0, btoi(Controller.ButtonX.pressing()), 0, btoi(Controller.ButtonY.pressing()), 0, btoi(Controller.ButtonA.pressing()), 0, btoi(Controller.ButtonB.pressing())};
    temp[6] = set_bits(temp_letters);
    temp[7] = atoc(read_x);
    file.write(temp, 8);
    vex::task::sleep(read_x);
  }
  char stops[8];
  stops[7] = atoc(100);
  file.write(stops, 8);
  char null_char[8];
  file.write(null_char, 8);
  file.close();
}

void autonomous(void){

}

void usercontrol(void){
  control(Controller.Axis1.value(), Controller.Axis2.value(), Controller.Axis3.value(), Controller.Axis4.value(), Controller.ButtonL2.pressing(), Controller.ButtonR2.pressing(), Controller.ButtonL1.pressing(), Controller.ButtonR1.pressing(), Controller.ButtonRight.pressing(), Controller.ButtonUp.pressing(), Controller.ButtonLeft.pressing(), Controller.ButtonDown.pressing(), Controller.ButtonX.pressing(), Controller.ButtonY.pressing(), Controller.ButtonA.pressing(), Controller.ButtonB.pressing());
}

int main(){
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  Brain.Screen.print(ctoa(atoc(100)));
  record_autonomous("new", 10);
  execute_autonomous("new");
}
