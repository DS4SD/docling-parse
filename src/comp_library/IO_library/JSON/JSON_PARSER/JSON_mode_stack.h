//-*-C++-*-

#ifndef JSONPARSER_MODESMIXING_H
#define JSONPARSER_MODESMIXING_H

namespace IO
{
  namespace JSONPARSER
  {
    class JSON_mode_stack 
    {
    public:
    
      JSON_mode_stack();
      ~JSON_mode_stack();

      std::string modeName(JSON_mode_type m);

      void push(JSON_mode_type mode);

      void pop(JSON_mode_type expectedMode);

      const JSON_mode_type& currentMode();

    public:
    
      std::vector<JSON_mode_type> stack;
    };

    JSON_mode_stack::JSON_mode_stack():
      stack(1,MODE_DONE)
    {}

    JSON_mode_stack::~JSON_mode_stack()
    {}
    
    std::string JSON_mode_stack::modeName(JSON_mode_type m) 
    {
      switch (m) 
	{
	case MODE_ARRAY:
	  return "MODE_ARRAY";
	case MODE_DONE:
	  return "MODE_DONE";
	case MODE_KEY:
	  return "MODE_KEY";
	case MODE_OBJECT:
	  return "MODE_OBJECT";
	default:
	  return "MODE_UNKNOWN";
	}
    }

    void JSON_mode_stack::push(JSON_mode_type mode) 
    {
      stack.push_back(mode);
    }
    
    void JSON_mode_stack::pop(JSON_mode_type expectedMode) 
    {
      if (stack.size() == 0) 
	{
	  std::ostringstream msg;
	  msg << "JsonParser.pop was expecting mode " << modeName(expectedMode) << " to be on the back of the stack. \n"
	      << "However the stack was empty!\n";
	  throw std::logic_error(msg.str());
	}
    
      if (expectedMode != stack.back())
	{
	  std::ostringstream msg;
	  msg << "JsonParser.pop was expecting mode " << modeName(expectedMode) << " to be on the back of the stack. \n"
	      << "However the back of the stack contained " << modeName(stack.back()) << "\n";
	  throw std::logic_error(msg.str());
	}
    
      stack.pop_back();
    }
  
    const JSON_mode_type& JSON_mode_stack::currentMode() 
    {
      return stack.back();
    }

  } 

}

#endif

