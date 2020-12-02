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
		Shaders::Phong m_SphereShader{Shaders::Phong::Flag::ObjectId};
		GL::Mesh m_MeshSphere{NoCreate};
	};
}
#endif // !_NMSTOOLKIT_NMSTOOLKITUI_H