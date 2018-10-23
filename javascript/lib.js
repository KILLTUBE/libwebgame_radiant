shittyconsole = function(code, sel_from, sel_to) {
	if (sel_to < sel_from) {
		tmp = sel_to;
		sel_to = sel_from;
		sel_from = tmp;
	}
	if (sel_from != sel_to)
		code = code.substr(sel_from, sel_to-sel_from);
	//printf("code: % % %", code, sel_from, sel_to);
	handle_input(code, get_global());
}

print = function() {
	for (var i=0; i<arguments.length; i++) {
		imgui_log(arguments[i]);
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
		
		imgui_log("> ");
		var_dump(ret);
		imgui_log("\n");
		
	} catch (e) {
		print("handle_input> error: ", e.stack, "\n");
	}
	
	//repl_settext("asdddd");
}

console_execute = function(global, widget_pointer) {
	try {
		var code = widget_get_selected_text(widget_pointer);
		// if there is no selected text, execute everything
		if (code == "")
			code = widget_get_text(widget_pointer);
		handle_input(code, global);
	} catch (e) {
		print("console_execute() failed on JS level: ", e.stack, "\n");
	}
}

/*
Lets say we have this code:			
	reload()
	function foo() { aaa = []; aaa["aaa"] = aaa; return aaa; }
	foo().aaa.aaa.aa
We wan't it to auto complete to "foo().aaa.aaa.aaa";
At first we need to iterate backwards over the chars, to understand where the statement begins (before foo())
*/
//js_call("get_auto_completion", "siiii", replbuffer, data->CursorPos, data->SelectionStart, data->SelectionEnd, returnbuffer);
get_auto_completion = function(text, cursor_pos, selection_start, selection_end, returnbuffer) {
	//printf("text %, cursor_pos %, selection_start %, selection_end %, returnbuffer %\n", text, cursor_pos, selection_start, selection_end, returnbuffer);
	
	
	//print("Cursor pos: ", cursor_pos, "\n");
	//print("Text: ", text, "\n");
	var i;
	for (i=cursor_pos - 1; i >= 0; i--)
	{
		var cc = text[i];
		//print(i, " is ", cc, "\n");
		if (cc >= "a" && cc <= "z")
			continue;
		if (cc >= "A" && cc <= "Z")
			continue;
		if (cc >= "0" && cc <= "9")
			continue;
		if (cc == ".")
			continue;
		if (cc == "[")
			continue;
		if (cc == "]")
			continue;
		if (cc == "(")
			continue;
		if (cc == ")")
			continue;
		if (cc == "_")
			continue;
		break;
	}
	
	var tmp = text.substring(i+1, cursor_pos);
	var lastDot = tmp.lastIndexOf(".");
	var evalstr = tmp.substring(0, lastDot);
	var lastpart = tmp.substring(lastDot + 1);

	if (lastDot == -1) {
		evalstr = "this";
		lastpart = tmp;
	}

	// .par[TAB]
	if (lastDot == 0)
		return
	
	lastPartFrom = i + 1;
	lastPartTo = cursor_pos;
	
	//print("last part goes from ", lastPartFrom, " to ", lastPartTo, "\n");
	//print("tmp: ", tmp, "\n");
	//print("evalstr: ", evalstr, "\n");
	//print("lastpart: ", lastpart, "\n");
	//printf("i=% lastDot=%\n", i, lastDot);
	//return;
	
	candidates = [];
	try {
		var ret = eval(evalstr);
		switch (typeof ret) {
			case "object": 
			
				//if (ret.constructor.name == "Array") {
				//	candidates.push("slice");
				//	candidates.push("push");
				//	candidates.push("pop");
				//	candidates.push("sort");
				//	candidates.push("shift");
				//	
				//}
			
				for (var i in ret)
					if (i.indexOf(lastpart) != -1)
						candidates.push(i);
				
				// dont show a single candidate, just autocomplete silently
				if (candidates.length > 1)
					print("Auto complete candidates: ", candidates.join(", "), "\n")
				break;
			case "undefined":
				printf("% is undefined\n", evalstr);
				break;
			default:
				printf("no clue how to auto complete a % m8\n", typeof ret);
			
		}
		
		//var_dump(ret);
	} catch (e) {
		print("Autocomplete error: ", e.stack, "\n");
	}
	
	if (candidates.length == 1) {
		var candidate = candidates[0];
		//print("ONLY ONE!");
		if (lastDot == -1) {
			// if there was no dot, we can replace the whole thing, it searched on global namespace
			//widget_replace_text_between(widget_pointer, lastPartFrom, lastPartTo, candidates[0]);
			//printf("case 1: lastPartFrom = %, lastPartTo = %, candidates[0] = %\n", lastPartFrom, lastPartTo, candidate);
			
			var needed = candidate.substr(lastPartTo - lastPartFrom);
			// example: "test memo" lastPartFrom=5 lastPartTo = 9, "memory_write_string".substr(9-5) is "ry_write_string", which will be added then
			memory_write_string(returnbuffer, needed);
		} else {
			// example a.par[TAB]
			// here we replace the "par"
			//widget_replace_text_between(widget_pointer, lastPartFrom + lastDot + 1, cursor_pos, candidates[0]);
			
			// example: "foo().aa" from=6 to=8 candidate=aaa
			var needed = candidate.substr(cursor_pos - (lastPartFrom + lastDot + 1));
			//printf("needed=%\n", needed);
			memory_write_string(returnbuffer, needed);
			//printf("case 2: lastPartFrom + lastDot + 1 = %, cursor_pos = %, candidates[0] = %\n", lastPartFrom + lastDot + 1, cursor_pos, candidate);
		}
	}
	
	
	// the returnbuffer will be added to the cursor_pos
	//memory_write_string(returnbuffer, "full of shit");
}



