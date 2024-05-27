#include "Renderer.h"
#include <algorithm>
#include <d3dcompiler.h>
#include <DirectXMath.h>
using namespace DirectX;

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
	XMFLOAT2 tex;
};

Renderer::Renderer()
{
    inputLayout = nullptr;
    vertexShader = nullptr;
    pixelShader = nullptr;
    rasterState = nullptr;
    sampler = nullptr;
    vertexBuffer = nullptr;
    indexBuffer = nullptr;
    constantBuffer = nullptr;

    vertexBufferPosition = 0;
}

Renderer::~Renderer()
{
    if (constantBuffer)
    {
        constantBuffer->Release();
        constantBuffer = nullptr;
    }

    if (indexBuffer)
    {
        indexBuffer->Release();
        indexBuffer = nullptr;
    }

    if (vertexBuffer)
    {
        vertexBuffer->Release();
        vertexBuffer = nullptr;
    }

    if (sampler)
    {
        sampler->Release();
        sampler = nullptr;
    }

    if (rasterState)
    {
        rasterState->Release();
        rasterState = nullptr;
    }

    if (pixelShader)
    {
        pixelShader->Release();
        pixelShader = nullptr;
    }

    if (vertexShader)
    {
        vertexShader->Release();
        vertexShader = nullptr;
    }

    if (inputLayout)
    {
        inputLayout->Release();
        inputLayout = nullptr;
    }
}

bool Renderer::Initialize(Window* window, Direct3D* direct3D)
{
    ID3DBlob* vShader = nullptr;

    if FAILED(D3DReadFileToBlob(L"Shaders/VertexShader.cso", &vShader))
    {
        return false;
    }

    if FAILED(direct3D->Device->CreateVertexShader(vShader->GetBufferPointer(), vShader->GetBufferSize(), NULL, &vertexShader))
    {
        return false;
    }

    D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    if FAILED(direct3D->Device->CreateInputLayout(layoutDesc, 3, vShader->GetBufferPointer(), vShader->GetBufferSize(), &inputLayout))
    {
        return false;
    }
    vShader->Release();

    ID3DBlob* pShader = nullptr;
    if FAILED(D3DReadFileToBlob(L"Shaders/PixelShader.cso", &pShader))
    {
        return false;
    }

    if FAILED(direct3D->Device->CreatePixelShader(pShader->GetBufferPointer(), pShader->GetBufferSize(), NULL, &pixelShader))
    {
        return false;
    }

    pShader->Release();

    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    //rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
    rasterDesc.CullMode = D3D11_CULL_NONE;
    rasterDesc.DepthClipEnable = true;

    if FAILED(direct3D->Device->CreateRasterizerState(&rasterDesc, &rasterState))
    {
        return false;
    }

    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * VerticesPerSprite * MaxBatchSize;
    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    if FAILED(direct3D->Device->CreateBuffer(&vertexBufferDesc, nullptr, &vertexBuffer))
    {
        return false;
    }

    D3D11_BUFFER_DESC indexBufferDesc = { 0 };
    indexBufferDesc.ByteWidth = sizeof(short) * IndicesPerSprite * MaxBatchSize;
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    vector<short> indices;
    indices.reserve(MaxBatchSize * IndicesPerSprite);
    for (short i = 0; i < MaxBatchSize * VerticesPerSprite; i += VerticesPerSprite)
    {
        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(i + 2);

        indices.push_back(i + 1);
        indices.push_back(i + 3);
        indices.push_back(i + 2);
    }

    D3D11_SUBRESOURCE_DATA indexData = { 0 };
    indexData.pSysMem = &indices.front();

    if FAILED(direct3D->Device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer))
    {
        return false;
    }

    D3D11_BUFFER_DESC constBufferDesc = { 0 };
    constBufferDesc.ByteWidth = sizeof(XMMATRIX);
    constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    float xScale = (direct3D->Viewport.Width > 0) ? 2.0f / direct3D->Viewport.Width : 0.0f;
    float yScale = (direct3D->Viewport.Height > 0) ? 2.0f / direct3D->Viewport.Height : 0.0f;

    XMMATRIX transformMatrix
    (
        xScale, 0, 0, 0,
        0, -yScale, 0, 0,
        0, 0, 1, 0,
        -1, 1, 0, 1
    );

    D3D11_SUBRESOURCE_DATA constantData = { 0 };
    XMMATRIX worldViewProj = XMMatrixTranspose(transformMatrix);
    constantData.pSysMem = &worldViewProj;

    if FAILED(direct3D->Device->CreateBuffer(&constBufferDesc, &constantData, &constantBuffer))
    {
        return false;
    }

    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = (direct3D->Device->GetFeatureLevel() > D3D_FEATURE_LEVEL_9_1) ? 16 : 2;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.BorderColor[0] = 0.0f;
    samplerDesc.BorderColor[1] = 0.0f;
    samplerDesc.BorderColor[2] = 0.0f;
    samplerDesc.BorderColor[3] = 0.0f;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    if FAILED(direct3D->Device->CreateSamplerState(&samplerDesc, &sampler))
    {
        return false;
    }

    unsigned int vertexStride = sizeof(Vertex);
    unsigned int vertexOffset = 0;
    direct3D->Context->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexStride, &vertexOffset);
    direct3D->Context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
    direct3D->Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    direct3D->Context->IASetInputLayout(inputLayout);
    direct3D->Context->VSSetShader(vertexShader, NULL, 0);
    direct3D->Context->VSSetConstantBuffers(0, 1, &constantBuffer);
    direct3D->Context->PSSetShader(pixelShader, NULL, 0);
    direct3D->Context->PSSetSamplers(0, 1, &sampler);
    direct3D->Context->RSSetState(rasterState);

    return true;
}

