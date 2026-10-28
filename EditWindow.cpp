#include "EditWindow.h"
#include "imgui.h"

EditWindow::EditWindow(Database& database, const std::string& windowTitle)
    : Window(windowTitle), db(database) {
    closable = true;
    open = false;                       // hidden until a ShowNew / ShowEdit call
}

bool EditWindow::TakeChanged() {
    bool changed = dataChanged;
    dataChanged = false;                // only report a change once
    return changed;
}

void EditWindow::DrawContent() {
    DrawFields();                       // the derived class draws its own fields
    ImGui::Separator();

    if (!errorMessage.empty()) {
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "%s", errorMessage.c_str());
    }

    if (ImGui::Button("Opslaan")) {
        errorMessage.clear();
        if (Save()) {                   // on failure Save() fills errorMessage itself
            dataChanged = true;
            open = false;
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Annuleren")) {
        open = false;
    }

    if (CanDelete()) {
        ImGui::SameLine();
        if (ImGui::Button("Verwijderen")) {
            ImGui::OpenPopup("Bevestig verwijderen");
        }
    }

    // Confirmation, because deleting cannot be undone
    if (ImGui::BeginPopupModal("Bevestig verwijderen", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Weet je zeker dat je dit wilt verwijderen?");

        if (ImGui::Button("Ja, verwijder")) {
            errorMessage.clear();
            if (Delete()) {
                dataChanged = true;
                open = false;
            }
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Nee")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}