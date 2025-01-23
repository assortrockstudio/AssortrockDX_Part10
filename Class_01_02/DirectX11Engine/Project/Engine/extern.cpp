#include "pch.h"

tTransform g_Trans = {};
tGlobalData g_GlobalData = {};

extern Vec3	XAxis(1.f, 0.f, 0.f);
extern Vec3	YAxis(0.f, 1.f, 0.f);
extern Vec3	ZAxis(0.f, 0.f, 1.f);


const char* COMPONENT_TYPE_STRING[(UINT)COMPONENT_TYPE::END]
=
{
	"TRANSFORM",
	"CAMERA",
	"COLLIDER2D",
	"COLLIDER3D",
	"ANIMATOR2D",
	"ANIMATOR3D",
	"LIGHT2D",
	"LIGHT3D",
	"BOUNDING_BOX"
	"MESHRENDER",
	"SKYBOX",
	"DECAL",
	"PARTICLESYSTEM",
	"TILEMAP",
	"LANDSCAPE",
};

const char* ASSET_TYPE_STRING[(UINT)ASSET_TYPE::END]
=
{
	"PREFAB",
	"MESH",
	"MESH_DATA",
	"MATERIAL",
	"TEXTURE",
	"SOUND",
	"GRAPHICS_SHADER",
	"COMPUTE_SHADER",
};