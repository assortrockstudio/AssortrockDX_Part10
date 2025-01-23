#ifndef _TILEMAP
#define _TILEMAP

#include "value.fx"

struct tTileInfo
{
    uint ImgIdx;
    float3 vPadding;
};

struct VS_IN
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

// ============================
// TileMapShader
// Parameter
#define AtlasMaxRow         g_int_0
#define AtlasMaxCol         g_int_1
#define TileImgIdx          g_int_2
#define AtlasResolution     g_vec2_0
#define AtlasTileSize       g_vec2_1
#define TileColRow          g_vec2_2

#define IsCaptureMode       g_int_3

StructuredBuffer<tTileInfo> g_TileInfo : register(t20);
// ============================

VS_OUT VS_TileMap(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    if(IsCaptureMode)
    {
        output.vPosition = float4(float3(_in.vLocalPos * 2.f), 1.f);
    }
    else
    {
        _in.vLocalPos.xy += float2(0.5f, -0.5f);
        output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    }
    output.vUV = _in.vUV;
    
    return output;
}


float4 PS_TileMap(VS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    // 입력된 UV (0 ~ 1 사이) 를 실제 타일 행렬 개수에 맞추어 확장한다.   
    _in.vUV *= TileColRow;
        
    // 확장시킨 UV 에서 해당 픽셀쉐이더의 타일상에서의 위치값을 행렬로 알아낼수 있다.
    int2 PixelColRowIdx = (int2) _in.vUV;    
    
    // 확장시킨 UV 에서 소수점만 취한다. 해당픽셀이 해당 칸 내에서 어느위치인지를 알 수 있다.
    _in.vUV = _in.vUV - floor(_in.vUV);
    
    // 아틀라스 텍스처 상에서 하나의 타일 이미지가 차지하는 크기를 UV 단위로 구한다.
    float2 TileSize = AtlasTileSize / AtlasResolution;    
    
    // 해당 픽셀이 참조해야 하는 타일 좌상단에서 참조할 UV 좌표를 구함
    float2 vUV = TileSize * _in.vUV;
    
    // 픽셀이 위치하고 있는 타일이 누군지 알아내서, 그 타일의 이미지인덱스 정보를 구조화버퍼에서 꺼낸다.
    int imgidx = g_TileInfo[PixelColRowIdx.y * TileColRow.x + PixelColRowIdx.x].ImgIdx;    
    
    // 아틀라스 상에서 내가 참조하려는 타일의 좌상단 UV 를 구한다.
    int row = imgidx / AtlasMaxCol;
    int col = imgidx % AtlasMaxCol;
    float2 LeftTop = float2(col * TileSize.x, row * TileSize.y);
    
    // 아틀라스 전체 내에서 해당 이미지 좌상단 좌표 + 이미지 좌상단에서 참조할 위치 UV
    vUV = LeftTop + vUV;
    
    vOutColor = g_tex_0.Sample(g_sam_0, vUV);
    
    return vOutColor;
}

#endif