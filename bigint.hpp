#include <iostream>
#include <string>
#include <vector>

using namespace std;

// helper functions ----------------------------------------------------------

typedef char byte;

size_t max(size_t a, size_t b)
{
    return a > b ? a : b;
}

// safe array access in reverse
byte reverse_at(const vector<byte> &v, size_t i)
{
    if (i < v.size())
        return v[v.size() - i - 1];
    return 0;
}

size_t next_power_of_2(size_t a)
{
    for (size_t p = 2;; p *= 2)
        if (p >= a)
            return p;
}

void remove_leading_zeros(vector<byte> &v)
{
    size_t r;
    for (r = 0; r < v.size(); r++)
        if (v[r] != 0)
            break;
    v.erase(v.begin(), v.begin() + r);
}

// v = p * 10 ^ (n / 2) + q
void split_in_half(const vector<byte> &v, vector<byte> &p, vector<byte> &q, size_t n)
{
    p = vector<byte>(n / 2, 0);
    q = vector<byte>(n / 2, 0);
    for (size_t i = 0; i < n / 2; i++)
        q[n / 2 - i - 1] = reverse_at(v, i);
    for (size_t i = 0; i < n / 2; i++)
        p[n / 2 - i - 1] = reverse_at(v, n / 2 + i);
    remove_leading_zeros(p);
    remove_leading_zeros(q);
}

vector<byte> internal_add(const vector<byte> &left, const vector<byte> &right)
{
    size_t n = max(left.size(), right.size());
    vector<byte> sum(n, 0);
    byte carry = 0;
    for (size_t i = 0; i < n; i++)
    {
        byte a = reverse_at(left, i) + reverse_at(right, i) + carry;
        sum[n - i - 1] = a % 10;
        carry = a / 10;
    }
    if (carry != 0)
        sum.insert(sum.begin(), carry);
    return sum;
}

// note: left must be always bigger than right.
vector<byte> internal_sub(const vector<byte> &left, const vector<byte> &right)
{
    size_t n = max(left.size(), right.size());
    vector<byte> dif(n, 0);
    byte carry = 0;
    for (size_t i = 0; i < n; i++)
    {
        byte a = reverse_at(left, i) - reverse_at(right, i) + carry;
        carry = a < 0 ? -1 : 0;
        if (a < 0)
            a += 10;
        dif[n - i - 1] = a;
    }
    remove_leading_zeros(dif);
    return dif;
}

// bigint class definition -------------------------------------------------
class bigint
{
public:
    bigint() {}

    bigint(long long d)
    {
        sign = d < 0 ? -1 : 1;
        d = sign * d;
        while (d > 0)
        {
            data.insert(data.begin(), d % 10);
            d /= 10;
        }
    }

    bigint(const string &s)
    {
        if (s == "0")
            return;

        int shift = 0;
        if (s[0] == '-')
        {
            sign = -1;
            shift = 1;
        }

        data = vector<byte>(s.size() - shift, 0);
        for (size_t i = shift; i < s.size(); i++)
        {
            data[i - shift] = s[i] - '0';
        }
    }

    string to_string() const
    {
        if (data.size() == 0)
            return "0";

        string s = string(data.size(), '0');
        for (size_t i = 0; i < data.size(); i++)
            s[i] = data[i] + '0';
        if (sign == -1)
            return '-' + s;
        return s;
    }

    long long to_longlong() const
    {
        long long u = 0;
        for (size_t i = 0; i < data.size(); i++)
            u = u * 10 + data[i];
        return u * sign;
    }

    bigint abs() const
    {
        bigint d = *this;
        d.sign = 1;
        return d;
    }

    ostream friend &operator<<(ostream &out, const bigint &b);
    istream friend &operator>>(istream &in, bigint &b);
    bool friend operator>(const bigint &left, const bigint &right);
    bool friend operator==(const bigint &left, const bigint &right);
    bigint friend operator-(const bigint &right);
    bigint friend operator+(const bigint &left, const bigint &right);
    bigint friend operator-(const bigint &left, const bigint &right);
    bigint friend operator*(const bigint &left, const bigint &right);
    bigint friend operator/(const bigint &left, const bigint &right);
    bigint friend operator%(const bigint &left, const bigint &right);

private:
    vector<byte> data;
    byte sign = 1;
};

ostream &operator<<(ostream &out, const bigint &b)
{
    return out << b.to_string();
}

istream &operator>>(istream &in, bigint &b)
{
    string s;
    in >> s;
    b = s;
    return in;
}