keys_pressed = {};
function isKeyPressed(key) {
	// wasn't even pressed, so it's not in keys_pressed object
	if (key in keys_pressed == false)
		return false;
	// otherwise return the true/false value
	//print("Was ", key, " pressed? ", keys_pressed[key], "\n");
	return keys_pressed[key];
}

// GTK magic, maybe replace with event.preventDefault()
var preventDefault = 1;
var runDefault = 0;
function console_key_press(widget_pointer, key, global) {
	keys_pressed[key] = true;
	
	if (isKeyPressed("Control Left") && isKeyPressed("r")) {
		reload();
		return preventDefault;
	}
	//if (isKeyPressed("Control Left") && isKeyPressed("Space")) {
	//	console_auto_complete(widget_pointer);
	//	return preventDefault;
	//}
	if (isKeyPressed("Control Left") && isKeyPressed("Enter")) {
		console_execute(global, widget_pointer);
		return preventDefault;
	}
	return runDefault;
}
function console_key_release(widget_pointer, key, global) {
	keys_pressed[key] = false;
	//print("Released key: ", key, "\n");
	return runDefault;
}


function delete_widget_under_mouse() {
	w = get_widget_under_mouse();
	widget_destroy(w)
}

function get_widget_hierarchy(w) {
	var parents = [];
	while (w) {
		parents.push(w);
		w = widget_get_parent(w);
	}
	parents.reverse()
	return parents;
}

function get_widget_under_mouse() {
	var m = get_mouse_position()
	return widget_at_mouse_position(m[0], m[1])
}


function widget_dump_hierarchy(hierarchy) {
	
	if (typeof hierarchy == "number")
		hierarchy = get_widget_hierarchy(hierarchy);
	
	for (var i=0; i<hierarchy.length; i++) {
		w = hierarchy[i];
		print(i, " = ", widget_get_type(w), " ", w, "\n")
	}
}

// check if its defined, otherwise ctrl+r will fuck up the callbacks
if (typeof callbacks == "undefined")
	callbacks = [];
function callback_to_id(callback) {
	callbacks.push(callback);
	return callbacks.length - 1;
}
function callback_call(id) {
	// I would like to call arguments.slice(1), to remove the callback_id, but the "arguments" is not a real array. At least in FireBug, it only has .length and .callee
	// So I just call 
	callbacks[id].apply(undefined,arguments);
}



function tabbar_of_widget(w) {
	while (w) {
		if ( widget_get_data(w, "isTabBar"))
			return w;

		w = widget_get_parent(w);
	}
	return 0;
}

