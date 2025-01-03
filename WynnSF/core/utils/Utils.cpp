#include "Utils.hpp"

namespace Core {
	namespace Utils {
		float fisqrt(float num) {
			//QUAKE GODS HAVE SPOKEN

			long i;
			float x2, y;
			const float threehalfs = 1.5F;

			x2 = num * 0.5F;
			y = num;
			i = *(long*)&y;
			i = 0x5f3759df - (i >> 1);
			y = *(float*)&i;
			y = y * (threehalfs - (x2 * y * y));
			// no need to have second guess thanks to the newtonian method, first and second guess resolve the same tangent on the curve, mind blowing...

			return y;
		}
	}
}