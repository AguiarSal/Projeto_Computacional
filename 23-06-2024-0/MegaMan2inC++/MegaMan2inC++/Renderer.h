#ifndef RENDERER_H
#define RENDERER_H

#include "Window.h"
#include "Direct3D.h"
#include "Sprites.h"
#include "Geom.h"
#include <vector>
#include <DirectXMath.h>
using std::vector;
using namespace DirectX;

struct Vertex
{
    XMFLOAT3 pos;
    XMFLOAT4 color;
    XMFLOAT2 tex;
};

class Renderer
{
private:
    Window* window;
    Direct3D* direct3D;

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

    ID3D11Texture2D* pixelPlotTexture;                
    ID3D11ShaderResourceView* pixelPlotView;                   
    SpriteData                 pixelPlotSprite;                
    long                       videoMemoryPitch;                
    unsigned long* videoMemory;

    void PlotPixel(int x, int y, unsigned long color);
    void PlotLine(int x, int y, int flag, unsigned long color);
    void DrawLine(int a1, int b1, int a2, int b2, unsigned long color);
    int  ClipLine(int& x1, int& y1, int& x2, int& y2);

    vector<SpriteData*> spriteVector;

    void RenderBatch(ID3D11ShaderResourceView* texture, SpriteData** sprites, unsigned int cont);

public:
    Renderer();
    ~Renderer();

    void BeginPixels();
    void Draw(Geom* shape, unsigned long color);
    void Draw(Point* point, unsigned long color);
    void Draw(Line* line, unsigned long color);
    void Draw(Rect* rect, unsigned long color);
    void Draw(Circle* circ, unsigned long color);
    void EndPixels();

    bool Initialize(Window* window, Direct3D* direct3D);
    void RenderDraw(SpriteData* sprt);
    void Render();

};

inline void Renderer::PlotPixel(int x, int y, unsigned long color) {
    videoMemory[x + y * videoMemoryPitch] = color;
}

inline void Renderer::PlotLine(int x, int y, int flag, unsigned long color) {
    flag ? PlotPixel(y, x, color) : PlotPixel(x, y, color);
}

#endif

