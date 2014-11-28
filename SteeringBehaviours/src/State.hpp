#ifndef STATE_HPP
#define STATE_HPP

template<class entity_type>
struct State
{
public:
  virtual ~State(){}

  //this will execute when the state is entered
  virtual void enter(entity_type*)=0;

  //this is the states normal update function
  virtual void execute(entity_type*)=0;

  //this will execute when the state is exited. (My word, isn't
  //life full of surprises... ;o))
  virtual void exit(entity_type*)=0;
};

#endif // STATE_HPP
