#include <atomic>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>


#include <Corrade/Containers/GrowableArray.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Primitives/Icosphere.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Primitives/Cylinder.h>
#include <Magnum/Primitives/UVSphere.h>
#include <Magnum/Trade/MeshData.h>

#include "NMSToolKitUI.hpp"

namespace NMSToolKit
{
	NMSToolKitUI::NMSToolKitUI(const Arguments& arguments) : PickableApplication{"No Man's Sky ToolKit", arguments}
	{
		m_MeshSphere = MeshTools::compile(Primitives::uvSphereSolid(16,32));
		m_MeshFloor = MeshTools::compile(Primitives::cubeSolid());
		m_MeshWall = MeshTools::compile(Primitives::cubeSolid());
		m_MeshWall_Q = MeshTools::compile(Primitives::cubeSolid());
		m_MeshExtractor = MeshTools::compile(Primitives::cubeSolid());

		const char* fileName = "C:\\Users\\dhr\\Desktop\\items.csv";
		std::ifstream infile(fileName);

		uint64_t timeStamp;
		char c;
		std::string objectId;
		double p0, u0, a0;
		double p1, u1, a1;
		double p2, u2, a2;
		
		int i = 0;
		int n = 0;

		m_wallScale = Vector3(1.0f, 1.0f, 1.0f);
		m_qwallScale = Vector3(1.0f, 1.0f, 1.0f);
		m_floorScale = Vector3(1.0f, 1.0f, 1.0f);
		m_extractorScale = Vector3(1.0f, 1.0f, 1.0f);
		m_homeScale = Vector3(0.05f);

		while ((infile >> std::setprecision(std::numeric_limits<long double>::digits10 + 3)
				>> timeStamp >> c
				>> p0 >> c
				>> p1 >> c
				>> p2 >> c
				>> u0 >> c
				>> u1 >> c
				>> u2 >> c
				>> a0 >> c
				>> a1 >> c
				>> a2 >> c
				>> objectId) && (c == ','))
		{
			std::cout << std::setprecision(std::numeric_limits<long double>::digits10 + 3)
				<< "Timestamp: " << timeStamp << std::endl
				<< "Object Id: " << objectId << std::endl
				<< "Position: " << p0 << ", " << p1 << ", " << p2 << std::endl
				<< "Up: " << u0 << ", " << u1 << ", " << u2 << std::endl
				<< "At: " << a0 << ", " << a1 << ", " << a2 << std::endl
				<< std::endl;

			const Vector3 position = Vector3(p0, p1, p2);

			arrayAppend(m_Points, Containers::InPlaceInit, position);

			if (objectId == "^C_WALL")
			{
				arrayAppend(m_DrawablePoints, Containers::InPlaceInit, new PickableObject{ m_WallShader, Color3::red(), m_MeshWall, &m_Scene, &m_Drawables});
				m_DrawablePoints[i]->setTransformation(Matrix4::translation(m_Points[i]) * Matrix4::scaling(m_wallScale));
			}
			else if (objectId == "^C_WALL_Q")
			{
				arrayAppend(m_DrawablePoints, Containers::InPlaceInit, new PickableObject{ m_WallShader, Color3::green(), m_MeshWall_Q, &m_Scene, &m_Drawables});
				m_DrawablePoints[i]->setTransformation(Matrix4::translation(m_Points[i]) * Matrix4::scaling(m_qwallScale));
			}
			else if (objectId == "^C_FLOOR")
			{
				arrayAppend(m_DrawablePoints, Containers::InPlaceInit, new PickableObject{ m_FloorShader, Color3::blue(), m_MeshFloor, &m_Scene, &m_Drawables});
				m_DrawablePoints[i]->setTransformation(Matrix4::translation(m_Points[i]) * Matrix4::scaling(m_floorScale));
			}
			else if (objectId == "^U_EXTRACTOR_S")
			{
				arrayAppend(m_DrawablePoints, Containers::InPlaceInit, new PickableObject{ m_ExtractorShader, Color3::cyan(), m_MeshExtractor, &m_Scene, &m_Drawables});
				m_DrawablePoints[i]->setTransformation(Matrix4::translation(m_Points[i]) * Matrix4::scaling(m_extractorScale));
			}
			else
			{
				arrayAppend(m_DrawablePoints, Containers::InPlaceInit, new PickableObject { m_SphereShader, Color3::yellow(), m_MeshSphere, &m_Scene, &m_Drawables});
				m_DrawablePoints[i]->setTransformation(Matrix4::translation(m_Points[i]) * Matrix4::scaling(m_homeScale));
			}
			
			m_mDrawableIdxToPointIdx[m_DrawablePoints[i]->idx()] = i;
			i++;
		}

		infile.close();
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
			ImGui::Text("Test:");
			if (ImGui::InputFloat3("Position", m_wallScale.data(), 3))
			{

			};
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
					
					selectedPoint->setTransformation(Matrix4::translation(objMat.translation()) * Matrix4::scaling(m_wallScale));      /* Update drawable transformation */
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