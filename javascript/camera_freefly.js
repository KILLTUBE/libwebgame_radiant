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

freefly_on = false;

freefly_start = function() {
	freefly_on = true;
	oldmousepos = mouse.position;
	console.log("FREEFLY");
}

freefly_stop = function() {
	freefly_on = false;
}

freefly_update = function() {
	if ( ! freefly_on)
		return;
	
	
	
	// do thread stuff etc. later
	var newDir = getNewDir(camera);
	var campos = camera.origin;
	campos.add(newDir);
	camera.origin = campos;
	
	
	// update angles from mouse pos
	var deltaMousePos = new pc.Vec2().sub2(mouse.position, oldmousepos );
	console.log("deltaMousePos", deltaMousePos)
	deltaMousePos.scale(1/5.333) // 1920 / 360 = 5.333
	camera.angles = camera.angles.add(new pc.Vec3( -deltaMousePos.y, -deltaMousePos.x, 0)) 
	mouse.position = oldmousepos; // reset mouse pos
	
	
	Sys_UpdateWindows()
}