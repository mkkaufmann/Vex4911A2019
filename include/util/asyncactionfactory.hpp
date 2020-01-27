#ifndef ASYNC_ACTION_FACTORY_HPP
#define ASYNC_ACTION_FACTORY_HPP
#include "asyncaction.hpp"
// creates AsyncActions
class AsyncActionFactory {
public:
  // creates an async action
  static AsyncAction *makeAsyncAction();

private:
};
#endif