function is_between(min, val, max) {
	return val >= min && val <= max;
}
function Button(id, left, top, right, bottom) {
	this.id = id;
	this.left = left;
	this.top = top;
	this.right = right;
	this.bottom = bottom;
	this.hittest = function(mouse_left, mouse_top) {
		if ( ! is_between(this.left, mouse_left, this.right))
			return false;
		if ( ! is_between(this.top, mouse_top, this.bottom))
			return false;
		return true;
	}
}
function IMHits() {
	this.buttons = [];
	this.button = function(id, left, top, right, bottom) {
		this.buttons.push( new Button(id, left, top, right, bottom) ); // apply with arguments? maybe faster
	}
	this.hittest = function(mouse_left, mouse_top) {
		for (var i=0; i<this.buttons.length; i++) {
			button = this.buttons[i];
			if (button.hittest(mouse_left, mouse_top)) {
				//console.log("hit", button.id)
				return button;
			}
		}
		return undefined;
	}
}
function generate_buttons_for_window(width, height) {
	var im = new IMHits();
	half_w = width / 2;
	half_h = height / 2;
	im.button("center", half_w - 10, half_h-10, half_w + 10, half_h + 10);
	var distance = 30;
	offset_x = -distance;
	offset_y = 0;
	im.button("left button", offset_x + (half_w - 10), offset_y + (half_h - 10), offset_x + (half_w + 10), offset_y + (half_h + 10));
	offset_x = -distance * 2;
	offset_y = 0;
	im.button("very left button", offset_x + (half_w - 10), offset_y + (half_h - 10), offset_x + (half_w + 10), offset_y + (half_h + 10));
	offset_x = distance;
	offset_y = 0;
	im.button("right button", offset_x + (half_w - 10), offset_y + (half_h - 10), offset_x + (half_w + 10), offset_y + (half_h + 10));
	offset_x = distance * 2;
	offset_y = 0;
	im.button("very right button", offset_x + (half_w - 10), offset_y + (half_h - 10), offset_x + (half_w + 10), offset_y + (half_h + 10));
	offset_x = 0;
	offset_y = -distance;
	im.button("top button", offset_x + (half_w - 10), offset_y + (half_h - 10), offset_x + (half_w + 10), offset_y + (half_h + 10));
	offset_x = 0;
	offset_y = -distance * 2;
	im.button("very top button", offset_x + (half_w - 10), offset_y + (half_h - 10), offset_x + (half_w + 10), offset_y + (half_h + 10));
	offset_x = 0;
	offset_y = distance;
	im.button("bottom button", offset_x + (half_w - 10), offset_y + (half_h - 10), offset_x + (half_w + 10), offset_y + (half_h + 10));
	offset_x = 0;
	offset_y = distance * 2;
	im.button("very bottom button", offset_x + (half_w - 10), offset_y + (half_h - 10), offset_x + (half_w + 10), offset_y + (half_h + 10));
	//im.button("very left button", 10, half_h - 10, 30, half_h + 10);
	//im.button("very top button", half_w - 10, 10, half_w + 10, 30);
	//im.button("very right button", width - 30, half_h - 10, width - 10, half_h + 10);
	//im.button("very bottom button", half_w - 10, height - 30, half_w + 10, height - 10);	
	return im;
}
//im = generate_buttons_for_window(800, 600);
//im.hittest(400, 300)

		
function get_tabsystem_under_mouse() {
	var undermouse = get_widget_under_mouse();
	if ( ! undermouse)
		return 0;
	undermouse = get_parent_pane_as_child(undermouse);
	return undermouse; // always check for 0
}

