// this file is pretty much standalone, it only depends on exported duktape functions,
// used for bootstrapping all the other .js files

log = imgui_log;

printf = function() {
	var ret = "";
	var param = 1; // maps to first %
	var msg = arguments[0];
	for (var i=0; i<msg.length; i++) {
		if (msg[i] == "%") {
			// %% will be printed as normal %
			if (msg[i+1] == "%") {
				ret += "%";
				i++;
			} else {
				ret += arguments[param++];
			}
		} else {
			ret += msg[i];
		}
	}
	imgui_log(ret);
	return ret.length;
}

require = function(filename, bindTo) {
	var content = file_get_contents( filename );
	try {
		if (bindTo == undefined) {
			eval.bind( get_global() )(content);
		} else {
			
			(function() {
				eval(content);
			}).call(eval(bindTo)); // bindTo == "window"...because its define in other requires
		}
	} catch (exception) {
		printf("require(%): error %\n", filename, exception.stack);
	}
}

// prevent that playcanvas fucks "window" up into the global
// it does something like this, line 18: window = _window || window;
_window = {fakewindow: "yes"};

includes = function() {
	// this is a bit of a mess, but it works lol
	// todo: some proper module loading like nodejs or something
	
	var files = [
		["javascript/pre_create.js"       , undefined],
		["javascript/lib.js"              , undefined],
		["javascript/lib_quake.js"        , undefined],
		["javascript/glfw.js"             , undefined],
		["javascript/html5.js"            , undefined],
		["javascript/html5_console.js"    , undefined],
		["javascript/html5_gl.js"         , undefined],
		["javascript/playcanvas-1-10-0.js", "window" ],
		["javascript/camera.js"           , undefined],
		["javascript/camera_freefly.js"   , undefined],
		["javascript/mouse.js"            , undefined],
		["javascript/update.js"           , undefined],
		["javascript/quickscripts.js"     , undefined],
	];
	for (var i=0; i<files.length; i++) {
		var file = files[i][0];
		var bindTo = files[i][1];
		try {
			require(file, bindTo);
		} catch (exception) {
			printf(exception);
		}
	}
	
	pc = window.pc;
}
