//-*-C++-*-

#ifndef JSONPARSER_TRANSLATION_TABLE_H
#define JSONPARSER_TRANSLATION_TABLE_H

namespace IO
{
  namespace JSONPARSER
  {
    class JSON_translation_table: 
      public JSON_character_mapper// , 
    //     public JSON_state// ,
    //     public ActionsMixin
    {
    public:

      static state_and_action_pair get_state_and_action_pair(JSON_state_type& state, JSON_character_class_type& cls);

    private:

      static bool check_state_and_class(JSON_state_type& state, JSON_character_class_type& cls);

      static state_and_action_pair begin_object_or_array(JSON_character_class_type& cls);

      static state_and_action_pair begin_value(JSON_character_class_type& cls);
      static state_and_action_pair end_value  (JSON_character_class_type& cls);

      static state_and_action_pair begin_string(JSON_character_class_type& cls);
      static state_and_action_pair read_string (JSON_character_class_type& cls);

      static state_and_action_pair read_integer(JSON_character_class_type& cls);
      static state_and_action_pair read_fraction(JSON_character_class_type& cls);

      static state_and_action_pair begin_exponent(JSON_character_class_type& cls);
      static state_and_action_pair read_exponent(JSON_character_class_type& cls);

      static state_and_action_pair read_NULL(JSON_state_type& state, JSON_character_class_type& cls);

      static state_and_action_pair read_false(JSON_state_type& state, JSON_character_class_type& cls);
      static state_and_action_pair read_true (JSON_state_type& state, JSON_character_class_type& cls);

      static state_and_action_pair read_array(JSON_state_type& state, JSON_character_class_type& cls);
    };

    bool JSON_translation_table::check_state_and_class(JSON_state_type& state, JSON_character_class_type& cls) 
    {
      if(!(state < NR_STATES) || !(cls < NR_CLASSES) ) 
	{
	  std::cout << "StateTranslationTable ("<< state << " < " << NR_STATES << "," << cls << " < " << NR_CLASSES << ")\n";
	  std::cout << "StateTranslationTable ("<< name(state) << "," << name(cls) << ")\n";
	  {
	  std::stringstream ss;
	  ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
	  throw std::logic_error(ss.str());
	}
	}
    
      return true;
    }
  
    state_and_action_pair JSON_translation_table::get_state_and_action_pair(JSON_state_type& state, JSON_character_class_type& cls) 
    {      
      assert(check_state_and_class(state, cls));

      switch(state) 
	{
	case GO:
	  return begin_object_or_array(cls);

	case VA:
	  return begin_value(cls);

	case EV: 
	  return end_value(cls);

	case BS: 
	  return begin_string(cls);

	case ST:
	  return read_string(cls);
	  
	case IT:
	  return read_integer(cls);
	  
	case FR: 
	  return read_fraction(cls);
	
	case EX:
	  return begin_exponent(cls);
	
	case EX2: 
	  return read_exponent(cls);
		
	case T:
	case TR:
	case TRU: 
	  return read_true(state, cls);

	case N:
	case NU:
	case NUL:
	  return read_NULL(state, cls);

	case F:
	case FA:
	case FAL:
	case FALS:
	  return read_false(state, cls);

	case A:
	case AR:
	case ARR:
	case ARRA:
	case ARRAY:
	  return read_array(state, cls);

	case MS: 
	  switch (cls) 
	    {
	    case C_RPARN: return state_and_action_pair(EV,EndMatrix);
	    default:      return state_and_action_pair(MS,Consume);
	    }
	  
	default:
	  std::cout << "JSON_translation_table::getStateActionPair was passed a unkown state " << name(state) << "\n";
	  {
	  std::stringstream ss;
	  ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
	  throw std::logic_error(ss.str());
	}
	}
    }
  
    state_and_action_pair JSON_translation_table::begin_object_or_array(JSON_character_class_type& cls)
    {
      switch(cls) 
	{
	case C_SPACE: return state_and_action_pair(GO,Consume);
	case C_WHITE: return state_and_action_pair(GO,Consume);
	case C_LCURB: return state_and_action_pair(BS,BeginObject);
	case C_LSQRB: return state_and_action_pair(VA,BeginArray);
	default:      return state_and_action_pair(END,Abort);
	}
    }