function handle_buttons_on_tabbars() {
	// callbacks[cb_press] =
	function callback_button_press(id, widget, x, y) {
		//print("callback_button_press: widget=", widget, " x=", x, " y=", y, "\n");
		widget_set_data(widget, "pressed", 1);
		
		// 
		//signal_id_onmousemove = widget_onmousemove(widget, cb_onmousemove)
		//printf("signal_id_onmousemove = %\n", signal_id_onmousemove);
	}
	// callbacks[cb_release] =
	function callback_button_release(id, widget, mouse_x, mouse_y) {
		widget_set_data(widget, "pressed", 0);
		// this doesn't work, maybe its not possible to connect a signal while being in a signal? :S
		//widget_signal_disconnect(widget, signal_id_onmousemove);
		
		// hide the tab dock overlay
		//win32_window_move(hwnd, 0, 0, 0, 0);
		//win32_window_move(hwnd2, 0, 0, 0, 0);
		// for some reason the window dies or something, then its just not visible anymore... hence delete it every time and recreate it
		win32_window_destroy(hwnd);
		win32_window_destroy(hwnd2);
		hwnd = undefined;
		hwnd2 = undefined;
		
		thisTabbar = tabbar_of_widget(widget);
		//printf("tabbar of widget=%\n", thisTabbar);
		//print("callback_button_release lol: widget=", widget, " x=", mouse_x, " y=", mouse_y, "\n");
		var undermouse = get_widget_under_mouse(mouse_x, mouse_y)
		var target = tabbar_of_widget(undermouse);
		//printf("tabbar of target widget=%\n", target);

		// user dropped mouse on a tabbar
		if (target && target != thisTabbar) {
			//printf("Link from % to % asd !\n", thisTabbar, target);			
			move_button_to_other_tabbar(widget, target);
			return;
		}

		//var mode = widget_pane_determine_split_mode_under_mouse();

		var tabsystem = get_tabsystem_under_mouse();
		if ( ! tabsystem)
			return;
		var rect = widget_get_rect(tabsystem);
		var x = rect["x"], y = rect["y"], w = rect["width"], h = rect["height"];
		im = generate_buttons_for_window(w, h);
		winpos = window_get_position();
		mouse_2_win_x = winpos.x + (mouse_x - x);
		mouse_2_win_y = winpos.y + (mouse_y - y);
		hit = im.hittest(mouse_2_win_x, mouse_2_win_y)
		printf("relative mouse: left=% top=%\n", mouse_2_win_x, mouse_2_win_y);
		var r = widget_get_rect( get_root_pane() ); // "r"ect
		if (hit) {
			printf("MOUSE RELEASE> Got hit: %\n", hit.id);
			
			var mode = "";
			var very = false;
			switch (hit.id) {
				case "top button":    mode = "va"; break;
				case "bottom button": mode = "vb"; break;
				case "left button":   mode = "ha"; break;
				case "right button":  mode = "hb"; break;
				case "very top button":    mode = "va"; very = true; break;
				case "very bottom button": mode = "vb"; very = true; break;
				case "very left button":   mode = "ha"; very = true; break;
				case "very right button":  mode = "hb"; very = true; break;
				case "center":
					printf("add widget to center\n");
					return;
			}
			printf("MOUSE RELEASE> very=% mode=%\n", very, mode);
			// save this, before "widget" is deleted, used at end
			var tabsystem_from = get_parent_pane_as_child( widget );
			
			var content = widget_get_data(widget, "tab_widget");
			if ( ! content) {
				printf("button has no tab_widget data set");
				return;
			}
			widget_ref( content );
			widget_remove_from_container( content );
			printf("split...\n");
			if (very)
				split_rootpane(mode, content);
			else
				widget_split_pane(tabsystem, mode, content);
			widget_unref( content );
			printf("widget_tabify...\n");
			widget_tabify( content );
			var tabbar = tabbar_of_widget(widget);
			widget_destroy(widget);
			printf("tabsystem_restore_last_tab...\n");
			
			printf("tabsystem_from=% type=%", tabsystem_from, widget_get_type( tabsystem_from ));
			tabsystem_restore_last_tab( tabsystem_from );
			
			printf("delete_tabsystem_if_empty...\n");
			delete_tabsystem_if_empty(tabbar);
			
			printf("handle_buttons_on_tabbars...\n");
			handle_buttons_on_tabbars(); // since widget_tabify created a new button (coz we just destroyed the old one) we need to re-init the onmousemove and click/release/press events
			//printf("do mode % tosplit % content %\n", mode, tosplit, content);
			return;
		}
	}
	// callbacks[cb_click] =
	function callback_button_click(id, button, x, y) {
		//print("callback_button_click: widget=", widget, " x=", x, " y=", y, "\n");
		restore_saved_widget_of_button(button);
	}
	// callbacks[cb_onmousemove] = 
	function callback_button_onmousemove(id, widget, mouse_x, mouse_y) {
		if ( widget_get_data(widget, "pressed") == 0 )
			return;
			
			
		if (typeof hwnd == "undefined")
			hwnd = win32_popup(0, 0, 0, 0)
		if (typeof hwnd2 == "undefined")
			hwnd2 = win32_popup2(0, 0, 0, 0)
			

		//printf("onmousemove id=% x=% y=%\n", id, mouse_x, mouse_y);
		var tabsystem = get_tabsystem_under_mouse();
		if ( ! tabsystem)
			return;
		
		var rect = widget_get_rect(tabsystem);
		var x = rect["x"], y = rect["y"], w = rect["width"], h = rect["height"];
		//var mouse_diff_y = mouse_y - (y + h / 2); // delta = to - from
		//var mouse_diff_x = mouse_x - (x + w / 2); // delta = to - from
		//if (Math.abs(mouse_diff_y) > Math.abs(mouse_diff_x)) {
		//	if (mouse_diff_y > 0) { // draw lower part
		//		win32_window_move(hwnd,  x, y + h / 2, x + w, y + h );
		//		
		//	} else { // draw upper part
		//		win32_window_move(hwnd,  x, y , x + w, y + h / 2 );
		//	}
		//} else {
		//	if (mouse_diff_x > 0) { // draw right part
		//		win32_window_move(hwnd,  x + w / 2, y , x + w, y + h );
		//	} else { // draw left part
		//		win32_window_move(hwnd, x, y , x + w / 2, y + h );
		//	}
		//}
		im = generate_buttons_for_window(w, h);
		winpos = window_get_position();
		mouse_2_win_x = winpos.x + (mouse_x - x);
		mouse_2_win_y = winpos.y + (mouse_y - y);
		hit = im.hittest(mouse_2_win_x, mouse_2_win_y)
		printf("relative mouse: left=% top=%\n", mouse_2_win_x, mouse_2_win_y);
		var r = widget_get_rect( get_root_pane() ); // "r"ect
		
		if (hit) {
			printf("Got hit: %\n", hit.id);
			switch (hit.id) {
				
				case "top button":    win32_window_move(hwnd2, x, y , x + w, y + h / 2 );      break;
				case "bottom button": win32_window_move(hwnd2, x, y + h / 2, x + w, y + h );   break;
				case "left button":   win32_window_move(hwnd2, x, y , x + w / 2, y + h );      break;
				case "right button":  win32_window_move(hwnd2, x + w / 2, y , x + w, y + h );  break;
				case "very top button":    win32_window_move(hwnd2, r.left, r.top, r.right, r.top + r.height * 1/4);    break;
				case "very bottom button": win32_window_move(hwnd2, r.left, r.top + r.height * 3/4, r.right, r.bottom); break;
				case "very left button":   win32_window_move(hwnd2, r.left, r.top, r.width * 1/4, r.bottom);            break;
				case "very right button":  win32_window_move(hwnd2, r.left + r.width * 3/4, r.top, r.right, r.bottom);  break;
				case "center":  win32_window_move(hwnd2, x, y , x + w, y + h);  break;
			}
		} else {
			win32_window_move(hwnd2, 0, 0, 0, 0);
		}
		win32_window_move(hwnd, x, y , x + w, y + h );
	}	
	
	cb_press = callback_to_id(callback_button_press);
	cb_release = callback_to_id(callback_button_release);
	cb_click = callback_to_id(callback_button_click);
	cb_onmousemove = callback_to_id(callback_button_onmousemove);
	tabbars = widget_get_widget_by_name("tabbar")
	for (var j in tabbars) {
		var tabbar = tabbars[j];
		var childs = widget_get_childs( tabbar );
		for (var i=0; i<childs.length; i++) {
			var button = childs[i];
			widget_onmouseclick(button, cb_press, cb_release, cb_click)
			widget_onmousemove(button, cb_onmousemove)
			//printf("% ", widget_get_type( button ))
		}	
	}
}