void Renderer::RenderBatch(ID3D11ShaderResourceView* texture, SpriteData** sprites, unsigned int cont)
{
    Direct3D::Context->PSSetShaderResources(0, 1, &texture);

    while (cont > 0)
    {
        unsigned int batchSize = cont;

        unsigned int remainingSpace = MaxBatchSize - vertexBufferPosition;

        if (batchSize > remainingSpace)
        {
            if (remainingSpace < MinBatchSize)
            {
                vertexBufferPosition = 0;
                batchSize = (cont < MaxBatchSize) ? cont : MaxBatchSize;
            }
            else
            {
                batchSize = remainingSpace;
            }
        }

        D3D11_MAP mapType = (vertexBufferPosition == 0) ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE_NO_OVERWRITE;
        D3D11_MAPPED_SUBRESOURCE mappedBuffer;
        Direct3D::Context->Map(vertexBuffer, 0, mapType, 0, &mappedBuffer);

        Vertex* vertices = (Vertex*)mappedBuffer.pData + vertexBufferPosition * VerticesPerSprite;

        for (unsigned int i = 0; i < batchSize; ++i)
        {
            XMVECTOR size = XMVectorMergeXY(XMLoadInt(&sprites[i]->SprtWidth), XMLoadInt(&sprites[i]->SprtHeight));
            XMVECTOR textureSize = XMConvertVectorUIntToFloat(size, 0);
            XMVECTOR inverseTextureSize = XMVectorReciprocal(textureSize);

            XMFLOAT2 positionxy(sprites[i]->SprtX, sprites[i]->SprtY);
            float scale = sprites[i]->SprtScale;
            XMFLOAT2 center(0.0f, 0.0f);
            float rotation = sprites[i]->SprtRotation;
            float layerDepth = sprites[i]->SprtDepth;

            XMVECTOR source = XMVectorSet(0, 0, 1, 1);
            XMVECTOR destination = XMVectorPermute<0, 1, 4, 4>(XMLoadFloat2(&positionxy), XMLoadFloat(&scale));
            XMVECTOR color = XMVectorSet(1, 1, 1, 1);
            XMVECTOR originRotationDepth = XMVectorSet(center.x, center.y, rotation, layerDepth);

            XMVECTOR sourceSize = XMVectorSwizzle<2, 3, 2, 3>(source);
            XMVECTOR destinationSize = XMVectorSwizzle<2, 3, 2, 3>(destination);

            XMVECTOR isZeroMask = XMVectorEqual(sourceSize, XMVectorZero());
            XMVECTOR nonZeroSourceSize = XMVectorSelect(sourceSize, g_XMEpsilon, isZeroMask);

            XMVECTOR origin = XMVectorDivide(originRotationDepth, nonZeroSourceSize);

            origin *= inverseTextureSize;

            destinationSize *= textureSize;

            XMVECTOR rotationMatrix1;
            XMVECTOR rotationMatrix2;

            if (rotation != 0)
            {
                float sin, cos;

                XMScalarSinCos(&sin, &cos, rotation);

                XMVECTOR sinV = XMLoadFloat(&sin);
                XMVECTOR cosV = XMLoadFloat(&cos);

                rotationMatrix1 = XMVectorMergeXY(cosV, sinV);
                rotationMatrix2 = XMVectorMergeXY(-sinV, cosV);
            }
            else
            {
                rotationMatrix1 = g_XMIdentityR0;
                rotationMatrix2 = g_XMIdentityR1;
            }

            static XMVECTORF32 cornerOffsets[VerticesPerSprite] =
            {
                { 0, 0 },
                { 1, 0 },
                { 0, 1 },
                { 1, 1 },
            };

            int mirrorBits = 0;

            for (int i = 0; i < VerticesPerSprite; ++i)
            {
                XMVECTOR cornerOffset = (cornerOffsets[i] - origin) * destinationSize;

                XMVECTOR position1 = XMVectorMultiplyAdd(XMVectorSplatX(cornerOffset), rotationMatrix1, destination);
                XMVECTOR position2 = XMVectorMultiplyAdd(XMVectorSplatY(cornerOffset), rotationMatrix2, position1);

                XMVECTOR position = XMVectorPermute<0, 1, 7, 6>(position2, originRotationDepth);

                XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&vertices[i].pos), position);

                XMStoreFloat4(&vertices[i].color, color);

                XMVECTOR textureCoordinate = XMVectorMultiplyAdd(cornerOffsets[i ^ mirrorBits], sourceSize, source);

                XMStoreFloat2(&vertices[i].tex, textureCoordinate);
            }

            vertices += VerticesPerSprite;
        }

        Direct3D::Context->Unmap(vertexBuffer, 0);

        unsigned int startIndex = (unsigned int)vertexBufferPosition * IndicesPerSprite;
        unsigned int indexCount = (unsigned int)batchSize * IndicesPerSprite;
        Direct3D::Context->DrawIndexed(indexCount, startIndex, 0);

        vertexBufferPosition += batchSize;

        sprites += batchSize;

        cont -= batchSize;
    }
}

void Renderer::Render()
{
    sort(spriteVector.begin(), spriteVector.end(),
        [](SpriteData* a, SpriteData* b) -> bool
        { return a->SprtDepth > b->SprtDepth; });

    unsigned int spriteVectorSize = unsigned int(spriteVector.size());

    if (spriteVectorSize == 0)
        return;

    ID3D11ShaderResourceView* batchTexture = nullptr;
    unsigned int batchStart = 0;

    for (unsigned int pos = 0; pos < spriteVectorSize; ++pos)
    {
        ID3D11ShaderResourceView* texture = spriteVector[pos]->SprtTexture;

        if (texture != batchTexture)
        {
            if (pos > batchStart)
            {
                RenderBatch(batchTexture, &spriteVector[batchStart], pos - batchStart);
            }

            batchTexture = texture;
            batchStart = pos;
        }
    }

    RenderBatch(batchTexture, &spriteVector[batchStart], spriteVectorSize - batchStart);

    spriteVector.clear();
}

void Renderer::RenderDraw(SpriteData* Sprt)
{
    spriteVector.push_back(Sprt);
}