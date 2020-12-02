#ifndef _NMSTOOLKIT_GRID_H
#define _NMSTOOLKIT_GRID_H

#include <Corrade/Containers/Pointer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include "FlatShaderObject.hpp"

namespace NMSToolKit
{
    using namespace Magnum;

    class Grid
    {
    public:
        explicit Grid(Scene3D* const scene, SceneGraph::DrawableGroup3D* const drawableGroup);

        Grid& setColor(const Color3& color)
        {
            m_DrawableObj->color() = color; return *this;
        }
        Grid& setEnable(bool bStatus)
        {
            m_DrawableObj->setEnable(bStatus); return *this;
        }
        Grid& transform(const Matrix4& transMatrix)
        {
            m_Obj3D->transform(transMatrix); return *this;
        }

        Color3& color()
        {
            return m_DrawableObj->color();
        }
        bool& enabled()
        {
            return m_DrawableObj->enabled();
        }

    private:
        GL::Mesh                             m_Mesh{NoCreate};
        Shaders::Flat3D                      m_FlatShader{NoCreate};
        Containers::Pointer<Object3D>        m_Obj3D;
        Containers::Pointer<FlatShadeObject> m_DrawableObj;
    };
}

#endif // !_NMSTOOLKIT_GRID_H