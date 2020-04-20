#include "Matrix.hh"

Matrix::Matrix() {
    this->content[0][0] = 1;
    this->content[0][1] = 0;
    this->content[0][2] = 0;
    this->content[0][3] = 0;
    this->content[1][0] = 0;
    this->content[1][1] = 1;
    this->content[1][2] = 0;
    this->content[1][3] = 0;
    this->content[2][0] = 0;
    this->content[2][1] = 0;
    this->content[2][2] = 1;
    this->content[2][3] = 0;
    this->content[3][0] = 0;
    this->content[3][1] = 0;
    this->content[3][2] = 0;
    this->content[3][3] = 1;
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
    this->content[0][0] = c00;
    this->content[0][1] = c01;
    this->content[0][2] = c02;
    this->content[0][3] = 0;
    this->content[1][0] = c10;
    this->content[1][1] = c11;
    this->content[1][2] = c12;
    this->content[1][3] = 0;
    this->content[2][0] = c20;
    this->content[2][1] = c21;
    this->content[2][2] = c22;
    this->content[2][3] = 0;
    this->content[3][0] = 0;
    this->content[3][1] = 0;
    this->content[3][2] = 0;
    this->content[3][3] = 1;
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
    float c22,
    float c30,
    float c31,
    float c32
) {
    this->content[0][0] = c00;
    this->content[0][1] = c01;
    this->content[0][2] = c02;
    this->content[0][3] = 0;
    this->content[1][0] = c10;
    this->content[1][1] = c11;
    this->content[1][2] = c12;
    this->content[1][3] = 0;
    this->content[2][0] = c20;
    this->content[2][1] = c21;
    this->content[2][2] = c22;
    this->content[2][3] = 0;
    this->content[3][0] = c30;
    this->content[3][1] = c31;
    this->content[3][2] = c32;
    this->content[3][3] = 1;
}

Matrix Matrix::combine(Matrix const &other) const {
    Matrix result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.content[i][j] =
                this->content[i][0] * other.content[0][j] +
                this->content[i][1] * other.content[1][j] +
                this->content[i][2] * other.content[2][j] +
                this->content[i][3] * other.content[3][j];
        }
    }
    return result;
}

std::ostream &operator<<(std::ostream &out, Matrix const &matrix) {
    for (int i = 0; i < 4; i++) {
        out << "[ ";
        for (int j = 0; j < 4; j++) {
            out << matrix.content[i][j] << " ";
        }
        out << "]";
    }
    return out;
}
