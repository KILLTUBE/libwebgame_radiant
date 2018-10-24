/*
	tmp = camera.origin;
	tmp.z += 8;
	camera.origin = tmp;
*/

Camera = function() {}

Object.defineProperty(Camera.prototype, "origin", {
	get: function() {
		var tmp = radiant_camera_origin_get();
		return new pc.Vec3(tmp.x, tmp.y, tmp.z);
	},
	set: function(newOrigin) {
		radiant_camera_origin_set(newOrigin.x, newOrigin.y, newOrigin.z);
	}
});

Object.defineProperty(Camera.prototype, "forward", {
	get: function() {
		var tmp = radiant_camera_forward_get();
		return new pc.Vec3(tmp.x, tmp.y, tmp.z);
	}
});

camera = new Camera();