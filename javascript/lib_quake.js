// MIT License, have fun

function sprintf() {
	var ret = "";
	var param = 1; // maps to first %
	var msg = arguments[0];
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
	var msg = arguments[0];
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

function require(filename) {
	var content = file_get_contents( filename );
	try {
		eval.bind( get_global() )(content);
	} catch (e) {
		printf("require(%): error %\n", filename, e.stack);
	}
}


if (typeof date_start == "undefined")
	date_start = Date.now();
// time since loading this file for first time
function now() {
	return Date.now() - date_start;
}

print = function() {
	for (var i=0; i<arguments.length; i++) {
		log(arguments[i]);
	}
}

function var_dump(ret) {
	switch (typeof ret) {
		case "number": {
			print("ret = ", ret, ";");
			break;
		}
		case "string": {
			print("ret = \"", ret, "\";");
			break;
		}
		case "function": {
			// print infos like byte codes or length of byte codes
			print("Function: ", ret);
			break;
		}
		case "boolean": {
			print("ret = ", ret, ";");
			break;
		}
		case "object": {
			if (ret.constructor.name == "Array") {
				
				print("ret = [\n");
				for (var i=0; i<ret.length; i++) {
					if (typeof ret[i] == "object")
						printf("\t%: % {...},\n", i, ret[i].constructor.name);
					else
						printf("\t%: %,\n", i, ret[i]);
				}
				print("];\n");
			}
			
			// An array still can have properties, so print them aswell:
			{
				printf("% {\n", ret.constructor.name);
				for (var i in ret) {
					if (typeof ret[i] == "object")
						printf("\t%: % {...},\n", i, ret[i].constructor.name);
					else
						printf("\t%: %,\n", i, ret[i]);
				}
				print("};\n");
			}
			break;
		}
		case "undefined": {
			// print infos like byte codes or length of byte codes
			print("undefined;");
			break;
		}
		default:
			print("Unhandled type: ", typeof ret);
	}
	
}

handle_input = function(code, global) {
	try {
		var ret = eval.bind(global)(code); 
		
		log("> ");
		var_dump(ret);
		log("\n");
		
	} catch (e) {
		print("handle_input> error: ", e.stack, "\n");
	}
	
	// The user might have changed a prototype definition, so we gotta make sure every op has a fresh object pointer to the prototype
	//rebuild();
	
	//repl_settext("asdddd");
}


//global = get_global();
//printf("Global: %", global);
//var_dump(global);

consolecommand = function() {
	var args = getargs();
	var cmdline = "";
	for (var i=1; i<args.length; i++)
		cmdline += args[i] + " ";
	//printf("cmdline: %\n", cmdline);
	handle_input(cmdline, get_global());
}

//function ThreadState()
function handleYield(runframeData) {
	var ret = eval(runframeData);
	Duktape.Thread.yield( ret ); // put thread back to sleep, we just wanted to eval something in its context
}
function wait(time)         { var resumeArg = Duktape.Thread.yield( ["wait",            time * 1000] ); if (resumeArg) handleYield(resumeArg); }
function waittillframeend() { var resumeArg = Duktape.Thread.yield( ["waittillframeend"            ] ); if (resumeArg) handleYield(resumeArg); }
function waittill(what)     { var resumeArg = Duktape.Thread.yield( ["waittill",        what       ] ); if (resumeArg) handleYield(resumeArg); }
function killthread()       { level.currentthread.userkill = true;                   }

if (typeof level == "undefined") {
	level = {};
	level.time = 0;
}

/*
pos = entities[0].traceEyeEntity(1024).position
ent = spawnentity()
ent.setOrigin(pos)
ent.setModel("kungmodels/heart.kung1")
*/
function spawnentity() {
	return entities[g_spawn()];
}

function Entity(id) {
	this.id = id;
	this.threads = [];
	this.thread = function(func) {
		var self = this;
		return function() {
			var t = new Duktape.Thread(function(args) {
				func.bind(self)(args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]);
				//func.apply(self, arguments) // waiting for: https://github.com/svaarala/duktape/issues/1420
			});
			t.nextrun = level.time;
			t.parameters = arguments;
			t.events = [];
			t.entity = self;
			t.userkill = false;
			t.eval = function(code) { return Duktape.Thread.resume(t, code); }
			t.doFirstCall = true;
			self.threads.push(t)
			//level.currentthread = t;
			//Duktape.Thread.resume(t, arguments);
			
		}
	}
	

	this.runframe = function() {
		// iterate backwards over it, so we can delete finished threads instantly via ID
		for (var i=this.threads.length-1; i>=0; i--) {
			var thread = this.threads[i];

			var state = Duktape.info(thread);
			//printf("entid=% threadid=% state=%,%\n", this.id, i, state[0], state[1]);

			if (thread.nextrun > level.time) {
				//printf("entid=% threadid=%> No run yet: nextrun in %\n", this.id, i, thread.nextrun - level.time)
				continue;
			}

			// user called killthread()
			if (thread.userkill) {
				//printf("DELETE THREAD %d\n", this.id);
				this.threads.splice(i, 1); // delete thread from array
				continue;
			}		
			
			try {
				level.currentthread = thread;
				var arg = undefined;
				if (thread.doFirstCall) {
					arg = thread.parameters;
					thread.doFirstCall = false;
				}
				var whatnow = undefined;
				try {
					whatnow = Duktape.Thread.resume(thread, arg);
				} catch (e) {
					printf("error runframe: %\n", e);
					
				}

				var state = Duktape.info(thread);
				//printf("AFTER RESUME: entid=% threadid=% state=%,%\n", this.id, i, state[0], state[1]);

				switch (whatnow[0]) {
					case "wait": thread.nextrun += whatnow[1];	break;
				}
				//printf("whatnow: %\n", whatnow);
			} catch (e) {
				this.threads.splice(i, 1); // delete thread from array
				//printf("Entity::runframe> Finished Thread id=% entityid=% level.time=%\n", i, this.id, level.time);

			}
		}
	}

	
	this.disconnect = function() {
		printf("js> entityid=%d disconnected\n", this.id);

		for (var i=0; i<this.threads.length; i++) {
			var thread = this.threads[i];
			var callbacks = thread.events["disconnect"];
			
			// yea honestly, some threads do not define a disconnect event :^)
			if (typeof callbacks == "undefined") {
				continue;
			}
			
			// iterate over all callbacks and call them bounded to this entity
			for (var j=0; j<callbacks.length; j++)
			{
				callbacks[j].bind(this)();
			}
			
			// when we have called all callbacks, remove them
			thread.events["disconnect"] = [];
			
		}
	}
	
	this.useButtonPressed    = function()                      { return entity_usebuttonpressed   (this.id);                                           }
	this.sprintButtonPressed = function()                      { return entity_sprintbuttonpressed(this.id);                                           }
	this.attackButtonPressed = function()                      { return entity_attackbuttonpressed(this.id);                                           }
	this.getEye              = function()                      { return entity_get_eye            (this.id);                                           }
	this.getOrigin           = function()                      { return entity_get_origin         (this.id);                                           }
	this.setOrigin           = function(origin)                { return entity_set_origin         (this.id, origin[0], origin[1], origin[2]);          }
	this.addEvent            = function(normal)                { return entity_add_event          (this.id, normal[0], normal[1], normal[2]);          }
	this.getAngles           = function()                      { return entity_get_angles         (this.id);                                           }
	this.getWeapon           = function()                      { return entity_get_weapon         (this.id);                                           }
	this.getWeaponState      = function()                      { return entity_get_weaponstate    (this.id);                                           }
	this.getWeaponTime       = function()                      { return entity_get_weapontime     (this.id);                                           }
	this.setAngles           = function(angles)                { return entity_set_angles         (this.id, angles[0], angles[1], angles[2]);          }	
	this.getForward          = function()                      { return entity_get_forward        (this.id);                                           }
	this.getClassname        = function()                      { return entity_get_classname      (this.id);                                           }
	this.moveTo              = function(pos, durationSeconds)  { return entity_moveto             (this.id, pos[0], pos[1], pos[2], durationSeconds);  }
	this.solid               = function()                      { return entity_solid              (this.id);                                           }
	this.suicide             = function()                      { return entity_suicide            (this.id);                                           }
	this.die                 = function(self, inflictor, attacker, damage, mod)                   { return entity_die                (self.id, inflictor.id, attacker.id, damage, mod);                                           }
	this.free                = function()                      { return entity_delete             (this.id);                                           } // same, whatever u prefer
	this.unlink              = function()                      { return entity_delete             (this.id);                                           } // same, whatever u prefer
	this.notSolid            = function()                      { return entity_notsolid           (this.id);                                           }
	this.setModel            = function(modelname)             { return entity_set_model          (this.id, modelname);                                }
	
	this.printf = function() {
		var msg = sprintf.apply(undefined, arguments);
		sendservercommand(this.id, "print \"" + msg + "\"");
	}
}




