#pragma once

#include <Eigen/Core>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <random>

#include "util/forSolvers.hpp"
#include "util/grid.hpp"
#include "util/hex.hpp"
#include "util/problem.hpp"

void addHist2(int loopCnt, const int LOOP_CNT, const bool measureTime, Grid& grid,
              std::vector<Eigen::VectorXf>& positions) {
  if (measureTime) return;
  if (loopCnt % (LOOP_CNT / 20) == 0) positions.push_back(grid.toPosition());
}

std::vector<Eigen::VectorXf> solve_init2(const Problem& problem, const bool measureTime,
                                         const int seed) {
  auto t0 = std::chrono::high_resolution_clock::now();

  Grid grid(problem.n, problem.k);
  std::vector<Eigen::VectorXf> positions = {grid.toPosition()};

  std::mt19937 gen(seed);
  std::uniform_int_distribution<int> dist(0, problem.n - 1);
  const int LOOP_CNT = 300;
  for (int loopCnt = 0; loopCnt < LOOP_CNT; loopCnt++) {
    Eigen::VectorXf newPos(2 * problem.n);

    for (size_t i = 0; i < problem.n; i++) {
      // calculate gradient and Hessian
      double gx = 0.0, gy = 0.0;
      double hxx = 0.0, hxy = 0.0, hyy = 0.0;
      for (auto [j, w] : problem.adj[i]) {
        int dq = grid.points[i].q - grid.points[j].q;
        int dr = grid.points[i].r - grid.points[j].r;
        grid.calc_grad_hess(dq, dr, w, gx, gy, hxx, hxy, hyy);
      }

      // compute Newton's direction (Hess^{-1} @ (-grad)), if possible
      auto [x, y] = grid.hex2xy(i);
      auto [dx, dy] = computeDxDy(gx, gy, hxx, hxy, hyy, problem.k);
      newPos[2 * i] = x + dx;
      newPos[2 * i + 1] = y + dy;
    }

    if (loopCnt % (LOOP_CNT / 20) == 0) positions.push_back(newPos);

    // update
    bool success = grid.updateToNewPos(newPos, loopCnt);
    if (!success) {
      std::cerr << "Early stop (iter: " << loopCnt << ")" << std::endl;
      break;
    }

    addHist2(loopCnt, LOOP_CNT, measureTime, grid, positions);
  }
  positions.push_back(grid.toPosition());

  auto t1 = std::chrono::high_resolution_clock::now();
  std::cout << "Init Elapsed time: " << std::chrono::duration<double>(t1 - t0).count()
            << " seconds\n";

  return positions;
}
