#include <algorithm>

#include "Polygon.h"

std::istream& operator>>(std::istream& in, Polygon& polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t numPoints;
  in >> numPoints;
  if (numPoints < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > tmp;
  std::generate_n(std::back_inserter(tmp), numPoints, [&in]() {
    Point tmpPoint;
    in >> tmpPoint;
    return tmpPoint;
  });
  if (!in.fail())
  {
    polygon.points = tmp;
    polygon.points.shrink_to_fit();
  }
  return in;
}