Entity.prototype.traceEyeEntity = function(distance, ignoredEntity) {
	var eye = this.getEye();
	var forward = this.getForward();
	var endpos = vec3_new();
	vec3_a_is_b_plus_c_times_d(endpos, eye, forward, distance)
	//printf("endpos=%, eye=%, forward=%, distance=%\n", endpos, eye, forward, distance);
	var tmp = bullettrace(eye, endpos, "useless", ignoredEntity);
	return tmp;
}

Entity.prototype.setMoney = function(money) {
	this.money = money;	
	this.hudMoney.setText("Money: " + this.money);
}

function vec4(a,b,c,d) {
	var ret = new Float32Array(4);
	ret[0] = a;
	ret[1] = b;
	ret[2] = c;
	ret[3] = d;
	return ret;
}

function ClientHudElem() {this.id = 0; this.playerid=0; }

ClientHudElem.prototype.setText = function(text) { entity_hudelem_settext(this.playerid, this.id, text); }
ClientHudElem.prototype.show    = function(text) { entity_hudelem_show   (this.playerid, this.id      ); }
ClientHudElem.prototype.hide    = function(text) { entity_hudelem_hide   (this.playerid, this.id      ); }
ClientHudElem.prototype.free    = function(text) { entity_hudelem_free   (this.playerid, this.id      ); }

