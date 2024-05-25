#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <stack>
#include <string>

using namespace std;

class fistream
{
public:
	static const int bufsz = 1 << 16;

	class buffer
	{
	public:
		int isz;
		char ibuf[bufsz];
	};

	FILE * pin;
	stack<buffer> ibuf_stk;
	int ipt, isz;
	char ibuf[bufsz];
	char word[64];

	fistream();
	fistream(FILE *);

	operator bool ();

	void stack_buffer();
	void next_buffer();
	bool trim();

	bool integral();
	bool floating();
	bool getline(char *, const int &);

	bool eof();

	void putback(const char &);

	fistream & operator >> (char &);
	fistream & operator >> (char *);
	fistream & operator >> (int &);
	fistream & operator >> (unsigned int &);
	fistream & operator >> (long long &);
	fistream & operator >> (unsigned long long &);
	fistream & operator >> (float &);
	fistream & operator >> (double &);
	fistream & operator >> (long double &);
} fin(stdin), fin_null(0);

fistream::fistream() : pin(0), ipt(bufsz), isz(bufsz)
{

}

fistream::fistream(FILE * pin) : pin(pin), ipt(bufsz), isz(bufsz)
{

}

inline fistream::operator bool ()
{
	return this != &fin_null;
}

inline void fistream::stack_buffer()
{
	if (ipt == 0)
	{
		ibuf_stk.push(buffer());
		ibuf_stk.top().isz = isz;
		memcpy(ibuf_stk.top().ibuf, ibuf, sizeof ibuf[0] * isz);
		isz = bufsz;
		ipt = isz;
	}
}

inline void fistream::next_buffer()
{
	if (ibuf_stk.empty())
		isz = fread(ibuf, 1, bufsz, pin);
	else
	{
		isz = ibuf_stk.top().isz;
		memcpy(ibuf, ibuf_stk.top().ibuf, sizeof ibuf[0] * isz);
		ibuf_stk.pop();
	}
	ipt = 0;
}

inline bool fistream::trim()
{
	for (; isz > 0; next_buffer())
	{
		for (; ipt < isz && ibuf[ipt] <= ' '; ipt++);
		if (ipt < isz)
			break;
	}
	return eof();
}

inline bool fistream::integral()
{
	if (trim())
		return false;

	int len = 0;
	bool vld = false;
	if (ibuf[ipt] == '-' || ibuf[ipt] == '+')
		word[len++] = ibuf[ipt++];
	for (; isz > 0; next_buffer())
	{
		for (; ipt < isz && isdigit(ibuf[ipt]); ipt++)
		{
			word[len++] = ibuf[ipt];
			vld = true;
		}
		if (ipt < isz)
			break;
	}
	word[len] = 0;

	return vld;
}

inline bool fistream::floating()
{
	if (trim())
		return false;

	int len = 0;
	bool vld = false, rht = false;
	if (ibuf[ipt] == '-' || ibuf[ipt] == '+')
		word[len++] = ibuf[ipt++];
	for (; isz > 0; next_buffer())
	{
		for (; ipt < isz && (isdigit(ibuf[ipt]) || ibuf[ipt] == '.'); ipt++)
		{
			if (ibuf[ipt] == '.')
			{
				if (rht)
					break;
				else
					rht = true;
			}
			else
				vld = true;
			word[len++] = ibuf[ipt];
		}
		if (ipt < isz)
			break;
	}
	word[len] = 0;

	return vld;
}

inline bool fistream::getline(char * s, const int & n)
{
	if (this == &fin_null || eof())
		return false;

	int slen = 0;
	for (; isz > 0; next_buffer())
	{
		for (; ipt < isz && slen < n - 1 && ibuf[ipt] != '\n'; ipt++)
			s[slen++] = ibuf[ipt];
		if (ipt < isz)
			break;
	}
	s[slen] = 0;
	for (; isz > 0 && ipt == isz; next_buffer());
	if (ipt < isz && ibuf[ipt] == '\n')
		ipt++;
	return true;
}

inline bool fistream::eof()
{
	for (; isz > 0 && ipt == isz; next_buffer());
	return isz == 0;
}

inline void fistream::putback(const char & c)
{
	if (ipt == 0)
		stack_buffer();
	ibuf[--ipt] = c;
}

inline fistream & fistream::operator >> (char & c)
{
	if (this == &fin_null || trim())
		return fin_null;
	else
	{
		c = ibuf[ipt++];
		return *this;
	}
}

