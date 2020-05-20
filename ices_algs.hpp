///////////////////////////////////////////////////////////////////////////////
// crossing_algs.hpp
//
// Algorithms that solve the iceberg avoiding problem.
//
// All of the TODO sections for this project reside in this file.
//
// This file builds on ices_types.hpp, so you should familiarize yourself
// with that file before working on this file.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cassert>
#include <iostream>

#include "ices_types.hpp"

namespace ices {
// Solve the iceberg avoiding problem for the given grid, using an exhaustive
// optimization algorithm.
//
// This algorithm is expected to run in exponential time, so the grid's
// width+height must be small enough to fit in a 64-bit int; this is enforced
// with an assertion.
//
// The grid must be non-empty.
unsigned int iceberg_avoiding_exhaustive(const grid& setting) {

  // grid must be non-empty.
  assert(setting.rows() > 0);
  assert(setting.columns() > 0);

  // Compute the path length, and check that it is legal.
  const size_t steps = setting.rows() + setting.columns() - 2;
  assert(steps < 64);

  unsigned int count_paths = 0;

  // TODO: implement the exhaustive optimization algorithm, then delete this
  // comment.

  long bits = 1;

  //populate bits which is the number of possible paths in total
  for (size_t i = 0; i < steps; i++) {
    bits *=2;
  }

  //iterate through each possible path to check if it's valid
  for (long i = 0; i < bits; i++) {
    long unsigned int curRow = 0, curCol = 0;

    //check each step in a path to see if this path is valid
    for (long unsigned int j = 0; j < steps; j++) {
      int bit = (i >> j) & 1;
      //bit == 1 means move right
      if (bit == 1){
        curRow++;
      }
      //bit == 0 means move down
      else{
        curCol++;
      }

      //if current row and col is not valid number, check another path
      if (!setting.is_row(curRow) || !setting.is_column(curCol)) break;

      //if current position is an iceburg, check another path
      if (setting.get(curRow, curCol) == CELL_ICEBERG)
      {
        curRow = 0;
        curCol = 0;
        break;
      }
    }

    //if reach the end of setting, means current path is valid, update total path count
    if (curRow == setting.rows() - 1 && curCol == setting.columns() - 1)
    {
      count_paths++;
    }
  }

  return count_paths;
}

// Solve the iceberg avoiding problem for the given grid, using a dynamic
// programming algorithm.
//
// The grid must be non-empty.
unsigned int iceberg_avoiding_dyn_prog(const grid& setting) {

  // grid must be non-empty.
  assert(setting.rows() > 0);
  assert(setting.columns() > 0);


  const int DIM=100;
  std::vector<std::vector<unsigned>> A(DIM, std::vector<unsigned>(DIM));

  A[0][0] = 1;

  // TODO: implement the dynamic programming algorithm, then delete this
  // comment.
  size_t r = setting.rows(), c = setting.columns();

  //outter loop iterates for rows
  for (size_t i = 0; i < r; i++)
  {
    //inner loop iterates for cols
    for (size_t j = 0; j < c; j++)
    {
      //skip position [0][0], as inited to 1
      if (i == 0 && j == 0) continue;

      //if current position is an iceberg, set A value to 0
      if (setting.get(i, j) == CELL_ICEBERG)
      {
        A[i][j] = 0;
        continue;
      }

      unsigned from_above = 0, from_left = 0;

      //check paths coming from above
      if (i > 0 && A[i-1][j] != 0)
      {
        from_above = A[i-1][j];
      }

      //check paths coming from left
      if (j > 0 && A[i][j-1] != 0)
      {
        from_left = A[i][j-1];
      }

      //update total possible paths to current position
      A[i][j] = from_left + from_above;
    }
  }

  return A[setting.rows()-1][setting.columns()-1];
}

}
