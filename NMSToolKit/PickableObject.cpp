#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/GL/Mesh.h>

#include "PickableObject.hpp"

namespace NMSToolKit
{
    PickableObject::PickableObject(Shaders::Phong& shader, const Color3& color,
                                   GL::Mesh& mesh, Scene3D* parent,
                                   SceneGraph::DrawableGroup3D* drawables) :
        Object3D{parent},
        SceneGraph::Drawable3D{*this, drawables},
        m_bSelected{false},
        m_idx{getUniqueID()},
        m_Shader(shader),
        m_Color{color},
        m_Mesh(mesh) {
        /* Register this object into the global list */
        s_GeneratedObjs.push_back(this);
    }

    /****************************************************************************************************/
    PickableObject::~PickableObject()
    {
        /* Remove this object from the global list */
        for (size_t i = 0; i < s_GeneratedObjs.size(); ++i)
        {
            if (s_GeneratedObjs[i] == this)
            {
                s_GeneratedObjs[i] = s_GeneratedObjs.back();
                s_GeneratedObjs.resize(s_GeneratedObjs.size() - 1);
                break;
            }
        }
    }

    /****************************************************************************************************/
    void PickableObject::draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera)
    {
        Matrix4 directionalLight;

        m_Shader.setTransformationMatrix(transformationMatrix)
            .setNormalMatrix(transformationMatrix.normalMatrix())
            .setProjectionMatrix(camera.projectionMatrix())
            .setAmbientColor(m_bSelected ? m_Color * 0.3f : Color3{})
            .setDiffuseColor(m_Color * (m_bSelected ? 2.0f : 1.0f))
            /* relative to the camera */
            .setLightPositions({Vector4{directionalLight.up(), 0.0f}})
            .setObjectId(m_idx)
            .draw(m_Mesh);
    }

    /****************************************************************************************************/
    void PickableObject::updateSelectedObject(uint32_t selectedIdx)
    {
        s_SelectedObj = nullptr;
        for (size_t i = 0; i < s_GeneratedObjs.size(); ++i)
        {
            s_GeneratedObjs[i]->setSelected((selectedIdx == s_GeneratedObjs[i]->idx())
                                            && s_GeneratedObjs[i]->isSelectable());
            if (selectedIdx == s_GeneratedObjs[i]->idx())
            {
                s_SelectedObj = s_GeneratedObjs[i];
                break;
            }
        }
    }

    /****************************************************************************************************/
    uint32_t PickableObject::getUniqueID()
    {
        static uint32_t s_nObjects = 0;
        return ++s_nObjects;
    }
}