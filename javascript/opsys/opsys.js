
// called when user clicks another OpSystem node
OnChangeOpSystem = function() {
	//printf("change opsys\n");
	cursys = GetCurrentOpSystem();
	cursys.__proto__ = OpSystem;
}

OpSystem = MakeClass("OpSystem");

OpSystem.prototype.GetOp = function(name) {
	return opsystem_get_op(this.address, name);
}

OpSystem.prototype.Rebuild = function(name) {
	return opsystem_rebuild(this.address);
}

OpSystem.prototype.GetNewOpPos = function(name) {
	return opsystem_get_new_op_pos(this.address);
}
OpSystem.prototype.NewOp = function(proto) {
	var op = opsystem_new_op(this.address);
	op.__proto__ = proto.prototype;
	op.Init();
	return op;
}