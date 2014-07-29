#ifndef DILAY_HISTORY
#define DILAY_HISTORY

#include "action/transformer.hpp"

class Action;
class WingedMesh;

class History {
  public: History            ();
          History            (const History&) = delete;
    const History& operator= (const History&) = delete;
         ~History            ();

    template <typename A>
    A& add () { 
      A& action = *new A ();
      this->addAction (action); 
      return action; 
    }

    template <typename A, typename T>
    A& add (T& t) { 
      A& action = *new A ();
      this->addAction (*new ActionTransformer <T> (t, action));
      return action; 
    }

    void addAction (Action&);
    void reset     ();
    void undo      ();
    void redo      ();
  private:
    class Impl;
    Impl* impl;
};

#endif
