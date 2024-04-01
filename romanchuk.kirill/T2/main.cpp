﻿#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>

#include "DataStruct.h"
#include "InputFormatters.h"


int main()
{
  std::vector< DataStruct > dataStructVector;
  while (!std::cin.eof())
  {
    std::copy(std::istream_iterator< DataStruct >(std::cin),
      std::istream_iterator< DataStruct >(),
      std::back_inserter(dataStructVector));

    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::sort(dataStructVector.begin(), dataStructVector.end());

  std::copy(dataStructVector.begin(),
    dataStructVector.end(),
    std::ostream_iterator< DataStruct >(std::cout, "\n"));

  return 0;
}
