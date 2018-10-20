#include <stdio.h>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

struct Bitmap {
	Bitmap(int w, int h) : w(w), h(h), pixels(make_shared<vector<uint8_t>>(w*h)) {
	}

	uint8_t& operator[](const int &i) {
		return (*pixels)[i];
	}

    class {
        int value;
        public:
            int & operator = (const int &i) { return value = i; }
            operator int () const { return value; }
    } alpha;

	const int w;
	const int h;
private:
	shared_ptr<vector<uint8_t>> pixels;
};

void fill(Bitmap bm, uint32_t color) {
	for(int i=0; i<bm.w*bm.h; i++)
		bm[i] = color;
}

int main() {
	Bitmap bitmap(16, 16);
	fill(bitmap, 0x33);
	bitmap.alpha = 5;
	printf("width: %d height: %d alpha=%d\n", bitmap.w, bitmap.h, bitmap.alpha);
	getchar();
	return 0;
}