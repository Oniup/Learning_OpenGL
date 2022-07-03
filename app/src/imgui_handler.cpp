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

void init_imgui(Application* app)
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

  ImGui_ImplGlfw_InitForOpenGL(current_app->native_window(), true);
  ImGui_ImplOpenGL3_Init("#version 450");
}

void terminate_imgui()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void start_imgui()
{
  ImGui_ImplGlfw_NewFrame();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui::NewFrame();
}

void end_imgui()
{
  ImGuiIO& io = ImGui::GetIO();
  int width, height;
  glfwGetFramebufferSize(current_app->native_window(), &width, &height);
  io.DisplaySize = ImVec2(width, height);
  
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
    GLFWwindow* backup_window = current_app->native_window();

    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_window);
  }
}

void light_controller_imgui(bool using_phong_lighting, std::vector<Light>& lights)
{
  const char* panel_name = using_phong_lighting ? "Phong Lighting Controller" : "Gouraud Lighting Controller";
  if (ImGui::CollapsingHeader(panel_name))
  {
    ImGui::Indent();

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
        if (lights[i].type == LIGHT_TYPE_DIRECTIONAL)
          ImGui::ColorEdit3(std::string(name + "ambient").c_str(), &lights[i].ambient[0]);
        ImGui::ColorEdit3(std::string(name + "diffuse").c_str(), &lights[i].diffuse[0]);
        ImGui::ColorEdit3(std::string(name + "specular").c_str(), &lights[i].specular[0]);
        ImGui::Spacing();
        ImGui::InputFloat3(std::string(name + "position").c_str(), &lights[i].transform.position[0]);
        if (lights[i].type != LIGHT_TYPE_POINT)        
          ImGui::InputFloat3(std::string(name + "direction").c_str(), &lights[i].direction[0]);
      }
    }
  }
}
