#ifndef _NMSTOOLKIT_NMSTOOLKITUI_H
#define _NMSTOOLKIT_NMSTOOLKITUI_H

#include <Corrade/Containers/Array.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/Phong.h>

#include "PickableApplication.hpp"
#include "PickableObject.hpp"

namespace NMSToolKit
{
	class NMSToolKitUI final : public PickableApplication
	{
	public:
		explicit NMSToolKitUI(const Arguments& arguments);

		void drawEvent() override;

	private:
		Containers::Array<Vector3> m_Points;
		Containers::Array<PickableObject*> m_DrawablePoints;
		Containers::Array<PickableObject*> m_DrawableFloors;
		Containers::Array<PickableObject*> m_DrawableWalls;
		Containers::Array<PickableObject*> m_DrawableQWalls;
		Containers::Array<PickableObject*> m_DrawableExtractors;

		Shaders::Phong m_SphereShader { Shaders::Phong::Flag::ObjectId };
		Shaders::Phong m_WallShader { Shaders::Phong::Flag::ObjectId };
		Shaders::Phong m_FloorShader { Shaders::Phong::Flag::ObjectId };
		Shaders::Phong m_ExtractorShader { Shaders::Phong::Flag::ObjectId };

		GL::Mesh m_MeshSphere { NoCreate };
		GL::Mesh m_MeshFloor { NoCreate };
		GL::Mesh m_MeshWall { NoCreate };
		GL::Mesh m_MeshWall_Q { NoCreate };
		GL::Mesh m_MeshExtractor { NoCreate };

		Vector3 m_wallScale;
		Vector3 m_qwallScale;
		Vector3 m_floorScale;
		Vector3 m_extractorScale;
		Vector3 m_homeScale;

	};
}
#endif // !_NMSTOOLKIT_NMSTOOLKITUI_H