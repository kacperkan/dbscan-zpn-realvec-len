#pragma once

#include <math.h>
#include <stdlib.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

class VecXd {
   public:
    double* data;

    VecXd() : m_size(0) {}
    VecXd(int size) : m_size(size) {
        data = new double[size];
        for (int i = 0; i < size; ++i) {
            data[i] = 0.0;
        }
    }
    VecXd(const VecXd& other) {
        this->m_size = other.size();
        this->data = (double*)malloc(m_size * sizeof(double));
        memcpy(this->data, other.data, m_size * sizeof(double));
    }
    VecXd(std::initializer_list<double> list) {
        m_size = list.size();
        data = (double*)malloc(m_size * sizeof(double));
        std::copy(list.begin(), list.end(), data);
    }

    VecXd(std::vector<double> list) {
        m_size = list.size();
        this->data = new double[list.size()];
        std::copy(list.begin(), list.end(), data);
    }

    VecXd(double* data, int size) : data(data), m_size(size) {}
    ~VecXd() {
        if (size() > 0) delete[] data;
    }

    inline int size() const { return m_size; }

    VecXd& operator=(const VecXd& other) {
        if (this == &other) return *this;

        if (other.size() != m_size) {
            delete[] data;
            m_size = 0;
            data = nullptr;
            data = new double[other.size()];
            m_size = other.size();
        }
        std::copy(other.data, other.data + other.size(), data);
        return *this;
    }

    // vec2vec
    VecXd& operator+=(const VecXd& rhs) {
        for (int i = 0; i < this->size(); ++i) {
            this->data[i] = this->data[i] + rhs[i];
        }
        return *this;
    }

    friend VecXd operator+(VecXd lhs, const VecXd& rhs) {
        VecXd new_vec(lhs.size());
        for (int i = 0; i < lhs.size(); ++i) {
            new_vec[i] = lhs[i] + rhs[i];
        }
        return new_vec;
    }

    VecXd& operator-=(const VecXd& rhs) {
        for (int i = 0; i < this->size(); ++i) {
            this->data[i] = this->data[i] - rhs[i];
        }
        return *this;
    }

    friend VecXd operator-(VecXd lhs, const VecXd& rhs) {
        VecXd new_vec(lhs.size());
        for (int i = 0; i < lhs.size(); ++i) {
            new_vec.data[i] = lhs[i] - rhs[i];
        }
        return new_vec;
    }
    VecXd& operator*=(const VecXd& rhs) {
        for (int i = 0; i < this->size(); ++i) {
            this->data[i] = this->data[i] * rhs[i];
        }
        return *this;
    }

    friend VecXd operator*(VecXd lhs, const VecXd& rhs) {
        VecXd new_vec(lhs.size());
        for (int i = 0; i < lhs.size(); ++i) {
            new_vec.data[i] = lhs[i] * rhs[i];
        }
        return new_vec;
    }
    VecXd& operator/=(const VecXd& rhs) {
        for (int i = 0; i < this->size(); ++i) {
            this->data[i] = this->data[i] / rhs[i];
        }
        return *this;
    }

    VecXd max(const VecXd& rhs) {
        VecXd new_vec(this->size());
        for (int i = 0; i < new_vec.size(); ++i) {
            new_vec.data[i] =
                this->data[i] >= rhs[i] ? this->data[i] : rhs.data[i];
        }
        return new_vec;
    }

    void div_(const VecXd& rhs) {
        for (int i = 0; i < this->size(); ++i) {
            this->data[i] = this->data[i] / rhs[i];
        }
    }
    void mul_(const VecXd& rhs) {
        for (int i = 0; i < this->size(); ++i) {
            this->data[i] = this->data[i] * rhs[i];
        }
    }
    void add_(const VecXd& rhs) {
        for (int i = 0; i < this->size(); ++i) {
            this->data[i] = this->data[i] + rhs[i];
        }
    }
    void sub_(const VecXd& rhs) {
        for (int i = 0; i < this->size(); ++i) {
            this->data[i] = this->data[i] - rhs[i];
        }
    }