// entities[0].newClientHudElem(20, 30, 40, 50, vec4(1,0,0,0.5), "hurr durr")
Entity.prototype.newClientHudElem = function(left, top, width, height, color, text) {
	var clienthudelem = new ClientHudElem();
	clienthudelem.playerid = this.id;
	clienthudelem.id = entity_send_hud(this.id, left, top, width, height, color[0], color[1], color[2], color[3], text);
	return clienthudelem;
}


function JS_getPlayersInRange(position, maxDistance) {
	var ret = getPlayersInRange(position[0], position[1], position[2], maxDistance);
	ents = [];
	for (var i=0; i<ret.length; i++) {
		ents.push( entities[ret[i]] );
	}
	return ents;
}

Entity.prototype.getPlayersInRange = function(maxDistance) {
	return JS_getPlayersInRange(this.getOrigin(), maxDistance);	
}

on = function(event, callback) {
	var thread = level.currentthread;
	if (typeof thread.events[event] == "undefined") {
		thread.events[event] = [];
	}
	//printf("Add event % to thread id %\n", event, thread.entity.id);
	thread.events[event].push(callback);
}

function vec3(x,y,z) {
	var ret = new Float32Array(3);
	ret[0] = x;
	ret[1] = y;
	ret[2] = z;
	return ret;
}
function vec3_new() {
	return new Float32Array(3);
}

