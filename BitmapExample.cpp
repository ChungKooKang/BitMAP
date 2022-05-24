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

	// DrawPixelToBuffer(10, 10, D2D1::ColorF::Red);
	ClearBuffer(D2D1::ColorF::Orange);
	FillRectToBuffer(0, 0, 100, 100, D2D1::ColorF::Red);
	FillRectToBuffer(50, 50, 100, 100, D2D1::ColorF(0.0f, 1.0f, 0.0f, 0.5f));

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

	float inverse = 1.0f - color.a;
	UINT8 r = static_cast<UINT8>(color.r * 255);
	UINT8 g = static_cast<UINT8>(color.g * 255);
	UINT8 b = static_cast<UINT8>(color.b * 255);
	UINT8 a = static_cast<UINT8>(color.a * 255);

	mspBackBuffer[index]	 = static_cast<UINT8>(mspBackBuffer[index] * inverse + r * color.a);
	mspBackBuffer[index + 1] = static_cast<UINT8>(mspBackBuffer[index + 1] * inverse + g * color.a);
	mspBackBuffer[index + 2] = static_cast<UINT8>(mspBackBuffer[index + 2] * inverse + b * color.a);
	mspBackBuffer[index + 3] = 255;
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
	//UINT8* pCurrent = &mspBackBuffer[(left * BITMAP_BYTECOUNT) + (top * BITMAP_BYTECOUNT * BITMAP_WIDTH)]; // 시작점 넣기

	//for (int i = 0; i < height; i++)
	//{	
	//	for (int j = 0;j < width; j++)
	//	{
	//		*pCurrent = static_cast<UINT8>(color.r * 255);
	//		*(pCurrent + 1) = static_cast<UINT8>(color.g * 255);
	//		*(pCurrent + 2) = static_cast<UINT8>(color.b * 255);
	//		*(pCurrent + 3) = static_cast<UINT8>(color.a * 255);
	//		pCurrent += BITMAP_BYTECOUNT;
	//	}
	//	pCurrent += (BITMAP_WIDTH * BITMAP_BYTECOUNT - left * BITMAP_BYTECOUNT);
	//}

	// 모범답안

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; ++y)
		{
			DrawPixelToBuffer(left + x, top + y, color);
		}
	}
}

void BitmapExample::DrawCircleToBuffer(int centerX, int centerY, int radius, D2D1::ColorF color)
{
	const double PI{ 3.141592f };
	const int DIVIDEANGLE{ 1000 };
	UINT8* pCurrent = &mspBackBuffer[0];
	for (int y = centerY - radius, double radian = -PI / 2; y < centerY + radius; sin(y), radian = radian + (1 / 1000))
	{
		for (int x = 0; x < 2; x++)
		{
			DrawPixelToBuffer(cos(radian)((-1)^x),y , color);
		}
	}

	
}


