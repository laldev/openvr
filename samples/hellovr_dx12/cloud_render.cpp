/*
* File that does all the rendering on cloud server. The function calls to this file
* will be replaced by socket communication to move this to external PC/cloud
*/


#include "render.h"

void CMainApplication::RenderScene(vr::Hmd_Eye nEye)
{
	if (m_bShowCubes)
	{
		m_pCommandList->SetPipelineState(m_pScenePipelineState.Get());

		// Select the CBV (left or right eye)
		CD3DX12_GPU_DESCRIPTOR_HANDLE cbvHandle(m_pCBVSRVHeap->GetGPUDescriptorHandleForHeapStart());
		cbvHandle.Offset(nEye, m_nCBVSRVDescriptorSize);
		m_pCommandList->SetGraphicsRootDescriptorTable(0, cbvHandle);

		// SRV is just after the left eye 
		CD3DX12_GPU_DESCRIPTOR_HANDLE srvHandle(m_pCBVSRVHeap->GetGPUDescriptorHandleForHeapStart());
		srvHandle.Offset(SRV_TEXTURE_MAP, m_nCBVSRVDescriptorSize);
		m_pCommandList->SetGraphicsRootDescriptorTable(1, srvHandle);

		// Update the persistently mapped pointer to the CB data with the latest matrix
		memcpy(m_pSceneConstantBufferData[nEye], GetCurrentViewProjectionMatrix(nEye).get(), sizeof(Matrix4));

		// Draw
		m_pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_pCommandList->IASetVertexBuffers(0, 1, &m_sceneVertexBufferView);
		m_pCommandList->DrawInstanced(m_uiVertcount, 1, 0, 0);
	}

	bool bIsInputAvailable = m_pHMD->IsInputAvailable();

	if (bIsInputAvailable && m_pControllerAxisVertexBuffer)
	{
		// draw the controller axis lines
		m_pCommandList->SetPipelineState(m_pAxesPipelineState.Get());

		m_pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
		m_pCommandList->IASetVertexBuffers(0, 1, &m_controllerAxisVertexBufferView);
		m_pCommandList->DrawInstanced(m_uiControllerVertcount, 1, 0, 0);
	}

	// ----- Render Model rendering -----
	m_pCommandList->SetPipelineState(m_pRenderModelPipelineState.Get());
	for (uint32_t unTrackedDevice = 0; unTrackedDevice < vr::k_unMaxTrackedDeviceCount; unTrackedDevice++)
	{
		if (!m_rTrackedDeviceToRenderModel[unTrackedDevice] || !m_rbShowTrackedDevice[unTrackedDevice])
			continue;

		const vr::TrackedDevicePose_t& pose = m_rTrackedDevicePose[unTrackedDevice];
		if (!pose.bPoseIsValid)
			continue;

		if (!bIsInputAvailable && m_pHMD->GetTrackedDeviceClass(unTrackedDevice) == vr::TrackedDeviceClass_Controller)
			continue;

		const Matrix4& matDeviceToTracking = m_rmat4DevicePose[unTrackedDevice];
		Matrix4 matMVP = GetCurrentViewProjectionMatrix(nEye) * matDeviceToTracking;

		m_rTrackedDeviceToRenderModel[unTrackedDevice]->Draw(nEye, m_pCommandList.Get(), m_nCBVSRVDescriptorSize, matMVP);
	}
}

vr::Hmd_Eye RenderScene(vr::Hmd_Eye nEye);
{
	// call render
	// copy paste object changes
	// send object
	return nEye
}