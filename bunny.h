#include "include/Angel.h"
#include <vector>
#include <fstream>

typedef vec4  color4;
typedef vec4  point4;

struct Bunny {
  std::vector<point4> vertices;
  std::vector<int> triangles;
  std::vector<point4> points;
  std::vector<color4> colors;
  int numOfPoints;
  int numOfTriangles;

  //Creates the bunny object by reading the points and their configuation
  //from the bunny.off file
  void create() {

    std::string input;
    int i;

    std::ifstream file("bunny.off"); //Read bunny.off
    file >> input;	//"OFF" word 
    file >> numOfPoints >> numOfTriangles  >> i;

    //Put the vertices to the vertices vector one by one
    for(i = 0; i < numOfPoints; i++) {
        GLfloat x,y,z;
        file >> x >> y >> z;
        vertices.push_back(point4(x, y, z, (GLfloat)1.0));
      }

    //Put the vertex alignment numbers to the triangles vector one by one
    for(i = 0; i < numOfTriangles; i++) {
      int dummy;
      int x,y,z;
      file >> dummy >> x >> y >> z;

      triangles.push_back(x);
      triangles.push_back(y);
      triangles.push_back(z);
    }

    file.close();

    //Check prints
    /*
    std::cout << "\""<< "bunny.off" << "\""<< " succefully loaded" << std::endl;
    std::cout << "number of vertexes: " << numOfPoints << std::endl;
    std::cout << "number of triangles: " << numOfTriangles << std::endl;
    */
    
    fill();
  }

  //Fills bunny's color array with the colors in the Cube example alternately
  void changeColorRGB() {
    for(int i = 0; i < colors.size(); i++) {
      colors[i] = vertex_colors[i % 8];
    }
  }

  //Fills bunny's color array with the input color
  void changeColor(color4 newColor) {
    this -> color = newColor;

    for(int i = 0; i < colors.size(); i++) {
      colors[i] = color;
    }
  }
private:

  //Initial color -> magenta
  color4 color = color4(1.0, 0.0, 1.0, 1.0);

  //Iterates over triangles vector and puts the vertices to the points vector
  //according to their alignment numbers in the triangles vector
  void fill() {
    for(std::vector<int>::iterator it = triangles.begin(); it != triangles.end(); ++it) {
      points.push_back(vertices.at(*it));
      colors.push_back(color);
    }
  }

};
