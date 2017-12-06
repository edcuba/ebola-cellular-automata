#ifndef MATRIX_H
#define MATRIX_H

#include "cell.h"
#include <string>
#include <vector>

/**
 * Wrapper for a vector with extended boundaries
 */
template <typename T>
class CAVector
{
  protected:
    std::vector<T> data;
    typedef typename std::vector<T>::size_type size_type;

  public:
    T &operator[] (int index)
    {
        if (index == -1) {
            index = size () - 1;
        }
        return data[index % size ()];
    }
    size_type size () const { return data.size (); }
    typename std::vector<T>::iterator begin () { return data.begin (); }
    typename std::vector<T>::iterator end () { return data.end (); }
    virtual std::string dump () = 0;
};

/**
 * An array of Cells with extended boundaries
 */
class Row : public CAVector<Cell>
{
  public:
    Row (size_type columns) { data.resize (columns); }
    std::string dump ()
    {
        std::string s;
        for (auto v : *this) {
            s += std::to_string (v);
        }
        return s;
    }
};

/**
 * An array of Rows with extended boundaries
 */
class Matrix : public CAVector<Row>
{
  public:
    Matrix (size_type rows, size_type columns)
    {
        for (size_type i = 0; i < rows; ++i) {
            data.push_back (Row (columns));
        }
    }
    std::string dump ()
    {
        std::string s;
        for (auto &r : *this) {
            s += r.dump () + "\n";
        }
        return s;
    }
};

#endif
