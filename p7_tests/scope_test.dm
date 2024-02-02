a : int = 3;
func : () void {
    a : int;
    a = 5;
    give a;
}

main : () int {
    give a;
    a : int = 6;
    give a;
    give "hello\n";
    func();
    give a;
    return a;
}