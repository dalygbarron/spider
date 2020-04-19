#ifndef MATRIX_H
#define MATRIX_H

#include "spdlog/ostr.h"
#include <iostream>

/**
 * A 4x4 matrix because the sfml classes aint up to the job.
 */
class Matrix {
    public:
        float content[16];

        /**
         * Creates an identity matrix.
         */
        Matrix();

        /**
         * Creates a matrix where the top left 3x3 matrix is filled with the
         * given values and the rest are 0 except for the bottom right one
         * which is 1.
         * @param c00 is the first coefficient of the first row.
         * @param c01 is the second coefficient of the first row.
         * @param c02 is the third coefficient of the first row.
         * @param c10 is the first coefficient of the second row.
         * @param c11 is the second coefficient of the second row.
         * @param c12 is the third coefficient of the second row.
         * @param c20 is the first coefficient of the third row.
         * @param c21 is the second coefficient of the third row.
         * @param c22 is the third coefficient of the third row.
         */
        Matrix(
            float c00,
            float c01,
            float c02,
            float c10,
            float c11,
            float c12,
            float c20,
            float c21,
            float c22
        );

        /**
         * Combines this matrix with another and edits this one as a result.
         * @param other is the other matrix.
         * @return this matrix so you can chain calls.
         */
        Matrix &combine(Matrix const &other);

        /**
         * Stream operator for writing down matrix.
         * @param out is the out stream.
         * @param matrix is the matrix to write.
         * @return the out stream.
         */
        friend std::ostream &operator<<(
            std::ostream &out,
            Matrix const &matrix
        );
};

#endif
