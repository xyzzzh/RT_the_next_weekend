//
// Created by Chamelz on 2022/9/25.
//

#ifndef RAY_TRACING_IN_ONE_WEEKEND_PERLIN_H
#define RAY_TRACING_IN_ONE_WEEKEND_PERLIN_H
#include "rtweekend.h"

class perlin{
public:
    perlin(){
        ranfloat = new double[point_count];
        for(int i=0;i<point_count;i++){
            ranfloat[i] = random_double();
        }

        permx = perlin_generate_perm();
        permy = perlin_generate_perm();
        permz = perlin_generate_perm();
    }
    ~perlin(){
        delete[] ranfloat;
        delete[] permx;
        delete[] permy;
        delete[] permz;
    }

    double nosie(const point3& p) const {
        // auto i = static_cast<int>(4*p.x()) & 255;
        // auto j = static_cast<int>(4*p.y()) & 255;
        // auto k = static_cast<int>(4*p.z()) & 255;
        // return ranfloat[permx[i]^permy[j]^permz[k]];
        auto u = p.x() - floor(p.x());
        auto v = p.y() - floor(p.y());
        auto w = p.z() - floor(p.z());
        u = u*u*(3-2*u);
        v = v*v*(3-2*v);
        w = w*w*(3-2*w);

        auto i = static_cast<int>(floor(p.x()));
        auto j = static_cast<int>(floor(p.y()));
        auto k = static_cast<int>(floor(p.z()));
        double c[2][2][2];

        for (int di=0; di < 2; di++)
            for (int dj=0; dj < 2; dj++)
                for (int dk=0; dk < 2; dk++)
                    c[di][dj][dk] = ranfloat[
                            permx[(i+di) & 255] ^
                            permy[(j+dj) & 255] ^
                            permz[(k+dk) & 255]
                    ];

        return trilinear_interp(c, u, v, w);
    }

private:
    static const int point_count = 256;
    double* ranfloat;
    int* permx;
    int* permy;
    int* permz;

    static int* perlin_generate_perm(){
        auto p = new int[point_count];

        for(int i=0;i<perlin::point_count;i++){
            p[i] = i;
        }
        permute(p, point_count);
        return p;
    }
    static void permute(int* p, int n){
        for(int i=n-1;i>0;i--){
            int target = random_int(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }
    static double trilinear_interp(double c[2][2][2], double u, double v, double w) {
        auto accum = 0.0;
        for (int i=0; i < 2; i++)
            for (int j=0; j < 2; j++)
                for (int k=0; k < 2; k++)
                    accum += (i*u + (1-i)*(1-u))*
                             (j*v + (1-j)*(1-v))*
                             (k*w + (1-k)*(1-w))*c[i][j][k];

        return accum;
    }
};
#endif //RAY_TRACING_IN_ONE_WEEKEND_PERLIN_H
