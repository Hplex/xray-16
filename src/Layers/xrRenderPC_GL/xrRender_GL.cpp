#include "stdafx.h"
#include "Layers/xrRender/dxRenderFactory.h"
#include "Layers/xrRender/dxUIRender.h"
#include "Layers/xrRender/dxDebugRender.h"
#include "Layers/xrRender/D3DUtils.h"

constexpr pcstr RENDERER_RGL_MODE = "renderer_rgl";

class RGLRendererModule final : public RendererModule
{
    xr_vector<pcstr> modes;

public:
    const xr_vector<pcstr>& ObtainSupportedModes() override
    {
        if (!modes.empty())
        {
            return modes;
        }
        if (xrRender_test_hw())
        {
            modes.emplace_back(RENDERER_RGL_MODE);
        }
        return modes;
    }

    void CheckModeConsistency(pcstr mode) const
    {
        R_ASSERT3(0 == xr_strcmp(mode, RENDERER_RGL_MODE),
            "Wrong mode passed to xrRender_GL.dll", mode);
    }

    void SetupEnv(pcstr mode) override
    {
        CheckModeConsistency(mode);
        ps_r2_sun_static = false;
        ps_r2_advanced_pp = true;
        GEnv.Render = &RImplementation;
        GEnv.RenderFactory = &RenderFactoryImpl;
        GEnv.DU = &DUImpl;
        GEnv.UIRender = &UIRenderImpl;
#ifdef DEBUG
        GEnv.DRender = &DebugRenderImpl;
#endif
        xrRender_initconsole();
    }
} static s_rgl_module;

extern "C"
{
XR_EXPORT RendererModule* GetRendererModule()
{
    return &s_rgl_module;
}
}
