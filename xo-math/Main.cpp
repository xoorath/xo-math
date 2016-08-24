#include <iostream>
#include <cmath>
using namespace std;

//#define XO_NO_INVERSE_DIVISION
#include "../xo-math.h"
using namespace xo::math;

#include "Reference/bullet3-master/src/LinearMath/btVector3.h"

#define VCL_NAMESPACE VectorClasses
#include "Reference/vectorclass/special/vector3d.h"

struct Closeness {
    float XomathToBullet;
    float XomathToVectorClasses;
    float BulletToVectorClasses;
    Closeness& operator += (const Closeness& o) {
        XomathToBullet += o.XomathToBullet;
        XomathToVectorClasses += o.XomathToVectorClasses;
        BulletToVectorClasses += o.BulletToVectorClasses;
        return *this;
    }
};

float Diff(const Vector3& xov, const btVector3& btv) {
    float diffx = Abs(btv.getX() - xov.x);
    float diffy = Abs(btv.getY() - xov.y);
    float diffz = Abs(btv.getZ() - xov.z);
    return diffx + diffy + diffz;
}

float Diff(const btVector3& btv, const VectorClasses::Vec3f& vcv) {
    float diffx = Abs(btv.getX() - vcv[0]);
    float diffy = Abs(btv.getY() - vcv[1]);
    float diffz = Abs(btv.getZ() - vcv[2]);
    return diffx + diffy + diffz;
}

float Diff(const Vector3& xov, const VectorClasses::Vec3f& vcv) {
    float diffx = Abs(vcv[0] - xov.x);
    float diffy = Abs(vcv[1] - xov.y);
    float diffz = Abs(vcv[2] - xov.z);
    return diffx + diffy + diffz;
}


float Diff(float left, float right) {
    return Abs(left-right);
}

void PrintVec(const Vector3& xov) {
    printf("(%f, %f, %f)", xov[0], xov[1], xov[2]);
}

void PrintVec(const btVector3& btv) {
    printf("(%f, %f, %f)", btv[0], btv[1], btv[2]);
}

void PrintVec(const VectorClasses::Vec3f& vcv) {
    printf("(%f, %f, %f)", vcv[0], vcv[1], vcv[2]);
}

float maxf(float a, float b) {
    return a > b ? a : b;
}

float maxf(float a, float b, float c) {
    return maxf(maxf(a, b), maxf(b, c));
}

float maxf(float a, float b, float c, float d) {
    return maxf(maxf(a, b), maxf(c, d));
}

Closeness CheckAgreement(const Vector3& xov, const btVector3& btv, const VectorClasses::Vec3f& vcv, int print = 0) {
    float xomathToBullet = Diff(xov, btv);
    float xomathToVectorClasses = Diff(xov, vcv);
    float bulletToVectorClasses = Diff(btv, vcv);

    float maxdiff = maxf(xomathToBullet, xomathToVectorClasses, bulletToVectorClasses);
    
    if (print > 0) {

        bool shouldprint = false;
        if (maxdiff > 1.0f) {
            cout << "[very different]    xo:";
            shouldprint = true;
        }
        else if (maxdiff > 0.01f) {
            cout << "[pretty far]        xo:";
            shouldprint = true;
        }
        else if (maxdiff > 0.0001f && print > 1) {
            cout << "[not very close]    xo:";
            shouldprint = true;
        }
        else if (maxdiff > 0.000001f && print > 2) {
            cout << "[pretty darn close] xo:";
            shouldprint = true;
        }
        else if (maxdiff > 0.0000001f && print > 3) {
            cout << "[essentially equal] xo:";
            shouldprint = true;
        }
        else if(print > 4) {
            cout << "[equal]             xo:";
            shouldprint = true;
        }
        if (shouldprint) {
            PrintVec(xov);
            cout << " bt:";
            PrintVec(btv);
            cout << " vc:";
            PrintVec(vcv);
            cout << " diff: " << maxdiff << "\n";
        }
    }
    return{ xomathToBullet, xomathToVectorClasses, bulletToVectorClasses };
}

Closeness CheckAgreement(float xov, float btv, float vcv, int print = 0) {
    float xomathToBullet = Diff(xov, btv);
    float xomathToVectorClasses = Diff(xov, vcv);
    float bulletToVectorClasses = Diff(btv, vcv);

    float maxdiff = maxf(xomathToBullet, xomathToVectorClasses, bulletToVectorClasses);

    if (print > 0) {

        bool shouldprint = false;
        if (maxdiff > 1.0f) {
            cout << "[very different]    xo:";
            shouldprint = true;
        }
        else if (maxdiff > 0.01f) {
            cout << "[pretty far]        xo:";
            shouldprint = true;
        }
        else if (maxdiff > 0.0001f && print > 1) {
            cout << "[not very close]    xo:";
            shouldprint = true;
        }
        else if (maxdiff > 0.000001f && print > 2) {
            cout << "[pretty darn close] xo:";
            shouldprint = true;
        }
        else if (maxdiff > 0.0000001f && print > 3) {
            cout << "[essentially equal] xo:";
            shouldprint = true;
        }
        else if (print > 4) {
            cout << "[equal]             xo:";
            shouldprint = true;
        }
        if (shouldprint) {
            PrintVec(xov);
            cout << " bt:";
            PrintVec(btv);
            cout << " vc:";
            PrintVec(vcv);
            cout << " diff: " << maxdiff << "\n";
        }
    }
    return{ xomathToBullet, xomathToVectorClasses, bulletToVectorClasses };
}

