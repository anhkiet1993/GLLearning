#ifndef __MATRIX_H__
#define __MATRIX_H__
//#include <array>
#include <assert.h>
#include <iostream>

template <typename T>
class Matrix
{
public:
	inline Matrix(unsigned int _w, unsigned int _h);
	inline Matrix(unsigned int _w, unsigned int _h, T *input);
	inline Matrix();
	inline Matrix(const Matrix<T> &m);

	inline ~Matrix();

	inline const Matrix<T> operator=(const Matrix<T>& m0);
	inline T* operator[] (unsigned int idx);
	inline const Matrix<T> operator*(const Matrix<T>& m0);
	inline void identity();
private:
	T **m_data;
	unsigned int m_width, m_height;
};

template <typename T>
inline Matrix<T>::Matrix(unsigned int _w, unsigned int _h) : m_width(_w), m_height(_h)
{
	m_data = new T*[m_height];
	for (int i = 0; i < m_height; i++)
	{
		m_data[i] = new T[m_width];
	}
}

template <typename T>
inline Matrix<T>::Matrix(unsigned int _w, unsigned int _h, T *input) : m_width(_w), m_height(_h)
{
	m_data = new T*[m_height];

	for (int i = 0; i < m_height; i++)
	{
		m_data[i] = new T[m_width];
		for (int j = 0; j < m_width; j++)
		{
			m_data[i][j] = input[i*m_width + j];
		}
	}
}

template <typename T>
inline Matrix<T>::Matrix()
{

}
template <typename T>
inline Matrix<T>::Matrix(const Matrix<T> &m)
{
	m_width = m.m_width;
	m_height = m.m_height;

	m_data = new T*[m_height];

	for (int i = 0; i < m_height; i++)
	{
		m_data[i] = new T[m_width];
		for (int j = 0; j < m_width; j++)
		{
			m_data[i][j] = m.m_data[i][j];
		}
	}
}

template <typename T>
inline Matrix<T>::~Matrix()
{
	for (int i = 0; i < m_height; i++)
	{
		delete []m_data[i];
	}
	delete []m_data;
}

template <typename T>
inline const Matrix<T> Matrix<T>::operator=(const Matrix<T>& m0)
{
	m_width = m0.m_width;
	m_height = m0.m_height;

	m_data = new T*[m_height];

	for (int i = 0; i < m_height; i++)
	{
		m_data[i] = new T[m_width];

		for (int j = 0; j < m_width; j++)
		{
			m_data[i][j] = m0.m_data[i][j];
		}
	}
	return *this;
}

template <typename T>
inline T* Matrix<T>::operator[] (unsigned int index)
{
	assert(index < m_height);
	return m_data[index];
}

template <typename T>
inline const Matrix<T> Matrix<T>::operator*(const Matrix<T>& m0)
{
	assert(m_width == m0.m_height);

	Matrix<T> r_matrix(m0.m_width, m_height);

	for (int i = 0; i < r_matrix.m_height; i++)
	{
		for (int j = 0; j < r_matrix.m_width; j++)
		{
			r_matrix[i][j] = 0;
			for (int k = 0; k < m_width; k++) 
				r_matrix[i][j] += m_data[i][k] * m0.m_data[k][j];
		}
	}

	return r_matrix;
}

template <typename T>
inline void Matrix<T>::identity()
{
	assert(m_width == m_height);
	for (int i = 0; i < m_width; i++)
	{
		m_data[i][i] = T(1);
	}
}



#endif // !__MATRIX_H_