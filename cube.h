#include "include/Angel.h"

typedef vec4  color4;
typedef vec4  point4;

const int NumVertices = 36;

// Vertices of a unit cube centered at origin, sides aligned with axes
const point4 vertices[8] = {
  point4( -1.0, -1.0,  1.0, 1.0 ),
  point4( -1.0,  1.0,  1.0, 1.0 ),
  point4(  1.0,  1.0,  1.0, 1.0 ),
  point4(  1.0, -1.0,  1.0, 1.0 ),
  point4( -1.0, -1.0, -1.0, 1.0 ),
  point4( -1.0,  1.0, -1.0, 1.0 ),
  point4(  1.0,  1.0, -1.0, 1.0 ),
  point4(  1.0, -1.0, -1.0, 1.0 )
};

// RGBA olors
const color4 vertex_colors[8] = {
  color4( 0.0, 0.0, 0.0, 1.0 ),  // black
  color4( 1.0, 0.0, 0.0, 1.0 ),  // red
  color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
  color4( 0.0, 1.0, 0.0, 1.0 ),  // green
  color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
  color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
  color4( 1.0, 1.0, 1.0, 1.0 ),  // white
  color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
};

struct Cube {
  int numOfVertices = NumVertices;
  vec4 points[NumVertices];
  vec4 colors[NumVertices];

  // generate 12 triangles: 36 vertices and 36 colors
  void create() {
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
     
  }

  //Change the cube's color to the input color
  void changeColor(color4 newColor) {
    this -> color = newColor;

    for(int i = 0; i < numOfVertices; i++) {
      colors[i] = color;
    }
  }

  //Change the cube's color to multicolor
  void changeColorRGB() {
    Index = 0;
    create();
  }

private:

  color4 color = point4(0.0, 0.0, 0.0, 1.0);

  int Index = 0;

  void quad( int a, int b, int c, int d ) {
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
  }

};
