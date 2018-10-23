function Stack() {
	this.arr = [];
	this.push = function(element) { this.arr.push(element);               }
	this.pop  = function()        { return this.arr.splice(-1);           }
	this.peek = function()        { return this.arr[this.arr.length - 1]; }
}