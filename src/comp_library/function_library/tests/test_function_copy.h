//-*-C++-*-

#ifndef TEST_FUNCTION_COPY_H
#define TEST_FUNCTION_COPY_H

namespace func_lib
{

  template<typename          scalar_type,
           typename          mtx_dmn_type,
           architecture_type source_type,
           architecture_type target_type>
  struct test_copy
  {

    static void test_accuracy()
    {
      //cout << "\n\t========================\n\n";

      function<scalar_type, mtx_dmn_type, source_type> A("A", 8, 6);
      function<scalar_type, mtx_dmn_type, target_type> B("B", 9, 10);
      function<scalar_type, mtx_dmn_type, source_type> C("C", 13, 11);

      A = 0;
      C = 0;

      int cnt=0;
      for(int j=0; j<6; j++)
        for(int i=0; i<8; i++)
          A(i,j) = scalar_type(++cnt);

      //A.print();

      B=A;

      //C.print();

      C=B;

      //C.print();

      {
        bool OK=true;
        for(int j=0; j<6; j++)
          for(int i=0; i<8; i++)
            if(abs(A(i,j)-C(i,j))>1.e-6)
              OK=false;

        if(OK)
          {
	    std::cout << "\t test_accuracy : passed!\n";
          }
        else
          {
	    std::cout << "\t test_accuracy : failed!\n";
	    //A.print();
	    //B.print();
	    //C.print();
          }
      }

      //cout << "\n\t========================\n\n";
    }

    static void test_accuracy(int device_id)
    {
      func_lib::device_parameters params(func_lib::CPU, 1, 1, device_id);

      //cout << "\n\t========================\n\n";

      function<scalar_type, mtx_dmn_type, source_type> A("A", 8, 6);
      function<scalar_type, mtx_dmn_type, target_type> B("B", 9, 10);
      function<scalar_type, mtx_dmn_type, source_type> C("C", 13, 11);

      A = 0;
      C = 0;

      int cnt=0;
      for(int j=0; j<6; j++)
        for(int i=0; i<8; i++)
          A(i,j) = scalar_type(++cnt);

      A.print();

      B=A;

      C.print();

      C=B;

      C.print();

      /*
	A = 1;
	C = 0;

	B=A;
	C=B;
      */

      {
        bool OK=true;
        for(int j=0; j<6; j++)
          for(int i=0; i<8; i++)
            if(abs(A(i,j)-C(i,j))>1.e-6)
              OK=false;

        if(OK)
          {
	    std::cout << "\t test_accuracy : passed!\n";
          }
        else
          {
	    std::cout << "\t test_accuracy : failed!\n";
	    A.print();
	    //B.print();
	    C.print();
          }
      }

      //cout << "\n\t========================\n\n";
    }

    static void test_accuracy_asynchronous()
    {
      //cout << "\n\t========================\n\n";

      function<scalar_type, mtx_dmn_type, source_type> A("A", 8, 6);
      function<scalar_type, mtx_dmn_type, target_type> B("B", 9, 10);
      function<scalar_type, mtx_dmn_type, source_type> C("C", 13, 11);

      A.get_thread_id() = 0; A.get_stream_id() = 0; 
      B.get_thread_id() = 0; B.get_stream_id() = 0; 
      C.get_thread_id() = 0; C.get_stream_id() = 0; 

      A = 0;
      C = 0;

      int cnt=0;
      for(int j=0; j<6; j++)
        for(int i=0; i<8; i++)
          A(i,j) = scalar_type(++cnt);

      //A.print();

      B.copy_asynchronous_from(A);
      
      //C.print();

      C.copy_asynchronous_from(B);

      //C.print();

      {
        bool OK=true;
        for(int j=0; j<6; j++)
          for(int i=0; i<8; i++)
            if(abs(A(i,j)-C(i,j))>1.e-6)
              OK=false;

        if(OK)
          {
	    std::cout << "\t test_accuracy : passed!\n";
          }
        else
          {
	    std::cout << "\t test_accuracy : failed!\n";
	    A.print();
	    B.print();
	    C.print();
          }
      }

      //cout << "\n\t========================\n\n";
    }

  };

}

#endif
