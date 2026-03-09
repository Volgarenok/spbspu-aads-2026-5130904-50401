#include <iostream>
#include <string>

namespace petrov
{
  template<class T>
  class List;
  template<class T>
  class LIter
  {
    List<T>* curr;
    LIter(List<T>* p) : curr(p)
    {}  
  public:
    T& operator*()
    {
      return curr->val;
    }
    LIter& operator++()
    {
      curr = curr->next;
      return *this;
    }
    bool operator==(const LIter& LIt) const
    {
      return curr == LIt.curr;
    }
    bool operator!=(const LIter& LIt) const
    {
      return curr != LIt.curr;
    }
    bool operator!=(LIter& LIt)
    {
      return curr != LIt.curr;
    }
    bool operator==(LIter& LIt)
    {
      return curr == LIt.curr;
    }
    friend class List<T>;
  };
    template<class T>
  class List
  {
    public:
      T val;
      List<T>* next;
      LIter<T> begin()
      {
        return LIter<T>(this);
      }
      LIter<T> end()
      {
        return LIter<T>(static_cast<List<T>*>(nullptr));
      }
  };
}

int string_to_int(std::string string_to_record)
{
  int c = 0;
  size_t i = 0;
  while (string_to_record[i] != '\0')
  {
    c = c*10 + (string_to_record[i] - 48);
    i++;
  }
  return c;
}

int main()
{
  petrov::List<std::pair<std::string, petrov::List<int>*>> data;
  data.next = nullptr;
  petrov::List<std::pair<std::string, petrov::List<int>*>>* data_ptr = &data;
  std::string string_to_record;
  petrov::List<int>* numbers_ptr = nullptr;
  while (std::cin >> string_to_record)
  {
     if (!(string_to_record[0] > 47 && string_to_record[0] < 59))
    {
      petrov::List<std::pair<std::string, petrov::List<int>*>>* new_data_ptr = new petrov::List<std::pair<std::string, petrov::List<int>*>>;
      new_data_ptr->next = nullptr;
      new_data_ptr->val.first = string_to_record;
      new_data_ptr->val.second = nullptr;
      data_ptr->next = new_data_ptr;
      data_ptr = new_data_ptr;
      numbers_ptr = nullptr; 
    }
    else
    {
      petrov::List<int>* new_numbers_ptr = new petrov::List<int>;
      new_numbers_ptr->val = string_to_int(string_to_record);
      new_numbers_ptr->next = nullptr;
      if (data_ptr->val.second == nullptr)
      {
        data_ptr->val.second = new_numbers_ptr;
      }
      else
      {
        numbers_ptr->next = new_numbers_ptr;
      }
      numbers_ptr = new_numbers_ptr;
    }
  }
  petrov::List<std::pair<std::string, petrov::List<int>*>>* data_ptr_2 = &data;
  while (data_ptr_2 != nullptr)
  {
    std::cout << data_ptr_2->val.first << "\n";
    petrov::List<int>* numbers_ptr_2 = data_ptr_2->val.second;
    while (numbers_ptr_2 != nullptr)
    {
      std::cout << numbers_ptr_2->val << "\n";
      numbers_ptr_2 = numbers_ptr_2->next;
    }
    data_ptr_2 = data_ptr_2->next;
  }
}

