#include "App.h"
#include "BrandWindow.h"
#include "TypeWindow.h"
#include "DetailedWindow.h"
#include "EditTypeWindow.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <iostream>

App::App(Database& database)
    : db(database),
    editTypeWindow(database),
    detailedWindow(database, editTypeWindow),
    typeWindow(database, detailedWindow, editTypeWindow),
    brandWindow(database, typeWindow) {
    windows = { &brandWindow, &typeWindow, &detailedWindow, &editTypeWindow };
}

int App::Run() {
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed.\n";
        return 1;
    }

    window = glfwCreateWindow(
        1920, 1080, "OO-Programmeren", nullptr, nullptr
    );

    if (window == nullptr) {
        std::cerr << "Could not create GLFW window.\n";
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        for (Window* screen : windows) {
            screen->Render();
        }

        if (editTypeWindow.TakeChanged()) {          // er is iets toegevoegd, gewijzigd of verwijderd
            for (Window* screen : windows) {
                screen->Reload();                    // elk window laadt zijn gegevens opnieuw
            }
        }

        ImGui::Render();

        int w, h;
        glfwGetFramebufferSize(window, &w, &h);

        glViewport(0, 0, w, h);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(
            ImGui::GetDrawData()
        );

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    window = nullptr;
    glfwTerminate();

    return 0;
}