function tabify_stuff() {
	z = widget_get_widget_by_name("ZWindow")[0]
	widget_tabify(z)
	xy = widget_get_widget_by_name("XYWindow")[0]
	widget_tabify(xy)
	js = widget_get_widget_by_name("jsconsole")[0]
	widget_tabify(js)
		
	texwnd = widget_get_widget_by_name("texwindow")[0]
	/*
	0 = GtkWindow 83288064
	1 = GtkVBox 83284160
	2 = GtkVPaned 137764472
	3 = GtkHPaned 137764632
	4 = GtkHPaned 138297376
	5 = GtkVPaned 138297536
	6 = GtkFrame 138261008
	7 = GtkHBox 101907472
	8 = GtkVBox 101908088
	9 = GtkDrawingArea 138265416
	*/
	h = get_widget_hierarchy(texwnd)
	//widget_dump_hierarchy(h)
	widget_tabify(h[h.length - 4])
	
	camwindow = widget_get_widget_by_name("camwindow")[0]
	widget_tabify(camwindow)
	
	
	handle_buttons_on_tabbars();
}


function get_parent_pane(w) {
	while (w) {
		if ( widget_get_type(w) == "GtkHPaned")
			return w;
		if ( widget_get_type(w) == "GtkVPaned")
			return w;
		w = widget_get_parent( w );
	}
	return 0;
}

