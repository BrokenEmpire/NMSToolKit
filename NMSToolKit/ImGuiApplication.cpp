#include <Magnum/GL/Renderer.h>
#include "ImGuiApplication.hpp"

namespace NMSToolKit
{
    /****************************************************************************************************/
    ImGuiApplication::ImGuiApplication(const std::string& title, const Arguments& arguments,
                                       const Vector2i& defaultWindowSize) :
        Sdl2Application{title, arguments, defaultWindowSize}
    {
        /* Setup ImGui and ImGuizmo */
        m_ImGuiContext = ImGuiIntegration::Context(Vector2{windowSize()} / dpiScaling(),
                                                   windowSize(), framebufferSize());
        ImGui::StyleColorsDark();

        /* Setup proper blending to be used by ImGui. There's a great chance
           you'll need this exact behavior for the rest of your scene. If not, set
           this only for the drawFrame() call. */
        GL::Renderer::setBlendFunction(GL::Renderer::BlendFunction::SourceAlpha,
                                       GL::Renderer::BlendFunction::OneMinusSourceAlpha);
    }

    /****************************************************************************************************/
    void ImGuiApplication::viewportEvent(ViewportEvent& event)
    {
        Sdl2Application::viewportEvent(event);

        /* Relayout ImGui */
        m_ImGuiContext.relayout(Vector2{event.windowSize()} / event.dpiScaling(),
                                event.windowSize(), event.framebufferSize());
    }

    /****************************************************************************************************/
    void ImGuiApplication::keyPressEvent(KeyEvent& event)
    {
        if (m_ImGuiContext.handleKeyPressEvent(event))
        {
            event.setAccepted(true);
        }
        else
        {
            Sdl2Application::keyPressEvent(event);
            if (!event.isAccepted())
            {
                if (event.key() == KeyEvent::Key::H)
                {
                    m_bShowMenu ^= true;
                    event.setAccepted(true);
                }
            }
        }
    }

    void ImGuiApplication::keyReleaseEvent(KeyEvent& event)
    {
        if (m_ImGuiContext.handleKeyReleaseEvent(event))
        {
            event.setAccepted(true);
        }
    }

    /****************************************************************************************************/
    void ImGuiApplication::mousePressEvent(MouseEvent& event)
    {
        if (m_ImGuiContext.handleMousePressEvent(event))
        {
            event.setAccepted(true);
        }
        else
        {
            Sdl2Application::mousePressEvent(event);
        }
    }

    /****************************************************************************************************/
    void ImGuiApplication::mouseReleaseEvent(MouseEvent& event)
    {
        if (m_ImGuiContext.handleMouseReleaseEvent(event))
        {
            event.setAccepted(true);
        }
    }

    /****************************************************************************************************/
    void ImGuiApplication::mouseMoveEvent(MouseMoveEvent& event)
    {
        if (m_ImGuiContext.handleMouseMoveEvent(event))
        {
            event.setAccepted(true);
        }
        else
        {
            Sdl2Application::mouseMoveEvent(event);
        }
    }

    /****************************************************************************************************/
    void ImGuiApplication::mouseScrollEvent(MouseScrollEvent& event)
    {
        if (m_ImGuiContext.handleMouseScrollEvent(event))
        {
            /* Prevent scrolling the page */
            event.setAccepted(true);
        }
        else
        {
            Sdl2Application::mouseScrollEvent(event);
        }
    }

    /****************************************************************************************************/
    void ImGuiApplication::textInputEvent(TextInputEvent& event)
    {
        if (m_ImGuiContext.handleTextInputEvent(event))
        {
            event.setAccepted(true);
        }
    }

    /****************************************************************************************************/
    void ImGuiApplication::beginFrame()
    {
        m_ImGuiContext.newFrame();
        /* Enable text input, if needed */
        if (ImGui::GetIO().WantTextInput && !isTextInputActive())
        {
            startTextInput();
        }
        else if (!ImGui::GetIO().WantTextInput && isTextInputActive())
        {
            stopTextInput();
        }
    }

    /****************************************************************************************************/
    void ImGuiApplication::endFrame()
    {
        /* Update application cursor */
        m_ImGuiContext.updateApplicationCursor(*this);

        /* Set appropriate states. If you only draw imgui UI, it is sufficient to do this once in the constructor. */
        GL::Renderer::enable(GL::Renderer::Feature::Blending);
        GL::Renderer::disable(GL::Renderer::Feature::FaceCulling);
        GL::Renderer::disable(GL::Renderer::Feature::DepthTest);
        GL::Renderer::enable(GL::Renderer::Feature::ScissorTest);

        m_ImGuiContext.drawFrame();

        /* Reset state. Only needed if you want to draw something else with different state next frame. */
        GL::Renderer::disable(GL::Renderer::Feature::ScissorTest);
        GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
        GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
        GL::Renderer::disable(GL::Renderer::Feature::Blending);
    }

    /****************************************************************************************************/
    void ImGuiApplication::showMenuHeader()
    {
        ImGui::SetNextWindowBgAlpha(0.5f);
        ImGui::Begin("Options", nullptr);
        ImGui::Text("Hide/show menu: H | Exit: ESC");
        ImGui::Text("%3.2f FPS", static_cast<double>(ImGui::GetIO().Framerate));
        ImGui::SameLine(100);
        if (ImGui::Checkbox("VSync", &m_bVsync))
        {
            setSwapInterval(m_bVsync);
        }
        ImGui::Spacing();
        ImGui::Checkbox("Render grid", &m_Grid->enabled());
        if (ImGui::ColorEdit3("Background color", m_BkgColor.data()))
        {
            GL::Renderer::setClearColor(m_BkgColor);
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
    }

    void ImGuiApplication::showMenuFooter(bool bButtonResetCamera)
    {
        if (bButtonResetCamera)
        {
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            if (ImGui::Button("Reset camera"))
            {
                m_Camera->reset();
            }
        }
        ImGui::End();
    }

}