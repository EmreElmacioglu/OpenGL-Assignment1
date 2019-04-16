#include "include/Angel.h"
#include "cube.h"
#include "sphere.h"
#include "bunny.h"
#include "physics.h"
#include <fstream>

//Default ENUM initialization
SHAPES shape = SHAPE_SPHERE;
DRAWING_MODE mode = DRAWING_MODE_WIREFRAME;
bool render = false;

//Gravity simulation variables
GLsizei width=1000, height=1000;
GLfloat widthRatio = 1.0, heightRatio = 1.0;

//Shape objects
Cube cube;
Sphere sphere;
Bunny bunny;
physics_engine engine;

//OpenGL variables
GLuint program;
GLuint buffers[3];
GLint ModelView, Projection;
mat4 model_view, projection;
GLuint vColor1, vColor2, vColor3;


/* OpenGL initialization */

void init() {
  //Create object instances
  sphere.create();
  cube.create();
  bunny.create();
  //Initialize physics engine
  //Initial vertical velocity: 2.0
  //Last parameter is initial vertical velocity. Set between -0.05 and 0.05
  engine.bindValues(-8.0, 8.0, 0.2, 0.0);

  //Generate a vertex arrays for each object
  glGenVertexArrays(3, buffers);
  GLuint buffer1;
  GLuint buffer2;
  GLuint buffer3;

  program = InitShader("vshader.glsl", "fshader.glsl");

  /*Setup VBOs and VBAs here */

  //Sphere
  glBindVertexArray(buffers[0]);

  glGenBuffers(1, &buffer1);
  glBindBuffer(GL_ARRAY_BUFFER, buffer1);
  glBufferData( GL_ARRAY_BUFFER, sizeof(sphere.points) + sizeof(sphere.colors),
                NULL, GL_STATIC_DRAW );
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(sphere.points), sphere.points );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(sphere.points), sizeof(sphere.colors), sphere.colors );

  
  // set up vertex arrays
  GLuint vPosition = glGetAttribLocation( program, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                         BUFFER_OFFSET(0) );

  
  vColor1 = glGetAttribLocation( program, "vColor" );
  glEnableVertexAttribArray( vColor1 );
  glVertexAttribPointer( vColor1, 4, GL_FLOAT, GL_FALSE, 0,
                         BUFFER_OFFSET(sizeof(sphere.points)) );

  //Cube

  glBindVertexArray(buffers[1]);
  // Create and initialize a buffer object
  glGenBuffers( 1, &buffer2);
  glBindBuffer( GL_ARRAY_BUFFER, buffer2);
  glBufferData( GL_ARRAY_BUFFER, sizeof(cube.points) + sizeof(cube.colors),
                NULL, GL_STATIC_DRAW );
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(cube.points), cube.points );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(cube.points), sizeof(cube.colors), cube.colors);

  // set up vertex arrays
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                         BUFFER_OFFSET(0) );

  vColor2 = glGetAttribLocation( program, "vColor" );
  glEnableVertexAttribArray( vColor2 );
  glVertexAttribPointer( vColor2, 4, GL_FLOAT, GL_FALSE, 0,
                         BUFFER_OFFSET(sizeof(cube.points)) );

  //Bunny

  glBindVertexArray(buffers[2]);

  int size = bunny.points.size();
  vec4 arr[size];
  vec4 arr2[size];
  
  for(int i = 0; i < size; i++) {
    arr[i] = bunny.points.at(i);
    arr2[i] = bunny.colors.at(i);
  }

  glGenBuffers(1, &buffer3);
  glBindBuffer(GL_ARRAY_BUFFER, buffer3);
  glBufferData( GL_ARRAY_BUFFER, sizeof(arr) + sizeof(arr2),
                NULL, GL_STATIC_DRAW );
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(arr), arr );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(arr), sizeof(arr2), arr2);

  // set up vertex arrays
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                         BUFFER_OFFSET(0) );

  
  vColor3 = glGetAttribLocation( program, "vColor" );
  glEnableVertexAttribArray( vColor3 );
  glVertexAttribPointer( vColor3, 4, GL_FLOAT, GL_FALSE, 0,
                         BUFFER_OFFSET(sizeof(arr2)) );
  

  // Retrieve transformation uniform variable locations
  ModelView = glGetUniformLocation( program, "model_view" );
  Projection = glGetUniformLocation( program, "projection" );
    
  glEnable( GL_DEPTH_TEST );
  glClearColor( 1.0, 1.0, 1.0, 1.0 );

  glUseProgram( program );
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Responsive projection matrice according to the view dimensions
  projection = Ortho(-10.0 * widthRatio, 10.00 * widthRatio, -10.00 * heightRatio, 10.00 * heightRatio, -10.00, 10.00);

  //Set model view matrices
  //Adds Rotation in Z-axis if the current shape is bunny
  if(shape == SHAPE_BUNNY) {
    int theta;
    if(engine.bunnyDirection) theta = 0;
    else theta = 180;

    model_view = (Translate(engine.locX, engine.locY, 0.0) *
                  Scale(0.08, 0.08, 0.08) *
                  RotateY(90) *
                  RotateX(270) *
                  RotateZ(theta));

  } else {
    model_view = (Translate(engine.locX, engine.locY, 0.0) *
                  RotateX(30) *
                  RotateY(45));  
  }

  if(render)
    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
  glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);

  //Configure drawing of the object according to the current object
  switch(shape) {

  case SHAPE_SPHERE:

    glBindVertexArray(buffers[0]);

    if(mode == DRAWING_MODE_SOLID) {
      for(int i = 0; i<sphere.numOfVertices; i+=3) glDrawArrays(GL_TRIANGLES, i, 3);
    } else if( mode == DRAWING_MODE_WIREFRAME) {
      for(int i = 0; i<sphere.numOfVertices; i+=3) glDrawArrays(GL_LINE_LOOP, i, 3);
    }
    break;

  case SHAPE_CUBE:

    glBindVertexArray(buffers[1]);

    if(mode == DRAWING_MODE_SOLID) {
      glDrawArrays( GL_TRIANGLES, 0, cube.numOfVertices);
    } else if( mode == DRAWING_MODE_WIREFRAME) {
      glDrawArrays( GL_LINE_LOOP, 0, cube.numOfVertices);
    }
    break;

  case SHAPE_BUNNY:

    glBindVertexArray(buffers[2]);

    if(mode == DRAWING_MODE_SOLID) {
      glDrawArrays( GL_TRIANGLES, 0, (bunny.points.size() * 3));
    } else if( mode == DRAWING_MODE_WIREFRAME) {
      glDrawArrays( GL_LINE_LOOP, 0, (bunny.points.size() * 3));
    }
    break;
  }

  glutSwapBuffers();
}