function get_parent_pane_as_child(w) {
	var last_w = 0;
	while (w) {
		if ( widget_get_type(w) == "GtkHPaned")
			return last_w;
		if ( widget_get_type(w) == "GtkVPaned")
			return last_w;
		last_w = w;
		w = widget_get_parent( w );
	}
	return 0;
}

function get_other_widget_of_pane(w) {
	var pane = widget_get_parent(w);
	var childs = widget_get_childs( pane );
	var a = childs[0];
	var b = childs[1];
	if (a == w)
		return b;
	return a;
}

function move_button_to_other_tabbar(from_button, to) {
	// todo: assert stuff
	// that from_button is actually a button and not a label e.g.
	// and that both actually are tabbars
	
	// init from stuff
	var from_tabbar = tabbar_of_widget(from_button)
	var from_tab = widget_get_parent( from_tabbar );
	var from_childs = widget_get_childs( from_tab )
	var from_content = from_childs[1]

	// init to stuff
	var to_tabbar = tabbar_of_widget(to)
	var to_tab = widget_get_parent(to_tabbar)
	var to_childs = widget_get_childs(to_tab)
	var to_content = to_childs[1]
	
	// remove from_content. but keep reference (unref at end)
	widget_ref(from_content)
	widget_remove_from_container(from_content)

	// replace to_content with from_content
	widget_ref( to_content )
	widget_remove_from_container( to_content )
	widget_add_child( to_tab, from_content )
	widget_unref(from_content)
	widget_expandfill( from_content )

	// remove the button from from_tabbar and add it to to_tabbar
	widget_ref( from_button );
	widget_remove_from_container( from_button )
	widget_add_child(to_tabbar, from_button)
	widget_unref(from_button)
	
	delete_tabsystem_if_empty(from_tabbar);
}

function delete_tabsystem_if_empty(tabbar) {
	if (widget_get_childs( tabbar ).length == 0) {
		var pane = get_parent_pane( tabbar )
		var tabsystem = get_parent_pane_as_child( tabbar )
		var otherTabsystem = get_other_widget_of_pane( tabsystem )
		widget_ref( otherTabsystem )
		widget_remove_from_container( otherTabsystem )
		widget_replace(pane, otherTabsystem)
		widget_unref( otherTabsystem )
		
		widget_expandfill( otherTabsystem )
	}	
}

function restore_saved_widget_of_button(button) {
	widget_for_button = widget_get_data(button, "tab_widget")
	tabbar = tabbar_of_widget(button)
	tab = widget_get_parent( tabbar )
	//widget_dump_hierarchy(tab)
	childs = widget_get_childs(tab)

	// Our Hbox which represents a "tabsystem" can handle multiple widget, but "normally" we should just have 1 or 2
	// but if we have more, we gotta delete all first (not the first one tho, thats our tabbar vbox)
	if (childs.length >= 2) {
		for (var i=1; i<childs.length; i++) {
			widget_ref( childs[i] )
			widget_remove_from_container( childs[i] )
		}
	}

	//widget_remove_from_container(childs[0])
	if (widget_for_button) {
		widget_add_child(tab, widget_for_button)
		//widget_unref(widget_for_button)
		widget_expandfill(widget_for_button)
	}
	//widget_dump_hierarchy(button)
	//widget_get_type(widget_for_button)
}