function vec3_scale(vec, scalar, output) {
	output[0] = vec[0] * scalar;
	output[1] = vec[1] * scalar;
	output[2] = vec[2] * scalar;
}
function vec3_copy(from_, to) {
	to[0] = from_[0];
	to[1] = from_[1];
	to[2] = from_[2];
}
function vec3_add(a, b, c) {
	c[0] = a[0] + b[0];
	c[1] = a[1] + b[1];
	c[2] = a[2] + b[2];
}
function vec3_sub(a, b, ret) {
  ret[0] = a[0] - b[0];
  ret[1] = a[1] - b[1];
  ret[2] = a[2] - b[2];
}




function vec3_a_is_b_times_c(output, vec, scalar) {
	output[0] = vec[0] * scalar;
	output[1] = vec[1] * scalar;
	output[2] = vec[2] * scalar;
}
function vec3_a_is_b(to, from_) {
	to[0] = from_[0];
	to[1] = from_[1];
	to[2] = from_[2];
}
function vec3_a_is_b_plus_c(a, b, c) {
	a[0] = b[0] + c[0];
	a[1] = b[1] + c[1];
	a[2] = b[2] + c[2];
}
function vec3_a_is_b_minus_c(a, b, c) {
  a[0] = b[0] - c[0];
  a[1] = b[1] - c[1];
  a[2] = b[2] - c[2];
}
function vec3_a_is_b_plus_c_times_d(ret, eye, forward, distance) {
	ret[0] = eye[0] + forward[0] * distance;
	ret[1] = eye[1] + forward[1] * distance;
	ret[2] = eye[2] + forward[2] * distance;
}




// vec3_sub always fucks my brain to remember the order, this functions makes it VERY clear
// it calculates the delta vector from "from_" to "to" into "ret"
// calculating the vector "a to b" is "b minus a"
function vec3_fromtoret(from_, to, ret) {
	ret[0] = to[0] - from_[0];
	ret[1] = to[1] - from_[1];
	ret[2] = to[2] - from_[2];
}
function vec3_length(a) {
  return Math.sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
}
function vec3_distance(a, b) {
  var delta = vec3_new();
  vec3_sub(b, a, delta);
  return vec3_length(delta);
}
function vec3_distancesquared(a, b) {
  var delta = vec3_new();
  vec3_sub(b, a, delta);
  return delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2];
}


function vec3_ret_a_plus_b(a, b) {
	var tmp = vec3_new();
	vec3_a_is_b_plus_c(tmp, a, b);
	return tmp;
}

function TraceResult() {
}
TraceResult.prototype.distance        = function() { return vec3_distance       (this.from, this.position); }
TraceResult.prototype.distanceSquared = function() { return vec3_distancesquared(this.from, this.position); }

function bullettrace(from_, to, hit_players, entity_to_ignore) {
	var ignoreEntity = 1023; // ENTITYNUM_NONE is MAX_GENTITIES-1
	if (entity_to_ignore != undefined)
		ignoreEntity = entity_to_ignore.id;
	var tmp = trace(from_[0], from_[1], from_[2], to[0], to[1], to[2], ignoreEntity);
	var tr = new TraceResult();
	tr.position = tmp.endpos;
	tr.normal = tmp.normal;
	tr.fraction = tmp.fraction;
	tr.entity = entities[tmp.entityNum];
	tr.from = from_;
	tr.to = to;
	return tr;
}

WP_TOOLGUN = 5




teddybearExplode = function() {
	try {
	while (1) {
		var players = this.getPlayersInRange(200);
		if (players.length) {
			//players.printf("in range\n");
			
			
			var jumpAir = vec3_ret_a_plus_b(this.getOrigin(), vec3(0,0,10));
			this.moveTo(jumpAir, 0.20) // todo: fix that every 2nd moveto doesnt work... debug moverState shit
			wait(0.20);
			printf("BOOM\n");
			this.free();
			//this.hide();
			killthread();
		}
		//printf("wait for player % \n", players.length);
		wait(0.05);
	}
	} catch (e) {
		printf("teddybearExplode exception=%\n", e);
		
	}
	
}