    state_and_action_pair JSON_translation_table::begin_value(JSON_character_class_type& cls)
    {
      switch(cls) 
	{
	case C_SPACE: return state_and_action_pair(VA,Consume);
	case C_WHITE: return state_and_action_pair(VA,Consume);

	case C_LCURB: return state_and_action_pair(BS,BeginObject);
	case C_LSQRB: return state_and_action_pair(VA,BeginArray);
	case C_RCURB: return state_and_action_pair(EV,EndObject);  // test
	case C_RSQRB: return state_and_action_pair(EV,EndArray);   // test

	case C_QUOTE: return state_and_action_pair(ST,Consume);
	case C_LOW_A: return state_and_action_pair(A,Consume);
	case C_MINUS:
	case C_PLUS:
	case C_ZERO:
	case C_DIGIT: return state_and_action_pair(IT,RecordChar);
	case C_POINT: return state_and_action_pair(FR,RecordChar);
	case C_LOW_T: return state_and_action_pair(T,Consume);
	case C_LOW_F: return state_and_action_pair(F,Consume);
	case C_LOW_N: return state_and_action_pair(N,Consume);
	default:      return state_and_action_pair(END,Abort);
	}
    }

    state_and_action_pair JSON_translation_table::end_value(JSON_character_class_type& cls)
    {
      switch(cls) 
	{
	case C_SPACE: return state_and_action_pair(EV,Consume);
	case C_WHITE: return state_and_action_pair(EV,Consume);
	case C_RCURB: return state_and_action_pair(EV,EndObject);
	case C_RSQRB: return state_and_action_pair(EV,EndArray);
	case C_COMMA: return state_and_action_pair(VA,DoNext);
	case C_COLON: return state_and_action_pair(VA,RecordKey);
	case C_EOF:   return state_and_action_pair(EV,EndFile);
	default:      return state_and_action_pair(END,Abort);
	}
    }

    state_and_action_pair JSON_translation_table::begin_string(JSON_character_class_type& cls)
    {
      switch(cls) 
	{
	case C_SPACE: return state_and_action_pair(BS,Consume);
	case C_WHITE: return state_and_action_pair(BS,Consume);
	case C_QUOTE: return state_and_action_pair(ST,Consume);
	case C_RCURB: return state_and_action_pair(EV,EndObject);  // test
	default:      return state_and_action_pair(END,Abort);
	}
    }

    state_and_action_pair JSON_translation_table::read_string (JSON_character_class_type& cls)
    {
      switch(cls) 
	{
	case C_QUOTE: return state_and_action_pair(EV,RecordString);
	default:      return state_and_action_pair(ST,RecordChar);
	}
    }

    state_and_action_pair JSON_translation_table::read_integer(JSON_character_class_type& cls)
    {
      switch (cls) 
	{
	case C_ZERO:
	case C_DIGIT: return state_and_action_pair(IT,RecordChar);
	case C_POINT: return state_and_action_pair(FR,RecordChar);
	case C_E:     return state_and_action_pair(EX,RecordChar);
	case C_LOW_E: return state_and_action_pair(EX,RecordChar);
	case C_RCURB: return state_and_action_pair(EV,RecordInteger,EndObject);
	case C_RSQRB: return state_and_action_pair(EV,RecordInteger,EndArray);
	case C_COMMA: return state_and_action_pair(VA,RecordInteger,DoNext);
	default:      
	  return state_and_action_pair(END,Abort); 
	}
    }

    state_and_action_pair JSON_translation_table::read_fraction(JSON_character_class_type& cls)
    {
      switch (cls) 
	{
	case C_ZERO:
	case C_DIGIT: return state_and_action_pair(FR,RecordChar);
	case C_POINT: return state_and_action_pair(END,Abort);
	case C_E:     return state_and_action_pair(EX,RecordChar);
	case C_LOW_E: return state_and_action_pair(EX,RecordChar); // TEST
	case C_RCURB: return state_and_action_pair(EV,RecordFloat,EndObject);
	case C_RSQRB: return state_and_action_pair(EV,RecordFloat,EndArray);
	case C_COMMA: return state_and_action_pair(VA,RecordFloat,DoNext);
	default:      return state_and_action_pair(END,Abort); 
	}    
    }
  
    state_and_action_pair JSON_translation_table::begin_exponent(JSON_character_class_type& cls)
    {
      switch (cls) 
	{
	case C_MINUS:
	case C_PLUS:
	case C_ZERO:
	case C_DIGIT: return state_and_action_pair(EX2,RecordChar);
	default:      return state_and_action_pair(END,Abort);
	}    
    }

    state_and_action_pair JSON_translation_table::read_exponent(JSON_character_class_type& cls)
    {
      switch (cls) 
	{
	case C_ZERO:
	case C_DIGIT: return state_and_action_pair(EX2,RecordChar);
	case C_RCURB: return state_and_action_pair(EV,RecordFloat,EndObject);
	case C_RSQRB: return state_and_action_pair(EV,RecordFloat,EndArray);
	case C_COMMA: return state_and_action_pair(VA,RecordFloat,DoNext);
	default:      return state_and_action_pair(END,Abort);
	}    
    }

