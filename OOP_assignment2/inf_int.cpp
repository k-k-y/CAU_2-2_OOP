#include "inf_int.h"

inf_int::inf_int()
{
	this->digits=new char[2];	// dynamic allocation 

	this->digits[0]='0';		// default set value 0
	this->digits[1]='\0';
	this->length=1;
	this->thesign=true;
}

inf_int::inf_int(int n) {
	char buf[100];

	if(n<0){		// handling negative numbers
		this->thesign=false;
		n=-n;
	}else{
		this->thesign=true;
	}

	int i=0;
	while(n>0){			// The process of converting numbers to strings
		buf[i]=n%10+'0';

		n /= 10;
		i++;
	}

	if(i==0){	// If the absolute value of a number is zero
		new (this) inf_int();	// Constructor re-call...gcc says there is a compilation error. In the case of inf_int(), a separate instance is created.
	}else{
		buf[i]='\0';

		this->digits = new char[i + 1];
		this->length = i;
		strcpy(this->digits, buf);
	}

}

inf_int::inf_int(const char* str)
{
	if (str[0] == '-') {
		thesign = false;
		str++;  
	}
	else {
		thesign = true;
	}
	length = strlen(str);
	char* buf = new char[length + 1];
	for (int i = 0; i < length; i++)
	{
		buf[i] = str[length - i - 1];
	}
	buf[length] = '\0';
	digits = new char[length + 1];
	strcpy(digits, buf);
	// ex) "-1053" -> thesign=false, digits="3501", len=4

}

inf_int::inf_int(const inf_int& a) {
	this->digits = new char[a.length + 1];

	strcpy(this->digits, a.digits);
	this->length = a.length;
	this->thesign = a.thesign;
}

inf_int::~inf_int(){
	delete digits;		// Deallocate memory
}

inf_int& inf_int::operator=(const inf_int& a)
{
	if(this->digits) {
		delete this->digits;		// Remove if the string already exists.
	}
	this->digits = new char[a.length + 1];

	strcpy(this->digits, a.digits);
	this->length = a.length;
	this->thesign = a.thesign;

	return *this;
}

bool operator==(const inf_int &a, const inf_int &b)
{
    // we assume 0 is always positive.
    if ( (strcmp(a.digits , b.digits)==0) && a.thesign==b.thesign )	// Must have the same sign and matching absolute values.
        return true;
    return false;
}

bool operator!=(const inf_int& a, const inf_int& b)
{
	return !operator==(a, b);
}

bool operator>(const inf_int& a, const inf_int& b)
{
	if (a.thesign == true && b.thesign == true) // all positive
    {
        if (a.length > b.length) return true;
        else if (a.length < b.length) return false;
        else
        {
            for (int i = a.length - 1; i >= 0; i--)
            {
                if (a.digits[i] > b.digits[i]) return true;
                else if (a.digits[i] < b.digits[i]) return false;
            }
            return false; // in case of a == b
        }
    }
    else if (a.thesign == true && b.thesign == false)
        return true;
    else if (a.thesign == false && b.thesign == true)
        return false;
    else // all negative
    {
        if(a.length < b.length) return true;
        else if (a.length > b.length) return false;
        else
        {
            for (int i = a.length - 1; i >= 0; i--)
            {
                if (a.digits[i] < b.digits[i]) return true;
                else if (a.digits[i] > b.digits[i]) return false;
            }
            return false; // in case of a == b
        }
    }
}

bool operator<(const inf_int& a, const inf_int& b)
{
	if (operator>(a, b) || operator==(a, b)) {
		return false;
	}
	else {
		return true;
	}
}

char* reverseString(const char* str) {
    int length = strlen(str);
    char* reversed = new char[length + 1];

    for (int i = 0; i < length; i++) {
        reversed[i] = str[length - 1 - i];
    }
    reversed[length] = '\0';

    return reversed;
}

bool compareCharString(char *a, char *b) {
	int len_a = strlen(a);
	int len_b = strlen(b);

	if (len_a > len_b) {
		return true;
	} else if (len_a < len_b) {
		return false;
	} else {
		for (int i = 0; i < len_a; i++) {
			if (a[i] > b[i]) {
				return true;
			} else if (a[i] < b[i]) {
				return false;
			}
		}
		return true;
	}
}

