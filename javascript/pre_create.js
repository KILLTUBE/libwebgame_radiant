function sprintf() {
	var ret = "";
	var param = 1; // maps to first %
	msg = arguments[0];
	for (var i=0; i<msg.length; i++) {
		if (msg[i] == "%") {
			// %% will be printed as normal %
			if (msg[i+1] == "%") {
				ret += "%";
				i++;
			} else
				ret += arguments[param++];
		} else {
			ret += msg[i];
		}
	}
	return ret;
}

function printf() {
	var ret = "";
	var param = 1; // maps to first %
	msg = arguments[0];
	for (var i=0; i<msg.length; i++) {
		if (msg[i] == "%") {
			// %% will be printed as normal %
			if (msg[i+1] == "%") {
				ret += "%";
				i++;
			} else
				ret += arguments[param++];
		} else {
			ret += msg[i];
		}
	}
	log(ret);
	return ret.length;
}

//function require(filename) {
//	var content = file_get_contents( filename );
//	try {
//		eval.bind( get_global() )(content);
//	} catch (e) {
//		printf("require(%): error %\n", filename, e.stack);
//	}
//}
//
//
//if (typeof date_start == "undefined")
//	date_start = Date.now();
//// time since loading this file for first time
//function now() {
//	return Date.now() - date_start;
//}
//
//require("assets/javascript/stack.js")
//require("assets/javascript/hierarchy.js")
//require("assets/javascript/hierarchy_dumped.js")
//require("assets/javascript/contextmenu.js")
//
//// oh well, I just overwrite the function defined in hierarchy.js, so this is called in kungcode.cpp:390, right after the root node is created... maybe i could put all the code just in JS
//add_hierarchy_to_root = function() {
//	create_hierarchy(root);
//	node_init_root();
//	ForEachNode(root, function(node) {
//		printf("enter node %\n", node.address);
//		node.Rebuild();
//	});
//	
//	
//	// rebuild all the intern pointers to the prototypes
//	rebuild();
//	
//	opsys.Rebuild();
//	
//	// lets keep track of all available JS operators here
//	ops = [OpAdd, OpAdd, OpAdd];
//	initialized = true;
//}
//
//PostReload = function() {
//	if (initialized)
//		rebuild();
//}
//
//
//
//require("assets/javascript/simplex1d.js")
//
////require("javascript/opengl.js")
////glDefines();
//
////require("javascript/matrixvector.js")
////require("javascript/zwindow.js")
////require("javascript/bezier.js")
////require("javascript/imgui.js")
////require("javascript/scenegraph.js")