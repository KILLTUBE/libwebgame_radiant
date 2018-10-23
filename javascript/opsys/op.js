Op = MakeClass("Op");

Op.prototype.GetInput = function(id) {
	var input = op_get_input(this.address, id);
	if (input == undefined)
		return undefined;
	input.__proto__ = Input.prototype;
	return input;
}
Op.prototype.GetOutput = function(id) {
	var output = op_get_output(this.address, id);
	if (output == undefined)
		return undefined;
	output.__proto__ = Output.prototype;
	return output;
}

Op.prototype.SetNumberOfInputs = function(num) {
	op_set_number_of_inputs(this.address, num);
}

Op.prototype.SetNumberOfOutputs = function(num) {
	op_set_number_of_outputs(this.address, num);
}

Op.prototype.Rebuild = function() {
	op_set_javascript_object(this.address, this, this.Update);
}