//-*-C++-*-

#ifndef GEOMETRY_OBJECT_RECTANGLE_H
#define GEOMETRY_OBJECT_RECTANGLE_H

namespace geometry_lib
{
  /*
   *  PAGE-LAYOUT
   *
   *  Y-axis
   *   ^
   *   |
   *   |         |----------------------(x1, y1)
   *   |         |                         |
   *   |         |                         |
   *   |      (x0, y0)----------------------
   *   |
   *   |
   *   |
   *   -----------------------------------------------> X-axis
   */

  template<typename scalar_type>
  class object<RECTANGLE, 2, scalar_type>
  {
    typedef object<RECTANGLE, 2, scalar_type> this_type;

  public:

    object();
    //object(const object<RECTANGLE, 2, scalar_type>& other);

    object(scalar_type x0, scalar_type y0, scalar_type x1, scalar_type y1);

    ~object();

    this_type& operator=(const this_type& other);
    
    void set(std::vector<scalar_type> vec);
    void set(scalar_type x0, scalar_type y0, scalar_type x1, scalar_type y1);

    std::vector<scalar_type> get_bbox();
    void                     set_bbox(std::vector<scalar_type> vec);

    std::array<object<POINT, 2, scalar_type>, 2> get_points() const;
    void                                         set_points(std::array<object<POINT, 2, scalar_type>, 2> points);

    //

    template<typename index_type>
    object<POINT, 2, scalar_type>& operator()(index_type i);

    template<typename index_type>
    object<POINT, 2, scalar_type>& operator[](index_type i);

    object<RECTANGLE, 2, scalar_type>& operator*=(scalar_type alpha);
    object<RECTANGLE, 2, scalar_type>& operator/=(scalar_type alpha);

    object<RECTANGLE, 2, scalar_type>& operator+=(object<POINT, 2, scalar_type>& other);
    object<RECTANGLE, 2, scalar_type>& operator-=(object<POINT, 2, scalar_type>& other);

    scalar_type& x0() { return points[0][0]; }
    scalar_type& y0() { return points[0][1]; }
    scalar_type& x1() { return points[1][0]; }
    scalar_type& y1() { return points[1][1]; }

    bool is_below(this_type& other) { return y1()<other.y0(); }
    bool is_above(this_type& other) { return y0()>other.y1(); }
    bool is_left (this_type& other) { return x1()<other.x0(); }
    bool is_right(this_type& other) { return x0()<other.y1(); }

    object<POINT, 2, scalar_type> cm() { return object<POINT, 2, scalar_type>({this->cm_x(),this->cm_y()}); }

    scalar_type cm_x() { return (points[0][0]+points[1][0])/2.;}
    scalar_type cm_y() { return (points[0][1]+points[1][1])/2.;}

    scalar_type width () { return points[1][0]-points[0][0];}
    scalar_type height() { return points[1][1]-points[0][1];}

    scalar_type surface() { return height()*width(); }

    void scale  (float xf, float yf);
    void enlarge(float xd, float yd);

    bool contains(object<POINT    , 2, scalar_type>& p,
                  scalar_type                        eps=1.e-16);

    bool overlaps(object<RECTANGLE, 2, scalar_type>& p,
                  scalar_type                        eps=1.e-16);

    bool overlaps_horizontally(object<RECTANGLE, 2, scalar_type>& p,
                               scalar_type                        eps=1.e-16);

    bool overlaps_vertically(object<RECTANGLE, 2, scalar_type>& p,
                             scalar_type                        eps=1.e-16);

    scalar_type intersect(object<RECTANGLE, 2, scalar_type>& other);

    object<RECTANGLE, 2, scalar_type> bounding_box();
    //object<RECTANGLE, 2, scalar_type> bounding_box(object<RECTANGLE, 2, scalar_type>& other);

    scalar_type relative_overlap(object<RECTANGLE, 2, scalar_type>& other);

    void bbox_union       (object<RECTANGLE, 2, scalar_type>& other);
    void bbox_intersection(object<RECTANGLE, 2, scalar_type>& other);

