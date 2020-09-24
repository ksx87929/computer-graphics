---------------- www.spacesimulator.net --------------
  ---- Space simulators and 3d engine tutorials ----

Author: Damiano Vitulli



--------------------------------------------------------
1.List of commands
--------------------------------------------------------

1.1 SPACESHIP COMMANDS
Arrow keys: Rotate the active object around x,z axis
k,l: Rotate the active object around y axis
j,m: Translate the active object around its z axis
page UP, page DOWN: make active the next object

1.2 CAMERA COMMANDS
w,s: Translate the camera around its z axis
e,c: Rotate the camera around its x axis
a,d: Rotate the camera around its y axis
z,x: Rotate the camera around its z axis

1.3 OTHER COMMANDS
ESC: Exit the program



--------------------------------------------------------
2.Instruction for developers
--------------------------------------------------------

2.1 PROGRAM COMPILATION
To compile this project you must include the following libraries:
For OpenGL: opengl32.lib, glu32.lib 
For the GLUT FRAMEWORK: glut32.lib and glut.h
For the SDL FRAMEWORK: SDL.lib, SDLmain.lib, sdl.h (sdl.h links to other sdl headers)
To use a specific framework please enable the preprocessor directive: FRAMEWORK_x (where x is the name of the framework, for example GLUT = FRAMEWORK_GLUT)



--------------------------------------------------------
3.Known bugs
--------------------------------------------------------

3.1 WinXP and Glut
Under WinXP and Glut Framework when you close the window using the 
controlbox close button, the program continue to run
in background, you can check it using the task manager.
The only way to close safely the program is pressing ESC!
This is a Glut problem, I had not success to solve it.



--------------------------------------------------------
4.Feedback
--------------------------------------------------------

Please use spacesimulator.net discussion forum to ask questions or post comments



--------------------------------------------------------
5.Copyrights
--------------------------------------------------------

5.1 Spacesimulator.net - Zetadeck 
Spacesimulator.net - Damiano Vitulli

5.2 Third party libraries (SDL)
SDL - Simple DirectMedia Layer - Sam Lantinga - www.libsdl.org
This library is distributed under the terms of the GNU LGPL license:
http://www.gnu.org/copyleft/lesser.html

5.3 3D Models
fighter1.3ds - created by: Dario Vitulli 
fighter2.3ds - taken from http://www.3dcafe.com/asp/meshes.asp

5.4 File formats
3DS - 3D Studio Copyright Autodesk

5.5 Trademarks
All the trademarks mentioned in this program are property of their respective owners.



--------------------------------------------------------
6.Disclaimer
--------------------------------------------------------

This program is released under the BSD licence
By using this program you agree to licence terms on spacesimulator.net copyright page
