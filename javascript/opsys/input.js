Input = MakeClass("Input");

Input.prototype.SetName = function(name) {        input_set_name(this, name); }
Input.prototype.GetName = function()     { return input_get_name(this);       }