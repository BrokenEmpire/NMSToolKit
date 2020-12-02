#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/Primitives/Grid.h>

#include "Grid.hpp"

namespace NMSToolKit
{
    Grid::Grid(Scene3D* const scene, SceneGraph::DrawableGroup3D* const drawableGroup)
    {
        using namespace Magnum::Math::Literals;
        m_Mesh = MeshTools::compile(Primitives::grid3DWireframe({20, 20}));

        m_Obj3D.emplace(scene);
        m_Obj3D->scale(Vector3(10.0f));
        m_Obj3D->rotateX(90.0_degf);

        m_FlatShader = Shaders::Flat3D{};
        m_DrawableObj.emplace(*m_Obj3D.get(), m_FlatShader, Color3(0.75f), m_Mesh, drawableGroup);
    }
}