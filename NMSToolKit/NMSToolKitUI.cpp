#include <Corrade/Containers/GrowableArray.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Primitives/Icosphere.h>
#include <Magnum/Trade/MeshData.h>

#include "NMSToolKitUI.hpp"

namespace NMSToolKit
{
	NMSToolKitUI::NMSToolKitUI(const Arguments& arguments) : PickableApplication{"No Man's Sky ToolKit", arguments}
	{


		m_MeshSphere = MeshTools::compile(Primitives::icosphereSolid(3));

		for (size_t i = 0; i < 8; ++i)
		{
			const Vector3 tmpPos = Vector3(std::rand(), std::rand(), std::rand()) / Float(RAND_MAX);

			arrayAppend(m_Points, Containers::InPlaceInit, tmpPos * 2.0f - Vector3{1.0f});
			arrayAppend(m_DrawablePoints, Containers::InPlaceInit, new PickableObject{ m_SphereShader, Color3{ tmpPos }, m_MeshSphere, &m_Scene, &m_Drawables });

			m_Points[i].y() += 1.0f;
			m_DrawablePoints[i]->setTransformation(Matrix4::translation(m_Points[i]) * Matrix4::scaling(Vector3(0.05f)));
			m_mDrawableIdxToPointIdx[m_DrawablePoints[i]->idx()] = i;
		}
	}

	void NMSToolKitUI::drawEvent()
	{
		GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);
		ImGuiApplication::beginFrame();

		/* Update camera */
		m_Camera->update();

		/* Draw to custom framebuffer */
		m_FrameBuffer
			.clearColor(0, m_BkgColor)
			.clearColor(1, Vector4ui{})
			.clearDepth(1.0f)
			.bind();

		/* Draw other objects (grid) */
		m_Camera->draw(m_Drawables);

		/* Bind the default framebuffer back, as only the clickable objects need to be render to custom framebuffer */
		GL::defaultFramebuffer.bind();

		/* Blit color to window framebuffer */
		m_FrameBuffer.mapForRead(GL::Framebuffer::ColorAttachment{0});
		GL::AbstractFramebuffer::blit(m_FrameBuffer, GL::defaultFramebuffer,
									  {{}, m_FrameBuffer.viewport().size()}, GL::FramebufferBlit::Color);

		/* Menu for controllers */
		if (m_bShowMenu)
		{
			showMenuHeader();
			showMenuFooter();
		}

		/* Manipulate nodes' transformation */
		PickableObject* selectedPoint = PickableObject::selectedObj();
		if (selectedPoint)
		{
			if (selectedPoint->isSelectable()
				&& selectedPoint->isMovable())
			{
				ImGui::Begin("Editor");
				std::string str = "Point: #" + std::to_string(selectedPoint->idx());
				ImGui::Text("%s", str.c_str());
				ImGui::Separator();
				ImGui::Spacing();

				Matrix4 objMat = selectedPoint->transformation();
				if (editPointTransformation(objMat))
				{
					selectedPoint->setTransformation(objMat);                       /* Update drawable transformation */
					setPointTransformation(selectedPoint->idx(), objMat, m_Points); /* Update real data point */
				}
				ImGui::End();
			}
		}

		ImGuiApplication::endFrame();
		swapBuffers();
		redraw();
	}
}

MAGNUM_APPLICATION_MAIN(NMSToolKit::NMSToolKitUI)