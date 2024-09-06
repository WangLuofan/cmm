int fn(int a, int b, int c, int d) {
	return (a + b) + (c + d) + (c + d) + (c + d) + (c + d) + (c + d);
}

int main(void) {
    return fn(3, 4, 5, 6);
}
