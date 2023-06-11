#pragma once

#include <windows.h>
#include "d3dx12.h"
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl.h>
#include <D3Dcompiler.h>

#include <stdio.h>
#include <string>
#include <cstdlib>

#include "shared/Matrices.h"

#include <openvr.h>

#include "types.h"

using Microsoft::WRL::ComPtr;

class DX12RenderModel
{
public:
	DX12RenderModel(const std::string& sRenderModelName);
	~DX12RenderModel();

	bool BInit(ID3D12Device* pDevice, ID3D12GraphicsCommandList* pCommandList, ID3D12DescriptorHeap* pCBVSRVHeap, vr::TrackedDeviceIndex_t unTrackedDeviceIndex, const vr::RenderModel_t& vrModel, const vr::RenderModel_TextureMap_t& vrDiffuseTexture);
	void Cleanup();
	void Draw(vr::EVREye nEye, ID3D12GraphicsCommandList* pCommandList, UINT nCBVSRVDescriptorSize, const Matrix4& matMVP);
	const std::string& GetName() const { return m_sModelName; }
	void GenMipMapRGBA(const UINT8* pSrc, UINT8** ppDst, int nSrcWidth, int nSrcHeight, int* pDstWidthOut, int* pDstHeightOut);

private:
	ComPtr< ID3D12Resource > m_pVertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
	ComPtr< ID3D12Resource > m_pIndexBuffer;
	D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
	ComPtr< ID3D12Resource > m_pTexture;
	ComPtr< ID3D12Resource > m_pTextureUploadHeap;
	ComPtr< ID3D12Resource > m_pConstantBuffer;
	UINT8* m_pConstantBufferData[2];
	size_t m_unVertexCount;
	vr::TrackedDeviceIndex_t m_unTrackedDeviceIndex;
	ID3D12DescriptorHeap* m_pCBVSRVHeap;
	std::string m_sModelName;
};
