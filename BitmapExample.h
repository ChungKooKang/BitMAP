#pragma once
#include "D2DFramework.h"

const int BITMAP_WIDTH{ 1024 };
const int BITMAP_HEIGHT{ 768 };
const int BITMAP_BYTECOUNT{ 4 };

class BitmapExample : public D2DFramework
{
private :
	Microsoft::WRL::ComPtr<ID2D1Bitmap>		mspFrameBitmap{}; // VRAM : 행열형태
	std::unique_ptr<UINT8[]>				mspBackBuffer; // RAM : 배열형태


public :
	virtual HRESULT Initialize(HINSTANCE hInstance, LPCWSTR title = L"D2D Bitmap Example", UINT width = 1024, UINT height = 768) override;
	virtual void Render() override;

	void PresentBuffer(); // 메모리에서 그림파일로 복사
	void DrawPixelToBuffer(int x, int y, D2D1::ColorF color);
	void ClearBuffer(D2D1::ColorF color);
	void FillRectToBuffer(int left, int top, int width, int height, D2D1::ColorF color);


};

