#pragma once
#include "../Utils/Utils.hpp"
#include <cmath>
namespace Core {
	namespace Physics {
		class Vec2D {
		public:
			float x = 0;
			float y = 0;

			Vec2D(float x, float y) {
				this->x = x;
				this->y = y;
			};

			Vec2D() {};

			Vec2D operator+(Vec2D rhs) {
				return Vec2D(rhs.x + this->x, rhs.y + this->y);
			};

			Vec2D operator-(Vec2D rhs) {
				return Vec2D(rhs.x - this->x, rhs.y - this->y);
			};

			Vec2D operator/(Vec2D rhs) {
				return Vec2D(rhs.x / this->x, rhs.y / this->y);
			};

			Vec2D operator*(Vec2D rhs) {
				return Vec2D(rhs.x * this->x, rhs.y * this->y);
			};

			void operator+=(Vec2D rhs) {
				this->x += rhs.x;
				this->y += rhs.y;
			}

			void operator-=(Vec2D rhs) {
				this->x -= rhs.x;
				this->y -= rhs.y;
			}

			void operator/=(Vec2D rhs) {
				this->x /= rhs.x;
				this->y /= rhs.y;
			}

			void operator*=(Vec2D rhs) {
				this->x *= rhs.x;
				this->y *= rhs.y;
			}

			Vec2D operator*(float rhs) {
				return Vec2D(this->x * rhs, this->y * rhs);
			}

			bool operator==(Vec2D rhs) {
				return (this->x == rhs.x && this->y == rhs.y);
			}

			bool operator!=(Vec2D rhs) {
				return (this->x != rhs.x || this->y != rhs.y);
			}

			void Normalize() {
				float invlength = Core::Utils::fisqrt(((x * x) + (y * y))); //shout out quake
				this->x *= invlength;
				this->y *= invlength;

			};



		};
	}
}