#include "TestHelper.h"
#include "test.h"

// #define CAPTURE_TEST 1

#if defined(__linux__) || defined(__APPLE__) || defined(WIN32)
class ClearTest : public ::testing::Test
{
};

TEST_F(ClearTest, Clear1) {}
#endif

void test_clear_update(LLGI::DeviceType deviceType)
{
	int count = 0;

	auto window = std::unique_ptr<LLGI::Window>(LLGI::CreateWindow("ClearUpdate", LLGI::Vec2I(1280, 720)));
	auto platform = LLGI::CreatePlatform(deviceType, window.get());

	auto graphics = platform->CreateGraphics();
	auto sfMemoryPool = graphics->CreateSingleFrameMemoryPool(1024 * 1024, 128);
	LLGI::CommandList* commandList[] = {
		graphics->CreateCommandList(sfMemoryPool), graphics->CreateCommandList(sfMemoryPool), graphics->CreateCommandList(sfMemoryPool)};

	while (count < 1000)
	{
		if (!platform->NewFrame())
			break;

		sfMemoryPool->NewFrame();

		LLGI::Color8 color;
		color.R = count % 255;
		color.G = 0;
		color.B = 0;
		color.A = 255;

		commandList[count % 3]->Begin();
		commandList[count % 3]->BeginRenderPass(platform->GetCurrentScreen(color, true));
		commandList[count % 3]->EndRenderPass();
		commandList[count % 3]->End();

		graphics->Execute(commandList[count % 3]);

		platform->Present();
		count++;
	}

	graphics->WaitFinish();

	LLGI::SafeRelease(sfMemoryPool);
	LLGI::SafeRelease(commandList[0]);
	LLGI::SafeRelease(commandList[1]);
	LLGI::SafeRelease(commandList[2]);
	LLGI::SafeRelease(graphics);
	LLGI::SafeRelease(platform);
}

void test_clear(LLGI::DeviceType deviceType)
{
	int count = 0;

	auto window = std::unique_ptr<LLGI::Window>(LLGI::CreateWindow("Clear", LLGI::Vec2I(1280, 720)));
	auto platform = LLGI::CreatePlatform(deviceType, window.get());

	auto graphics = platform->CreateGraphics();
	auto sfMemoryPool = graphics->CreateSingleFrameMemoryPool(1024 * 1024, 128);
	LLGI::CommandList* commandList[] = {
		graphics->CreateCommandList(sfMemoryPool), graphics->CreateCommandList(sfMemoryPool), graphics->CreateCommandList(sfMemoryPool)};

	LLGI::Color8 color;
	color.R = 255;
	color.G = 0;
	color.B = 0;
	color.A = 255;

	while (count < 1000)
	{
		if (!platform->NewFrame())
			break;

		sfMemoryPool->NewFrame();

		// It need to create a command buffer between NewFrame and Present.
		// Because get current screen returns other values by every frame.
		commandList[count % 3]->Begin();
		commandList[count % 3]->BeginRenderPass(platform->GetCurrentScreen(color, true));
		commandList[count % 3]->EndRenderPass();
		commandList[count % 3]->End();

		graphics->Execute(commandList[count % 3]);

		platform->Present();
		count++;

#if CAPTURE_TEST
		if (count == 5)
		{
			// TODO wait to finish commandList
			auto texture = platform->GetCurrentScreen(color, true)->GetColorBuffer(0);
			auto data = graphics->CaptureRenderTarget(texture);

			// save
			Bitmap2D(data, texture->GetSizeAs2D().X, texture->GetSizeAs2D().Y, true).Save("stbpng.png");

			// test
			int rate = Bitmap2D::CompareBitmap(
				Bitmap2D(data, texture->GetSizeAs2D().X, texture->GetSizeAs2D().Y, true), Bitmap2D("stbpng.png"), 5);
			std::cout << rate;
		}
#endif
	}

	graphics->WaitFinish();

	LLGI::SafeRelease(sfMemoryPool);
	LLGI::SafeRelease(commandList[0]);
	LLGI::SafeRelease(commandList[1]);
	LLGI::SafeRelease(commandList[2]);
	LLGI::SafeRelease(graphics);
	LLGI::SafeRelease(platform);
}