char* removeLeadingZeros(char* str) {
    if (str == nullptr) {
        return nullptr;
    }

    // Find the position of the first non-zero digit
    int i = 0;
    while (str[i] == '0' && str[i] != '\0') {
        i++;
    }

    if (i > 0) {
        // If leading zeros were found, shift the string to the left
        int j = 0;
        while (str[i] != '\0') {
            str[j] = str[i];
            i++;
            j++;
        }
        str[j] = '\0'; // Null-terminate the new string
    }
    return str;
}

inf_int operator+(const inf_int& a, const inf_int& b)
{
	inf_int c;
	unsigned int i;

	if(a.thesign==b.thesign){	// 이항의 부호가 같을 경우 + 연산자로 연산 / + operator when binomials have the same sign
		for(i=0; i<a.length; i++){
			c.Add(a.digits[i], i+1);
		}	
		for(i=0; i<b.length; i++){
			c.Add(b.digits[i], i+1);
		}

		c.thesign = a.thesign;

		return c;
	}else{	// 이항의 부호가 다를 경우 - 연산자로 연산 Operate on binomials with different signs with the - operator
		c=b;
		c.thesign=a.thesign;

		return a - c;
	}
}

inf_int operator-(const inf_int& a, const inf_int& b) {
    inf_int c;
    inf_int d;
    if (a.thesign != b.thesign) {
        d = b;
        if (b.thesign)
            d.thesign = false;
        else
            d.thesign = true;
        c = a + d;
    } else {
        if (a.thesign) {
            // Both operands are positive.
            if (compareCharString(reverseString(a.digits), reverseString(b.digits))) {
                c = a; // Initialize c with a's value
                c.Sub(reverseString(a.digits), reverseString(b.digits));
            } else {
                c.Sub(reverseString(b.digits), reverseString(a.digits));
                c.thesign = false; // Set the result as negative since b > a
            }
        } else {
            // Both operands are negative.
            //c = b - a; // Subtract a from b, which is equivalent to -(|a| - |b|).
            //c.thesign = true; // Set the result as positive since a > b
			if (compareCharString(reverseString(a.digits), reverseString(b.digits))) {
				c.Sub(reverseString(a.digits), reverseString(b.digits));
				c.thesign = false;
			} else {
				c.Sub(reverseString(b.digits), reverseString(a.digits));
				c.thesign = true;
			}
			
        }
    } 
    c.length = strlen(c.digits);
    return c;
}

