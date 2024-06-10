#ifdef _MSC_VER
#pragma once
#endif

#include <d3d11.h>

#pragma warning(push)
#pragma warning(disable : 4005)
#include <stdint.h>
#pragma warning(pop)

HRESULT D3D11CreateTextureFromFile(_In_ ID3D11Device* d3dDevice,
    _In_opt_ ID3D11DeviceContext* d3dContext,
    _In_z_ const char* szFileName,
    _Out_opt_ ID3D11Resource** texture,
    _Out_opt_ ID3D11ShaderResourceView** textureView,
    unsigned& width,
    unsigned& height,
    _In_ size_t maxsize = 0);
