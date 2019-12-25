//
// Created by Деев Роман on 22.12.2019.
//
#ifndef FRACTAL_COMPLEX_H
#define FRACTAL_COMPLEX_H

template<typename T>
class complex {
public:
    complex(T real, T imag) : real(real), imag(imag) {}

    T real, imag;

    complex operator*(complex &oth) const {
        return complex<T>(real * oth.real - imag * oth.imag,
                          imag * oth.real + real * oth.imag);
    }

    complex operator+(complex &oth) const {
        return complex<T>(real + oth.real, imag + oth.imag);
    }

    T norm() {
        return real * real + imag * imag;
    }
};

#endif //FRACTAL_COMPLEX_H
