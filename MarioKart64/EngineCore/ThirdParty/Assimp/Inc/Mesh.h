#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <d3d11_1.h>
#include <DirectXMath.h>

#include "SafeRelease.hpp"

struct VERTEX
{
	FLOAT X, Y, Z;
    DirectX::XMFLOAT2 texcoord;
};

struct AiTexture
{
	std::string type;
	std::string path;
	ID3D11ShaderResourceView *AiTexture;

	void Release() {
		SafeRelease(AiTexture);
	}
};

class Mesh {
public:
    std::vector<VERTEX> vertices_;
    std::vector<UINT> indices_;
    std::vector<AiTexture> AiTextures_;
    ID3D11Device *dev_;

    Mesh(ID3D11Device *dev, const std::vector<VERTEX>& vertices, const std::vector<UINT>& indices, const std::vector<AiTexture>& AiTextures) :
            vertices_(vertices),
            indices_(indices),
            AiTextures_(AiTextures),
            dev_(dev),
            VertexBuffer_(nullptr),
            IndexBuffer_(nullptr) {
        this->setupMesh(this->dev_);
    }

    void Draw(HWND _hwnd, ID3D11DeviceContext *devcon) {
        UINT stride = sizeof(VERTEX);
        UINT offset = 0;

        // MessageBoxA(_hwnd, "1", "TEMP", MB_OK);
        std::string log = "";

        devcon->IASetVertexBuffers(0, 1, &VertexBuffer_, &stride, &offset);
        // MessageBoxA(_hwnd, "2", "TEMP", MB_OK);

        devcon->IASetIndexBuffer(IndexBuffer_, DXGI_FORMAT_R32_UINT, 0);
        log = "3. AiTextures_: " + std::to_string(AiTextures_.size()) + "\n";
        // MessageBoxA(_hwnd, log.c_str(), "TEMP", MB_OK);

        if (AiTextures_.size() > 0)
        {
            devcon->PSSetShaderResources(0, 1, &AiTextures_[0].AiTexture);
        }
        log = "4. indices_: " + std::to_string(indices_.size()) + "\n";
        // MessageBoxA(_hwnd, log.c_str(), "TEMP", MB_OK);

        devcon->DrawIndexed(static_cast<UINT>(indices_.size()), 0, 0);
        // MessageBoxA(_hwnd, "5", "TEMP", MB_OK);
    }

    void Close() {
        SafeRelease(VertexBuffer_);
        SafeRelease(IndexBuffer_);
    }
private:
    // Render data
    ID3D11Buffer *VertexBuffer_, *IndexBuffer_;

    // Functions
    // Initializes all the buffer objects/arrays
    void setupMesh(ID3D11Device *dev) {
        HRESULT hr;

        D3D11_BUFFER_DESC vbd;
        vbd.Usage = D3D11_USAGE_IMMUTABLE;
        vbd.ByteWidth = static_cast<UINT>(sizeof(VERTEX) * vertices_.size());
        vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vbd.CPUAccessFlags = 0;
        vbd.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA initData;
        initData.pSysMem = &vertices_[0];

        hr = dev->CreateBuffer(&vbd, &initData, &VertexBuffer_);
        if (FAILED(hr)) {
            Close();
            throw std::runtime_error("Failed to create vertex buffer.");
        }

        D3D11_BUFFER_DESC ibd;
        ibd.Usage = D3D11_USAGE_IMMUTABLE;
        ibd.ByteWidth = static_cast<UINT>(sizeof(UINT) * indices_.size());
        ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        ibd.CPUAccessFlags = 0;
        ibd.MiscFlags = 0;

        initData.pSysMem = &indices_[0];

        hr = dev->CreateBuffer(&ibd, &initData, &IndexBuffer_);
        if (FAILED(hr)) {
            Close();
            throw std::runtime_error("Failed to create index buffer.");
        }
    }
};
