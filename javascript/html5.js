
Element = function(name) {
    console.log("Create Element: ", name);    
}

Document = function() {}

Document.prototype.addEventListener = function(name) {    
}

Window = function() {
	this.id = glfw_create_window();
	if (this.id == -1)
		throw Error("max 8 windows");
}

Window.prototype.close = function() {
	// todo
}

Window.prototype.addEventListener = function(name) {
}

Canvas = function() {
	this.id = 0;
}

Canvas.prototype.addEventListener = function(name) {    
}

Canvas.prototype.getContext = function(name) {
	return new GL;
}

DOMRect = function() {
	this.bottom = 920;
	this.height = 920;
	this.left = 0;
	this.right = 1086;
	this.top = 0;
	this.width = 1086;
	this.x = 0;
	this.y = 0;
}

Canvas.prototype.getBoundingClientRect = function() {
    return new DOMRect;
}

Navigator = function() {
    this.userAgent = "duktape";
}


HTMLCanvasElement = function() {}
HTMLImageElement  = function() {}
HTMLVideoElement  = function() {}

document = new Document;
window = new Window;
canvas = new Canvas;
window.navigator = new Navigator;