//Timer function is called every 10 milliseconds
//Object's new position is calculated here
void Timer(int value) {
  glutTimerFunc(1000 / 100, Timer, value);

  engine.calculateNewPosition(shape, widthRatio, heightRatio);
  glutPostRedisplay();
}

//Color change methods

//Adapter method colorChange for different object types
void changeColorAdapter(vec4 color) {
  switch(shape) {
  case SHAPE_SPHERE:
    sphere.changeColor(color);
    break;
  case SHAPE_CUBE:
    cube.changeColor(color);
    break;
  case SHAPE_BUNNY:
    bunny.changeColor(color);
    break;
  }
}

//Adapter method colorChangeRGB for different object types
void changeColorRGBAdapter() {
  switch(shape) {
  case SHAPE_SPHERE:
    sphere.changeColorRGB();
    break;
  case SHAPE_CUBE:
    cube.changeColorRGB();
    break;
  case SHAPE_BUNNY:
    bunny.changeColorRGB();
    break;
  }
}

//Menu operations

void main_menu(int choice) {
  /* Do nothing */
}

void object_menu(int choice) {

  switch(choice) {
  case 1:
    shape = SHAPE_SPHERE;
    break;
  case 2:
    shape = SHAPE_CUBE;
    break;
  case 3:
    shape = SHAPE_BUNNY;
    break;
  }
}

void mode_menu(int choice) {

  switch(choice) {
  case 1:
    mode = DRAWING_MODE_SOLID;
    break;
  case 2:
    mode = DRAWING_MODE_WIREFRAME;
    break;
  }
}

//Puts the changed color arrays to the respective subbuffers of VBOs
void applyColorChange() {

  switch(shape) {

  case SHAPE_SPHERE:

    glBufferSubData( GL_ARRAY_BUFFER, sizeof(sphere.points), sizeof(sphere.colors), sphere.colors);
    vColor1 = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor1 );
    glVertexAttribPointer( vColor1, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(sizeof(sphere.points)));
    break;

  case SHAPE_CUBE:

    glBufferSubData( GL_ARRAY_BUFFER, sizeof(cube.points), sizeof(cube.colors), cube.colors);
    vColor2 = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor2 );
    glVertexAttribPointer( vColor2, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(sizeof(cube.points)));
    break;

  case SHAPE_BUNNY:

    int size = bunny.colors.size();
    vec4 arr[size];
    vec4 arr2[size];

    for(int i = 0; i < size; i++) {
      arr[i] = bunny.points.at(i);
      arr2[i] = bunny.colors.at(i);
    }

    glBufferSubData( GL_ARRAY_BUFFER, sizeof(arr), sizeof(arr2), arr2);
    vColor3 = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor3 );
    glVertexAttribPointer( vColor3, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(sizeof(arr)));
    break;
  }
}

