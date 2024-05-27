#ifndef RENDERER_H
#define RENDERER_H

#include "Window.h"
#include "Direct3D.h"
#include "Sprites.h"
#include <vector>
using std::vector;

class Renderer
{
private:
    ID3D11InputLayout* inputLayout;                 
    ID3D11VertexShader* vertexShader;                
    ID3D11PixelShader* pixelShader;                 
    ID3D11RasterizerState* rasterState;                 
    ID3D11SamplerState* sampler;                     
    ID3D11Buffer* vertexBuffer;                
    ID3D11Buffer* indexBuffer;                 
    ID3D11Buffer* constantBuffer;              
    unsigned int vertexBufferPosition;

    static const unsigned int MinBatchSize = 128;           // tamanho mínimo do lote de sprites
    static const unsigned int MaxBatchSize = 4096;          // tamanho máxmo do lote de sprites (max de sprites na tela)
    static const unsigned int VerticesPerSprite = 4;
    static const unsigned int IndicesPerSprite = 6;

    vector<SpriteData*> spriteVector;

    void RenderBatch(ID3D11ShaderResourceView* texture, SpriteData** sprites, unsigned int cont);

public:
    Renderer();
    ~Renderer();

    bool Initialize(Window* window, Direct3D* direct3D);
    void RenderDraw(SpriteData* sprt);
    void Render();

};



#endif

