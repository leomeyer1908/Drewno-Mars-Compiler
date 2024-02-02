fact : (num : int) int {
    if (num == 0) {
        return 1;
    }
    return fact(num-1)*num;
}

main : () int {
    a : int;
    give "Enter a number: ";
    take a;
    give "Your Number Is: ";
    give fact(a);
    give "\n";
    return 0;
}