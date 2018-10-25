
keyPressed = function(str) {
	var code = str.charCodeAt(0);
	return GetAsyncKeyState(code);
}

getNewDir = function(camera) {
	var newDir = new pc.Vec3();
	if (keyPressed('W')) newDir.add( camera.forward )
	if (keyPressed('S')) newDir.add( camera.backward )
	if (keyPressed('A')) newDir.add( camera.left )
	if (keyPressed('D')) newDir.add( camera.right )
	if (keyPressed('E')) newDir.add( camera.up )
	if (keyPressed('Q')) newDir.add( camera.down )
	if (newDir.equals(pc.Vec3.ZERO)) // dont normalize zero vector
		return newDir;
	return newDir.normalize();
}

update = function() {
	// do thread stuff etc. later
	var newDir = getNewDir(camera);
	var campos = camera.origin;
	campos.add(newDir);
	camera.origin = campos;
	Sys_UpdateWindows()
}