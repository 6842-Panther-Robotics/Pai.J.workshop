#include "main.h"

void presidentialauton(){}
void redFront(){
  moveForwardP(24);
}
void blueFront(){
  moveForwardP(48);
}

void autonomous(){
  switch(autonIndex){
    case 0:
      redFront();
      break;
    case 1:
      blueFront();
      break;
  }
}
