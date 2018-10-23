node_init_root()
node = root.SearchChild("Projection")
opsys = node.GetOpSystem()
op = opsys.GetOp("OpNop");
op.__proto__ = OpAdd.prototype;
op.Init();