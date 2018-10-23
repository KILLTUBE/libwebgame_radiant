function add_hierarchy_to_root() {
	create_hierarchy(root);
}
function create_hierarchy(add_to) {
	stack = new Stack();
	stack.push(add_to);

	tmp = new Node("xmodel", "opsystem/a.opsys")
	stack.peek().addChild( tmp );
	stack.push(tmp);
	tmp = new Node("aa", "opsystem/aa.opsys")
	stack.peek().addChild( tmp );
	stack.push(tmp);
	stack.pop(); // aa
	stack.pop(); // xmodel
	tmp = new Node("b", "opsystem/default.opsys")
	stack.peek().addChild( tmp );
	stack.push(tmp);
	tmp = new Node("bb", "opsystem/bb.opsys")
	stack.peek().addChild( tmp );
	stack.push(tmp);
	stack.pop(); // bb
	stack.pop(); // b
	tmp = new Node("c", "opsystem/default.opsys")
	stack.peek().addChild( tmp );
	stack.push(tmp);
	tmp = new Node("cc", "opsystem/default.opsys")
	stack.peek().addChild( tmp );
	stack.push(tmp);
	stack.pop(); // cc
	stack.pop(); // c
	tmp = new Node("menu", "opsystem/menu.opsys")
	stack.peek().addChild( tmp );
	stack.push(tmp);
	stack.pop(); // menu
	tmp = new Node("GLHacker", "opsystem/glhacker.opsys")
	stack.peek().addChild( tmp );
	stack.push(tmp);
	tmp = new Node("test1", "opsystem/glhacker1.opsys")
	stack.peek().addChild( tmp );
	stack.push(tmp);
	stack.pop(); // test1
	tmp = new Node("test2", "opsystem/glhacker2.opsys")
	stack.peek().addChild( tmp );
	stack.push(tmp);
	stack.pop(); // test2
	tmp = new Node("test3", "opsystem/glhacker3.opsys")
	stack.peek().addChild( tmp );
	stack.push(tmp);
	stack.pop(); // test3
	stack.pop(); // GLHacker
	tmp = new Node("opnode_test", "opsystem/opnode_test.opsys")
	stack.peek().addChild( tmp );
	stack.push(tmp);
	stack.pop(); // opnode_test
	tmp = new Node("OpAxisTest", "opsystem/OpAxisTest.opsys")
	stack.peek().addChild( tmp );
	stack.push(tmp);
	stack.pop(); // OpAxisTest
	tmp = new Node("Tests", "opsystem/default.opsys")
	stack.peek().addChild( tmp );
	stack.push(tmp);
	tmp = new Node("MatrixMultiplyPoint", "opsystem/matmulpoint.opsys")
	stack.peek().addChild( tmp );
	stack.push(tmp);
	stack.pop(); // MatrixMultiplyPoint
	tmp = new Node("MatrixDecompose", "opsystem/matdecompose.opsys")
	stack.peek().addChild( tmp );
	stack.push(tmp);
	stack.pop(); // MatrixDecompose
	tmp = new Node("QuatMultiply", "opsystem/quadmultiply.opsys")
	stack.peek().addChild( tmp );
	stack.push(tmp);
	stack.pop(); // QuatMultiply
	stack.pop(); // Tests
	tmp = new Node("Projection", "opsystem/projection.opsys")
	stack.peek().addChild( tmp );
	stack.push(tmp);
	stack.pop(); // Projection

	stack.pop(); // pop add_to, not really needed
}
