#include "pch.h"
#include "CppUnitTest.h"
#include "../bot.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{
	TEST_CLASS(tests)
	{
	public:
		
		TEST_METHOD(TestDetectsLapCompletion)
		{
			Track track;
			track.addCheckpoint(pair(10, 20));
			track.addCheckpoint(pair(10, 30));
			Assert::AreEqual(track.lap(), 1);
			track.addCheckpoint(pair(10, 20));
			Assert::AreEqual(track.lap(), 2);
		}
	};
}
