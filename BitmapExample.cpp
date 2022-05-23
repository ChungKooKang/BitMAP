#include "BitmapExample.h"

HRESULT BitmapExample::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	D2DFramework::Initialize(hInstance, title, width, height);

	mspBackBuffer = std::make_unique<UINT8[]>(BITMAP_WIDTH * BITMAP_HEIGHT * BITMAP_BYTECOUNT);  //heap에 저장하려면 동적으로 할당해야 한다!!

	HRESULT hr;
	hr = mspRenderTarget->CreateBitmap(
		D2D1::SizeU(BITMAP_WIDTH, BITMAP_HEIGHT),
		D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)),
		mspFrameBitmap.GetAddressOf()
	);

	ThrowIfFailed(hr);
	
	return hr;
}

void BitmapExample::Render()
{
	//D2DFramework::Render(); //이것을 안 쓰는 이유는 이 코드 내에 Begin end가 있어서 걍 끝나버린다.

	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

	DrawPixelToBuffer(10, 10, D2D1::ColorF::Red);
	ClearBuffer(D2D1::ColorF::Orange);
	FillRectToBuffer(100, 100, 100, 100, D2D1::ColorF::LawnGreen);

	PresentBuffer();
	mspRenderTarget->DrawBitmap(mspFrameBitmap.Get());

	mspRenderTarget->EndDraw();
}

void BitmapExample::PresentBuffer()
{
	mspFrameBitmap->CopyFromMemory(nullptr, mspBackBuffer.get(), BITMAP_WIDTH * BITMAP_BYTECOUNT);

}

void BitmapExample::DrawPixelToBuffer(int x, int y, D2D1::ColorF color)
{
	int pitch = BITMAP_WIDTH * BITMAP_BYTECOUNT;
	int index = x * BITMAP_BYTECOUNT + y * pitch;

	mspBackBuffer[index] = static_cast<UINT8>(color.r * 255);
	mspBackBuffer[index + 1] = static_cast<UINT8>(color.g * 255);
	mspBackBuffer[index + 2] = static_cast<UINT8>(color.b * 255);
	mspBackBuffer[index + 3] = static_cast<UINT8>(color.a * 255);
}

void BitmapExample::ClearBuffer(D2D1::ColorF color)
{
	//for (int x = 0; x < BITMAP_WIDTH; x++)
	//{
	//	for (int y = 0; y < BITMAP_HEIGHT; y++)
	//	{
	//		DrawPixelToBuffer(x, y, color);
	//	}
	//}

	UINT8* pCurrent = &mspBackBuffer[0];
	for (int i = 0; i < BITMAP_WIDTH * BITMAP_HEIGHT; i++)
	{
		*pCurrent = static_cast<UINT8>(color.r * 255);
		*(pCurrent + 1) = static_cast<UINT8>(color.g * 255);
		*(pCurrent + 2) = static_cast<UINT8>(color.b * 255);
		*(pCurrent + 3) = static_cast<UINT8>(color.a * 255);

		pCurrent+= BITMAP_BYTECOUNT;
	}
}

void BitmapExample::FillRectToBuffer(int left, int top, int width, int height, D2D1::ColorF color)
{
	UINT8* pCurrent = &mspBackBuffer[left * BITMAP_BYTECOUNT + (top * BITMAP_BYTECOUNT * BITMAP_WIDTH)];

	for (int i = 0; i < width * height * BITMAP_BYTECOUNT * BITMAP_BYTECOUNT; i++)
	{
		*pCurrent = static_cast<UINT8>(color.r * 255);
		*(pCurrent + 1) = static_cast<UINT8>(color.g * 255);
		*(pCurrent + 2) = static_cast<UINT8>(color.b * 255);
		*(pCurrent + 3) = static_cast<UINT8>(color.a * 255);

		pCurrent += BITMAP_BYTECOUNT;
	}
}
