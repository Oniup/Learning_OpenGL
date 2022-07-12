#include <app/imgui_handler.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui_internal.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <app/application.hpp>
#include <app/light.hpp>

#include <stdio.h>
#include <stdint.h>
#include <string>

static Application* current_app = nullptr;

void InitImGui(Application* app)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  ImGui::StyleColorsDark();

  current_app = app;

  ImGui_ImplGlfw_InitForOpenGL(current_app->GetNativeWindow(), true);
  ImGui_ImplOpenGL3_Init("#version 450");
}

void TerminateImGui()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void StartImGui()
{
  ImGui_ImplGlfw_NewFrame();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui::NewFrame();
}

void EndImGui()
{
  ImGuiIO& io = ImGui::GetIO();
  int width, height;
  glfwGetFramebufferSize(current_app->GetNativeWindow(), &width, &height);
  io.DisplaySize = ImVec2(width, height);
  
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
    GLFWwindow* backup_window = current_app->GetNativeWindow();

    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_window);
  }
}

void LightControllerImGui(bool using_phong_lighting, std::vector<Light>& lights, glm::vec3& ambient_colour, float* radius)
{
  const char* panel_name = using_phong_lighting ? "Phong Lighting Controller" : "Gouraud Lighting Controller";
  if (ImGui::CollapsingHeader(panel_name))
  {
    ImGui::Indent();

    ImGui::ColorEdit3("World Ambient Colour", &ambient_colour[0]);
    ImGui::InputFloat("Spinning Light Radius", radius);

    for (int i = 0; i < lights.size(); i++)
    {
      std::string name = "Light" + std::to_string(i);
      std::string title_name = name + ": ";
      switch (lights[i].type)
      {
        case LIGHT_TYPE_POINT:
          title_name += "Point";
          break;
        case LIGHT_TYPE_DIRECTIONAL:
          title_name += "Directional";
          break;
        case LIGHT_TYPE_SPOT:
          title_name += "Spot";
          break;
      }

      if (ImGui::CollapsingHeader(title_name.c_str()))
      {
        ImGui::ColorEdit3(std::string(name + "ambient").c_str(), &lights[i].ambient[0]);
        ImGui::ColorEdit3(std::string(name + "diffuse").c_str(), &lights[i].diffuse[0]);
        ImGui::ColorEdit3(std::string(name + "specular").c_str(), &lights[i].specular[0]);
        ImGui::Spacing();
        ImGui::InputFloat3(std::string(name + "position").c_str(), &lights[i].transform.position[0]);
        if (lights[i].type != LIGHT_TYPE_POINT)        
          ImGui::InputFloat3(std::string(name + "direction").c_str(), &lights[i].direction[0]);
        if (lights[i].type == LIGHT_TYPE_SPOT)
          ImGui::InputFloat(std::string(name + "cut off").c_str(), &lights[i].spot_end_fade);
      }
    }
  }
}
