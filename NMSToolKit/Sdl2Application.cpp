#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Context.h>
#include <Magnum/GL/Version.h>

#include "Sdl2Application.hpp"
#include "ImGuizmo.hpp"

namespace NMSToolKit
{
	using namespace Math::Literals;

	Sdl2Application::Sdl2Application(const std::string& title, const Arguments& arguments, const Vector2i& defaultWindowSize) : Platform::Application{arguments, NoCreate}
	{
		Debug suppressOutput{nullptr};
		(void)suppressOutput;

		create(Configuration{}
			   .setTitle(title)
			   .setSize(defaultWindowSize)
			   .setWindowFlags(Configuration::WindowFlag::Resizable),
			   GLConfiguration{}.setSampleCount(this->dpiScaling({}).max() < 2.0f ? 8 : 2));

		MAGNUM_ASSERT_GL_VERSION_SUPPORTED(GL::Version::GL330);
		GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
		GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
		GL::Renderer::setClearColor(m_BkgColor);
		setSwapInterval(1);

		m_Grid.emplace(&m_Scene, &m_Drawables);
		setupCamera();
	}

	void Sdl2Application::viewportEvent(ViewportEvent& event)
	{
		const auto newBufferSize = event.framebufferSize();

		GL::defaultFramebuffer.setViewport({{}, newBufferSize});

		m_Camera->reshape(event.windowSize(), event.framebufferSize());
	}

	void Sdl2Application::keyPressEvent(KeyEvent& event)
	{
		switch (event.key())
		{
			case  KeyEvent::Key::V:
				m_bVsync ^= true;
				setSwapInterval(m_bVsync);
				event.setAccepted(true);
				break;
			case KeyEvent::Key::S:
				if (m_Camera->lagging() > 0.0f)
				{
					Debug{} << "Disable camera smooth navigation";
					m_Camera->setLagging(0.0f);
				}
				else
				{
					Debug{} << "Enable camera smooth navigation";
					m_Camera->setLagging(0.85f);
				}
				break;
			case KeyEvent::Key::R:
				m_Camera->reset();
				break;
			case KeyEvent::Key::Esc:
				exit(0);
		}
	}

	void Sdl2Application::mousePressEvent(MouseEvent& event)
	{
		m_Camera->initTransformation(event.position());
	}

	void Sdl2Application::mouseMoveEvent(MouseMoveEvent& event)
	{
		if (!event.buttons())
		{
			return;
		}
		if (event.buttons() & MouseMoveEvent::Button::Left)
		{
			m_Camera->rotate(event.position());
		}
		else
		{
			m_Camera->translate(event.position());
		}
		event.setAccepted();
	}

	void Sdl2Application::mouseScrollEvent(MouseScrollEvent& event)
	{
		const float delta = event.offset().y();
		if (std::abs(delta) < 1.0e-2f)
		{
			return;
		}
		m_Camera->zoom(delta);
		event.setAccepted();
	}

	void Sdl2Application::setupCamera()
	{
		m_Camera.emplace(m_Scene, m_DefaultCamPosition, m_DefaultCamTarget, Vector3::yAxis(), 45.0_degf, windowSize(), framebufferSize());
		m_Camera->setLagging(0.85f);
	}
}