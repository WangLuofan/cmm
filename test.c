int fn(int a, int b, int c, int d) {
	if ((a && b) || (c && d)) {
        return a + b;
    }

    return c + d;
}

int main(void) {
    return fn(3, 4, 5, 6);
}
