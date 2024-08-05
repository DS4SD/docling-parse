//-*-C++-*-

#ifndef JSON_PARSER_H
#define JSON_PARSER_H

namespace IO
{
  namespace JSONPARSER
  {
    /*!   
     *
     */
    template<typename context_type=JSON_context>
    class JSON_parser: public JSON_character_mapper,
		       public JSON_mode_stack
    {
    public:

      JSON_parser(bool verbose_=false);
      ~JSON_parser();

      bool execute(std::istringstream& input); 

      bool execute(std::istream& inputStream); 

      context_type& get_JSON_tree();

      void update(container_lib::container& data);
      
    private:

      std::pair<std::string, JSON_character_class_type> get_next_character_and_class(std::istringstream& input);

      std::pair<std::string, JSON_character_class_type> get_next_character_and_class(std::istream& inputStream);

      void begin_entity(JSON_action_type& action);
      void end_entity  (JSON_action_type& action);
    
      void record_entity(JSON_action_type& action);

      void abort_parsing(std::string                   nextChar, 
			 JSON_character_class_type nextClass, 
			 JSON_state_type           state,
			 JSON_action_type          action);

      void setCommentState();
  
      void unloadBuffer();

      void performAction(std::string nextChar, 
			 JSON_character_class_type nextClass, 
			 JSON_state_type state, 
			 JSON_action_type action);
        
    private:

      bool verbose;
      
      context_type         ctx;
      ParseBuffer          buffer;
      JSON_state_type      state;

      JSON_value_type      current_value_type;    

      bool                 allow_comments; 
      JSON_state_type      before_comment_state;
      bool                 comment; 
      size_t               comment_begin_offset;

      size_t               numChar;
      size_t               numLines;

      bool                 trace;

      std::string          filename;
    };
  
    template<typename context_type>
    JSON_parser<context_type>::JSON_parser(bool verbose_):
      verbose              (verbose_),
      ctx                  (),
      buffer               (),
      state                (GO),
      current_value_type   (JSON_T_NONE),
      //escaped            (false),
      allow_comments       (true),
      before_comment_state (GO),
      comment              (false),
      comment_begin_offset (0),
      numChar              (0),
      numLines             (0),
      trace                (false),
      filename             ("")
    {}
  
    template<typename context_type>
    JSON_parser<context_type>::~JSON_parser()
    {}

    template<typename context_type>
    context_type& JSON_parser<context_type>::get_JSON_tree()
    {
      return ctx;
    }

    template<typename context_type>    
    void JSON_parser<context_type>::update(container_lib::container& data)
    {
      data <= ctx.result;
    }

    template<typename context_type>
    void JSON_parser<context_type>::setCommentState() 
    {
      switch (currentMode()) 
	{
	case MODE_ARRAY:
	case MODE_OBJECT:   
	  switch(state) 
	    {
	    case VA:    
	    case AR:    
	      before_comment_state = state;
	      break;
	    default:
	      before_comment_state = GO;
	      break;
	    }
	  break;
	
	default:
	  before_comment_state = state;
	  break;
	}
    
      comment = true;
    }
  
    template<typename context_type>
    void JSON_parser<context_type>::unloadBuffer() 
    {
      switch(current_value_type) 
	{
	case JSON_T_STRING:
	  ctx.String(buffer);	
	  break;
	default:
	  break;
	}

      current_value_type = JSON_T_NONE;

      buffer.clear();
    }
    
    template<typename context_type>
    void JSON_parser<context_type>::performAction(std::string               nextChar, 
						  JSON_character_class_type nextClass, 
						  JSON_state_type           state,
						  JSON_action_type          action) 
    {
      switch(action) 
	{
	case Consume:
	  return;

	case DoNext: 
	  {
	    unloadBuffer();
	    return; 
	  }

	case RecordChar:
	  {
	    buffer.put(nextChar);
	    return;
	  }

	case BeginArray:
	case BeginObject:
	case BeginMatrix:
	  {
	    begin_entity(action);
	    return;
	  }

	case EndArray:
	case EndObject:
	case EndMatrix:
	  {
	    end_entity(action);
	    return;
	  }
  
	case RecordString:
	case RecordKey:
	case RecordFloat:
	case RecordInteger:
	case RecordTrue:
	case RecordFalse:
	case RecordNull:
	  {
	    record_entity(action);
	    break;
	  }

	case Abort: 
	  {
	    abort_parsing(nextChar, nextClass, state, action);
	    break;
	  }

	case EndFile: 
	  {
	    if(verbose)
	      std::cout << "\n\n\t Parsing completed! read " << numChar
			<< " characters and " << numLines << " lines.\n";
	    break;
	  }
	
	default: 
	  {
	    std::cout << "ERROR in " << __FUNCTION__ << " in " << __FILE__ << ":" << __LINE__;
	    {
	  std::stringstream ss;
	  ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
	  throw std::logic_error(ss.str());
	}
	  }
	}
    }
  