teddyShopBuy = function() {
	try {
		var hud = this.newClientHudElem(300, 300, 100, 30, vec4(1,1,1,1), "Buy Teddie 100$")
		
		while (1) {
			if (this.useButtonPressed()) {
				this.setMoney(this.money - 100);
				break;
			}
			wait(0.05);
		}
		hud.free();
		this.inShop = false;
	} catch (e) {
		printf("teddyShopBuy exception=%\n", e);
		
	}
}

teddybearShop = function() {
	try {
	while (1) {
		var players = this.getPlayersInRange(100);
		
		for (var i=0; i<players.length; i++) {
			var player = players[i];
			if (player.inShop)
				continue;
			player.inShop = true;
			player.thread(teddyShopBuy)();
			
		}
		
		//printf("wait for player % \n", players.length);
		wait(0.05);
	}
	} catch (e) {
		printf("teddybearExplode exception=%\n", e);
		
	}
	
}

toolgun = function() {
	on("disconnect", function() {
		killthread();
	});
	var player = this;
	
	var teddy = undefined;
	
	while (1) {
		if (this.getWeapon() == WP_TOOLGUN && this.attackButtonPressed()) {
			this.printf("FIRE\n")


			pos = player.traceEyeEntity(1024).position;
			
			//if (teddy == undefined)
			//{
				teddy = spawnentity();
				teddy.setOrigin(pos);
				//teddy.setModel("kungmodels/teddy.kung1");
				//teddy.setModel("kungmodels/mrfixit.kung1");
				teddy.setModel("kungmodels/axisopsys.kung1");
				printf("new heart id=%d\n", teddy.id);
				//teddy.thread(teddybearExplode)();
				teddy.thread(teddybearShop)();
			//} else {
			//	
			//	//teddy.setOrigin(pos);
			//	teddy.moveTo(pos, 1);
			//}
			
			//setTimeout(function() {
			//printf("before teddy..\n");
				
			
			//printf("after teddy..\n");
			//}, 1);
	
			
			
			// dont do anything till player released mouse
			while (this.attackButtonPressed())
				wait(0.05);
		}
		wait(0.05);
	}
}


doshit = function(a, b, c) {
	
	on("disconnect", function() {
		printf("entity id: % DISCONNECT 1 nade.id=%\n", this.id, nade.id);
		nade.unlink(); // clean up stuff this thread created
		killthread();
	});	
	
	blubb = 123;
	
    try {
		var nadeid = spawngrenade();
		var nade = new Entity(nadeid);
		nade.notSolid();
		nade.setModel("kungmodels/cor_bazooka_rocket.kung1");
		
		var brush_distance = 1024;
		var grenade_oldpos = vec3_new();
		var brush_oldpos = vec3_new();
		var brush_moving = undefined;
		
		// load/save is blocked when moving a brush
		this.movingBrush = false;
		while (1) {			
			if (this.attackButtonPressed()) {
				tr = this.traceEyeEntity(1024*10, nade)
				
				if (tr["fraction"] != 0) {
					//printf("classname: %\n", tr["entity"].getClassname());
					if (tr["entity"].getClassname() == "func_plat") {
						brush_distance = tr.distance();
						vec3_a_is_b(grenade_oldpos, tr["position"]);
						vec3_a_is_b(brush_oldpos, tr["entity"].getOrigin());
						brush_moving = tr["entity"];
						this.printf("Selected Brush ID: %\n", brush_moving.id);
						
						this.movingBrush = true;
						
						while (this.attackButtonPressed()) {
							var change = 15;
							if (brush_distance > 150) change = 20;
							if (brush_distance > 300) change = 40;
							if (brush_distance > 600) change = 60;
							if (this.sprintButtonPressed()) brush_distance += change;
							if (this.useButtonPressed()   ) brush_distance -= change;
							
							var pos = this.getEye();
							var forward = this.getForward();
							var newnadepos = vec3_new();
							
							vec3_copy(pos, newnadepos);
							vec3_scale(forward, brush_distance, forward);
							vec3_add(newnadepos, forward, newnadepos);
							
							//nade.setOrigin(newnadepos);
							nade.moveTo(newnadepos, 0.05); // move the nade faster than the brush, so nade is leading, very useful as direction indicator while prop surfing
							
							
							var delta_grenade = vec3_new();
							//vec3_fromtoret(grenade_oldpos, newnadepos, delta_grenade);
							vec3_a_is_b_minus_c(delta_grenade, newnadepos, grenade_oldpos);
							var finalpos = vec3_new();
							vec3_add(brush_oldpos, delta_grenade, finalpos);

							brush_moving.moveTo(finalpos, 0.15); // 0.15 feels smooth and "follows" the faster nade indicator
							
							wait(0.05);
						}
						this.movingBrush = false;
					}
				}
			}
			wait(0.05);
		}
	} catch (e) {
		printf("Error: % %\n", e, e.stack);
	}
}

