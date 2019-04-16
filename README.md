# OpenGL-Assignment1
An introductory project to OpenGL which renders a basic bouncing ball.
Ball color and shape can be changed using the pop-up menu bound to the right mouse button.
Pressing 'h' will display the documentation below.
   
HELP
-----
-Press "i" to initialize the object. Once initialized, object will be start free falling with a set vertical velocity.

-Project configurations can be changed using the menu which opens with the click of the right mouse button. Available settings:
-- Object shape
-- Object drawing mode
-- Object color
-- Left and right screen border states

-Object configurations can be made both before and during object movement and changes will take place immediately.

-Pressing 'i' button after object stops if the borders are on and object leaves the screen id the borders are off, as well as while object is moving, will restart the program reinitialize the object.

-Pressing 'q' will quit the program.


Menu Operations
----------------

Menu text not being visible at Ubuntu 16.04 while using freeglut with an integrated GPU is a known bug.
Due to the said bug menu sequence is written below:

1. Shape
   - Sphere
   - Cube
   - Bunny

2. Drawing mode
   - Solid
   - Wireframe

3. Color
   - Red
   - Green
   - Blue
   - Yellow
   - Magenta
   - Cyan
   - Black
   - RGB(Muticolor)

4. Borders
   - Border on
   - Border off
   
   
Include Files:
--------------
Interactive Computer Graphics A Top-Down Approach with OpenGL
Edward Angel and Dave Shreiner Sixth Edition, Addison-Wesley 2012