    state_and_action_pair JSON_translation_table::read_NULL(JSON_state_type& state, JSON_character_class_type& cls)
    {
      switch(state) 
	{
	case N: // Null
	  switch(cls) 
	    {
	    case C_LOW_U: return state_and_action_pair(NU,Consume);
	    default:      return state_and_action_pair(END,Abort);
	    }
	 
	case NU: // Null
	  switch(cls) 
	    {
	    case C_LOW_L: return state_and_action_pair(NUL,Consume);
	    default:      return state_and_action_pair(END,Abort);
	    }
	 
	case NUL: // Null
	  switch(cls) 
	    {
	    case C_LOW_L: return state_and_action_pair(EV,RecordNull);
	    default:      return state_and_action_pair(END,Abort);
	    }
	 
	default:
	  std::cout << __FUNCTION__ << "\n";
	  {
	  std::stringstream ss;
	  ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
	  throw std::logic_error(ss.str());
	}
	}
    }

    state_and_action_pair JSON_translation_table::read_false(JSON_state_type& state, JSON_character_class_type& cls)
    {
      switch(state) 
	{
	case F: // False
	  switch (cls) 
	    {
	    case C_LOW_A: return state_and_action_pair(FA,Consume);
	    default:      return state_and_action_pair(END,Abort);
	    }
	
	case FA: // False
	  switch (cls) 
	    {
	    case C_LOW_L: return state_and_action_pair(FAL,Consume);
	    default:      return state_and_action_pair(END,Abort);
	    }
	
	case FAL: // False
	  switch (cls) 
	    {
	    case C_LOW_S: return state_and_action_pair(FALS,Consume);
	    default:      return state_and_action_pair(END,Abort);
	    }
	
	case FALS: // False
	  switch (cls) 
	    {
	    case C_LOW_E: return state_and_action_pair(EV,RecordFalse);
	    default:      return state_and_action_pair(END,Abort);
	    }
	
	default:
	  std::cout << __FUNCTION__ << "\n";
	  {
	  std::stringstream ss;
	  ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
	  throw std::logic_error(ss.str());
	}
	}
    }

    state_and_action_pair JSON_translation_table::read_true(JSON_state_type& state, JSON_character_class_type& cls)
    {
      switch(state) 
	{
	case T: // True
	  switch (cls) 
	    {
	    case C_LOW_R: return state_and_action_pair(TR,Consume);
	    default:      return state_and_action_pair(END,Abort);
	    }

	case TR: // True
	  switch (cls) 
	    {
	    case C_LOW_U: return state_and_action_pair(TRU,Consume);
	    default:      return state_and_action_pair(END,Abort);
	    }
	
	case TRU: // True
	  switch (cls) 
	    {
	    case C_LOW_E: return state_and_action_pair(EV,RecordTrue);
	    default:      return state_and_action_pair(END,Abort);
	    }

	default:
	  std::cout << __FUNCTION__ << "\n";
	  {
	  std::stringstream ss;
	  ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
	  throw std::logic_error(ss.str());
	}
	}
    }

    state_and_action_pair JSON_translation_table::read_array(JSON_state_type& state, JSON_character_class_type& cls)
    {
      switch(state) 
	{
	case A: // array
	  switch (cls) 
	    {
	    case C_LOW_R: return state_and_action_pair(AR,Consume);
	    default:      return state_and_action_pair(END,Abort);
	    }

	case AR: // array
	  switch (cls) 
	    {
	    case C_LOW_R: return state_and_action_pair(ARR,Consume);
	    default:      return state_and_action_pair(END,Abort);
	    }
	
	case ARR: // array
	  switch (cls) 
	    {
	    case C_LOW_A: return state_and_action_pair(ARRA,Consume);
	    default:      return state_and_action_pair(END,Abort);
	    }
	
	case ARRA: // False
	  switch (cls) 
	    {
	    case C_LOW_Y: return state_and_action_pair(ARRAY,Consume);
	    default:      return state_and_action_pair(END,Abort);
	    }
	
	case ARRAY: // False
	  switch (cls) 
	    {
	    case C_LPARN: 
	      //cout << "\n\t found array\n";
	      return state_and_action_pair(MS,Consume,BeginMatrix);
	  
	    default:      
	      return state_and_action_pair(END,Abort);
	    }
	
	default:
	  std::cout << __FUNCTION__ << "\n";
	  {
	  std::stringstream ss;
	  ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
	  throw std::logic_error(ss.str());
	}
	}
    }

  } 

}

#endif
