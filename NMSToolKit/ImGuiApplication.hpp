#ifndef _NMSTOOLKIT_IMAPPLICATION_H
#define _NMSTOOLKIT_IMAPPLICATION_H

#include <Magnum/ImGuiIntegration/Context.hpp>
#include "Sdl2Application.hpp"

namespace NMSToolKit
{
    class ImGuiApplication : public Sdl2Application
    {
    public:
        explicit ImGuiApplication(const std::string& title, const Arguments& arguments, const Vector2i& defaultWindowSize = Vector2i{1920, 1080});

    protected:
        void viewportEvent(ViewportEvent& event) override;
        void keyPressEvent(KeyEvent& event) override;
        void keyReleaseEvent(KeyEvent& event) override;
        void mousePressEvent(MouseEvent& event) override;
        void mouseReleaseEvent(MouseEvent& event) override;
        void mouseMoveEvent(MouseMoveEvent& event) override;
        void mouseScrollEvent(MouseScrollEvent& event) override;
        void textInputEvent(TextInputEvent& event) override;

        void beginFrame();
        void endFrame();

        void showMenuHeader();
        void showMenuFooter(bool bButtonResetCamera = true);

        /* Window control */
        bool m_bShowMenu{true};
        ImGuiIntegration::Context m_ImGuiContext{NoCreate};
    };
}

#endif // !_NMSTOOLKIT_IMAPPLICATION_H