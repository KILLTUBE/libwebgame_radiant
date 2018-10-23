Output = MakeClass("Output");

Output.prototype.SetName    = function(name) {        output_set_name   (this, name); }
Output.prototype.GetName    = function()     { return output_get_name   (this      ); }