#include "bigint.hpp"

int main()
{
    cout << "Enter a then operation then b (operation can be +, -, *, /, %, abs, gcd, or lcm)" << endl;

	bigint a, b;
    string op;
	cin >> a >> op >> b;

    if (op == "+")
        cout << a + b << endl;
    else if (op == "-")
        cout << a - b << endl;
    else if (op == "*")
        cout << a * b << endl;
    else if (op == "/")
        cout << a / b << endl;
    else if (op == "%")
        cout << a % b << endl;
    else if (op == "abs")
        cout << a.abs() << endl << b.abs() << endl;
    else if (op == "gcd")
        cout << gcd(a, b) << endl;
    else if (op == "lcm")
        cout << lcm(a, b) << endl;

	return 0;
}