void color_menu(int choice) {

  vec4 color;
  switch(choice) {

  case 1:
    color = vec4( 1.0, 0.0, 0.0, 1.0 );
    break;
  case 2:
    color = vec4( 0.0, 1.0, 0.0, 1.0 );
    break;
  case 3:
    color = vec4( 0.0, 0.0, 1.0, 1.0 );
    break;
  case 4:
    color = vec4( 1.0, 1.0, 0.0, 1.0 );
    break;
  case 5:
    color = vec4( 1.0, 0.0, 1.0, 1.0 );
    break;
  case 6:
    color = vec4( 0.0, 1.0, 1.0, 1.0 );
    break;
  case 7:
    color = vec4( 0.0, 0.0, 0.0, 1.0 );
    break;
  }

  changeColorAdapter(color);

  if(choice == 8) {
    changeColorRGBAdapter();
  }

  applyColorChange();
}

void border_menu(int choice) {

  switch(choice) {
  case 1:
    engine.borders = true;
    break;
  case 2:
    engine.borders = false;
    break;
  }
}

void createMenus(){

  int o_menu = glutCreateMenu(object_menu);
  glutAddMenuEntry("Sphere", 1);
  glutAddMenuEntry("Cube", 2);
  glutAddMenuEntry("Bunny", 3);

  int m_menu = glutCreateMenu(mode_menu);
  glutAddMenuEntry("Solid", 1);
  glutAddMenuEntry("Wireframe", 2);

  int c_menu = glutCreateMenu(color_menu);
  glutAddMenuEntry("Red",1);
  glutAddMenuEntry("Green",2);
  glutAddMenuEntry("Blue",3);
  glutAddMenuEntry("Yellow",4);
  glutAddMenuEntry("Magenta",5);
  glutAddMenuEntry("Cyan",6);
  glutAddMenuEntry("Black",7);
  glutAddMenuEntry("RGB",8);

  int b_menu = glutCreateMenu(border_menu);
  glutAddMenuEntry("Border on", 1);
  glutAddMenuEntry("Border off", 2);

  //Main Menu
  glutCreateMenu(main_menu);
  glutAddSubMenu("Shape", o_menu);
  glutAddSubMenu("Drawing mode", m_menu);
  glutAddSubMenu("Color", c_menu);
  glutAddSubMenu("Borders", b_menu);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  
}

void keyboard(unsigned char key, int x, int y)
{
  switch(key) {

  case 'i':
  case 'I':
    render = true;
    engine.reset(widthRatio, heightRatio);
    break;

  case 'h':
  case 'H':
    std::cout << "\n-----Help-----"<< std::endl;
    std::cout << "\n-Press \"i\" key to initialize the object. Once initialized, object will be start free falling with a set vertical velocity." << std::endl;
    std::cout << "\n-Project configurations can be changed using the menu which opens with the click of the right mouse button. Available settings:" << std::endl;
    std::cout << "  *Object shape" << std::endl;
    std::cout << "  *Object drawing mode" << std::endl;
    std::cout << "  *Object color" << std::endl;
    std::cout << "  *Left and right screen border states" << std::endl;
    std::cout << "\n-Object configurations can be made both before and during object movement and changes will take place immediately." << std::endl;
    std::cout << "\n-Pressing 'i' key after object stops if the borders are on and object leaves the screen id the borders are off, as well as while object is moving, will restart the program reinitialize the object." << std::endl;
    std::cout << "\n-Pressing 'q' key will quit the program." << std::endl;
    break;

  case 'q':
  case 'Q':
    exit( EXIT_SUCCESS );
    break;
  }
}

void reshape(GLsizei ww, GLsizei hh)
{
  glViewport(0,0, ww, hh);
  widthRatio = (float)ww/1000;
  heightRatio = (float)hh/1000;
  width = ww;
  height = hh;
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(width, height);
  glutInitContextVersion( 3, 2 );
  glutInitContextProfile( GLUT_CORE_PROFILE );
  
  glutCreateWindow("Color Cube");

  glewExperimental = GL_TRUE;
  glewInit();

  init();
  createMenus();

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  Timer(0);

  glutMainLoop();
  return 0;
}
