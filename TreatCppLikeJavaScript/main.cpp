#include <stdio.h>
#include <iostream>
#include <memory>
#include <vector>
#include "../jspp/object.h"
#include "../jspp/underscore.h"
#include "../jspp/operators.h"
#include "../jspp/constructors.h"
#include "../jspp/console.h"

// http://sasq64.github.io/cpp-javascript.html

// #define catch(e) catch(var e)
#define throw throw _=
//#define this This
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

// recursive
template<typename T, typename... Args>
void printf(const char *s, T value, Args... args)
{
    while (*s)
    {
        if (*s == '%')
        {
            if (*(s + 1) != '%')
            {
                std::cout << value;
                s += 2; // only works on 2-character format strings ( %d, %f, etc ); fails with %5.4f
                printf(s, args...); // called even when *s is 0 but does nothing in that case (and ignores extra arguments)
                return;
            }

            ++s;
        }

        std::cout << *s++;
    }    
}

void print_object(...) {
	//cout << o;
}

template<typename... Args> inline void pass(Args&&... asd) {
	cout << "passed: " << asd << endl;
}

template<typename... Args> inline void expand(Args&&... args) {
	printf("expand()\n");
	pass(args...);

	//pass( print_object(args)... );
}


template <typename T>
void bar(T t) {}

void foo2() {}

template <typename Car, typename... Cdr>
void foo2(Car car, Cdr... cdr) {
	bar(car);
	foo2(cdr...);
}

// expand(42, "answer", true);
// which will expand to something like:
// pass( some_function(arg1), some_function(arg2), some_function(arg3) etc... );

class CanvasInternal { public:
	int width;
	int height;
	vector<uint8_t> buffer;
};
//shared_ptr<CanvasInternal> canvasInternal;
//class CanvasWidth

class Canvas { public:
	void resize() {
		cout << "resize to " << width << " " << height << endl;
	}

	Canvas(int w, int h) : width(*this), height(*this) {
		cout << "new canvas " << w << " " << h << endl;
		width.value = w;
		height.value = h;
	}

	class Width { public:
		Canvas& canvas;
		int value;
		Width(Canvas& canvas): canvas(canvas) {}
		int & operator = (const int &i) {
			value = i;
			canvas.resize();
			return value;
		}
		operator int () const {
			return value;
		}
	} width;

	class Height { public:
		Canvas& canvas;
		int value;
		Height(Canvas& canvas): canvas(canvas) {}
		int & operator = (const int &i) {
			value = i;
			canvas.resize();
			return value;
		}
		operator int () const {
			return value;
		}
	} height;
};

int main() {

	Canvas canvas(256, 256);
	canvas.width = 128;
	canvas.height = 64;



	//Console console;
	//console.log("bunch", "of", "arguments");
	//console.warn("or some numbers:", 1, 2, 3);
	//console.error("just a prank", "bro");

	
	//try {
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

		auto args = {1,2,3};

		var a = 1;
		var b = 2;
		var c = a + b;
		Console console;
		console.log("a", a, "b", b, "c", c); // prints "a 1 b 2 c 3\n"
		//console.log(console);

		//var Console2 = function() {
		//	console.log("hello from Console2 constructor");
		//	return this;
		//};
		//Console2["log"] = function() {
		//	console.log("hello from Console2.log");
		//	return "idk";
		//};
		//var console2 = new(Console2);
		// throws "Object is not a function"... syntax is pretty aids nonetheless, gonna make real classes which inherit from Object
		//console2["log"]("HAI FROM LOG");



		//print(1, ':', " Hello", ',', " ", "World!!!");
		//expand(42, "answer", true);

	//} catch (e) {
	//	std::cout << "Uncatched error: " << e << std::endl;
	//}

	getchar();
	return 0;
}