bigint abs(const bigint &a)
{
    return a.abs();
}

bool operator>(const bigint &left, const bigint &right)
{
    if (left.sign < right.sign) // left < right
        return false;
    if (left.sign > right.sign) // left > right
        return true;

    //left.sign == right.sign;
    size_t n = max(left.data.size(), right.data.size());
    for (size_t i = 0; i < n; i++)
    {
        byte dif = reverse_at(left.data, n - i - 1) - reverse_at(right.data, n - i - 1);
        dif *= left.sign;
        if (dif > 0)
            return true;
        else if (dif < 0)
            return false;
    }
    return false;
}

bool operator<(const bigint &left, const bigint &right)
{
    return right > left;
}

bool operator>=(const bigint &left, const bigint &right)
{
    return !(left < right);
}

bool operator<=(const bigint &left, const bigint &right)
{
    return !(left > right);
}

bool operator==(const bigint &left, const bigint &right)
{
    if (left.data.size() != right.data.size())
        return false;

    if (left.data.size() == 0 && right.data.size() == 0) // left = right = 0
        return true;

    if (left.sign != right.sign)
        return false;

    size_t n = max(left.data.size(), right.data.size());
    for (size_t i = 0; i < n; i++)
        if (reverse_at(left.data, i) != reverse_at(right.data, i))
            return false;
    return true;
}

bool operator!=(const bigint &left, const bigint &right)
{
    return !(left == right);
}

bigint operator-(const bigint &right)
{
    bigint neg = right;
    if (neg.data.size() != 0) // neg != 0;
        neg.sign = -neg.sign;
    return neg;
}

bigint operator+(const bigint &left, const bigint &right)
{
    if (left.sign == right.sign)
    {
        bigint sum;
        sum.data = internal_add(left.data, right.data);
        sum.sign = left.sign;
        return sum;
    }
    else
    {
        if (left.abs() == right.abs())
        {
            return 0;
        }
        if (left.abs() > right.abs())
        {
            bigint dif;
            dif.data = internal_sub(left.data, right.data);
            dif.sign = left.sign;
            return dif;
        }
        else
        {
            bigint dif;
            dif.data = internal_sub(right.data, left.data);
            dif.sign = right.sign;
            return dif;
        }
    }
}

bigint operator-(const bigint &left, const bigint &right)
{
    return left + -right;
}

// fast multiplication using Karatsuba’s method
bigint operator*(const bigint &left, const bigint &right)
{
    size_t n = max(left.data.size(), right.data.size());
    n = next_power_of_2(n);
    if (n <= 4)
        return left.to_longlong() * right.to_longlong();

    bigint pro, p, q, r, s;
    split_in_half(left.data, p.data, q.data, n);
    split_in_half(right.data, r.data, s.data, n);

    bigint u = p * r, v = (q - p) * (s - r), w = q * s;
    pro = u;
    pro.data.insert(pro.data.end(), n / 2, 0);
    pro = pro + (u + w - v);
    pro.data.insert(pro.data.end(), n / 2, 0);
    pro = pro + w;
    remove_leading_zeros(pro.data);

    pro.sign = left.sign * right.sign;
    if (pro == 0)
        pro.sign = 1;

    return pro;
}

bigint operator/(const bigint &left, const bigint &right)
{
    if (right == 0)
        throw exception();
    if (left.sign == -1 && right.sign == 1)
        return -(-left / right);
    if (left.sign == -1 && right.sign == -1)
        return -left / -right;
    if (left.sign == 1 && right.sign == -1)
        return -(left / -right);

    if (right > left)
        return 0;

    size_t n = left.data.size() - right.data.size();
    bigint div = right, ret = 1;
    div.data.insert(div.data.end(), n, 0); // div = right * 10 ^ n
    ret.data.insert(ret.data.end(), n, 0); // ret = 10 ^ n
    if (left < div)
    {
        div.data.pop_back(); // div /= 10
        ret.data.pop_back(); // ret /= 10
    }
    return (left - div) / right + ret;
}

bigint operator%(const bigint &left, const bigint &right)
{
    return abs(left - left / right * right);
}

// note: a and b must be positive
bigint gcd(bigint a, bigint b)
{
    while (b != 0)
    {
        bigint m = a % b;
        a = b;
        b = m;
    }
    return a.abs();
}

// note: a and b must be positive
bigint lcm(const bigint &a, const bigint &b)
{
    return abs(a / gcd(a, b) * b);
}
