#include "Commands.h"

#include <string>

#include <cmath>
#include <functional>
#include <numeric>
#include <algorithm>
#include <iterator>

void cmd::area(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::string option;
  in >> option;
  std::function< double(const Polygon&) > resultFuncForArea;
  if (option == "EVEN")
  {
    resultFuncForArea = [](const Polygon& polygon) -> double
      {
        return polygon.points.size() % 2 != 0 ? cmd::subcmd::getPolygonArea(polygon) : 0.0;
      };
  }
  else if (option == "ODD")
  {
    resultFuncForArea = [](const Polygon& polygon) -> double
      {
        return polygon.points.size() % 2 == 0 ? cmd::subcmd::getPolygonArea(polygon) : 0.0;
      };
  }
  else if (option == "MEAN")
  {
    if (polygons.size() == 0)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    resultFuncForArea = [&polygons](const Polygon& polygon) -> double
      {
        return cmd::subcmd::getPolygonArea(polygon) / polygons.size();
      };
  }
  else
  {
    size_t numVertexes;
    try
    {
      numVertexes = std::stoull(option);
      if (numVertexes < 3)
      {
        throw std::invalid_argument("");
      }
      resultFuncForArea = [&numVertexes](const Polygon& polygon) -> double
        {
          return polygon.points.size() == numVertexes ? cmd::subcmd::getPolygonArea(polygon) : 0.0;
        };
    }
    catch (const std::invalid_argument&)
    {
      std::cout << "<INVALID COMMAND>";
    }
  }
  out << std::accumulate
  (
    polygons.begin(), polygons.end(), 0.0,
    [&resultFuncForArea](double sum, const Polygon& polygon) -> double
    {
      return sum + resultFuncForArea(polygon);
    }
  );
}

void cmd::min(const std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
  std::string option;
  in >> option;
  if (option == "AREA")
  {
    std::cout << "OUTPUT: MIN AREA\n";
    out << cmd::subcmd::getMinPolygonArea(polygons);
  }
  else if (option == "VERTEXES")
  {
    std::cout << "OUTPUT: MIN VERTEXES\n";
    out << cmd::subcmd::getMinPolygonVertexes(polygons);
  }
  else
  {
    std::cout << "<INVALID COMMAND>";
  }
}

void cmd::max(const std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
  std::string option;
  in >> option;
  std::function< double(const Polygon&) > resultFuncForMax;
  if (option == "AREA")
  {
    std::cout << "OUTPUT: MAX AREA\n";
    out << cmd::subcmd::getMaxPolygonArea(polygons);
  }
  else if (option == "VERTEXES")
  {
    std::cout << "OUTPUT: MAX VERTEXES\n";
    out << cmd::subcmd::getMaxPolygonVertexes(polygons);
  }
  else
  {
    std::cout << "<INVALID COMMAND>";
  }
}

void cmd::count(const std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
  std::string option;
  in >> option;
  if (option == "EVEN")
  {
    std::cout << "OUTPUT: COUNT EVEN";
  }
  else if (option == "ODD")
  {
    std::cout << "OUTPUT: COUNT ODD";
  }
  else if (option == "MEAN")
  {
    std::cout << "OUTPUT: COUNT MEAN";
  }
  else
  {
    size_t numVertexes;
    try
    {
      numVertexes = std::stoull(option);

      if (numVertexes < 3)
      {
        throw std::invalid_argument("");
      }

      std::cout << "OUTPUT: COUNT <num-of-vertexes>";
    }
    catch (const std::invalid_argument&)
    {
      std::cout << "<INVALID COMMAND>";
    }
  }
}

double cmd::subcmd::getTriangleArea(const Point& p1, const Point& p2, const Point& p3)
{
  return 0.5 * std::abs((p1.x - p3.x) * (p2.y - p1.y) - (p1.x - p2.x) * (p3.y - p1.y));
}

double cmd::subcmd::getPolygonArea(const Polygon& polygon)
{
  if (polygon.points.size() < 3)
  {
    return 0.0;
  }
  std::vector<double> triangleAreas(polygon.points.size() - 2);
  using namespace std::placeholders;
  std::transform
  (
    polygon.points.begin() + 2,
    polygon.points.end(),
    std::next(polygon.points.begin(), 1),
    triangleAreas.begin(),
    std::bind(&cmd::subcmd::getTriangleArea, polygon.points[0], _1, _2)
  );
  return std::accumulate(triangleAreas.begin(), triangleAreas.end(), 0.0);
}

double cmd::subcmd::getMaxPolygonArea(const std::vector< Polygon >& polygons)
{
  if (polygons.empty())
  {
    return 0.0;
  }
  auto maxIt = std::max_element(polygons.begin(), polygons.end(),
    [](const Polygon& a, const Polygon& b) -> bool
    {
      return cmd::subcmd::getPolygonArea(a) < cmd::subcmd::getPolygonArea(b);
    });
  return cmd::subcmd::getPolygonArea(*maxIt);
}

double cmd::subcmd::getMinPolygonArea(const std::vector< Polygon >& polygons)
{
  if (polygons.empty())
  {
    return 0.0;
  }
  auto minIt = std::min_element(polygons.begin(), polygons.end(),
    [](const Polygon& a, const Polygon& b) -> bool
    {
      return cmd::subcmd::getPolygonArea(a) < cmd::subcmd::getPolygonArea(b);
    });
  return cmd::subcmd::getPolygonArea(*minIt);
}

size_t cmd::subcmd::getMaxPolygonVertexes(const std::vector< Polygon >& polygons)
{
  if (polygons.empty())
  {
    return 0;
  }

  auto maxIt = std::max_element(polygons.begin(), polygons.end(),
    [](const Polygon& a, const Polygon& b) -> bool
    {
      return a.points.size() < b.points.size();
    });

  return maxIt->points.size();
}

size_t cmd::subcmd::getMinPolygonVertexes(const std::vector< Polygon >& polygons)
{
  if (polygons.empty())
  {
    return 0;
  }
  auto minIt = std::min_element(polygons.begin(), polygons.end(),
    [](const Polygon& a, const Polygon& b) -> bool
    {
      return a.points.size() < b.points.size();
    });
  return minIt->points.size();
}
