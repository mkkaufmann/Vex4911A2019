#include "asyncactionfactory.hpp"
#include "asyncaction.hpp"

AsyncAction* AsyncActionFactory::makeAsyncAction(){
  AsyncAction* action;
  action = new AsyncAction;
  *action = AsyncAction();
  return action;
}
