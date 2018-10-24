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

require = function(filename) {
	var content = file_get_contents( filename );
	try {
		eval.bind( get_global() )(content);
	} catch (exception) {
		printf("require(%): error %\n", filename, exception.stack);
	}
}

includes = function() {
	var files = [
		"javascript/pre_create.js",
		"javascript/lib.js",
		"javascript/lib_quake.js",
		"javascript/html5.js",
		"javascript/html5_console.js",
		"javascript/html5_gl.js",
		"javascript/playcanvas-1-10-0.js",
		"javascript/camera.js",
		"javascript/camera_freefly.js",
		"javascript/mouse.js",
		"javascript/update.js",
	];
	for (var i=0; i<files.length; i++) {
		var file = files[i];
		try {
			require(file);
		} catch (exception) {
			printf(exception);
		}
	}
}
