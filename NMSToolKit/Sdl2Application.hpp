#ifndef _NMSTOOLKIT_SDL2APPLICATION_H
#define _NMSTOOLKIT_SDL2APPLICATION_H

#include <Magnum/Platform/Sdl2Application.h>

#include "ArcBallCamera.hpp"
#include "Grid.hpp"

namespace NMSToolKit
{
    using namespace Magnum;
    class Sdl2Application : public Platform::Application
    {
    public:
        explicit Sdl2Application(const std::string& title, const Arguments& arguments,
                                 const Vector2i& defaultWindowSize = Vector2i{1920, 1080});

        virtual ~Sdl2Application() { }

    protected:
        void viewportEvent(ViewportEvent& event) override;
        void keyPressEvent(KeyEvent& event) override;
        void mousePressEvent(MouseEvent& event) override;
        void mouseMoveEvent(MouseMoveEvent& event) override;
        void mouseScrollEvent(MouseScrollEvent& event) override;

        void setupCamera();

        bool m_bVsync{true};
        Color3 m_BkgColor{0.35f};

        /* Scene and drawable group */
        Scene3D m_Scene;
        SceneGraph::DrawableGroup3D m_Drawables;

        /* Ground grid */
        Containers::Pointer<Grid> m_Grid;

        /* Camera helpers */
        Vector3 m_DefaultCamPosition{0.0f, 1.5f, 8.0f};
        Vector3 m_DefaultCamTarget{0.0f, 1.0f, 0.0f};
        Containers::Pointer<ArcBallCamera> m_Camera;
    };
}

#endif // !_NMSTOOLKIT_SDL2APPLICATION_H