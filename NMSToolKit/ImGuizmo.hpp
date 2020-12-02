#ifndef _NMSTOOLKIT_IMGUIZMO_H
#define _NMSTOOLKIT_IMGUIZMO_H

#include <Magnum/ImGuiIntegration/Context.hpp>

namespace NMSToolKit
{
    namespace ImGuizmo
    {
        // call inside your own window and before Manipulate() in order to draw gizmo to that window.
        // Or pass a specific ImDrawList to draw to (e.g. ImGui::GetForegroundDrawList()).
        IMGUI_API void SetDrawlist(ImDrawList* drawlist = nullptr);

        // call BeginFrame right after ImGui_XXXX_NewFrame();
        IMGUI_API void BeginFrame();

        // return true if mouse cursor is over any gizmo control (axis, plan or screen component)
        IMGUI_API bool IsOver();

        // return true if mouse IsOver or if the gizmo is in moving state
        IMGUI_API bool IsUsing();

        // enable/disable the gizmo. Stay in the state until next call to Enable.
        // gizmo is rendered with gray half transparent color when disabled
        IMGUI_API void Enable(bool enable);

        // helper functions for manualy editing translation/rotation/scale with an input float
        // translation, rotation and scale float points to 3 floats each
        // Angles are in degrees (more suitable for human editing)
        // example:
        // float matrixTranslation[3], matrixRotation[3], matrixScale[3];
        // ImGuizmo::DecomposeMatrixToComponents(gizmoMatrix.m16, matrixTranslation, matrixRotation, matrixScale);
        // ImGui::InputFloat3("Tr", matrixTranslation, 3);
        // ImGui::InputFloat3("Rt", matrixRotation, 3);
        // ImGui::InputFloat3("Sc", matrixScale, 3);
        // ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, gizmoMatrix.m16);
        //
        // These functions have some numerical stability issues for now. Use with caution.
        IMGUI_API void DecomposeMatrixToComponents(const float* matrix, float* translation, float* rotation, float* scale);
        IMGUI_API void RecomposeMatrixFromComponents(const float* translation, const float* rotation, const float* scale, float* matrix);

        IMGUI_API void SetRect(float x, float y, float width, float height);
        // default is false
        IMGUI_API void SetOrthographic(bool isOrthographic);

        // Render a cube with face color corresponding to face normal. Usefull for debug/tests
        IMGUI_API void DrawCubes(const float* view, const float* projection, const float* matrices, int matrixCount);
        IMGUI_API void DrawGrid(const float* view, const float* projection, const float* matrix, const float gridSize);

        // call it when you want a gizmo
        // Needs view and projection matrices. 
        // matrix parameter is the source matrix (where will be gizmo be drawn) and might be transformed by the function. Return deltaMatrix is optional
        // translation is applied in world space
        enum OPERATION
        {
            TRANSLATE,
            ROTATE,
            SCALE,
            BOUNDS,
        };

        enum MODE
        {
            LOCAL,
            WORLD
        };

        IMGUI_API bool Manipulate(const float* view, const float* projection, OPERATION operation, MODE mode, float* matrix, float* deltaMatrix = NULL, const float* snap = NULL, const float* localBounds = NULL, const float* boundsSnap = NULL);
        //
        // Please note that this cubeview is patented by Autodesk : https://patents.google.com/patent/US7782319B2/en
        // It seems to be a defensive patent in the US. I don't think it will bring troubles using it as
        // other software are using the same mechanics. But just in case, you are now warned!
        //
        IMGUI_API void ViewManipulate(float* view, float length, ImVec2 position, ImVec2 size, ImU32 backgroundColor);

        IMGUI_API void SetID(int id);

        // return true if the cursor is over the operation's gizmo
        IMGUI_API bool IsOver(OPERATION op);
        IMGUI_API void SetGizmoSizeClipSpace(float value);
    }
}

#endif // !_NMSTOOLKIT_IMGUIZMO_H