#include "Matrix.hh"

Matrix::Matrix() {
    this->content[0] = 1;
    this->content[1] = 0;
    this->content[2] = 0;
    this->content[3] = 0;
    this->content[4] = 1;
    this->content[5] = 0;
    this->content[6] = 0;
    this->content[7] = 0;
    this->content[8] = 1;
    this->content[9] = 0;
    this->content[10] = 0;
    this->content[11] = 0;
    this->content[12] = 1;
    this->content[13] = 0;
    this->content[14] = 0;
    this->content[15] = 0;
}

Matrix::Matrix(
    float c00,
    float c01,
    float c02,
    float c10,
    float c11,
    float c12,
    float c20,
    float c21,
    float c22
) {
    this->content[0] = c00;
    this->content[1] = c01;
    this->content[2] = c02;
    this->content[3] = 0;
    this->content[4] = c10;
    this->content[5] = c11;
    this->content[6] = c12;
    this->content[7] = 0;
    this->content[8] = c20;
    this->content[9] = c21;
    this->content[10] = c22;
    this->content[11] = 0;
    this->content[12] = 0;
    this->content[13] = 0;
    this->content[14] = 0;
    this->content[15] = 1;
}

Matrix &Matrix::combine(Matrix const &other) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            this->content[i * 4 + j] =
                this->content[i * 4] * other.content[j] +
                this->content[i * 4 + 1] * other.content[j + 4] +
                this->content[i * 4 + 2] * other.content[j + 8] +
                this->content[i * 4 + 3] * other.content[j + 12];
        }
    }
    return *this;
}

std::ostream &operator<<(std::ostream &out, Matrix const &matrix) {
    for (int i = 0; i < 4; i++) {
        out << "[ ";
        for (int j = 0; j < 4; j++) {
            out << matrix.content[i * 4 + j] << " ";
        }
        out << "]";
    }
    return out;
}