  protected:

    std::array<object<POINT, 2, scalar_type>, 2> points;
  };

  template<typename scalar_type>
  object<RECTANGLE, 2, scalar_type>::object()
  {
    points[0][0] = 0;
    points[0][1] = 0;
    points[1][0] = 0;
    points[1][1] = 0;
  }

  template<typename scalar_type>
  object<RECTANGLE, 2, scalar_type>::object(scalar_type x0, scalar_type y0,
                                            scalar_type x1, scalar_type y1)
  {
    set(x0, y0, x1, y1);
  }

  template<typename scalar_type>
  object<RECTANGLE, 2, scalar_type>::~object()
  {
  }

  template<typename scalar_type>
  object<RECTANGLE, 2, scalar_type>& object<RECTANGLE, 2, scalar_type>::operator=(const object<RECTANGLE, 2, scalar_type>& other)
  {
    //this->points = other.get_points();
    this->set_points(other.get_points());
    return *this;
  }
  
  template<typename scalar_type>
  void object<RECTANGLE, 2, scalar_type>::set(std::vector<scalar_type> vec)
  {
    set(vec[0], vec[1], vec[2], vec[3]);
  }

  template<typename scalar_type>
  void object<RECTANGLE, 2, scalar_type>::set(scalar_type x0, scalar_type y0,
                                              scalar_type x1, scalar_type y1)
  {
    if(x0>x1) std::swap(x0, x1);
    if(y0>y1) std::swap(y0, y1);

    points[0][0] = x0;
    points[0][1] = y0;
    points[1][0] = x1;
    points[1][1] = y1;
  }

  template<typename scalar_type>
  void object<RECTANGLE, 2, scalar_type>::set_bbox(std::vector<scalar_type> vec)
  {
    assert(vec.size()==4);
    set(vec[0], vec[1], vec[2], vec[3]);
  }

  template<typename scalar_type>
  std::vector<scalar_type> object<RECTANGLE, 2, scalar_type>::get_bbox()
  {
    std::vector<scalar_type> tmp = {points[0][0], points[0][1], points[1][0], points[1][1]};
    return tmp;
  }

  template<typename scalar_type>
  std::array<object<POINT, 2, scalar_type>, 2> object<RECTANGLE, 2, scalar_type>::get_points() const
  {
    return points;
  }

  template<typename scalar_type>
  void object<RECTANGLE, 2, scalar_type>::set_points(std::array<object<POINT, 2, scalar_type>, 2> points_)
  {
    points = points_;
  }

  template<typename scalar_type>
  template<typename index_type>
  object<POINT, 2, scalar_type>& object<RECTANGLE, 2, scalar_type>::operator()(index_type i)
  {
    assert(i<points.size());
    return points[i];
  }

  template<typename scalar_type>
  template<typename index_type>
  object<POINT, 2, scalar_type>& object<RECTANGLE, 2, scalar_type>::operator[](index_type i)
  {
    assert(i<points.size());
    return points[i];
  }

  /*
    template<typename scalar_type>
    object<RECTANGLE, 2, scalar_type>& object<RECTANGLE, 2, scalar_type>::operator=(object<RECTANGLE, 2, scalar_type>& other)
    {
    points[0] = other[0];
    points[1] = other[1];

    return *this;
    }
  */

  template<typename scalar_type>
  object<RECTANGLE, 2, scalar_type>& object<RECTANGLE, 2, scalar_type>::operator*=(scalar_type alpha)
  {
    points[0] *= alpha;
    points[1] *= alpha;

    return *this;
  }

  template<typename scalar_type>
  object<RECTANGLE, 2, scalar_type>& object<RECTANGLE, 2, scalar_type>::operator/=(scalar_type alpha)
  {
    points[0] /= alpha;
    points[1] /= alpha;

    return *this;
  }

  template<typename scalar_type>
  object<RECTANGLE, 2, scalar_type>& object<RECTANGLE, 2, scalar_type>::operator+=(object<POINT, 2, scalar_type>& p)
  {
    points[0] += p;
    points[1] += p;

