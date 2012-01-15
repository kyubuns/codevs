PROJECT(codevs)
ADD_EXECUTABLE(codevs.out src/main.cpp src/Loader.cpp src/Solver.cpp src/Task.cpp src/Simulator.cpp src/OldSolver.cpp src/RandSolver.cpp src/Logger.cpp)
target_link_libraries(codevs.out boost_thread-mt)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "-std=c++0x -stdlib=libc++ -O3")

