#include <stdio.h>
#include <memory>
#include <vector>

using namespace std;

struct Bitmap {
	Bitmap(int w, int h) : w(w), h(h), pixels(make_shared<vector<uint8_t>>(w*h)) {
	}

	uint8_t& operator[](const int &i) {
		return (*pixels)[i];
	}

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
	printf("width: %d height: %d\n", bitmap.w, bitmap.h);
	getchar();
	return 0;
}