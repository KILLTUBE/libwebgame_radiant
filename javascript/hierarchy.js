require("assets/javascript/opsys/input.js")
require("assets/javascript/opsys/output.js")
require("assets/javascript/opsys/op.js")
require("assets/javascript/opsys/opsys.js")

// I guess it's easier to implement "class inheritance" in JS when there is no constructor
// either lets do some Init() method or simply to op.address = opaddress e.g.
function MakeClass(name) {
	var existing = get_global()[name];
	// NOT overwriting the previous function makes it possible to just overwrite the methods to quick-reload via ctrl+r
	if (typeof existing != "undefined")
		return existing;
	var func = function() {};
	func.name = name;
	return func;	
}

function NewOp(typename) {
	var existing = get_global()[typename];
	if (typeof existing != "undefined")
		return existing;
	var func = function() {}
	func.name = typename;
	func.prototype = Op.prototype; // We copy the methods from Op
	func.prototype.constructor = func; // On vardump, this shows "OpAdd" instead of "Op" as function name
	return func;
}

require("assets/javascript/opsys/OpAdd.js")


function Node(name, opsystem_filename) {
    this.address = node_new(name, opsystem_filename);
    //log("new Node.......\n");
}
function NodeFromAddress(address) {
	var node = {};
	node.__proto__ = Node.prototype;
	node.address = address;
	return node;
}

// node is Node object, cb is function callback which accepts only a Node as first parameter
function ForEachNode(node, cb) {
	cb(node);
	var childs = node.GetChilds();
	for (var i=0; i<childs.length; i++) {
		ForEachNode(childs[i], cb);
	}
}

//ForEachNode(root, function(node) {
//	//printf("enter node %\n", node.address);
//	node.Rebuild();
//});

Node.prototype.addChild = function(node) {
    node_add_child(this.address, node.address);
}

Node.prototype.Rebuild = function(node) {
    node_rebuild(this.address);
}

Node.prototype.GetChilds = function(node) {
	var childs = node_get_childs(this.address);
	var arr = [];
	for (var i=0; i<childs.length; i++) {
		arr.push(NodeFromAddress(childs[i]));
	}
	return arr;
}

Node.prototype.GetOpSystem = function(node) {
	var tmp = node_get_opsystem(this.address);
	if (tmp == undefined)
		return undefined;
	tmp.__proto__ = OpSystem.prototype;
    return tmp;
}

Node.prototype.SearchChild = function(nodename) {
	var childaddress = node_search_child(this.address, nodename);
	//printf("childaddress %\n", childaddress);
	var node = address_to_node(childaddress);
	return node;
}

function address_to_node(address) {
	if (address == 0)
		return undefined;
	var node = {}
	node.__proto__ = Node.prototype;
	node.address = address;
	return node;
}

function node_init_root() {	
	root = {}
	root.__proto__ = Node.prototype;
	root.address = node_get_root();
}