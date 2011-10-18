#include<iostream>
#include<cstdio>
#include<string>
#include<cstring>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include<climits>
#include<algorithm>
#include<map>
using namespace std;

#define square(x) (x)*(x)
#define OFF(i, j) ((i) == (j) ? 1 : 0)
#define xmul(x1, y1, x2, y2) ((x1)*(y2)-(x2)*(y1))
const double pi = acos(-1.0);
struct pnt { 
	double x, y, z; 
	pnt operator+(const pnt &p) const {
		pnt ret; ret.x = x + p.x; ret.y = y + p.y;
		ret.z = z + p.z; return ret;
	} pnt operator-(const pnt &p) const {
		pnt ret; ret.x = x - p.x; ret.y = y - p.y;
		ret.z = z - p.z; return ret;
	} pnt operator*(const double c) const {
		pnt ret; ret.x = x * c; ret.y = y * c;
		ret.z = z * c; return ret;
	} pnt operator/(const double c) const {
		pnt ret; ret.x = x / c; ret.y = y / c;
		ret.z = z / c; return ret;
	}
};
typedef pnt vec;
struct plane { vec ori; double val; };
#define N 50010
pnt p[N], q1[N], q2[N], q3[N];
plane s[N];
int n, m;

double nummul(const vec &v1, const vec &v2) { return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z; }
vec submul(const vec &v1, const vec &v2) {
	vec ret; ret.x = xmul(v1.y, v1.z, v2.y, v2.z); ret.y = xmul(v1.z, v1.x, v2.z, v2.x);
	ret.z = xmul(v1.x, v1.y, v2.x, v2.y); return ret;
}

vec uvec(vec v) {
	double len = sqrt(square(v.x)+square(v.y)+square(v.z));
	vec ret; ret.x = v.x / len; ret.y = v.y / len; ret.z = v.z / len; return ret;
}

plane getplane(const pnt &p1, const pnt &p2, const pnt &p3) {
	vec v1 = p2 - p1, v2 = p3 - p1;
	plane ret; ret.ori = uvec(submul(v1, v2)); 
	ret.val = nummul(ret.ori, p1); return ret;
}
plane uplane(const plane &s) {
	double len = sqrt(square(s.ori.x)+square(s.ori.y)+square(s.ori.z));
	plane ret; ret.ori = s.ori / len; ret.val = s.val / len; return ret;
}

void matmul(double a[][4], double b[][4]) {
	int i, j, k; double c[4][4]; 
	for (i = 0; i < 4; ++i) for (j = 0; j < 4; ++j) { c[i][j] = a[i][j]; a[i][j] = 0; }
	for (i = 0; i < 4; ++i) for (j = 0; j < 4; ++j) for (k = 0; k < 4; ++k) a[i][j] += c[i][k] * b[k][j];
}

pnt pnttran(const pnt &p, double a[][4]) {
	double val[4] = { p.x, p.y, p.z, 1 }, rv[4] = { 0, 0, 0, 0 };
	for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) rv[i] += val[j] * a[j][i];
	pnt ret = { rv[0], rv[1], rv[2] }; return ret;
}

void mattranslate(double a[][4], double dx, double dy, double dz) {
	int i; double b[4][4]; 
	memset(b, 0, sizeof(b)); for (i = 0; i < 4; ++i) b[i][i] = 1;
	b[3][0] = dx; b[3][1] = dy; b[3][2] = dz; matmul(a, b);
}

void matrotate(double a[][4], const vec &axis, double dlt) {
	int i, j; double b[4][4]; vec v = uvec(axis); 
	double sind = sin(dlt), cosd = cos(dlt), val[3] = { v.x, v.y, v.z }, off[3] = { 0, 1, -1 };
	memset(b, 0, sizeof(b)); for (i = 0; i < 4; ++i) b[i][i] = 1;
	for (i = 0; i < 3; ++i) for (j = 0; j < 3; ++j) 
		b[i][j] = (1-cosd) * val[i] * val[j] + sind * off[(3-i+j)%3] * val[(6-i-j)%3] + cosd * OFF(i, j);
	matmul(a, b);
}

void matscale(double a[][4], double cx, double cy, double cz) {
	int i; double b[4][4];
	memset(b, 0, sizeof(b)); for (i = 0; i < 4; ++i) b[i][i] = 1;
	b[0][0] = cx; b[1][1] = cy; b[2][2] = cz; matmul(a, b);
}

void conduct()
{
	double val[3], off[3] = { 0, 1, -1 };
	int i, j, req; char cmd[100];
	double tra[4][4], tm[4][4];
	double a, b, c, dlt, cd, sd; vec tv;
	scanf("%d%d%d", &n, &m, &req);
	for (i = 0; i < n; ++i) scanf("%lf%lf%lf", &p[i].x, &p[i].y, &p[i].z);
	for (i = 0; i < m; ++i) { scanf("%lf%lf%lf%lf", &s[i].ori.x, &s[i].ori.y, &s[i].ori.z, &s[i].val); s[i].val *= -1.0; }
	for (i = 0; i < m; ++i) { 
		s[i] = uplane(s[i]); q1[i] = s[i].ori * s[i].val; 
		tv = (vec){ s[i].ori.y-s[i].ori.z, s[i].ori.z-s[i].ori.x, s[i].ori.x-s[i].ori.y }; q2[i] = q1[i]+tv;
		tv = submul(tv, s[i].ori); q3[i] = q2[i]+tv;
	}
	for (i = 0; i < 4; ++i) for (j = 0; j < 4; ++j) tra[i][j] = 0;
	for (i = 0; i < 4; ++i) tra[i][i] = 1;
	while (req--) {
		scanf("%s", cmd);
		if (!strcmp(cmd, "TRANSLATE")) {
			scanf("%lf%lf%lf", &a, &b, &c); mattranslate(tra, a, b, c);
		} else if (!strcmp(cmd, "ROTATE")) {
			scanf("%lf%lf%lf%lf", &tv.x, &tv.y, &tv.z, &dlt); matrotate(tra, tv, dlt/180.0*pi);
		} else {
			scanf("%lf%lf%lf", &a, &b, &c); matscale(tra, a, b, c);
		} 
	} for (i = 0; i < n; ++i) p[i] = pnttran(p[i], tra);
	for (i = 0; i < m; ++i) {
		q1[i] = pnttran(q1[i], tra); q2[i] = pnttran(q2[i], tra); q3[i] = pnttran(q3[i], tra);
		s[i] = getplane(q1[i], q2[i], q3[i]);
	} for (i = 0; i < n; ++i) printf("%.2f %.2f %.2f\n", p[i].x, p[i].y, p[i].z);
	for (i = 0; i < m; ++i) printf("%.2f %.2f %.2f %.2f\n", s[i].ori.x, s[i].ori.y, s[i].ori.z, -s[i].val);
}

int main()
{
	conduct();
	return 0;
}