inline fistream & fistream::operator >> (char * s)
{
	if (this == &fin_null || trim())
		return fin_null;

	int slen = 0;
	for (; isz > 0; next_buffer())
	{
		for (; ipt < isz && ibuf[ipt] > ' '; ipt++)
			s[slen++] = ibuf[ipt];
		if (ipt < isz)
			break;
	}
	s[slen] = 0;

	return *this;
}

inline fistream & fistream::operator >> (int & x)
{
	if (this == &fin_null || !(this->integral()))
		return fin_null;

	int i = 0, sgn;
	int ans = 0;
	if (word[0] == '-')
		sgn = -1, i++;
	else if (word[0] == '+')
		sgn = 1, i++;
	else
		sgn = 1;
	for (; word[i]; i++)
		ans = ans * 10 + word[i] - '0';
	x = ans * sgn;

	return *this;
}

inline fistream & fistream::operator >> (unsigned int & x)
{
	if (this == &fin_null || !(this->integral()))
		return fin_null;

	int i = 0, sgn;
	int ans = 0;
	if (word[0] == '-')
		sgn = -1, i++;
	else if (word[0] == '+')
		sgn = 1, i++;
	else
		sgn = 1;
	for (; word[i]; i++)
		ans = ans * 10 + word[i] - '0';
	x = ans * sgn;

	return *this;
}

inline fistream & fistream::operator >> (long long & x)
{
	if (this == &fin_null || !(this->integral()))
		return fin_null;

	int i = 0, sgn;
	long long ans = 0;
	if (word[0] == '-')
		sgn = -1, i++;
	else if (word[0] == '+')
		sgn = 1, i++;
	else
		sgn = 1;
	for (; word[i]; i++)
		ans = ans * 10 + word[i] - '0';
	x = ans * sgn;

	return *this;
}

inline fistream & fistream::operator >> (unsigned long long & x)
{
	if (this == &fin_null || !(this->integral()))
		return fin_null;

	int i = 0, sgn;
	long long ans = 0;
	if (word[0] == '-')
		sgn = -1, i++;
	else if (word[0] == '+')
		sgn = 1, i++;
	else
		sgn = 1;
	for (; word[i]; i++)
		ans = ans * 10 + word[i] - '0';
	x = ans * sgn;

	return *this;
}

inline fistream & fistream::operator >> (float & x)
{
	if (this == &fin_null || !(this->floating()))
		return fin_null;

	int i = 0, sgn;
	bool vis = false;
	float ans = 0, factor = 1;
	if (word[0] == '-')
		sgn = -1, i++;
	else if (word[0] == '+')
		sgn = 1, i++;
	else
		sgn = 1;
	for (; word[i]; i++)
	{
		if (word[i] == '.')
		{
			vis = true;
			continue;
		}

		if (!vis)
			ans = ans * 10 + word[i] - '0';
		else
		{
			factor /= 10;
			ans += factor * (word[i] - '0');
		}
	}
	x = ans * sgn;

	return *this;
}

inline fistream & fistream::operator >> (double & x)
{
	if (this == &fin_null || !(this->floating()))
		return fin_null;

	int i = 0, sgn;
	bool vis = false;
	double ans = 0, factor = 1;
	if (word[0] == '-')
		sgn = -1, i++;
	else if (word[0] == '+')
		sgn = 1, i++;
	else
		sgn = 1;
	for (; word[i]; i++)
	{
		if (word[i] == '.')
		{
			vis = true;
			continue;
		}

		if (!vis)
			ans = ans * 10 + word[i] - '0';
		else
		{
			factor /= 10;
			ans += factor * (word[i] - '0');
		}
	}
	x = ans * sgn;

	return *this;
}

inline fistream & fistream::operator >> (long double & x)
{
	if (this == &fin_null || !(this->floating()))
		return fin_null;

	int i = 0, sgn;
	bool vis = false;
	long double ans = 0, factor = 1;
	if (word[0] == '-')
		sgn = -1, i++;
	else if (word[0] == '+')
		sgn = 1, i++;
	else
		sgn = 1;
	for (; word[i]; i++)
	{
		if (word[i] == '.')
		{
			vis = true;
			continue;
		}

		if (!vis)
			ans = ans * 10 + word[i] - '0';
		else
		{
			factor /= 10;
			ans += factor * (word[i] - '0');
		}
	}
	x = ans * sgn;

	return *this;
}

