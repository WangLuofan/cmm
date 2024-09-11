int fn(int a, int b, int c, int d) {
    if (a > b) {
        return a;
    }

    return d;
}

int main(void) {
    return fn(3, 4, 5, 6);
}
