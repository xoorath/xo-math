#include <iostream>
#include <cmath>
using namespace std;

//#define XO_NO_INVERSE_DIVISION
#include "../xo-math.h"
using namespace xo::math;

#include "Reference/bullet3-master/src/LinearMath/btVector3.h"

enum Winner {
    TIE_CORRECT = 0,
    TIE_INCORRECT,
    WIN_XOMATH,
    WIN_BULLET,

    COUNT
};

struct floatVec3 {
    float x, y, z;
    float MagSquared() const { return x * x + y * y + z * z; }
    float Dot(const floatVec3& v) const { return x * v.x + y * v.y + z * v.z; }
    float Angle(const floatVec3& v) const {
        float s = Sqrt(MagSquared() * v.MagSquared());
        return ACos(Dot(v) / s);
    }
    bool operator < (const floatVec3& v) { return MagSquared() < v.MagSquared(); }
    bool operator <= (const floatVec3& v) { return MagSquared() < v.MagSquared(); }
    bool operator > (const floatVec3& v) { return MagSquared() > v.MagSquared(); }
    bool operator >= (const floatVec3& v) { return MagSquared() > v.MagSquared(); }
    floatVec3 operator + (const floatVec3& v) { return { x + v.x, y + v.y, z + v.z}; }
    floatVec3 operator - (const floatVec3& v) { return { x - v.x, y - v.y, z - v.z}; }
    floatVec3 operator * (const floatVec3& v) { return { x * v.x, y * v.y, z * v.z}; }
    floatVec3 operator / (const floatVec3& v) { return { x / v.x, y / v.y, z / v.z}; }
    floatVec3& operator += (const floatVec3& v) { *this = (*this) + v; return *this; }
    floatVec3& operator -= (const floatVec3& v) { *this = (*this) - v; return *this; }
    floatVec3& operator *= (const floatVec3& v) { *this = (*this) * v; return *this; }
    floatVec3& operator /= (const floatVec3& v) { *this = (*this) / v; return *this; }
};

float Diff(const floatVec3& left, const Vector3& xov) {
    float diffx = Abs(left.x - xov.x);
    float diffy = Abs(left.y - xov.y);
    float diffz = Abs(left.z - xov.z);
    // get the worst difference of all elements.
    return diffx > diffy ? (diffx > diffz ? diffx : diffz) : (diffy > diffz ? diffy : diffz);
}

float Diff(const floatVec3& left, const btVector3& btv) {
    float diffx = Abs(left.x - btv.getX());
    float diffy = Abs(left.y - btv.getY());
    float diffz = Abs(left.z - btv.getZ());
    // get the worst difference of all elements.
    return diffx > diffy ? (diffx > diffz ? diffx : diffz) : (diffy > diffz ? diffy : diffz);
}

float Diff(float left, float right) {
    return Abs(left-right);
}

Winner CheckAgreement(const floatVec3& known, const Vector3& xov, const btVector3& btv, bool print = false) {
    float xod = Diff(known, xov);
    float btd = Diff(known, btv);
    if(Diff(xod, btd) <= FloatEpsilon) { // we agree
        if(xod > FloatEpsilon) {
            if(print) cout << "[bullet v xomath] both got it wrong. diff:" << fixed << xod << "\n";
            return Winner::TIE_INCORRECT;
        }
        else {
            if(print) cout << "[bullet v xomath] both got it right\n";
            return Winner::TIE_CORRECT;
        }
    }
    else if(xod > btd) {
        if(print) cout << "[bullet] wins. error: " << fixed << btd << " [xomath] error: " << xod << "\n";
        return Winner::WIN_BULLET;
    }
    else {
        if(print) cout << "[xomath] wins. error: " << fixed << xod << " [bullet] error: " << btd << "\n";
        return Winner::WIN_XOMATH;
    }
}

Winner CheckAgreement(float known, float xov, float btv, bool print = false) {
    float xod = Diff(known, xov);
    float btd = Diff(known, btv);
    if(Diff(xod, btd) <= FloatEpsilon) { // we agree
        if(xod > FloatEpsilon) {
            if(print) cout << "[bullet v xomath] both got it wrong. diff:" << fixed << xod << "\n";
            return Winner::TIE_INCORRECT;
        }
        else {
            if(print) cout << "[bullet v xomath] both got it right\n";
            return Winner::TIE_CORRECT;
        }
    }
    else if(xod > btd) {
        if(print) cout << "[bullet] wins. error: " << fixed << btd << " [xomath] error: " << xod << "\n";
        return Winner::WIN_BULLET;
    }
    else {
        if(print) cout << "[xomath] wins. error: " << fixed << xod << " [bullet] error: " << btd << "\n";
        return Winner::WIN_XOMATH;
    }
}

