OpAdd = NewOp("OpAdd");

OpAdd.prototype.Init = function() {
	this.SetNumberOfInputs(2);
	this.in_a = this.GetInput(0).vec1;
	this.in_b = this.GetInput(1).vec1;
	this.SetNumberOfOutputs(1);
	this.out = this.GetOutput(0).vec1;
	this.GetInput(0).SetName("a");
	this.GetInput(1).SetName("b");
	this.GetOutput(0).SetName("c");
	this.size.set([200,45])
	printf("OpAdd.Init()\n");
}

OpAdd.prototype.Update = function() {
	var a = this.in_a[0];
	var b = this.in_b[0];
	this.out[0] = a + b;
}

OpAdd.prototype.Render = function() {
	var x = GUI.GetCursorPosX();
	GUI.Text("OpAdd");
	GUI.SetCursorPosX(x);
	GUI.DragFloat("hurr", this.GetOutput(0).vec1);
}