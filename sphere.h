#include "include/Angel.h"
#include <cmath>

typedef vec4  color4;
typedef vec4  point4;

const int subdivisionCount = 4;
const int vertexNumber =  pow(pow(2, subdivisionCount + 1), 2) * 3;

struct Sphere {

  //Point and color arrays
  point4 points[vertexNumber];
  color4 colors[vertexNumber];
  color4 colorsRGB[vertexNumber];

  int numOfVertices = vertexNumber;

  //Creates a sphere recursively using tetrahedron-subdivison n times
  void create()
  {
    int n = subdivisionCount;
    divide_triangle(v[0], v[1], v[2], n);
    divide_triangle(v[3], v[2], v[1], n );
    divide_triangle(v[0], v[3], v[1], n );
    divide_triangle(v[0], v[3], v[2], n );
  }

  //Fills sphere's color array with the input color
  void changeColor(color4 newColor) {
    this -> color = newColor;

    for(int i = 0; i < numOfVertices; i++) {
      colors[i] = color;
    }
  }

  //Fills sphere's colorRGB array with the colors in the Cube example alternately
  void changeColorRGB() {
    for(int i = 0; i < numOfVertices; i++) {
      colors[i] = vertex_colors[i % 8];
    }
  }

private:

  color4 color = color4(1.0, 0.0, 0.0, 1.0);

  //Initial tetrahedron's vertices
  point4 v[4]= {
    vec4(0.0, 0.0, 1.0, 1.0),
    vec4(0.0, 0.942809, -0.333333, 1.0),
    vec4(-0.816497, -0.471405, -0.333333, 1.0),
    vec4(0.816497, -0.471405, -0.333333, 1.0)
  };

  int index = 0;

  //Get unit vector of a vector
  point4 unit(const point4 &p)
  {
    point4 c;
    double d=0.0;
    for(int i=0; i<3; i++) d+=p[i]*p[i];
    d=sqrt(d);
    if(d > 0.0) for(int i=0; i<3; i++) c[i] = p[i]/d;
    c[3] = 1.0;
    return c;
  }

  //Fills the point array with the input point objects
  //and color array with the initial color
  void triangle( point4  a, point4 b, point4 c)
  {
    points[index]= a;
    colors[index]= color;
    index++;
    points[index]= b;
    colors[index]= color;
    index++;
    points[index]= c;
    colors[index]= color;
    index++;
  }

  //Performs subdivision on a triangle recursively until n is 0
  void divide_triangle(point4 a, point4 b, point4 c, int n)
  {
    point4 v1, v2, v3;
    if(n>0)
      {
        v1 = unit(a + b);
        v2 = unit(a + c);
        v3 = unit(b + c);   
        divide_triangle(a ,v2, v1, n-1);
        divide_triangle(c ,v3, v2, n-1);
        divide_triangle(b ,v1, v3, n-1);
        divide_triangle(v1 ,v2, v3, n-1);
      }
    else triangle(a, b, c);
  }

};
