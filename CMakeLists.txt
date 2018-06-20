PROJECT(ls CXX)
SET(CMAKE_CXX_COMPILER)
SET(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
SET (LSSRC
  domain.cc
  random.cc
  srandom.cc
  solis.cc
  simplex.cc
  cmaeshan.cc
  origcmaes.cc
  problemcec2014.cc
  cec2014_func.cpp
  problem.cc
  example.cc
  CDOA.cc
  buffer.cpp
)

ADD_LIBRARY(localsearch SHARED ${LSSRC})
TARGET_LINK_LIBRARIES(localsearch)

ADD_LIBRARY(CDOA SHARED ${LSSRC})
TARGET_LINK_LIBRARIES(CDOA)

ADD_EXECUTABLE(example_ls example.cc)
TARGET_LINK_LIBRARIES(example_ls localsearch)

ADD_EXECUTABLE(main_CDOA main_CDOA.cc)
TARGET_LINK_LIBRARIES(main_CDOA CDOA)
