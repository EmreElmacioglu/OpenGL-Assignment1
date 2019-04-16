#include "include/Angel.h"
#include "enums.h"

struct physics_engine {

  GLfloat locX;
  GLfloat locY;
  GLfloat velocityX;
  GLfloat velocityY;
  GLfloat initialVelocityX;
  int lastY;
  SHAPES shape;
  bool bunnyDirection = true;
  bool borders = true;

  //Set the initial values for movement
  //Starting position can be set using locX and locY
  //Initial horizontal velocity is velocityX
  //Initial vertical velocity is velocityY
  void bindValues(GLfloat locX, GLfloat locY, GLfloat velocityX, GLfloat velocityY) {
    this -> locX = locX;
    this -> locY = locY;
    this -> initialVelocityX = velocityX;
    this -> velocityX = velocityX;
    this -> velocityY = velocityY;
  }

  //Reinitializes the object and the animation
  void reset(GLfloat widthRatio, GLfloat heightRatio) {

    locX = -8.0 * widthRatio;
    locY = 8.0 * heightRatio;

    //Bunny configuration starts from bottom so if its
    //initial location is not lower its head will peek out
    //of the window
    if(shape == SHAPE_BUNNY)
      locY = 6.0 * heightRatio;
    velocityY = 0.0;
    velocityX = this -> initialVelocityX;

    bunnyDirection = true;
  }

  void calculateNewPosition(SHAPES active_shape, GLfloat widthRatio, GLfloat heightRatio) {

    shape = active_shape;

    GLfloat radiusX;
    GLfloat radiusY;

    //Modify the active object radius accordingly
    switch(active_shape) {
    case SHAPE_SPHERE:
      radiusX = 1.0;
      radiusY = 1.0;
      break;
    case SHAPE_CUBE:
      radiusX = 1.4;
      radiusY = 1.6;
      break;
    case SHAPE_BUNNY:
      radiusX = 1.15;
      radiusY = 1.0;
      break;
    }

    if(active_shape != SHAPE_BUNNY) {

      // y-axis floor collision check
      if(locY <= -10.0 * heightRatio + radiusY) {
        velocityY *= -0.9;
        velocityX *= 0.8;

        if(lastY <= -10.0 * heightRatio + (radiusY * 2) - 0.10 )
          locY = -10.0 * heightRatio + radiusY;  
      }

      // x-axis wall collision check
      if(borders && (locX >= 10.0 * widthRatio - radiusX || locX <= -10.0 * widthRatio + radiusX)) {
        bunnyDirection = !bunnyDirection;
        velocityX *= -1.0;
      }

    } else {

      // y-axis floor collision check
      if(locY <= -10.0 * heightRatio + 0.1) {
        velocityY *= -0.9;
        velocityX *= 0.8;

        if(lastY <= -10.0 * heightRatio + 1.0 )
          locY = -10.0 * heightRatio;  
      }

      // x-axis wall collision check
      if(borders &&
         (locX >= 10.0 * widthRatio - radiusX + 0.2 || locX <= -10.0 * widthRatio + radiusX)) {
        bunnyDirection = !bunnyDirection;
        velocityX *= -1.0;
      }

    }
    

    velocityY += -0.00098; // Gravity accelerates downwards
    locX += velocityX;
    locY += velocityY * 10 * radiusY; // Position updated by velocity
    lastY = locY; // For checking if the object has stoppped moving in vertical
  }

};
