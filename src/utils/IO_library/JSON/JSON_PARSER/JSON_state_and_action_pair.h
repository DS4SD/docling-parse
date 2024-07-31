//-*-C++-*-

#ifndef JSON_STATE_AND_ACTION_PAIR_H
#define JSON_STATE_AND_ACTION_PAIR_H

namespace IO
{
  namespace JSONPARSER
  {
    class state_and_action_pair//:
    //     public JSON_state// ,
    //     public ActionsMixin
    {    
    public:

      state_and_action_pair(const state_and_action_pair& other);
    
      state_and_action_pair(JSON_state_type s, JSON_action_type a);
    
      state_and_action_pair(JSON_state_type s, JSON_action_type a, JSON_action_type b);

      ~state_and_action_pair();

    public:

      JSON_state_type               newState;
      std::vector<JSON_action_type> actions;
    
    };

    state_and_action_pair::state_and_action_pair(const state_and_action_pair& other):
      newState(other.newState),
      actions(other.actions)
    {}
    
    state_and_action_pair::state_and_action_pair(JSON_state_type s, JSON_action_type a):
      newState(s),
      actions(1)
    {
      actions[0] = a;
    }
  
    state_and_action_pair::state_and_action_pair(JSON_state_type s, JSON_action_type a, JSON_action_type b):
      newState(s),
      actions(2)
    {
      actions[0] = a;
      actions[1] = b;
    }
 
    state_and_action_pair::~state_and_action_pair()
    {}
  }

}

#endif
