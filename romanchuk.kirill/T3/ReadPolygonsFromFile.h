#ifndef READPOLYGONSFROMFILE_H
#define READPOLYGONSFROMFILE_H

#include <fstream>
#include <iterator>
#include <limits>

#include "Polygon.h"

void cleanIStream(std::istream& in)
{
  in.clear();
  in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
}

std::vector< Polygon > readPolygonsFromFile(std::string pathToFile)
{
  std::ifstream file(pathToFile);
  if (!file.is_open())
  {
    throw std::runtime_error("Error reading the file");
  }
  std::vector< Polygon > polygons;
  while (!file.eof())
  {
    std::copy(
      std::istream_iterator< Polygon >{ file }, std::istream_iterator< Polygon >{}, std::back_inserter(polygons)
    );
    if (file.fail() && !file.eof())
    {
      cleanIStream(file);
    }
  }
  file.close();
  polygons.shrink_to_fit();
  return polygons;
}

#endif // !READPOLYGONSFROMFILE_H
