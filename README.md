# Learning more Complex stuff with OpenGL

Purpose of this is to learn OpenGL for 3D applications, this will include more complex rendering techniques that
doesn't just involve rendering 2D planes. The knowledge gained from this will be applied to Safire Engine in the future

from https://learnopengl.com

# Version History


* ***(3/07/2022)***
  * Implemented specular
  * Implemented the Gouraud version of the shader
  * Moved the ImGui Window for Control outside of the Light part of the controller so we can add more to it later
  * Implemented basic material in both Gouraud and Phong Lighting
* ***(1/07/2022)***
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