void inf_int::Sub(char *a, char *b) {

	int len_a = strlen(a);
    int len_b = strlen(b);

    int len = len_a > len_b ? len_a : len_b;

    char *c = (char *)malloc(len + 1);

    int i = len_a - 1;
    int j = len_b - 1;
    int k = len - 1;

    int borrow = 0;

    while (i >= 0 && j >= 0) {
        int x = a[i] - '0';
        int y = b[j] - '0';

        int z = x - y - borrow;

        if (z < 0) {
            z += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        c[k] = z + '0';

        i--;
        j--;
        k--;
    }

    while (i >= 0) {
        int x = a[i] - '0';
        int z = x - borrow;

        if (z < 0) {
            z += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        c[k] = z + '0';

        i--;
        k--;
    }

	c[len] = '\0';
	c = removeLeadingZeros(c);
    c = reverseString(c);
	
	this->digits = strdup(c);
}

inf_int abs(const inf_int& n) { return +n; }

inf_int operator+(const inf_int& a) {

	inf_int tmp(a);
	tmp.thesign = true;
	return tmp;
}

inf_int operator-(const inf_int& a)
{
	if (a == 0) {
		return 0;
	}
	inf_int tmp(a);
	tmp.thesign = a.thesign ? false : true;
	return tmp;
}

void FFT(vector<complex<double> >& f, bool inv = false)
{
	int size = f.size();
	for (int i = 1, j = 0; i < size; i++) {
		int tmp = size / 2;
		while (!((j ^= tmp) & tmp)) {
			tmp /= 2;
		}
		if (i < j) {
			swap(f[i], f[j]);
		}
	}
	for (int i = 1; i < size; i *= 2) {
		double tmp = (inv ? acos(-1) / i : -acos(-1) / i);
		complex<double> w(cos(tmp), sin(tmp));
		for (int j = 0; j < size; j += i * 2) {
			complex<double> wp(1, 0);
			for (int k = 0; k < i; k++) {
				complex<double> x = f[j + k], y = f[i + j + k] * wp;
				f[j + k] = x + y;
				f[i + j + k] = x - y;
				wp *= w;
			}
		}
	}
	if (inv) {
		for (int i = 0; i < size; i++) {
			f[i] /= size;
		}
	}
}

inf_int operator*(const inf_int& a, const inf_int& b)
{
	if (a == 0 || b == 0) {
		return 0;
	}
	if (a == 1 || a == -1) {
		return a.thesign ? b : -b;
	}
	if (b == 1 || b == -1) {
		return b.thesign ? a : -a;
	}
	if (a.thesign != b.thesign) {
		return -(abs(a) * abs(b));
	}
	else if (a.thesign == false && b.thesign == false) {
		return abs(a) * abs(b);
	}
	else {
		vector<complex<double> > A, B, C;
		string tempA = a.digits;
		string tempB = b.digits;
		
		
		for (auto i : tempA) {
			A.push_back(complex<double>(i - '0', 0));
		}
		for (auto i : tempB) {
			B.push_back(complex<double>(i - '0', 0));
		}
		int num = 1;
		int Asize = A.size();
		int Bsize = B.size();
		while (num < Asize + 1 || num < Bsize + 1) {
			num *= 2;
		}
		num *= 2;
		A.resize(num);
		B.resize(num);
		C.resize(num);
		FFT(A);
		FFT(B);
		for (int i = 0; i < num; i++) {
			C[i] = A[i] * B[i];
		}
		FFT(C, true);
		int carry = 0;
		vector<int> r;
		int Csize = C.size();
		for (int i = 0; i < Csize; i++) {
			int tmp = (int)round(C[i].real()) + carry;
			r.push_back(tmp % 10);
			carry = tmp / 10;
		}
		while (carry > 0) {
			r.push_back(carry % 10);
			carry /= 10;
		}
		int rsize = r.size();
		int i;
		for (i = rsize - 1; i >= 0 && r[i] == 0; i--) {}
		string ans = "";
		if (i < 0) {
			return 0;
		}
		else {
			for (; i >= 0; i--) {
				ans += (r[i] + '0');
			}
		}
		inf_int a(ans.c_str());
		return a;
	}
}

ostream& operator<<(ostream& out, const inf_int& a)
{
	int i;

	if (a.thesign == false) {
		out << '-';
	}
	for (i = a.length - 1; i >= 0; i--) {
		out << a.digits[i];
	}
	return out;
}

void inf_int::Add(const char num, const unsigned int index)	// a의 index 자리수에 n을 더한다. 0<=n<=9, ex) a가 391일때, Add(a, 2, 2)의 결과는 411
{
	if (this->length < index) {
		this->digits = (char*)realloc(this->digits, index + 1);

		if (this->digits == NULL) {		// 할당 실패 예외처리
			cout << "Memory reallocation failed, the program will terminate." << endl;

			exit(0);
		}

		this->length = index;					// 길이 지정
		this->digits[this->length] = '\0';	// 널문자 삽입
	}

	if (this->digits[index - 1] < '0') {	// 연산 전에 '0'보다 작은 아스키값인 경우 0으로 채움. 쓰여지지 않았던 새로운 자리수일 경우 발생
		this->digits[index - 1] = '0';
	}

	this->digits[index - 1] += num - '0';	// 값 연산


	if (this->digits[index - 1] > '9') {	// 자리올림이 발생할 경우
		this->digits[index - 1] -= 10;	// 현재 자릿수에서 (아스키값) 10을 빼고
		Add('1', index + 1);			// 윗자리에 1을 더한다
	}
}