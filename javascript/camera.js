/*
	campos = camera.origin;
	campos.z += 8;
	camera.origin = campos;
	
	camangles = camera.angles
	camangles.x += 0; // up/down
	camangles.y -= 0; // left/right
	camangles.z += 0; // not implemented
	camera.angles = camangles
*/

Camera = function() {}

Object.defineProperty(Camera.prototype, "origin", {
	get: function() {
		var tmp = radiant_camera_origin_get();
		return new pc.Vec3(tmp.x, tmp.y, tmp.z);
	},
	set: function(tmp) {
		radiant_camera_origin_set(tmp.x, tmp.y, tmp.z);
	}
});

Object.defineProperty(Camera.prototype, "angles", {
	get: function() {
		var tmp = radiant_camera_angles_get();
		return new pc.Vec3(tmp.x, tmp.y, tmp.z);
	},
	set: function(tmp) {
		radiant_camera_angles_set(tmp.x, tmp.y, tmp.z);
	}
});

Object.defineProperty(Camera.prototype, "forward", {
	get: function() {
		var tmp = radiant_camera_forward_get();
		return new pc.Vec3(tmp.x, tmp.y, tmp.z);
	}
});

camera = new Camera();