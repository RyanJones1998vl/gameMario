#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "textures.h"

CTextures * CTextures::__instance = NULL;

CTextures::CTextures()
{

}

CTextures *CTextures::GetInstance()
{
	if (__instance == NULL) __instance = new CTextures();
	return __instance;
}

//void CTextures::Add(int id, LPCWSTR filePath, D3DCOLOR transparentColor)
//{
//	textures[id] = CGame::GetInstance()->LoadTexture(filePath);
//}

//LPDIRECT3DTEXTURE9 CTextures::Get(unsigned int i) 
//{
//	return textures[i];
//}

/// Modifications
void CTextures::Add(int id, LPCWSTR filePath, D3DCOLOR transparentColor) {
	if (textures.count(id)!=0) {
		OutputDebugString(L"[PARA_ERROR] CTextures::Add() : id already exists;");
		return;
	}
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(filePath, &info);
	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] GetImageInfoFromFile failed: %s\n", filePath);
		return;
	}

	LPDIRECT3DDEVICE9 d3ddv = CGame::GetInstance()->GetDirect3DDevice();
	LPDIRECT3DTEXTURE9 texture;

	result = D3DXCreateTextureFromFileEx(
		d3ddv,								// Pointer to Direct3D device object
		filePath,							// Path to the image to load
		info.Width,							// Texture width
		info.Height,						// Texture height
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		transparentColor,
		&info,
		NULL,
		&texture);								// Created texture pointer

	if (result != D3D_OK)
	{
		OutputDebugString(L"[ERROR] CreateTextureFromFile failed\n");
		return;
	}

	textures[id] = texture;

	DebugOut(L"[INFO] Texture loaded Ok: id=%d, %s \n", id, filePath);
}
LPDIRECT3DTEXTURE9 CTextures::Get(unsigned int id)
{
	if (textures.find(id) == textures.end()) {
		OutputDebugString(L"[OUT_ERROR] CTextures::Get() : id does not exist;");
		return NULL;
	}
	return textures[id];
}
