/*
	mousepos = mouse.position
	mousepos.add(new pc.Vec2(10,-5))
	mouse.position = mousepos
*/

Mouse = function() {}

Mouse.prototype.show = function() {
	radiant_mouse_show();
}

Mouse.prototype.hide = function() {
	radiant_mouse_hide();
}

Object.defineProperty(Mouse.prototype, "position", {
	get: function() {
		var tmp = radiant_mouse_position_get();
		return new pc.Vec2(tmp.x, tmp.y);
	},
	set: function(tmp) {
		radiant_mouse_position_set(tmp.x, tmp.y);
	}
});

mouse = new Mouse();