function widget_pane_id(pane, child) {
	var a = widget_get_child_a(pane);
	var b = widget_get_child_b(pane);
	if (a == child)
		return "a";
	if (b == child)
		return "b";
	return "bastard";
}

function widget_add_child_id(pane, child, id) {
	if (id == "a")
		return widget_add_child_a(pane, child);
	if (id == "b")
		return widget_add_child_b(pane, child);
	printf("Can't add a bastard child % to pane %\n", child, pane);
}

// 4 modes of inserting: 
// "ha" -> make hpane -> new_one at: left (child_a)
// "hb" -> make hpane -> new_one at: right (child_b)
// "va" -> make vpane -> new_one at: top (child_a)
// "vb" -> make vpane -> new_one at bottom (child_b)
function widget_split_pane(tosplit, mode, new_one) {
	var pane = get_parent_pane(tosplit);
	var pane_id = widget_pane_id(pane, tosplit)
	if (pane_id == "bastard")
		return;
	
	widget_ref(tosplit)
	widget_remove_from_container(tosplit)

	newpane = mode[0] == "v" ? widget_make_vpane() : widget_make_hpane()
	//widget_expandfill(newpane);

	widget_add_child_id(pane, newpane, pane_id)

	if (mode[1] == "a") {
		widget_add_child_a(newpane, new_one)
		widget_add_child_b(newpane, tosplit)
	} else {
		widget_add_child_a(newpane, tosplit)
		widget_add_child_b(newpane, new_one)
	}
	widget_unref(tosplit)

}

/*
	Examples:
		int2str( str2int("test") ) == "test" // true
		int2str( str2int("t€st") ) // "t¬st", because "€".charCodeAt(0) is 8364, will be AND'ed with 0xff
	Limitations:
		max 4 chars, so it fits into an integer
*/
function str2int(the_str) {
	var ret = 0;
	var len = the_str.length;
	if (len >= 1) ret += (the_str.charCodeAt(0) & 0xff) <<  0;
	if (len >= 2) ret += (the_str.charCodeAt(1) & 0xff) <<  8;
	if (len >= 3) ret += (the_str.charCodeAt(2) & 0xff) << 16;
	if (len >= 4) ret += (the_str.charCodeAt(3) & 0xff) << 24;
	return ret;
}
function int2str(the_int) {
	var tmp = [
		(the_int & 0x000000ff) >>  0,
		(the_int & 0x0000ff00) >>  8,
		(the_int & 0x00ff0000) >> 16,
		(the_int & 0xff000000) >> 24
	];
	var ret = "";
	for (var i=0; i<4; i++) {
		if (tmp[i] == 0)
			break;
		ret += String.fromCharCode(tmp[i]);
	}
	return ret;
}

function widget_pane_determine_split_mode_under_mouse() {
	var undermouse = get_widget_under_mouse();
	if (undermouse == 0)
		return;
	undermouse = get_parent_pane_as_child(undermouse);
	if (undermouse == 0)
		return;
	var mouse_x = get_mouse_position()[0];
	var mouse_y = get_mouse_position()[1];

	var rect = widget_get_rect(undermouse);
	var x = rect["x"], y = rect["y"], w = rect["width"], h = rect["height"];
	var mouse_diff_y = mouse_y - (y + h / 2); // delta = to - from
	var mouse_diff_x = mouse_x - (x + w / 2); // delta = to - from
	var mode = "";
	if (Math.abs(mouse_diff_y) > Math.abs(mouse_diff_x)) {
		if (mouse_diff_y > 0) { // draw lower part
			mode = "vb";
		} else { // draw upper part
			mode = "va";
		}
	} else {
		if (mouse_diff_x > 0) { // draw right part
			mode = "hb";
		} else { // draw left part
			mode = "ha";
		}
	}
	return mode;
}

