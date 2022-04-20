#include "pch.h"
#include "CppUnitTest.h"
#include "../bot.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

bool DoublesAreEqual(double a, double b) {
	double error = 0.000001;
	return b - error < a && a < b + error;
}

double roundDouble(double x, int decimalPlaces) {
	return round(x * pow(10, decimalPlaces)) * pow(10, decimalPlaces);
}

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
		TEST_METHOD(Test_angleFrom_90deg_angle)
		{
			Point a(0, 0), b(0, 10), c(10, 10);
			Assert::IsTrue(DoublesAreEqual(pi / 2, util::angleFrom(a, b, c)));
			Assert::AreEqual(roundDouble(pi / 2, 4), roundDouble(util::angleFrom(a, b, c), 4));
		}

		TEST_METHOD(TestAngleBetweenMethod) {
			Assert::AreEqual(pi / 2, Vector::angleBetween(Vector(0, 1), Vector(1, 0)));

		}
	};
}
