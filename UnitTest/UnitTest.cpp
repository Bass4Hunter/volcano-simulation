#include "pch.h"
#include "CppUnitTest.h"
#include "../volcano-simulation/Game.cpp"
#include "../volcano-simulation/PSO.cpp"
#include <chrono>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;



namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		TEST_METHOD(TestPSO)
		{
			PSO* pso;
			 pso= new PSO(glm::vec3(0,0,0), 1, 0.1, 1.1, 1.1, 0.5f, glm::vec3(0.f, -0.1f, 0.f));

			Assert::IsTrue(pso  != nullptr);
		}
		TEST_METHOD(TestFrameRate)
		{
			Game game("volcan-simulation",
				1920, 1080,
				4, 4,
				false);
			//MAIN LOOP
			float TARGET_FPS = 0;
			auto t1 = std::chrono::high_resolution_clock::now();
			auto t2 = std::chrono::high_resolution_clock::now();
			long long duration = -1;
			while (!game.getWindowShouldClose()&& duration <= 1)
			{
				//UPDATE INPUT ---
				game.update();
				game.render();
				TARGET_FPS++;
				t2 = std::chrono::high_resolution_clock::now();
				duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
			}
			Assert::IsTrue(TARGET_FPS > 20);
		}

	};
}
