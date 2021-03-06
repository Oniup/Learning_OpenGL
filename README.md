# Learning more Complex stuff with OpenGL

Purpose of this is to learn OpenGL for 3D applications, this will include more complex rendering techniques that
doesn't just involve rendering 2D planes. The knowledge gained from this will be applied to Safire Engine in the future

from https://learnopengl.com

## build

```bash
~ ❯ git clone https://github.com/Oniup/Learning_OpenGL.git
~ ❯ cd Learning_OpenGL 
~/Learning_OpenGL master ❯ mkdir bin
~/Learning_OpenGL master ❯ cd bin 
~/Learning_OpenGL/bin master ❯ cmake ../
```

***if using linux:***

```bash
~/Learning_OpenGL/bin master ❯ make
```

***if using windows*** then you will have to open the sln file through visual studio community/pro. Then build using Ctr + b. Then you can find the .exe and run the program

## Version History

* ***(07/07/2022)***
  * smooth cutoff of spot lights
* ***(04/07/2022)***
  * More complex light colours, diffuse, specular and ambient colour values
  * Directional light implementation
  * Saved the Latest ImGui config if you want to copy it over when running the program
  * Added light attenuation
* ***(03/07/2022)***
  * Implemented specular
  * Implemented the Gouraud version of the shader
  * Moved the ImGui Window for Control outside of the Light part of the controller so we can add more to it later
  * Implemented basic material in both Gouraud and Phong Lighting
  * added build instructions in README.md
* ***(01/07/2022)***
  * Implement diffuse lighting
  * Fixed rendering everything in view space so calculating specular lighting later on will be easier
  * Implemented imgui using the docking branch so you can pull the imgui window out of the main window
    * Can change the colour of the lights with it
* ***(28/06/2022)***
  * Added a floor
  * Updated the textures so that they have a specular and diffuse textures
  * Light source structure
  * Added two light sources (both don't actually imit light)
    * Shader for the light source objects
    * One rotates around the cube
  * Ambient light implementation
  * Organised the code a little, transform, camera and light have their own .hpp files
  * Proper camera controller
    * Shift to move faster
* ***(27/06/2022)***
  * Initialise the repo
  * 3D cube that rotates
  * Camera can move up and down on the x and y axises 