    template<typename context_type>
    void JSON_parser<context_type>::begin_entity(JSON_action_type& action)
    {
      switch(action) 
	{	
	case BeginObject:
	  ctx.begin_object();
	  break;
	
	case BeginArray:
	  ctx.begin_array();
	  break;
        
	case BeginMatrix:
	  ctx.begin_numeric_array(filename,numChar);
	  break;
	
	default:
	  {
	    std::cout << "ERROR in " << __FUNCTION__ << " in " << __FILE__ << ":" << __LINE__;
	    {
	  std::stringstream ss;
	  ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
	  throw std::logic_error(ss.str());
	}
	  }
	}
    
      buffer.clear();
    }

    template<typename context_type>
    void JSON_parser<context_type>::end_entity(JSON_action_type& action)
    {
      switch(action) 
	{	
	case EndObject:
	  unloadBuffer();
	  ctx.end_object();
	  break;
	
	case EndArray:
	  unloadBuffer();
	  ctx.end_array();
	  break;
	
	case EndMatrix:
	  ctx.end_numeric_array(numChar);
	  buffer.clear();
	  break;
	
	default:
	  {
	    std::cout << "ERROR in " << __FUNCTION__ << " in " << __FILE__ << ":" << __LINE__;
	    {
	  std::stringstream ss;
	  ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
	  throw std::logic_error(ss.str());
	}
	  }    
	}
    }

    template<typename context_type>
    void JSON_parser<context_type>::record_entity(JSON_action_type& action)
    {
      switch(action) 
	{		
	case RecordString:
	  current_value_type = JSON_T_STRING;
	  break;
	
	case RecordKey:
	  current_value_type = JSON_T_NONE;
	  ctx.Key(buffer.str());
	  buffer.clear();
	  break;
	
	case RecordFloat:
	  current_value_type = JSON_T_FLOAT;
	  ctx.Float(buffer);
	  buffer.clear();
	  break;
	  
	case RecordInteger:
	  current_value_type = JSON_T_INTEGER;
	  ctx.Integer(buffer);
	  buffer.clear();
	  break;
	  
	case RecordTrue:
	  current_value_type = JSON_T_TRUE;
	  ctx.True();
	  break;
	
	case RecordFalse:
	  current_value_type = JSON_T_FALSE;
	  ctx.False();
	  break;
	  
	case RecordNull:
	  current_value_type = JSON_T_NULL;
	  ctx.Null();
	  break;

	default:
	  {
	    std::cout << "ERROR in " << __FUNCTION__ << " in " << __FILE__ << ":" << __LINE__;
	    {
	  std::stringstream ss;
	  ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
	  throw std::logic_error(ss.str());
	}
	  }    
	}
    }

    template<typename context_type>
    void JSON_parser<context_type>::abort_parsing(std::string                   nextChar, 
						  JSON_character_class_type nextClass, 
						  JSON_state_type           state,
						  JSON_action_type          action)
    {
      if(verbose)
	{
	  std::cout << "JsonParser::performAction was sent abort from JSON_parser : \n"
		    << "  nextChar   = '" << nextChar << "'\n" 
		    << "  nextClass  = "  << name(nextClass) << "\n"
		    << "  state      = "  << name(state) << "\n"
		    << "  action     = "  << name(action) << "\n"
		    << "  character# = " << numChar << "\n"
		    << "  line#      = " << numLines << "\n";
	  
	  std::cout << "\n\t the error was recorded in the buffer : " << buffer.to_string() << "\n\n";
	}
      
      {
	  std::stringstream ss;
	  ss << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << std::endl;
	  throw std::logic_error(ss.str());
	}
    }

    template<typename context_type>
    bool JSON_parser<context_type>::execute(std::istringstream& inputStream) 
    {
      std::pair<std::string, JSON_character_class_type> next = get_next_character_and_class(inputStream);
      //std::cout << next.first << "\t" << name(next.second) << "\n";

      std::string&               nextChar (next.first);
      JSON_character_class_type& nextClass(next.second);

      state_and_action_pair pair = JSON_translation_table::get_state_and_action_pair(state, nextClass);
    
      std::vector<JSON_action_type>& actions = pair.actions;
    
      //if(trace) 
      //std::cout << "actions.size() = " << actions.size() << std::endl;
    
      for(size_t i=0; i<actions.size(); i++) 
    	{
    	  performAction(nextChar, nextClass, state, actions[i]);
	  
    	  if(actions[i] == EndFile) 
    	    return false;
    	}
      
      state = pair.newState;
    
      return !inputStream.eof();
    }

    template<typename context_type>
    bool JSON_parser<context_type>::execute(std::istream& inputStream) 
    {
      std::pair<std::string,JSON_character_class_type> next = get_next_character_and_class(inputStream);

      std::string&                   nextChar (next.first);
      JSON_character_class_type& nextClass(next.second);
    
      state_and_action_pair pair = JSON_translation_table::get_state_and_action_pair(state, nextClass);
    
      std::vector<JSON_action_type>& actions = pair.actions;

      /*
      if(trace) 
	{
	  std::cout << "\t" << nextChar 
		    << "\t" << nextClass 
		    << "\tactions.size() = " << actions.size() << "\n";
	}
      */

      for(size_t i=0; i<actions.size(); i++) 
	{
	  performAction(nextChar, nextClass, state, actions[i]);
	
	  if(actions[i] == EndFile) 
	    return false;
	}
      
      state = pair.newState;
    
      return !inputStream.eof();
    }
  