Closeness CheckAgreement(bool xov, bool btv, bool vcv, int print = 0) {
    if (print > 0) {
        if ((xov && btv && vcv) || !(xov && btv && vcv)) {
            cout << "[not equal]         xo:" << xov << " bt:" << btv << " vc: " << vcv << "\n";
        }
        else if (print > 1) {
            cout << "[equal]             xo:" << xov << " bt:" << btv << " vc: " << vcv << "\n";
        }
    }
    return{ (xov == btv ? 0.0f : 100.0f), (xov == vcv ? 0.0f : 100.0f), (vcv == btv ? 0.0f : 100.0f) };
}

Closeness CheckAgreement(bool xov, bool btv, const VectorClasses::Vec4fb& vvcv, int print = 0) {
    bool vcv = vvcv[0];
    if (print > 0) {
        if ((xov && btv && vcv) || !(xov && btv && vcv)) {
            cout << "[not equal]         xo:" << xov << " bt:" << btv << " vc: " << vcv << "\n";
        }
        else if (print > 1) {
            cout << "[equal]             xo:" << xov << " bt:" << btv << " vc: " << vcv << "\n";
        }
    }
    Closeness c;
    c.XomathToBullet = xov == btv ? 0.0f : 100.0f;
    c.XomathToVectorClasses = xov == vcv ? 0.0f : 100.0f;
    c.BulletToVectorClasses = vcv == btv ? 0.0f : 100.0f;
    return c;
}

void GetRandomVecTypes(Vector3& xov, btVector3& btv, VectorClasses::Vec3f& vcv) {
    float x = RandomRange(-10.0f, 10.0f);
    float y = RandomRange(-10.0f, 10.0f);
    float z = RandomRange(-10.0f, 10.0f);
    xov = { x, y, z };
    btv = { x, y, z };
    vcv = { x, y, z };
}

void CompareBullet3() {
    const int Itterations = 100;

    int testNum = 0;
    
    Closeness c;

#define SCORE \
    cout << "xo-math vs. bullet average closeness: " << fixed << c.XomathToBullet / (float)Itterations << "\n"; \
    cout << "xo-math vs. vectorclasses average closeness: " << fixed << c.XomathToVectorClasses / (float)Itterations << "\n"; \
    cout << "bullet vs. vectorclasses average closeness: " << fixed << c.BulletToVectorClasses / (float)Itterations << "\n"; \
    cout << endl;

#define TEST_SETUP(name, ...) \
    cout << "=========" name "=========\n"; \
    memset(&c, 0, sizeof(Closeness)); \
    for (int i = 0; i < Itterations; i++) { \
        Vector3 xov1, xov2; \
        btVector3 btv1, btv2; \
        VectorClasses::Vec3f vcv1, vcv2;\
        GetRandomVecTypes(xov1, btv1, vcv1); \
        GetRandomVecTypes(xov2, btv2, vcv2); \
        auto result = CheckAgreement(__VA_ARGS__); \
        c += result; \
    } SCORE

#define TEST_SETUP_F(name, ...) \
    cout << "=========" name "=========\n"; \
    memset(&c, 0, sizeof(Closeness)); \
    for (int i = 0; i < Itterations; i++) { \
        Vector3 xov1, xov2; \
        btVector3 btv1, btv2; \
        VectorClasses::Vec3f vcv1, vcv2;\
        GetRandomVecTypes(xov1, btv1, vcv1); \
        GetRandomVecTypes(xov2, btv2, vcv2); \
        float f = RandomRange(-2.0f, 2.0f); \
        auto result = CheckAgreement(__VA_ARGS__); \
        c += result; \
    } SCORE

#define SIMPLE_TEST(name, op, debug)            TEST_SETUP(name, xov1 op xov2, btv1 op btv2, vcv1 op vcv2, debug)
#define AB_TEST(name, xofunc, btfunc, vcfunc, debug)    TEST_SETUP(name, xov1. xofunc (xov2), btv1. btfunc (btv2), vcfunc(vcv1, vcv2), debug)
#define AB_TEST_F(name, xofunc, btfunc, vcfunc, debug)  TEST_SETUP_F(name, xov1. xofunc (xov2, f), btv1. btfunc (btv2, f), vcfunc (vcv1, vcv2, f), debug)

    SIMPLE_TEST("addition",                 +,  0);
    SIMPLE_TEST("subtraction",              -,  0);
    SIMPLE_TEST("multiplication",           *,  0);
    SIMPLE_TEST("division",                 /,  0);
    SIMPLE_TEST("less than",                <,  0);
    SIMPLE_TEST("greater than",             >,  0);
    SIMPLE_TEST("less than or equal to",    <=, 0);
    SIMPLE_TEST("greater than or equal to", >=, 0);

    AB_TEST("Dot product",      Dot,                dot,    dot_product,   0);
    //AB_TEST("Angle between",    AngleRadians,       angle,      0);
    AB_TEST("Cross product",    Cross,              cross,  cross_product, 0);
    //AB_TEST("Distance",         Distance,           distance,   0);
    //AB_TEST("Distance",         DistanceSquared,    distance2,  0);
    //AB_TEST_F("Lerp",           Lerp,               lerp,       0);
    //AB_TEST_F("Rotate",         RotateRadians,      rotate,     0);


#undef SIMPLE_TEST
#undef AB_TEST
}

int main() {
#if defined(XO_SSE)
    SSE::UpdateControlWord();       // updates the thread-local state.
    SSE::SetDenormalsAreZero(true); // force all denormal values to 0
    SSE::SetFlushToZero(true);      // underflowing operations produce 0

    //xo_sse1::SSE::ThrowAllExceptions();
    SSE::ThrowNoExceptions();
#endif

    cout.precision(8);
    CompareBullet3();

    //SSE::GetAllMXCSRInfo(cout);
    cout << "\n" << XO_MATH_VERSION_TXT << "\n" << XO_MATH_COMPILER_INFO << endl;

#if defined(_MSC_VER)
    system("pause");
#endif

    return 0;
}