callback_runframe = function(levelTime) {
	runframe();
	
}

function runframe() {
	for (var i=0; i</*entities.length*/1024; i++) {
		if (entities[i] != undefined)
			entities[i].runframe()
	}
	level.time += 50;
}

thread_loadpos = function() {
	var lastpress = 0;
	while (1) {
		while ( ! this.useButtonPressed()) { wait(0.05); }
		while (   this.useButtonPressed()) { wait(0.05); }
		
		if (this.movingBrush)
			continue;
		
		var time = level.time - lastpress;
		if (time <= 600) {
			if (typeof this.lastpos != "undefined") {
				this.setOrigin(this.lastpos);		
				this.setAngles(this.lastangles);
			} else {
				this.printf("^1You have no last position saved");
			}
			this.printf("^2Position loaded");
		}
		lastpress = level.time;
	}
}
thread_savepos = function() {
	var lastpress = 0;
	while (1) {
		while ( ! this.sprintButtonPressed()) { wait(0.05); }
		while (   this.sprintButtonPressed()) { wait(0.05); }
		
		if (this.movingBrush)
			continue;
		
		var time = level.time - lastpress;
		if (time <= 600) {	
			this.lastpos = this.getOrigin();
			this.lastangles = this.getAngles();
			this.printf("^1Position saved");
		}
		lastpress = level.time;
	}
}


if (typeof entities == "undefined") {
	entities = Array(1024);
	for (var i=0; i<1024; i++)
		entities[i] = new Entity(i);
}

startThreads = function(player) {
	// fuckin shit, the duktape threads are such cpu eaters.... need to dig julia for this later
	//player.thread(doshit)(1,22,333);
	//player.thread(thread_loadpos)();
	//player.thread(thread_savepos)();
	//player.thread(toolgun)();
}

function callback_clientconnected(clientNum) {
	entities[clientNum] = new Entity(clientNum);
	var player = entities[clientNum];
	startThreads(player);
	
	player.money = 300;
	player.teddies = 0;
	player.mortar = 1;
	
	player.inShop = false;
	player.hudMoney  = player.newClientHudElem(480, 360 + 0 * 30, 100, 30, vec4(1,0,0,0.5), "Money: " + player.money);
	player.hudTeddy = player.newClientHudElem(480, 360 + 1 * 30, 100, 30, vec4(1,0,0,0.5), "Teddies: " + player.teddies);
	player.hudMortar = player.newClientHudElem(480, 360 + 2 * 30, 100, 30, vec4(1,0,0,0.5), "Mortars: " + player.mortar);
	
	player.setMoney(300);
	
	printf("js> client connected: %\n", clientNum);
}

function callback_clientdisconnected(clientNum) {
	try {
		entities[clientNum].disconnect();
		// delete all data saved in the Entity object, so new players won't have the load/save positions etc.
		entities[clientNum] = undefined;
		printf("js> Client disconnected: %\n", clientNum);
	} catch (e) {
		printf("callback_clientdisconnected> error: %\n", e.stack);
	}
}

print("lib.js loaded\n");