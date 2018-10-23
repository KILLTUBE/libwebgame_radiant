function contextmenu() {
	for(var i in ops) {
		var op = ops[i];
		GUI.PushID(i);
		if (GUI.Button("Add " + op.name + "\n")) {
			printf("add %\n", op.name);
		}
		GUI.PopID();
	}	
}