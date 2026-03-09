#include "liter.hpp"

template< class T >
hvostov::Liter< T >& hvostov::Liter< T >::operator++()
{
  curr_ = curr_->next;
  return curr_;
}

template< class T >
T hvostov::Liter< T >::operator*()
{
  return curr_->val_;
}

template< class T >
T hvostov::Liter< T >::operator->()
{
  return curr_->val_;
}

template< class T >
bool hvostov::Liter< T >::operator==(const Liter< T >& liter)
{
  return curr_ == liter;
}

template< class T >
bool hvostov::Liter< T >::operator!=(const Liter< T >& liter)
{
  return curr_ != liter;
}
