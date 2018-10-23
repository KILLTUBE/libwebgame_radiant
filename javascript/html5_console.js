Console = function() {}

Console.prototype.log = function() {
	for (var i=0; i<arguments.length; i++) {
		imgui_log(arguments[i] + " ");
	}
	imgui_log("\n");
}

Console.prototype.warn = function() {
	for (var i=0; i<arguments.length; i++) {
		imgui_log(arguments[i] + " ");
	}
	imgui_log("\n");
}

Console.prototype.error = function() {
	for (var i=0; i<arguments.length; i++) {
		imgui_log(arguments[i] + " ");
	}
	imgui_log("\n");
}

console = new Console();