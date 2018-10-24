#include <stdio.h>
#include <iostream>
#include <memory>
#include <vector>
#include "../jspp/object.h"
#include "../jspp/underscore.h"
#include "../jspp/operators.h"
#include "../jspp/constructors.h"

// http://sasq64.github.io/cpp-javascript.html
// 

#define catch(e) catch(var e)
#define throw throw _=
#define this This
#define new New
#define in :
#define function(...) [=] (var This, ##__VA_ARGS__) mutable -> Object



using namespace std;

class fancy_name {
public:
	// Getter
	std::string const& operator()() const {
		return "asd";	// Does some internal work
	}

	// Setter
	void operator()(std::string const& newName) {
		//_set_fancy_name(newName);		// Does some internal work

	}
};

// https://stackoverflow.com/questions/8368512/does-c11-have-c-style-properties
// https://codereview.stackexchange.com/questions/7786/c-like-class-properties
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
	try {
		// Initialize with a dummy value first or it'll go into an infinite loop
		global["Function"] = "Function";
		global["Function"] = function(var s) {
			throw _("Cannot eval in C++ sorry :(");
		};
		global["Function"]["prototype"] = {};
		global["Function"]["prototype"]["call"] = function (var self, var a, var b, var c, var d) {
			This.self = &self;
			return This(a, b, c, d);
		};
		var Function = global["Function"];

		global["typeof"] = function (Object o) {
			if (o.s->type == NUMBER) {
				return "number";
			}
			if (o.s->type == STRING) {
				return "string";
			}
			if (o.s->type == UNDEFINED) {
				return "undefined";
			}
			if (o.s->type == NULLL) {
				return "null";
			}
			if (o.s->type == OBJECT) {
				return "object";
			}
			return undefined;
		};
		var Typeof = global["typeof"];

		global["Array"] = function () {
			return {};
		};
		var Array = global["Array"];


		Bitmap bitmap(16, 16);
		fill(bitmap, 0x33);
		bitmap.alpha = 0.5;
		//printf("width: %d height: %d alpha=%f\n", bitmap.w, bitmap.h, bitmap.alpha);

		Float32 value;
		value = 0.5;
		//cout << "cout value: " << bitmap.alpha << endl; // correct way
		//printf("printf value: %f", value); // never use old C varargs functions with modern C++ operators, it will return garbage

	} catch (e) {
		std::cout << "Uncatched error: " << e << std::endl;
	}

	getchar();
	return 0;
}