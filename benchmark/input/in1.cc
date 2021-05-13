// input file for benchmarking

int main() {
    int x, y;
    cin >> x >> y;
    if (x != 0) {
        if (y > 0) {
            cout << y;
        } else {
            cout << -y;
        }
    }
    float z = 1 + 2 + 3 + 1000/ 50 * 23.2 * (x * y * 10);
    // cout << x << y << z;
    float z0 = 1 + 2 + 3 + 1000/ 50 * 23.2 * (x * y * 10);
    float z1 = 1 + 2 - 3 + 1000/ 50 * 23.2 * (x * y * 10);
    float z2 = 1 + 2 / 3 + 1000/ 50 * 23.2 * (x * y * 10);
    float z3 = 1 + 2 * 3 + 1000/ 50 * 23.2 * (x * y * 10);
    float z4 = 1 + 2 % 3 + 1000/ 50 * 23.2 * (x * y * 10);
    float z5 = 1 + 2 > 3 + 1000/ 50 * 23.2 * (x * y * 10);
    float z6 = 1 + 2 == 3 + 1000/ 50 * 23.2 * (x * y * 10);
    float z7 = 1 + 2 != 3 + 1000/ 50 * 23.2 * (x * y * 10);
    float z8 = 1 + 2 >= 3 + 1000/ 50 * 23.2 * (x * y * 10);
    float z9 = 1 + 2 <= 3 + 1000/ 50 * 23.2 * (x * y * 10);
    cout << z0 << z1 << z2 << z3 << z4;
    cout << z5 << z6 << z7 << z8 << z9;
}
