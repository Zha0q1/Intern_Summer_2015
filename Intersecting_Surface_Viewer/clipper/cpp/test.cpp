#include "clipper.hpp"
 
//...
 
//from clipper.hpp ...
//typedef long long cInt;
//struct IntPoint {cInt X; cInt Y;};
//typedef std::vector<IntPoint> Path;
//typedef std::vector<Polygon> Paths;
 
using namespace ClipperLib;
 
void GetEllipsePoints(IntRect& bounds, Path& p)
{/* ... */}
 
void DrawPolygons(Paths& p, unsigned color)
{/* ... */}
 
int main()
{
  //set up the subject and clip polygons ...
  Paths sub(3);
  GetEllipsePoints(IntRect(100,100,300,300), sub[0]);
  GetEllipsePoints(IntRect(125,130,275,180), sub[1]);
  GetEllipsePoints(IntRect(125,220,275,270), sub[2]);
   
  Paths clp(1);
  GetEllipsePoints(IntRect(140,70,220,320), clp[0]);
   
  //display the subject and clip polygons ...
  DrawPolygons(sub, 0x8033FFFF);
  DrawPolygons(clp, 0x80FFFF33);
   
  //get the intersection of the subject and clip polygons ...
  Clipper clpr;
  clpr.AddPaths(sub, ptSubject, true);
  clpr.AddPaths(clp, ptClip, true);
  Paths solution;
  clpr.Execute(ctIntersection, solution, pftEvenOdd, pftEvenOdd);
   
  //finally draw the intersection polygons ...
  DrawPolygons(solution, 0x40808080);
}