    template<typename context_type>
    std::pair<std::string, JSON_character_class_type> JSON_parser<context_type>::get_next_character_and_class(std::istringstream& inputStream) 
    {
      std::pair<std::string, JSON_character_class_type> result;

      std::string                prevChar  = result.first;
      std::string&               nextChar  = result.first;
      JSON_character_class_type& nextClass = result.second;

      do
    	{ 
    	  if(inputStream.eof()) 
    	    {
    	      nextChar  =   EOF;
    	      nextClass = C_EOF;
	    
    	      return result;
    	    }

	  int ind = inputStream.get();
	  
    	  if(ind==-1)
    	    {
    	      nextChar  =   EOF;
    	      nextClass = C_EOF;
	    
    	      return result;
    	    }
      
    	  nextChar = ind;
    	  numChar++;

	  //std::cout << "\t nextChar: " << nextChar << "\t" << name(this->state) << "\n";

    	  if(nextChar == "\n")
	    {
	      //std::cout << "\t --> nextChar equal to \"\\n\" \n";
	      numLines++;
	    }
	  
	  bool escaped_char=false;
    	  if(nextChar == "\\" and state == ST)  
    	    {
	      escaped_char=true;
	      
	      //std::cout << "\t --> nextChar --> get_escaped_character!  \n";
	      
    	      nextChar = get_escaped_character(inputStream);
	      //std::cout << nextChar;
	      
    	      numChar++;
	      
	      //std::cout << "\t nextChar: " << nextChar << "\n";
    	    }
	  
	  /*
	    std::cout << "prevChar == \"\\\" --> " << (prevChar == '\\') << "\n";
	    std::cout << "nextChar == \"\"\" --> " << (nextChar == "\"") << "\n";
	    std::cout << "state == ST        --> " << (state == ST)      << "\n";
	  */
	  
	  // if you have an escaped quote within a string, then don't end the string!!
	  if(escaped_char and nextChar == "\"" and state == ST)  	  
	    {
	      nextChar  = "\\\"";
	      nextClass = C_ETC;
	    }
	  else if(escaped_char and nextChar == "\n" and state == ST)  	  
	    {
	      nextChar  = "\\n";
	      nextClass = C_ETC;
	    }
	  else if(escaped_char and nextChar == "\t" and state == ST)  	  
	    {
	      nextChar  = "\\t";
	      nextClass = C_ETC;
	    }
	  else if(escaped_char and nextChar == "\b" and state == ST)  	  
	    {
	      nextChar  = "\\b";
	      nextClass = C_ETC;
	    }
	  else if(escaped_char and nextChar == "\r" and state == ST)  	  
	    {
	      nextChar  = "\\r";
	      nextClass = C_ETC;
	    }	  
	  else
	    {
	      nextClass = map_char_to_class(nextChar);
	    }
	  
	  //std::cout << "\t nextClass: " << nextClass << "\t" << name(nextClass) << "\n";

    	  if(state == ST || comment) 
    	    return result; 
    	}
      while(is_white_space(nextClass)); 
    
      return result;
    }

    template<typename context_type>
    std::pair<std::string, JSON_character_class_type> JSON_parser<context_type>::get_next_character_and_class(std::istream& inputStream) 
    {
      std::pair<std::string,JSON_character_class_type> result;

      std::string&               nextChar  = result.first;
      JSON_character_class_type& nextClass = result.second;

      do
	{ 
	  if(inputStream.eof()) 
	    {
	      nextChar  =   EOF;
	      nextClass = C_EOF;
	    
	      return result;
	    }
      
	  nextChar = inputStream.get();
	  numChar++;

	  /*
	  if(nextChar<0)
	    {
	      std::cout << "\t character on line " << numLines <<  " is not recognized!\n";
	      nextChar='?';
	    }
	  */

	  if(nextChar == "\n")
	    {
	      numLines++;
	    }

	  bool escaped_char=false;
	  if(nextChar == "\\" and state == ST)  
	    {
	      escaped_char=true;

	      nextChar = get_escaped_character(inputStream);
	      numChar++;
	    }
      
	  // if you have anescaped quote within a string, then don't end the string!!
	  if(escaped_char and nextChar == "\"" and state == ST)  	  
	    nextClass = C_ETC;
	  else
	    nextClass = map_char_to_class(nextChar);

	  if(state == ST || comment) 
	    return result; 
      
	  //if(nextChar<0)
	  //nextClass = C_WHITE;

	  if(nextChar.size()==1 and nextChar[0]<0)
	    nextClass = C_WHITE;
	}
      while(is_white_space(nextClass)); 
    
      return result;
    }
  
  }

}

#endif
