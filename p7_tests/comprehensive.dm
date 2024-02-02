a : int;
b : bool;
c : perfect int;
d : perfect bool;
e : int = 123 - 123 + 100 * -500 / 60;
f : bool = true == !false or 1 != 2 or 1 > 2 and 2 >= 3 or 5 < 2 and 6 <= 2;
g : perfect int;
h : perfect bool;

fn : (d : int, z : bool, x : perfect int, y : perfect bool) int {
	c : perfect int;
	a : int;
	a = 4;
	a = a + 4;
	if (a == 8) {
		give "test";
	}
	return -a;
}

fn2 : () void {
	c : int;
	d : bool;
	c = 5;
	d = true;
	c++;
	c--;
	give "hello world";
}

main : () int {
	o : int = a;
	p : bool = b;
	q : int;
	r : perfect bool;
	j : int = 123 - 123 + 100 * -500 / 60;
	k : bool = true == !false or 1 != 2 or 1 > 2 and 2 >= 3 or 5 < 2 and 6 <= 2;
	l : perfect int;
	m : perfect bool;
	o = 5;
	o--;
	o--;
	o++;
	a = 4;
	q = 5;
	b = false and m;
	fn2();
	give j;
	give "\n";
	give e;
	give "\n";
	if (j == e) {
		give j;
		give "\n";
	}
	if (k) {
		give "hello\n";
	}
	else {
		give "world\n";
	}
	if (o == q) {
		give "should not happen\n";
	}
	else {
		give "this should happend\n";
	}
	give a;
	give "\n\n";
	while (a > 0) {
		give "aaaaa";
		give "\n";
		a--;
	}
	fn2();
	a = fn(a, b, l, m);
	give a;
	give "\n";
	give fn(a, b, l, m);
	give "\nEnter a num: ";
	take o;
	give o;
	give "\nEnter a bool: ";
	take p;
	give p;
	give "\n";
	today I don't feel like doing any work;
	fn2();
	fn2();
}

