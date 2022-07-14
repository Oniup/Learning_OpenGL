#ifndef __IMGUI_HANDLER_HPP__
#define __IMGUI_HANDLER_HPP__

#include <vector>

#include <glm/glm.hpp>
#include <imgui/imgui.h>

class Application;
class Light;

void InitImGui(Application* app);
void TerminateImGui();
void StartImGui();
void EndImGui();

void LightControllerImGui(bool using_phong_lighting, std::vector<Light>& lights, glm::vec3& ambient_colour, float* radius);

#endif // __IMGUI_HANDLER_HPP__