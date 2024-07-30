//-*-C++-*-

#ifndef GEOMETRY_OBJECT_H
#define GEOMETRY_OBJECT_H

namespace geometry_lib
{
  enum object_name { POINT,
		     LINE,

		     PATH,
		     CPATH,   // COMBINED PATH
		     
		     TRIANGLE,
		     RECTANGLE};

  template<object_name name, int DIM, typename scalar_type=float>
  class object
  {
  public:

    object();
    object(const object& other);
    
    ~object();
    
    object& operator=(const object& other);
  };
  
}

#endif
