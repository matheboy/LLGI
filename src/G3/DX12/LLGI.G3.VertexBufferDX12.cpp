
#include "../LLGI.G3.VertexBuffer.h"
#include "LLGI.G3.VertexBufferDX12.h"

namespace LLGI
{
namespace G3
{

VertexBufferDX12::VertexBufferDX12(GraphicsDX12* graphics)
{
	Initialize(graphics);
}

bool VertexBufferDX12::Initialize(GraphicsDX12* graphics)
{
	heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 0;
	heapProperties.VisibleNodeMask = 0;

	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = 256;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;

	SafeAddRef(graphics);
	graphics_ = CreateSharedPtr(graphics);

	auto hr = graphics_->GetDevice()->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE,&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBuffer));
	if (FAILED(hr)) {
		goto FAILED_EXIT;
	}
	SafeAddRef(vertexBuffer);
	return true;

FAILED_EXIT:
	SafeRelease(vertexBuffer);
	return false;
}

void* VertexBufferDX12::Lock()
{
	return nullptr;
}

void* VertexBufferDX12::Lock(int32_t offset, int32_t size)
{
	return nullptr;
}

void VertexBufferDX12::Unlock()
{
}

int32_t VertexBufferDX12::GetSize()
{
	return 0;
}

}
}