//-*-C++-*-

#ifndef COORDINATE_MAP_TEMPLATE_H
#define COORDINATE_MAP_TEMPLATE_H

namespace func_lib
{
  /*!
   *  template coordinate-map.
   */
  template<layout_type           layout_t,
           composite_domain_type composite_dmn_t,
           int DIMENSION>
  class coordinate_map
  {
  public:

    template<typename index_type>
    static void initialize_steps(index_type* pad, index_type* step);

    template<typename index_type>
    static void initialize_steps(index_type* pad, index_type* step, index_type DIM);

    template<typename index_type, typename ind_0>
    static index_type sub_index_to_position_index(index_type* size, index_type* step, ind_0 i_0);

    template<typename index_type, typename ind_0, typename ... ind_i>
    static index_type sub_index_to_position_index(index_type* size, index_type* step, ind_0 i_0, ind_i ... indices);

    template<typename index_type>
    static index_type sub_index_to_position_index(index_type* size, index_type* step, index_type* lin_ind);

    template<typename index_type>
    static index_type sub_index_to_position_index(index_type* size, index_type* step, index_type DIM, index_type* lin_ind);

    template<typename index_type>
    static index_type linear_index_to_position_index(index_type* size, index_type* step, index_type lin_ind);

    template<typename index_type>
    static index_type linear_index_to_position_index(index_type* size, index_type* step, index_type DIM, index_type lin_ind);

    template<typename index_type>
    static index_type linear_index_to_sub_index(index_type* size, index_type* step, index_type lin_ind, index_type* indices);

    template<typename index_type>
    static index_type linear_index_to_sub_index(index_type* size, index_type* step, index_type DIM, index_type lin_ind, index_type* indices);
  };

}

#endif
