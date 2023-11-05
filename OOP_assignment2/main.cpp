#include "inf_int.h"

using namespace std;

int main()
{
	inf_int a(12341234);
	inf_int b("5678567800000000");
	inf_int c = a;
	inf_int d(b);
	inf_int e;
	inf_int f = a + b;

	cout << "a made by inf_int a(12341234); : " << a << "\n";
	cout << "b made by inf_int b(\"5678567800000000\"); : " << b << "\n"; 
	cout << "c made by inf_int c = a; : " << c << "\n";
	cout << "d made by inf_int d(b); : " << d << "\n"; 
	cout << "e made by inf_int e; : " << e << "\n"; 
	cout << "f made by inf_int f = a + b; : " << f << "\n\n";

	cout << "a + b : " << a + b << "\n";
	cout << "a - b : " << a - b << "\n";
	cout << "b - a : " << b - a << "\n";
	cout << "a * b : " << a * b << "\n";	
	cout << "a + c : " << a + c << "\n";
	cout << "b * b * a : " << a * a * b << "\n";	
	cout << "c + c - d : " << c + c - d << "\n";
	cout << "a * b * c * d * f : " << a * b * c * d * f << "\n";
	cout << "a * b * c * d * e * f : " << a * b * c * d * e * f << "\n";

	return 0;
}
