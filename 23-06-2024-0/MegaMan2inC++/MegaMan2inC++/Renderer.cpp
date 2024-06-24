#include "Renderer.h"
#include <algorithm>
#include <d3dcompiler.h>
#include <DirectXMath.h>

Renderer::Renderer()
{
    window = nullptr;
    direct3D = nullptr;
    inputLayout = nullptr;
    vertexShader = nullptr;
    pixelShader = nullptr;
    rasterState = nullptr;
    sampler = nullptr;
    vertexBuffer = nullptr;
    indexBuffer = nullptr;
    constantBuffer = nullptr;

    vertexBufferPosition = 0;

    pixelPlotTexture = nullptr;
    pixelPlotView = nullptr;
    ZeroMemory(&pixelPlotSprite, sizeof(pixelPlotSprite));
    videoMemoryPitch = 0;
    videoMemory = nullptr;
}

Renderer::~Renderer()
{
    if (pixelPlotView)
    {
        pixelPlotView->Release();
        pixelPlotView = nullptr;
    }

    if (pixelPlotTexture)
    {
        pixelPlotTexture->Release();
        pixelPlotTexture = nullptr;
    }

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

void Renderer::BeginPixels()
{
    D3D11_MAPPED_SUBRESOURCE mappedTex;
    direct3D->Context->Map(pixelPlotTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedTex);

    videoMemoryPitch = mappedTex.RowPitch >> 2;
    videoMemory = (unsigned long*)mappedTex.pData;

    memset(videoMemory, 0xff000000, mappedTex.RowPitch * window->GetWndHeigth());
}

void Renderer::Draw(Geom* shape, unsigned long color)
{
    switch (shape->GetType())
    {
    case POINT_TYPE:
        Draw((Point*)shape, color);
        break;
    case LINE_TYPE:
        Draw((Line*)shape, color);
        break;
    case RECT_TYPE:
        Draw((Rect*)shape, color);
        break;
    case CIRCLE_TYPE:
        Draw((Circle*)shape, color);
        break;
    }
}

void Renderer::Draw(Point* point, unsigned long color)
{
    if (point->GetX() >= 0 && point->GetX() < window->GetWndWidth())
    {
        if (point->GetY() >= 0 && point->GetY() < window->GetWndHeigth())
        {
            PlotPixel(int(point->GetX()), int(point->GetY()), color);
        }
    }
}

void Renderer::Draw(Line* line, unsigned long color)
{
    int x1 = int(line->Ax());
    int y1 = int(line->Ay());
    int x2 = int(line->Bx());
    int y2 = int(line->By());

    if (ClipLine(x1, y1, x2, y2))
    {
        DrawLine(x1, y1, x2, y2, color);
    }
}

int Renderer::ClipLine(int& x1, int& y1, int& x2, int& y2)
{
    // Clipping Line Algorithm 
    // Implementation by André LaMothe from "Tricks of The Windows Game Programming Gurus", SAMS, 2002

    // this function clips the sent line using the clipping region defined below
    int min_clip_x = 0;
    int min_clip_y = 0;
    int max_clip_x = window->GetWndWidth() - 1;
    int max_clip_y = window->GetWndHeigth() - 1;

    // internal clipping codes
#define CLIP_CODE_C  0x0000
#define CLIP_CODE_N  0x0008
#define CLIP_CODE_S  0x0004
#define CLIP_CODE_E  0x0002
#define CLIP_CODE_W  0x0001

#define CLIP_CODE_NE 0x000a
#define CLIP_CODE_SE 0x0006
#define CLIP_CODE_NW 0x0009 
#define CLIP_CODE_SW 0x0005

    int xc1 = x1,
        yc1 = y1,
        xc2 = x2,
        yc2 = y2;

    int p1_code = 0,
        p2_code = 0;

    // determine codes for p1 and p2
    if (y1 < min_clip_y)
        p1_code |= CLIP_CODE_N;
    else
        if (y1 > max_clip_y)
            p1_code |= CLIP_CODE_S;

    if (x1 < min_clip_x)
        p1_code |= CLIP_CODE_W;
    else
        if (x1 > max_clip_x)
            p1_code |= CLIP_CODE_E;

    if (y2 < min_clip_y)
        p2_code |= CLIP_CODE_N;
    else
        if (y2 > max_clip_y)
            p2_code |= CLIP_CODE_S;

    if (x2 < min_clip_x)
        p2_code |= CLIP_CODE_W;
    else
        if (x2 > max_clip_x)
            p2_code |= CLIP_CODE_E;

    // try and trivially reject
    if ((p1_code & p2_code))
        return 0;

    // test for totally visible, if so leave points untouched
    if (p1_code == 0 && p2_code == 0)
        return 1;

    // determine end clip point for p1
    switch (p1_code)
    {
    case CLIP_CODE_C: break;

    case CLIP_CODE_N:
    {
        yc1 = min_clip_y;
        xc1 = int(x1 + 0.5 + (min_clip_y - y1) * (x2 - x1) / (y2 - y1));
    } break;
    case CLIP_CODE_S:
    {
        yc1 = max_clip_y;
        xc1 = int(x1 + 0.5 + (max_clip_y - y1) * (x2 - x1) / (y2 - y1));
    } break;

    case CLIP_CODE_W:
    {
        xc1 = min_clip_x;
        yc1 = int(y1 + 0.5 + (min_clip_x - x1) * (y2 - y1) / (x2 - x1));
    } break;

    case CLIP_CODE_E:
    {
        xc1 = max_clip_x;
        yc1 = int(y1 + 0.5 + (max_clip_x - x1) * (y2 - y1) / (x2 - x1));
    } break;

    // these cases are more complex, must compute 2 intersections
    case CLIP_CODE_NE:
    {
        // north hline intersection
        yc1 = min_clip_y;
        xc1 = int(x1 + 0.5 + (min_clip_y - y1) * (x2 - x1) / (y2 - y1));

        // test if intersection is valid, of so then done, else compute next
        if (xc1 < min_clip_x || xc1 > max_clip_x)
        {
            // east vline intersection
            xc1 = max_clip_x;
            yc1 = int(y1 + 0.5 + (max_clip_x - x1) * (y2 - y1) / (x2 - x1));
        } // end if
    } break;

    case CLIP_CODE_SE:
    {
        // south hline intersection
        yc1 = max_clip_y;
        xc1 = int(x1 + 0.5 + (max_clip_y - y1) * (x2 - x1) / (y2 - y1));

        // test if intersection is valid, of so then done, else compute next
        if (xc1 < min_clip_x || xc1 > max_clip_x)
        {
            // east vline intersection
            xc1 = max_clip_x;
            yc1 = int(y1 + 0.5 + (max_clip_x - x1) * (y2 - y1) / (x2 - x1));
        } // end if
    } break;

    case CLIP_CODE_NW:
    {
        // north hline intersection
        yc1 = min_clip_y;
        xc1 = int(x1 + 0.5 + (min_clip_y - y1) * (x2 - x1) / (y2 - y1));

        // test if intersection is valid, of so then done, else compute next
        if (xc1 < min_clip_x || xc1 > max_clip_x)
        {
            xc1 = min_clip_x;
            yc1 = int(y1 + 0.5 + (min_clip_x - x1) * (y2 - y1) / (x2 - x1));
        } // end if
    } break;

    case CLIP_CODE_SW:
    {
        // south hline intersection
        yc1 = max_clip_y;
        xc1 = int(x1 + 0.5 + (max_clip_y - y1) * (x2 - x1) / (y2 - y1));

        // test if intersection is valid, of so then done, else compute next
        if (xc1 < min_clip_x || xc1 > max_clip_x)
        {
            xc1 = min_clip_x;
            yc1 = int(y1 + 0.5 + (min_clip_x - x1) * (y2 - y1) / (x2 - x1));
        } // end if
    } break;

    default:break;
    } // end switch

    // determine clip point for p2
    switch (p2_code)
    {
    case CLIP_CODE_C: break;

    case CLIP_CODE_N:
    {
        yc2 = min_clip_y;
        xc2 = x2 + (min_clip_y - y2) * (x1 - x2) / (y1 - y2);
    } break;

    case CLIP_CODE_S:
    {
        yc2 = max_clip_y;
        xc2 = x2 + (max_clip_y - y2) * (x1 - x2) / (y1 - y2);
    } break;

    case CLIP_CODE_W:
    {
        xc2 = min_clip_x;
        yc2 = y2 + (min_clip_x - x2) * (y1 - y2) / (x1 - x2);
    } break;

    case CLIP_CODE_E:
    {
        xc2 = max_clip_x;
        yc2 = y2 + (max_clip_x - x2) * (y1 - y2) / (x1 - x2);
    } break;

    // these cases are more complex, must compute 2 intersections
    case CLIP_CODE_NE:
    {
        // north hline intersection
        yc2 = min_clip_y;
        xc2 = int(x2 + 0.5 + (min_clip_y - y2) * (x1 - x2) / (y1 - y2));

        // test if intersection is valid, of so then done, else compute next
        if (xc2 < min_clip_x || xc2 > max_clip_x)
        {
            // east vline intersection
            xc2 = max_clip_x;
            yc2 = int(y2 + 0.5 + (max_clip_x - x2) * (y1 - y2) / (x1 - x2));
        } // end if
    } break;

    case CLIP_CODE_SE:
    {
        // south hline intersection
        yc2 = max_clip_y;
        xc2 = int(x2 + 0.5 + (max_clip_y - y2) * (x1 - x2) / (y1 - y2));

        // test if intersection is valid, of so then done, else compute next
        if (xc2 < min_clip_x || xc2 > max_clip_x)
        {
            // east vline intersection
            xc2 = max_clip_x;
            yc2 = int(y2 + 0.5 + (max_clip_x - x2) * (y1 - y2) / (x1 - x2));
        } // end if
    } break;

    case CLIP_CODE_NW:
    {
        // north hline intersection
        yc2 = min_clip_y;
        xc2 = int(x2 + 0.5 + (min_clip_y - y2) * (x1 - x2) / (y1 - y2));

        // test if intersection is valid, of so then done, else compute next
        if (xc2 < min_clip_x || xc2 > max_clip_x)
        {
            xc2 = min_clip_x;
            yc2 = int(y2 + 0.5 + (min_clip_x - x2) * (y1 - y2) / (x1 - x2));
        } // end if

    } break;

    case CLIP_CODE_SW:
    {
        // south hline intersection
        yc2 = max_clip_y;
        xc2 = int(x2 + 0.5 + (max_clip_y - y2) * (x1 - x2) / (y1 - y2));

        // test if intersection is valid, of so then done, else compute next
        if (xc2 < min_clip_x || xc2 > max_clip_x)
        {
            xc2 = min_clip_x;
            yc2 = int(y2 + 0.5 + (min_clip_x - x2) * (y1 - y2) / (x1 - x2));
        } // end if

    } break;

    default:break;

    } // end switch

    // do bounds check
    if ((xc1 < min_clip_x) || (xc1 > max_clip_x) ||
        (yc1 < min_clip_y) || (yc1 > max_clip_y) ||
        (xc2 < min_clip_x) || (xc2 > max_clip_x) ||
        (yc2 < min_clip_y) || (yc2 > max_clip_y))
    {
        return 0;
    }

    // store vars back
    x1 = xc1;
    y1 = yc1;
    x2 = xc2;
    y2 = yc2;

    return 1;
}

void Renderer::DrawLine(int a1, int b1, int a2, int b2, unsigned long color)
{
    // Symmetric Double Step Line Algorithm by Xialon Wu
    // It's 3 to 4 times faster than the standard Bressenham's algorithm
    // Implementation by Brian Wyvill from "Graphics Gems", Academic Press, 1990

    int dx, dy, incr1, incr2, D, x, y, xend, c, pixels_left;
    int x1, y1;
    int sign_x, sign_y, step, reverse, i;

    dx = (a2 - a1) * (sign_x = ((a2 - a1) < 0 ? -1 : 1));
    dy = (b2 - b1) * (sign_y = ((b2 - b1) < 0 ? -1 : 1));

    /* decide increment sign by the slope sign */
    if (sign_x == sign_y)
        step = 1;
    else
        step = -1;

    /* chooses axis of greatest movement (make dx) */
    if (dy > dx)
    {
        // operator ^= obtain the bitwise exclusive OR of the first and second operands; 
        // store the result in the object specified by the first operand
        a1 ^= b1; b1 ^= a1; a1 ^= b1;
        a2 ^= b2; b2 ^= a2; a2 ^= b2;
        dx ^= dy; dy ^= dx; dx ^= dy;
        reverse = 1;
    }
    else
        reverse = 0;

    /* note error check for dx==0 should be included here */
    /* start from the smaller coordinate */
    if (a1 > a2)
    {
        x = a2;
        y = b2;
        x1 = a1;
        y1 = b1;
    }
    else
    {
        x = a1;
        y = b1;
        x1 = a2;
        y1 = b2;
    }

    /* Note dx=n implies 0 - n or (dx+1) pixels to be set
    * Go round loop dx/4 times then plot last 0,1,2 or 3 pixels
    *  In fact (dx-1)/4 as 2 pixels are already plotted */
    xend = (dx - 1) / 4;
    pixels_left = (dx - 1) % 4;    /* number of pixels left over at the end */
    PlotLine(x, y, reverse, color);
    if (pixels_left < 0)
        return;    /* plot only one pixel for zero
                * length vectors */
    PlotLine(x1, y1, reverse, color);    /* plot first two points */
    incr2 = 4 * dy - 2 * dx;
    if (incr2 < 0) {    /* slope less than 1/2 */
        c = 2 * dy;
        incr1 = 2 * c;
        D = incr1 - dx;

        for (i = 0; i < xend; i++) {    /* plotting loop */
            ++x;
            --x1;
            if (D < 0) {
                /* pattern 1 forwards */
                PlotLine(x, y, reverse, color);
                PlotLine(++x, y, reverse, color);
                /* pattern 1 backwards */
                PlotLine(x1, y1, reverse, color);
                PlotLine(--x1, y1, reverse, color);
                D += incr1;
            }
            else {
                if (D < c) {
                    /* pattern 2 forwards */
                    PlotLine(x, y, reverse, color);
                    PlotLine(++x, y += step, reverse, color);
                    /* pattern 2 backwards */
                    PlotLine(x1, y1, reverse, color);
                    PlotLine(--x1, y1 -= step, reverse, color);
                }
                else {
                    /* pattern 3 forwards */
                    PlotLine(x, y += step, reverse, color);
                    PlotLine(++x, y, reverse, color);
                    /* pattern 3 backwards */
                    PlotLine(x1, y1 -= step, reverse, color);
                    PlotLine(--x1, y1, reverse, color);
                }
                D += incr2;
            }
        }        /* end for */
        /* plot last pattern */
        if (pixels_left) {
            if (D < 0) {
                PlotLine(++x, y, reverse, color);    /* pattern 1 */
                if (pixels_left > 1)
                    PlotLine(++x, y, reverse, color);
                if (pixels_left > 2)
                    PlotLine(--x1, y1, reverse, color);
            }
            else {
                if (D < c) {
                    PlotLine(++x, y, reverse, color);    /* pattern 2  */
                    if (pixels_left > 1)
                        PlotLine(++x, y += step, reverse, color);
                    if (pixels_left > 2)
                        PlotLine(--x1, y1, reverse, color);
                }
                else {
                    /* pattern 3 */
                    PlotLine(++x, y += step, reverse, color);
                    if (pixels_left > 1)
                        PlotLine(++x, y, reverse, color);
                    if (pixels_left > 2)
                        PlotLine(--x1, y1 -= step, reverse, color);
                }
            }
        }        /* end if pixels_left */
    }
    /* end slope < 1/2 */
    else {            /* slope greater than 1/2 */
        c = 2 * (dy - dx);
        incr1 = 2 * c;
        D = incr1 + dx;
        for (i = 0; i < xend; i++) {
            ++x;
            --x1;
            if (D > 0) {
                /* pattern 4 forwards */
                PlotLine(x, y += step, reverse, color);
                PlotLine(++x, y += step, reverse, color);
                /* pattern 4 backwards */
                PlotLine(x1, y1 -= step, reverse, color);
                PlotLine(--x1, y1 -= step, reverse, color);
                D += incr1;
            }
            else {
                if (D < c) {
                    /* pattern 2 forwards */
                    PlotLine(x, y, reverse, color);
                    PlotLine(++x, y += step, reverse, color);

                    /* pattern 2 backwards */
                    PlotLine(x1, y1, reverse, color);
                    PlotLine(--x1, y1 -= step, reverse, color);
                }
                else {
                    /* pattern 3 forwards */
                    PlotLine(x, y += step, reverse, color);
                    PlotLine(++x, y, reverse, color);
                    /* pattern 3 backwards */
                    PlotLine(x1, y1 -= step, reverse, color);
                    PlotLine(--x1, y1, reverse, color);
                }
                D += incr2;
            }
        }        /* end for */
        /* plot last pattern */
        if (pixels_left) {
            if (D > 0) {
                PlotLine(++x, y += step, reverse, color);    /* pattern 4 */
                if (pixels_left > 1)
                    PlotLine(++x, y += step, reverse, color);
                if (pixels_left > 2)
                    PlotLine(--x1, y1 -= step, reverse, color);
            }
            else {
                if (D < c) {
                    PlotLine(++x, y, reverse, color);    /* pattern 2  */
                    if (pixels_left > 1)
                        PlotLine(++x, y += step, reverse, color);
                    if (pixels_left > 2)
                        PlotLine(--x1, y1, reverse, color);
                }
                else {
                    /* pattern 3 */
                    PlotLine(++x, y += step, reverse, color);
                    if (pixels_left > 1)
                        PlotLine(++x, y, reverse, color);
                    if (pixels_left > 2) {
                        if (D > c) /* step 3 */
                            PlotLine(--x1, y1 -= step, reverse, color);
                        else /* step 2 */
                            PlotLine(--x1, y1, reverse, color);
                    }
                }
            }
        }
    }
}

void Renderer::Draw(Rect* rect, unsigned long color)
{
    Line top(rect->Left(), rect->Top(), rect->Right(), rect->Top());
    Line left(rect->Left(), rect->Top() + 1, rect->Left(), rect->Bottom());
    Line right(rect->Right(), rect->Top() + 1, rect->Right(), rect->Bottom());
    Line bottom(rect->Left() + 1, rect->Bottom(), rect->Right() - 1, rect->Bottom());

    Draw(&top, color);
    Draw(&left, color);
    Draw(&right, color);
    Draw(&bottom, color);
}

void Renderer::Draw(Circle* circ, unsigned long color)
{
    // Bresenham's circle algorithm

    int xpos = int(circ->GetCenterX());
    int ypos = int(circ->GetCenterY());

    int r = int(circ->Radius);

    int p = 3 - (2 * r);
    int x = 0;
    int y = r;

    Point a(xpos, ypos + r); Draw(&a, color);
    Point b(xpos, ypos - r); Draw(&b, color);
    Point c(xpos, ypos + r); Draw(&c, color);
    Point d(xpos, ypos - r); Draw(&d, color);
    Point e(xpos + r, ypos); Draw(&e, color);
    Point f(xpos + r, ypos); Draw(&f, color);
    Point g(xpos - r, ypos); Draw(&g, color);
    Point h(xpos - r, ypos); Draw(&h, color);

    //for(x=1; x <= int(radius/sqrt(2.0f)); x++)
    while (++x < y)
    {
        if (p < 0)
            p = p + (4 * x) + 6;
        else
        {
            p = p + 4 * (x - y) + 10;
            y = y - 1;
        }

        a.GeomMoveTo(float(xpos + x), float(ypos + y)); Draw(&a, color);
        b.GeomMoveTo(float(xpos + x), float(ypos - y)); Draw(&b, color);
        c.GeomMoveTo(float(xpos - x), float(ypos + y)); Draw(&c, color);
        d.GeomMoveTo(float(xpos - x), float(ypos - y)); Draw(&d, color);
        e.GeomMoveTo(float(xpos + y), float(ypos + x)); Draw(&e, color);
        f.GeomMoveTo(float(xpos + y), float(ypos - x)); Draw(&f, color);
        g.GeomMoveTo(float(xpos - y), float(ypos + x)); Draw(&g, color);
        h.GeomMoveTo(float(xpos - y), float(ypos - x)); Draw(&h, color);
    }
}

void Renderer::EndPixels()
{
    // destrava a textura de plotagem de pixels
    direct3D->Context->Unmap(pixelPlotTexture, 0);

    // adiciona o sprite na lista de desenho
    RenderDraw(&pixelPlotSprite);
}

bool Renderer::Initialize(Window* window, Direct3D* direct3D)
{
    this->window = window;
    this->direct3D = direct3D;

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

    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));

    desc.Width = int(window->GetWndWidth());                // largura da textura
    desc.Height = int(window->GetWndHeigth());            // altura da textura
    desc.MipLevels = 1;                                // usa apenas um nível
    desc.ArraySize = 1;                                // cria apenas uma textura
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;        // formato RGBA de 32 bits
    desc.SampleDesc.Count = 1;                        // uma amostra por pixel (sem antialiasing)
    desc.Usage = D3D11_USAGE_DYNAMIC;                // alocada em RAM para acesso rápido via CPU
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;    // será acessada por um shader
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // CPU pode escrever na textura

    // cria textura a ser preenchida com pixels
    if FAILED(direct3D->Device->CreateTexture2D(&desc, nullptr, &pixelPlotTexture))
        return false;

    // configura visualização para a textura de pixels
    D3D11_SHADER_RESOURCE_VIEW_DESC pixelPlotDesc;
    ZeroMemory(&pixelPlotDesc, sizeof(pixelPlotDesc));

    pixelPlotDesc.Format = desc.Format;
    pixelPlotDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    pixelPlotDesc.Texture2D.MipLevels = desc.MipLevels;
    pixelPlotDesc.Texture2D.MostDetailedMip = desc.MipLevels - 1;

    // cria uma visualização para a textura de pixels
    if FAILED(direct3D->Device->CreateShaderResourceView((ID3D11Resource*)pixelPlotTexture, &pixelPlotDesc, &pixelPlotView))
        return false;

    // ---------------------------------------------
    // Sprite 
    // ---------------------------------------------

    pixelPlotSprite.SprtX = 0;
    pixelPlotSprite.SprtY = 0;
    pixelPlotSprite.SprtScale = 1.0f;
    pixelPlotSprite.SprtDepth = 0.0f;
    pixelPlotSprite.SprtRotation = 0.0f;
    pixelPlotSprite.SprtWidth = window->GetWndWidth();
    pixelPlotSprite.SprtHeight = window->GetWndHeigth();
    pixelPlotSprite.SprtTexture = pixelPlotView;
    
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