    return *this;
  }

  template<typename scalar_type>
  object<RECTANGLE, 2, scalar_type>& object<RECTANGLE, 2, scalar_type>::operator-=(object<POINT, 2, scalar_type>& p)
  {
    points[0] -= p;
    points[1] -= p;

    return *this;
  }

  template<typename scalar_type>
  void object<RECTANGLE, 2, scalar_type>::scale(float xf, float yf)
  {
    scalar_type cm_x = this->cm_x();
    scalar_type cm_y = this->cm_y();

    points[0][0] = cm_x - xf*width ()/2.;
    points[0][1] = cm_y - yf*height()/2.;
    points[1][0] = cm_x + xf*width ()/2.;
    points[1][1] = cm_y + yf*height()/2.;
  }

  template<typename scalar_type>
  void object<RECTANGLE, 2, scalar_type>::enlarge(float xd, float yd)
  {
    points[0][0] -= xd;
    points[0][1] -= yd;
    points[1][0] += xd;
    points[1][1] += yd;
  }

  template<typename scalar_type>
  bool object<RECTANGLE, 2, scalar_type>::contains(object<POINT, 2, scalar_type>& p,
                                                   scalar_type                    eps)
  {
    return (x0()-eps<=p[0] and p[0]<=x1()+eps and
            y0()-eps<=p[1] and p[1]<=y1()+eps);
  }

  template<typename scalar_type>
  bool object<RECTANGLE, 2, scalar_type>::overlaps(object<RECTANGLE, 2, scalar_type>& other,
                                                   scalar_type                        eps)
  {
    return ((this->intersect(other))>eps);
  }

  template<typename scalar_type>
  bool object<RECTANGLE, 2, scalar_type>::overlaps_horizontally(object<RECTANGLE, 2, scalar_type>& other,
                                                                scalar_type                        eps)
  {
    return ( not ( this->y0()>other.y1() or this->y1()<other.y0()) );
  }

  template<typename scalar_type>
  bool object<RECTANGLE, 2, scalar_type>::overlaps_vertically(object<RECTANGLE, 2, scalar_type>& other,
                                                              scalar_type                        eps)
  {
    return ( not ( this->x0()>other.x1() or this->x1()<other.x0()) );
  }

  template<typename scalar_type>
  scalar_type object<RECTANGLE, 2, scalar_type>::intersect(object<RECTANGLE, 2, scalar_type>& other)
  {
    return
      std::max(scalar_type(0), std::min(x1(), other.x1()) - std::max(x0(), other.x0())) *
      std::max(scalar_type(0), std::min(y1(), other.y1()) - std::max(y0(), other.y0()));
  }

  template<typename scalar_type>
  object<RECTANGLE, 2, scalar_type> object<RECTANGLE, 2, scalar_type>::bounding_box()
  {
    return *this;
  }

  /*
    template<typename scalar_type>
    object<RECTANGLE, 2, scalar_type> object<RECTANGLE, 2, scalar_type>::bounding_box(object<RECTANGLE, 2, scalar_type>& other)
    {
    object<RECTANGLE, 2, scalar_type> bbox = this;


    }
  */

  template<typename scalar_type>
  scalar_type object<RECTANGLE, 2, scalar_type>::relative_overlap(object<RECTANGLE, 2, scalar_type>& other)
  {
    scalar_type SA = this->surface();
    scalar_type SB = other.surface();

    scalar_type SI = this->intersect(other);

    if(SA>0 and SB>0)
      return (SI)/(SA+SB-SI);
    else
      return 0;
  }

  template<typename scalar_type>
  void object<RECTANGLE, 2, scalar_type>::bbox_union(object<RECTANGLE, 2, scalar_type>& other)
  {
    this->x0() = std::min(this->x0(), other.x0());
    this->y0() = std::min(this->y0(), other.y0());

    this->x1() = std::max(this->x1(), other.x1());
    this->y1() = std::max(this->y1(), other.y1());
  }

}

#endif
