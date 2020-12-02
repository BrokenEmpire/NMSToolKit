#ifndef _NMSTOOLKIT_PICKABLEAPPLICATION_H
#define _NMSTOOLKIT_PICKABLEAPPLICATION_H

#include <unordered_map>
#include <Magnum/Magnum.h>
#include <Magnum/PixelFormat.h>
#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Renderbuffer.h>
#include <Magnum/GL/RenderbufferFormat.h>

#include "ImGuiApplication.hpp"

namespace NMSToolKit
{
	class PickableApplication : public ImGuiApplication
	{
	public:
		explicit PickableApplication(const std::string& title, const Arguments& arguments, size_t indexDataSize = 16, const Vector2i& defaultWindowSize = Vector2i{1366, 720});

	protected:
		void viewportEvent(ViewportEvent& event) override;
		void mousePressEvent(MouseEvent& event) override;

		bool editPointTransformation(Matrix4& objMat);
		void setPointTransformation(size_t selectedObjID, const Matrix4& objMat, Containers::Array<Vector3>& points);

		/* Object index format */
		GL::RenderbufferFormat m_IndexFormat{GL::RenderbufferFormat::R16UI};
		PixelFormat m_PixelFormat{PixelFormat::R16UI};
		std::unordered_map<uint32_t, size_t> m_mDrawableIdxToPointIdx;

		/* Framebuffer and render bufers */
		GL::Framebuffer  m_FrameBuffer{NoCreate};
		GL::Renderbuffer m_RBColor{NoCreate};
		GL::Renderbuffer m_RBObjectIdx{NoCreate};
		GL::Renderbuffer m_RBDepth{NoCreate};
	};
}

#endif // !_NMSTOOLKIT_PICKABLEAPPLICATION_H