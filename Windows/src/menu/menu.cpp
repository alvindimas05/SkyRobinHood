#include "menu.hpp"

#include "../dependencies/imgui/imgui.h"
#include "../dependencies/imgui/imgui_impl_win32.h"

// Include shared MVC components
#include "../../../Shared/src/controller/controller.hpp"
#include "../../../Shared/src/model/model.hpp"
#include "../../../Shared/src/view/view.hpp"

namespace ig = ImGui;

namespace Menu {
    // Initialize MVC components
    RHModel* model = nullptr;
    RHController* controller = nullptr;
    RHView* view = nullptr;

    void InitializeContext(HWND hwnd) {
        if (ig::GetCurrentContext( ))
            return;

        ImGui::CreateContext( );
        ImGui_ImplWin32_Init(hwnd);

        ImGuiIO& io = ImGui::GetIO( );
        io.IniFilename = io.LogFilename = nullptr;

        // Initialize shared MVC components
        model = new RHModel( );
        controller = new RHController(*model);
        view = new RHView(*model, *controller);

        model->Init( );
        controller->Init( );
        view->Init( );
    }

    void Render( ) {
        if (!bShowMenu)
            return;

        // Render actual mod UI
        if (view) {
            view->Draw( );
        }
    }
} // namespace Menu
