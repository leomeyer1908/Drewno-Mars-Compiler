fact : (num : int) int {
    if (num == 0) {
        return 1;
    }
    return fact(num-1)*num;
}

main : () int {
    give 24Kmagic;
    return 0;
}