class fostream
{
public:
	static const int bufsz = 1 << 16;

	FILE * pout;
	int opt;
	char obuf[bufsz];
	char word[64];
	int pcs;
	string format_float, format_double, format_long_double;

	class precision
	{
	public:
		int pcs;

		precision() : pcs(0)
		{

		}

		precision(const int & pcs) : pcs(pcs)
		{

		}
	};

	fostream();
	fostream(FILE *);
	~fostream();

	void fprecision(const int &);
	fostream & operator << (const precision &);

	fostream & operator << (const char &);
	fostream & operator << (const char *);
	fostream & operator << (const int &);
	fostream & operator << (const unsigned int &);
	fostream & operator << (const long long &);
	fostream & operator << (const unsigned long long &);
	fostream & operator << (const float &);
	fostream & operator << (const double &);
	fostream & operator << (const long double &);

	void flush();
} fout(stdout), ferr(stderr);

const char fendl = '\n';

fostream::fostream() : pout(0), opt(0), pcs(3), format_float("%.3f"), format_double("%.3lf"), format_long_double("%.3Lf")
{

}

fostream::fostream(FILE * pout) : pout(pout), opt(0), pcs(3), format_float("%.3f"), format_double("%.3lf"), format_long_double("%.3Lf")
{

}

fostream::~fostream()
{
	flush();
}

inline void fostream::fprecision(const int & x)
{
	pcs = x;
	sprintf(word, "%%.%d", pcs);
	(format_float = word) += "f";
	(format_double = word) += "lf";
	(format_long_double = word) += "Lf";
}

inline fostream & fostream::operator << (const precision & rhs)
{
	fprecision(rhs.pcs);
	return *this;
}

inline fostream & fostream::operator << (const char & c)
{
	obuf[opt++] = c;
	if (opt == bufsz)
		flush();
	return *this;
}

inline fostream & fostream::operator << (const char * s)
{
	for (int i = 0; s[i]; i++)
	{
		obuf[opt++] = s[i];
		if (opt == bufsz)
			flush();
	}
	return *this;
}

inline fostream & fostream::operator << (const int & x)
{
	int len = 0;
	unsigned int y;
	if (x < 0)
		word[len++] = '-', y = -x;
	else
		y = x;
	if (y == 0)
		word[len++] = '0';
	else
	{
		for (; y > 0; y /= 10)
			word[len++] = '0' + y % 10;
	}
	reverse(word + (x < 0 ? 1 : 0), word + len);
	word[len] = 0;
	*this << word;
	return *this;
}

inline fostream & fostream::operator << (const unsigned int & x)
{
	int len = 0;
	if (x == 0)
		word[len++] = '0';
	else
	{
		for (unsigned int y = x; y > 0; y /= 10)
			word[len++] = '0' + y % 10;
	}
	reverse(word, word + len);
	word[len] = 0;
	*this << word;
	return *this;
}

inline fostream & fostream::operator << (const long long & x)
{
	int len = 0;
	unsigned long long y;
	if (x < 0)
		word[len++] = '-', y = -x;
	else
		y = x;
	if (y == 0)
		word[len++] = '0';
	else
	{
		for (; y > 0; y /= 10)
			word[len++] = '0' + y % 10;
	}
	reverse(word + (x < 0 ? 1 : 0), word + len);
	word[len] = 0;
	*this << word;
	return *this;
}

inline fostream & fostream::operator << (const unsigned long long & x)
{
	int len = 0;
	if (x == 0)
		word[len++] = '0';
	else
	{
		for (unsigned long long y = x; y > 0; y /= 10)
			word[len++] = '0' + y % 10;
	}
	reverse(word, word + len);
	word[len] = 0;
	*this << word;
	return *this;
}

inline fostream & fostream::operator << (const float & x)
{
	sprintf(word, format_float.c_str(), x);
	*this << word;
	return *this;
}

inline fostream & fostream::operator << (const double & x)
{
	sprintf(word, format_double.c_str(), x);
	*this << word;
	return *this;
}

inline fostream & fostream::operator << (const long double & x)
{
	sprintf(word, format_long_double.c_str(), x);
	*this << word;
	return *this;
}

inline void fostream::flush()
{
	if (opt > 0)
	{
		fwrite(obuf, 1, opt, pout);
		opt = 0;
	}
}

inline fostream::precision fsetprecision(const int & x)
{
	return fostream::precision(x);
}

