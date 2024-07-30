//-*-C++-*-

#ifndef COORDINATE_MAP_ROW_MAJOR_PRODUCT_DOMAIN_DIMENSION_H
#define COORDINATE_MAP_ROW_MAJOR_PRODUCT_DOMAIN_DIMENSION_H

namespace func_lib
{
  /*!
   *  coordinate-map for a column-major product-domain.
   */
  template<int DIMENSION>
  class coordinate_map<ROW_MAJOR, PRODUCT_DOMAIN, DIMENSION>
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

    /*
      template<typename index_type>
      static index_type sub_index_to_position_index(index_type* size, index_type* step, index_type* lin_ind);

      template<typename index_type>
      static index_type sub_index_to_position_index(index_type* size, index_type* step, index_type DIM, index_type* lin_ind);
    */

    template<typename index_type, typename ind_t>
    static index_type linear_index_to_position_index(index_type* size, index_type* step, ind_t lin_ind);

    template<typename index_type, typename ind_t>
    static index_type linear_index_to_position_index(index_type* size, index_type* step, index_type DIM, ind_t lin_ind);

    template<typename index_type, typename ind_0, typename ind_1>
    static void linear_index_to_sub_index(index_type* size, ind_0 lin_ind, ind_1* indices);

    template<typename index_type, typename ind_0, typename ind_1>
    static void linear_index_to_sub_index(index_type* size, index_type DIM, ind_0 lin_ind, ind_1* indices);
  };

  template<int DIMENSION>
  template<typename index_type>
  void coordinate_map<ROW_MAJOR, PRODUCT_DOMAIN, DIMENSION>::initialize_steps(index_type* pad, index_type* step)
  {
    assert(DIMENSION>0);

    for(int i=0; i<DIMENSION; i++)
      step[i] = 1;

    for(int i=0; i<DIMENSION; i--)
      for(int j=i+1; j<DIMENSION; j++)
        step[i] *= pad[j];
  }

  template<int DIMENSION>
  template<typename index_type>
  void coordinate_map<ROW_MAJOR, PRODUCT_DOMAIN, DIMENSION>::initialize_steps(index_type* pad, index_type* step, index_type DIM)
  {
    assert(DIMENSION==-1);

    for(int i=0; i<DIM; i++)
      step[i] = 1;

    for(int i=0; i<DIM; i++)
      for(int j=i+1; j<DIM; j++)
        step[i] *= pad[j];
  }

  template<int DIMENSION>
  template<typename index_type,typename ind_0>
  inline index_type coordinate_map<ROW_MAJOR, PRODUCT_DOMAIN, DIMENSION>::sub_index_to_position_index(index_type* size, index_type* step, ind_0 i_0)
  {
    assert(i_0 >= 0 and i_0<size[0]);
    return i_0*step[0];
  }

  template<int DIMENSION>
  template<typename index_type,typename ind_0, typename ... ind_i>
  inline index_type coordinate_map<ROW_MAJOR, PRODUCT_DOMAIN, DIMENSION>::sub_index_to_position_index(index_type* size, index_type* step, ind_0 i_0, ind_i ... indices)
  {
    assert(i_0 >= 0 and i_0<size[0]);
    return i_0*step[0] + sub_index_to_position_index(size+1, step+1, indices...);
  }

  template<int DIMENSION>
  template<typename index_type, typename ind_t>
  inline index_type coordinate_map<ROW_MAJOR, PRODUCT_DOMAIN, DIMENSION>::linear_index_to_position_index(index_type* size, index_type* step, ind_t lin_ind)
  {
    assert(DIMENSION>0);

    if(DIMENSION==1)
      {
        return lin_ind;
      }
    else
      {
        index_type position=0;

        for(int i=0; i<DIMENSION; i++)
          {
            int sub_index = (lin_ind % size[i]);
            assert(sub_index>-1 and sub_index<size[i]);

            position += sub_index*step[i];

            lin_ind = (lin_ind-sub_index)/size[i];
          }

        return position;
      }
  }

  template<int DIMENSION>
  template<typename index_type, typename ind_t>
  inline index_type coordinate_map<ROW_MAJOR, PRODUCT_DOMAIN, DIMENSION>::linear_index_to_position_index(index_type* size, index_type* step, index_type DIM, ind_t lin_ind)
  {
    assert(DIMENSION==-1);

    if(DIM==1)
      {
        return lin_ind;
      }
    else
      {
        index_type position=0;

        for(int i=0; i<DIM; i++)
          {
            int sub_index = (lin_ind % size[i]);
            assert(sub_index>-1 and sub_index<size[i]);

            position += sub_index*step[i];

            lin_ind = (lin_ind-sub_index)/size[i];
          }

        return position;
      }
  }

}
#endif