Winner CheckAgreement(bool known, bool xov, bool btv, bool print = false) {
    if(xov == btv) {
        if(xov != btv) {
            if(print) cout << "[bullet v xomath] both got it wrong.\n";
            return Winner::TIE_INCORRECT;
        }
        else {
            if(print) cout << "[bullet v xomath] both got it right\n";
            return Winner::TIE_CORRECT;
        }
    }
    else if(btv == known) {
        if(print) cout << "[bullet] wins.\n";
        return Winner::WIN_BULLET;
    }
    else {
        if(print) cout << "[xomath] wins.\n";
        return Winner::WIN_XOMATH;
    }
}

void GetRandomVecTypes(floatVec3& fv3, Vector3& xov, btVector3& btv) {
    float x = RandomRange(-10000.0f, 10000.0f);
    float y = RandomRange(-10000.0f, 10000.0f);
    float z = RandomRange(-10000.0f, 10000.0f);
    fv3 = {x, y, z};
    xov = {x, y, z};
    btv = {x, y, z};
}

void CompareBullet3() {
    const int Itterations = 100;

    int testNum = 0;
    
    int results[(int)Winner::COUNT];
    memset(results, 0, sizeof(int)*(int)Winner::COUNT);

    #define SCORE \
    cout << "Tie correct: " << results[(int)Winner::TIE_CORRECT] << "\n"; \
    cout << "Tie incorrect: " << results[(int)Winner::TIE_INCORRECT] << "\n"; \
    cout << "xo-math wins: " << results[(int)Winner::WIN_XOMATH] << "\n"; \
    cout << "Bullet wins: " << results[(int)Winner::WIN_BULLET] << "\n"; \
    memset(results, 0, sizeof(int)*(int)Winner::COUNT);


#define SIMPLE_TEST(name, op) \
    cout << "=========" name "=========\n"; \
    for (int i = 0; i < Itterations; i++) { \
        Vector3 xov1, xov2; \
        btVector3 btv1, btv2; \
        floatVec3 fvv1, fvv2; \
        GetRandomVecTypes(fvv1, xov1, btv1); \
        GetRandomVecTypes(fvv2, xov2, btv2); \
        results[(int)CheckAgreement(fvv1 op fvv2 ,xov1 op xov2, btv1 op btv2)]++; \
    } SCORE

#define AB_TEST(name, floatfunc, xofunc, btfunc, debug) \
    cout << "=========" name "=========\n"; \
    for (int i = 0; i < Itterations; i++) { \
        Vector3 xov1, xov2; \
        btVector3 btv1, btv2; \
        floatVec3 fvv1, fvv2; \
        GetRandomVecTypes(fvv1, xov1, btv1); \
        GetRandomVecTypes(fvv2, xov2, btv2); \
        results[(int)CheckAgreement(fvv1. floatfunc (fvv2) ,xov1. xofunc (xov2), btv1. btfunc (btv2), debug)]++; \
    } SCORE

    SIMPLE_TEST("addition", +);
    SIMPLE_TEST("subtraction", -);
    SIMPLE_TEST("multiplication", *);
    SIMPLE_TEST("division", /);
    SIMPLE_TEST("less than", <);
    SIMPLE_TEST("greater than", >);
    SIMPLE_TEST("less than or equal to", <=);
    SIMPLE_TEST("greater than or equal to", >=);

    AB_TEST("Dot product", Dot, Dot, dot, false);
    AB_TEST("Angle between", Angle, AngleRadians, angle, true);
}

int main() {
    SSE::UpdateControlWord();       // updates the thread-local state.
    SSE::SetDenormalsAreZero(true); // force all denormal values to 0
    SSE::SetFlushToZero(true);      // underflowing operations produce 0

    //xo_sse1::SSE::ThrowAllExceptions();
    SSE::ThrowNoExceptions();

    cout.precision(16);
    CompareBullet3();

    //SSE::GetAllMXCSRInfo(cout);
    cout << "\n" << XO_MATH_VERSION_TXT << "\n" << XO_MATH_COMPILER_INFO << endl;

#if defined(_MSC_VER)
    system("pause");
#endif

    return 0;
}