#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "mavlink_connection.hpp"

int main() {

    char connection_url[20] {};
    bool is_connected = false;

    if (!glfwInit())
        return -1;

    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui Window", NULL, NULL);
    if (window == NULL)
        return -1;

    glfwMakeContextCurrent(window);
    // glfwSwapInterval(1); // Vsync

    // ImGui Contexti Başlatma
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Platform ve Renderer bağlama
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Stil ayarlama
    ImGui::StyleColorsDark();

    MavlinkConnection* mavlink_connection = MavlinkConnection::get_singleton();

    // Ana döngü
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Yeni bir ImGui frame'i başlatma
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowSize(ImVec2(400,100));
        ImGui::SetNextWindowPos(ImVec2(10,10));

        if (ImGui::Begin("Mavlink Access", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove))
        {
            ImGui::InputText("Connection url", connection_url, 20);
            if(ImGui::Button("Connect"))
            {
                if (mavlink_connection->init(connection_url)) {
                    is_connected = true;
                }
            }
            ImGui::End();
        }

        ImGui::SetNextWindowSize(ImVec2(400,500));
        ImGui::SetNextWindowPos(ImVec2(10,120));
        if (ImGui::Begin("Info", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove))
        {
            if (is_connected) {
                ImGui::Text("Connected");
                if (mavlink_connection->status_text()) 
                {
                    const char* status_text = mavlink_connection->status_text();
                    ImGui::Text("%s", status_text);
                }
            } else {
                ImGui::Text("Not Connected");
            }
            
            ImGui::End();
        }

        // Render
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Clean
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}