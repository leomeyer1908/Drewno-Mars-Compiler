sum : (a : int, b : int, c : int, d : int, e : int, f : int, g : int, h : int) int {
    n : int = a*b*c*d*e*f*g*h;
    i : int = 0;
    s : int = 0;
    while (i < a) {
        s = s + i;
        i++;
    }
    return s+n;
}

main : () int {
    a : int = -123 - 123 + 100 * 500 / 60;
    b : bool = true == !false or 1 != 2 or 1 > 2 and 2 >= 3 or 5 < 2 and 6 <= 2;
    give b;
    give "\n";
    c : int;
    take c;
    d : int = 10;
    if (c < d) {
        give c;
    } else {
        give d;
    }
    give "Enter number to sum: ";
    take c;
    give "Your number is: ";
    give sum(c, 50, 11, 12, 13, 14, 15, 16);
    give "\n";
    return 39;
} 