    friend VecXd operator/(VecXd lhs, const VecXd& rhs) {
        VecXd new_vec(lhs.size());
        for (int i = 0; i < lhs.size(); ++i) {
            new_vec.data[i] = lhs[i] / rhs[i];
        }
        return new_vec;
    }
    // vec2scalar
    VecXd& operator+=(const double rhs) {
        for (int i = 0; i < this->size(); ++i) {
            this->data[i] = this->data[i] + rhs;
        }
        return (*this);
    }

    friend VecXd operator+(VecXd lhs, const double rhs) {
        VecXd new_vec(lhs.size());
        for (int i = 0; i < lhs.size(); ++i) {
            new_vec.data[i] = lhs[i] + rhs;
        }
        return new_vec;
    }

    VecXd& operator-=(const double rhs) {
        for (int i = 0; i < this->size(); ++i) {
            this->data[i] = this->data[i] - rhs;
        }
        return (*this);
    }

    friend VecXd operator-(VecXd lhs, const double rhs) {
        VecXd new_vec(lhs.size());
        for (int i = 0; i < lhs.size(); ++i) {
            new_vec.data[i] = lhs[i] - rhs;
        }
        return new_vec;
    }
    VecXd& operator*=(const double rhs) {
        for (int i = 0; i < this->size(); ++i) {
            this->data[i] = this->data[i] * rhs;
        }
        return (*this);
    }

    friend VecXd operator*(VecXd lhs, const double rhs) {
        VecXd new_vec(lhs.size());
        for (int i = 0; i < lhs.size(); ++i) {
            new_vec.data[i] = lhs[i] * rhs;
        }
        return new_vec;
    }
    VecXd& operator/=(const double rhs) {
        for (int i = 0; i < this->size(); ++i) {
            this->data[i] = this->data[i] / rhs;
        }
        return (*this);
    }

    friend VecXd operator/(VecXd lhs, const double rhs) {
        VecXd new_vec(lhs.size());
        for (int i = 0; i < lhs.size(); ++i) {
            new_vec.data[i] = lhs[i] / rhs;
        }
        return new_vec;
    }

    void div_(const double rhs) {
        for (int i = 0; i < this->size(); ++i) {
            this->data[i] = this->data[i] / rhs;
        }
    }
    void mul_(const double rhs) {
        for (int i = 0; i < this->size(); ++i) {
            this->data[i] = this->data[i] * rhs;
        }
    }
    void add_(const double rhs) {
        for (int i = 0; i < this->size(); ++i) {
            this->data[i] = this->data[i] + rhs;
        }
    }
    void sub_(const double rhs) {
        for (int i = 0; i < this->size(); ++i) {
            this->data[i] = this->data[i] - rhs;
        }
    }

    // additional
    double dot(const VecXd& rhs) {
        double result = 0;
        for (int i = 0; i < rhs.size(); ++i) {
            result += this->data[i] * rhs[i];
        }
        return result;
    }

    VecXd pow(const double exponent) {
        VecXd new_vec(*this);
        for (int i = 0; i < new_vec.size(); ++i) {
            new_vec[i] = std::pow(new_vec[i], exponent);
        }
        return new_vec;
    }
    VecXd sqrt() {
        VecXd new_vec(*this);
        for (int i = 0; i < new_vec.size(); ++i) {
            new_vec[i] = std::sqrt(new_vec[i]);
        }
        return new_vec;
    }

    double& operator[](std::size_t idx) const {
        if (idx < 0 || idx >= size())
            throw std::invalid_argument("Index out of boundary");
        return data[idx];
    }

    double sum() {
        double total = 0.0;
        for (int i = 0; i < size(); ++i) {
            total += data[i];
        }
        return total;
    }

    double mean() { return sum() / size(); }

    double length() { return std::sqrt(this->pow(2.0).sum()); }

    friend std::ostream& operator<<(std::ostream& os, const VecXd& vec) {
        os << "Vec[" << vec.size() << "](";
        for (int i = 0; i < vec.size(); ++i) {
            os << vec[i];
            if (i != vec.size() - 1) {
                os << ", ";
            }
        }
        os << ")";
        return os;
    }

   private:
    int m_size = 0;
};