function get_root_pane() {
	var c = widget_get_childs( get_window() )
	childs = widget_get_childs( c[0] )
	var child = undefined;
	for (var i=0; i<childs.length; i++) {
		child = childs[i];
		//printf("childs type %\n", widget_get_type(child));
		if (widget_get_type(child) == "GtkVPaned")
			return child;
		if (widget_get_type(child) == "GtkHPaned")
			return child;
	}
	return undefined;
}

// split_rootpane("hb", widget_make_button("muh"))
function split_rootpane(mode, new_widget) {
	var rootpane = get_root_pane()
	var root = widget_get_parent( rootpane );
	widget_ref(rootpane)
	widget_remove_from_container(rootpane)
	var newpane = mode[0] == "v" ? widget_make_vpane() : widget_make_hpane()
	if (mode[1] == "a") {
		widget_add_child_a(newpane, new_widget);
		widget_add_child_b(newpane, rootpane);
	} else {
		widget_add_child_a(newpane, rootpane);
		widget_add_child_b(newpane, new_widget);

	}
	widget_add_child(root, newpane)
	widget_unref( rootpane );
	widget_expandfill(newpane);
	keys_pressed = {}; // GTK won't get the release event so the keys will still be "pressed"
}
// widget can be any child of the tabsystem
// removes the whole pane if tabbar will be empty
// example:
// widget = get_widget_under_mouse()
// delete_button_from_tabbar_via_content( widget );
function delete_button_from_tabbar_via_content( widget ) {
	// when used via get_widget_under_mouse(), the widget is probably in a frame or scrolled pane etc.
	// so we go up the hierarchy and just pick childs[1], which is the actual content
	var tabsystem = get_parent_pane_as_child( widget );
	if (widget_get_type(tabsystem) == "GtkFrame")
		tabsystem = widget_get_childs( tabsystem)[0];
	var childs = widget_get_childs( tabsystem );
	widget_get_type (tabsystem)
	var tabbar = childs[0];
	var tabcontent = childs[1];
	buttons = widget_get_childs( tabbar )
	for (var i=0; i<buttons.length; i++) {
		var b = buttons[i];
		var button_content = widget_get_data(b, "tab_widget");
		if (tabcontent == button_content) {
			widget_destroy( b );
			delete_tabsystem_if_empty( tabbar )
			return;
		}
	}
}

function tabsystem_get_tabs(tabsystem) {
	var childs = widget_get_childs( tabsystem )
	var tabbar = childs[0];
	var tabbar_buttons = widget_get_childs(tabbar);
	var tabs = [];
	//printf("tabbar_buttons.length = %\n", tabbar_buttons.length);
	for (var i=0; i<tabbar_buttons.length; i++) {
		var button = tabbar_buttons[i];
		var content = widget_get_data( button, "tab_widget" );
		tabs.push( content );
	}
	return tabs;
}
function tabsystem_set_content(tabsystem, content) {	
	if (widget_get_parent(content)) {
		printf("Cannot add content=%, it still has a parent");
		return;
	}
	var childs = widget_get_childs( tabsystem );
	if (childs.length == 1) {
		widget_add_child( tabsystem, content );
		widget_expandfill( content );
	} else {

		if (childs[1] == content) {
			//printf("already set\n");
			widget_expandfill( content );
			return;
		}

		printf("tabsystem_set_content(%, %)> childs.length !=1, please implement this case", tabsystem, content);
	}
}
function tabsystem_get_content(tabsystem) {
	var childs = widget_get_childs( tabsystem );
	if (childs.length < 2)
		return 0;
	return childs[1];
}
function tabsystem_restore_last_tab(tabsystem) {
	var tabs = tabsystem_get_tabs(tabsystem);
	var new_tab = tabs[tabs.length - 1];
	if ( ! new_tab) {
		printf("a new_tab doesnt exist");
		var_dump(tabs);
		return;
	}
	tabsystem_set_content(tabsystem, new_tab);
}

// zwindow = widget_make_zwindow(); widget_add(zwindow)
function widget_add(widget) {
	split_rootpane("hb", zwindow)
	widget_tabify(widget)
	handle_buttons_on_tabbars()
}


print("argh lib.js loaded ¯\\_(ツ)_/¯\n");