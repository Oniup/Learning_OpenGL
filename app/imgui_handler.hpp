#ifndef __IMGUI_HANDLER_HPP__
#define __IMGUI_HANDLER_HPP__

#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <vector>

class Application;
class Light;

void init_imgui(Application* app);
void terminate_imgui();
void start_imgui();
void end_imgui();

void light_controller_imgui(glm::vec3& ambient_colour, std::vector<Light>& lights);

#endif // __IMGUI_HANDLER_HPP__