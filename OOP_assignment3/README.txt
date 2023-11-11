Name : Kweon Ki Yeong
StudentID : 20222663
Environment : windows10, Visual Studio 2022
Complie : open VirtualLego.sln file, and press f5 button to run. If needed, set the environment variable PATH of DirectX SDK correctly.

Execution : click and holding LEFT or RIGHT MOUSE BUTTON to move the white ball. Press SPACE key to shoot the red ball.

Summary of Code Modification :
Implement the collision between two spheres and collision between a sphere and a wall.
YELLOW balls implemented as array, RED and WHITE ball implemented as SINGLETON.
WHITE ball is moved by the mouse click(holding).
The plane size is 9 * 14, and there are 36 yellow balls.
The life count is 5. If there are no remaining ball or life count is zero, the program is exited.
I remove the function of screen moving and decrease rate(fraction) and etc. ARKANOID game does not need that functions.
