#include <stdio.h>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

class Float32 {
	public:
		float & operator = (const float &newValue) {
			return value = newValue;
		}
		operator float () const {
			return value;
		}
	protected:
		float value;
};

struct Bitmap {
	Bitmap(int w, int h) : w(w), h(h), pixels(make_shared<vector<uint8_t>>(w*h)) {
	}

	uint8_t& operator[](const int &i) {
		return (*pixels)[i];
	}

    Float32 alpha;

	const int w;
	const int h;
private:
	shared_ptr<vector<uint8_t>> pixels;
};

void fill(Bitmap bm, uint32_t color) {
	for(int i=0; i<bm.w*bm.h; i++)
		bm[i] = color;
}

using namespace std;

int main() {
	Bitmap bitmap(16, 16);
	fill(bitmap, 0x33);
	bitmap.alpha = 0.5;
	printf("width: %d height: %d alpha=%f\n", bitmap.w, bitmap.h, bitmap.alpha);

	Float32 value;
	value = 0.5;
	cout << "cout value: " << bitmap.alpha << endl; // correct way
	//printf("printf value: %f", value); // never use old C varargs functions with modern C++ operators, it will return garbage

	getchar();
	return 0;
}