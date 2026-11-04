#include "App.h"
#include "database.h"
#include "ImageManager.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include <iostream>

int main()
{
    Database db;

    if (!db.Connect("cornell", "Password123"))
    {
        std::cerr << "Database connection failed: "
            << db.GetLastError() << std::endl;

        return 1;
    }

    std::cout << "Connected to MariaDB successfully." << std::endl;

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(
        1280,
        720,
        "Need for Speed",
        nullptr,
        nullptr
    );

    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwMaximizeWindow(window);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    ImageManager imageManager;
    App app(db, imageManager);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        app.Render();

        ImGui::Render();

        int displayWidth;
        int displayHeight;

        glfwGetFramebufferSize(
            window,
            &displayWidth,
            &displayHeight
        );

        glViewport(0, 0, displayWidth, displayHeight);
        glClearColor(0.10f, 0.10f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    imageManager.Clear();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}