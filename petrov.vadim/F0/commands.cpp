#include "commands.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <utility>

#include "help_functions.hpp"

void petrov::FuncManage::mk_user(std::ostream&, std::istream& in, const std::string& str)
{
  size_t h, w, o;
  in >> h >> w >> o;
  if (!in)
  {
    throw std::runtime_error("Read invalid");
  }

  if (users_.has(str))
  {
    throw std::runtime_error("User invalid");
  }

  using plans_t = KukuHashTable< User::train_ex_t, std::string, KKHash< std::string >, Equal< std::string > >;
  User u = User{h, w, o, detail::userStamina(h, w, o), plans_t()};
  users_.add(str, u);
}

void petrov::FuncManage::rm_user(std::ostream&, std::istream&, const std::string& str)
{
  if (!users_.has(str))
  {
    throw std::runtime_error("User invalid");
  }

  users_.drop(str);
}

void petrov::FuncManage::update_user(std::ostream&, std::istream& in, const std::string& str)
{
  size_t h, w, o;
  in >> h >> w >> o;
  if (!in)
  {
    throw std::runtime_error("Read invalid");
  }

  if (!users_.has(str))
  {
    throw std::runtime_error("User invalid");
  }

  User& u = users_.get(str);
  u.height_ = h;
  u.weight_ = w;
  u.old_ = o;
  u.stamina_ = petrov::detail::userStamina(h, w, o);
}

void petrov::FuncManage::show_users(std::ostream& out, std::istream&, const std::string&)
{
  out << "<USERS>\n";
  if (users_.empty())
  {
    return;
  }

  for (auto it = users_.cbegin(); it != users_.cend(); ++it)
  {
    out << it->key_ << '\n'
    << "Height: " << it->value_.height_ << '\n'
    << "Weight: " << it->value_.weight_ << '\n'
    << "Old: " << it->value_.old_ << '\n';

    if (!it->value_.plans_.empty())
    {
      out << "<PLANS>\n";
    }
    for (auto plan_it = it->value_.plans_.cbegin(); plan_it != it->value_.plans_.cend(); ++plan_it)
    {
      out << plan_it->key_ << '\n';

      auto ex_it = plan_it->value_.cbegin();
      if (!plan_it->value_.empty())
      {
        out << ex_it->key_ << ' '
            << ex_it->value_.muscleGroup_ << ' '
            << ex_it->value_.reps_ << ' '
            << ex_it->value_.sets_;
        ++ex_it;
      }
      for (; ex_it != plan_it->value_.cend(); ++ex_it)
      {
        out << '\n'
            << ex_it->key_ << ' '
            << ex_it->value_.muscleGroup_ << ' '
            << ex_it->value_.reps_ << ' '
            << ex_it->value_.sets_;
      }
      out << '\n';
    }
  }
}

void petrov::FuncManage::save(std::ostream&, std::istream&, const std::string& str)
{
  std::ofstream file(str);
  if (!file.is_open())
  {
    throw std::runtime_error("File invalid");
  }

  file << users_.size() << '\n';
  for (auto it = users_.cbegin(); it != users_.cend(); ++it)
  {
    file << it->key_ << '\n';
    file << it->value_.height_ << ' ' << it->value_.weight_ << ' ' << it->value_.old_ << '\n';

    file << it->value_.plans_.size() << '\n';
    auto plans_it = it->value_.plans_.cbegin();
    for (; plans_it != it->value_.plans_.cend(); ++plans_it)
    {
      file << plans_it->key_ << ' ' << plans_it->value_.size() << '\n';
      auto ex_it = plans_it->value_.cbegin();
      if (plans_it->value_.size() > 0)
      {
        file << ex_it->key_ << ' '
        << ex_it->value_.sets_ << ' '
        << ex_it->value_.reps_ << ' '
        << ex_it->value_.muscleGroup_ << ' '
        << ex_it->value_.stamina_;
        ++ex_it;
      }

      for (; ex_it != plans_it->value_.cend(); ++ex_it)
      {
        file << ' ' << ex_it->key_
        << ' ' << ex_it->value_.sets_
        << ' ' << ex_it->value_.reps_
        << ' ' << ex_it->value_.muscleGroup_
        << ' ' << ex_it->value_.stamina_;
      }
      file << '\n';
    }
  }

  file << pools_.size() << '\n';
  auto pool_it = pools_.cbegin();
  for (; pool_it != pools_.cend(); ++pool_it)
  {
    file << pool_it->key_ << ' ' << pool_it->value_.size() << '\n';
    auto ex_it = pool_it->value_.cbegin();
    if (pool_it->value_.size() > 0)
    {
      file << ex_it->key_ << ' '
      << ex_it->value_.muscleGroup_ << ' '
      << ex_it->value_.stamina_;
      ++ex_it;
    }

    for (; ex_it != pool_it->value_.cend(); ++ex_it)
    {
      file << ' ' << ex_it->key_
      << ' ' << ex_it->value_.muscleGroup_
      << ' ' << ex_it->value_.stamina_;
    }
    file << '\n';
  }
}

void petrov::FuncManage::load(std::ostream&, std::istream&, const std::string& str)
{
  std::ifstream file(str);
  if (!file.is_open())
  {
    throw std::runtime_error("Load invalid: not open");
  }

  size_t users_count;
  if (!(file >> users_count))
  {
    throw std::runtime_error("Load invalid: file read");
  }

  for (size_t user_i = 0; user_i < users_count; ++user_i)
  {
    std::string name;
    size_t h, w, o, plan_count;
    if (!(file >> name >> h >> w >> o >> plan_count))
    {
      throw std::runtime_error("Load invalid: file read");
    }

    KukuHashTable< User::train_ex_t, std::string, KKHash< std::string >, Equal< std::string > > pl(plan_count);
    for (size_t plan_i = 0; plan_i < plan_count; ++plan_i)
    {
      std::string plan_name;
      size_t ex_count;
      if (!(file >> plan_name >> ex_count))
      {
        throw std::runtime_error("Load invalid: file read");
      }

      User::train_ex_t ex(ex_count);
      for (size_t ex_i = 0; ex_i < ex_count; ++ex_i)
      {
        std::string ex_name, mg;
        size_t sets, reps;
        int st;
        if (!(file >> ex_name >> sets >> reps >> mg >> st))
        {
          throw std::runtime_error("Load invalid: file read");
        }

        ex.add(ex_name, UExercise(mg, st, sets, reps));
      }

      pl.add(plan_name, ex);
    }

    users_.add(name, {h, w, o, detail::userStamina(h, w, o), pl});
  }

  size_t pools_count;
  if (!(file >> pools_count))
  {
    throw std::runtime_error("Load invalid: file read");
  }

  for (size_t pool_i = 0; pool_i < pools_count; ++pool_i)
  {
    size_t ex_count;
    std::string pool_name;
    if (!(file >> pool_name >> ex_count))
    {
      throw std::runtime_error("Load invalid: file read");
    }

    trainPool_t pool(ex_count);
    for (size_t ex_i = 0; ex_i < ex_count; ++ex_i)
    {
      std::string ex_name, mg;
      int st;
      if (!(file >> ex_name >> mg >> st))
      {
        throw std::runtime_error("Load invalid: file read");
      }

      pool.add(ex_name, {mg, st});
      if (!exercisesPool_.has(ex_name))
      {
        exercisesPool_.add(ex_name, {mg, st});
      }
    }
    pools_.add(pool_name, pool);
  }
}

void petrov::FuncManage::train(std::ostream&, std::istream& in, const std::string& str)
{
  std::string res_plan_name = str;
  std::string basic_pool, mg, username;
  double stamina_k;
  in >> basic_pool >> stamina_k >> mg >> username;
  if (!in)
  {
    throw std::runtime_error("Read invalid");
  }

  if (!users_.has(username) || !pools_.has(basic_pool) || stamina_k < 0 || stamina_k > 1)
  {
    throw std::runtime_error("Train invalid");
  }
  User& u = users_.get(username);
  int w = static_cast< int >(u.stamina_ * stamina_k);
  trainPool_t& source_pool = pools_.get(basic_pool);

  topit::Vector< std::pair< size_t, size_t > > schemes;
  schemes.pushBack({3, 10});
  schemes.pushBack({4, 12});
  schemes.pushBack({5, 10});
  topit::Vector< ExerciseGroup > groups;

  for (auto it = source_pool.cbegin(); it != source_pool.cend(); ++it)
  {
    ExerciseGroup group;
    group.name_ = it->key_;
    group.mg_ = it->value_.muscleGroup_;

    for (const auto& scheme : schemes)
    {
      size_t s = scheme.first;
      size_t r = scheme.second;
      int cost = detail::exStamina(it->value_.stamina_, s, r);

      if (cost <= w && cost > 0)
      {
        int value = cost;
        if (it->value_.muscleGroup_ == mg)
        {
          value += 10000;
        }
        group.variants_.pushBack({cost, s, r, value});
      }
    }

    if (!group.variants_.isEmpty())
    {
      groups.pushBack(group);
    }
  }
  if (groups.isEmpty())
  {
    throw std::runtime_error("Train invalid");
  }

  size_t items_count = groups.getSize();
  topit::Vector< topit::Vector< int > > dp(items_count + 1, topit::Vector< int >(w + 1, 0));
  topit::Vector< topit::Vector< int > > choice(items_count + 1, topit::Vector< int >(w + 1, -1));
  for (size_t i = 1; i <= items_count; ++i)
  {
    const ExerciseGroup& group = groups[i - 1];
    for (int j = 0; j <= w; ++j)
    {
      dp[i][j] = dp[i - 1][j];
      choice[i][j] = -1;
      for (size_t v = 0; v < group.variants_.getSize(); ++v)
      {
        int weight = group.variants_[v].cost_;
        int value = group.variants_[v].value_;

        if (weight <= j)
        {
          int current_val = dp[i - 1][j - weight] + value;
          if (current_val > dp[i][j])
          {
            dp[i][j] = current_val;
            choice[i][j] = static_cast<int>(v);
          }
        }
      }
    }
  }

  User::train_ex_t new_plan(16);
  int current_w = w;
  for (size_t i = items_count; i > 0; --i)
  {
    int v = choice[i][current_w];
    if (v != -1)
    {
      const ExerciseGroup& group = groups[i - 1];
      const Variant& chosen = group.variants_[v];
      UExercise u_ex(group.mg_, chosen.cost_, chosen.sets_, chosen.reps_);
      new_plan.add(group.name_, u_ex);
      current_w -= chosen.cost_;
    }
  }
  u.plans_.add(res_plan_name, new_plan);
}

void petrov::FuncManage::mk_ex(std::ostream&, std::istream& in, const std::string& str)
{
  std::string mg;
  int st;
  in >> mg >> st;
  if (!in || st < 1 || exercisesPool_.has(str))
  {
    throw std::runtime_error("Make exercise invalid");
  }

  Exercise ex{mg, st};
  exercisesPool_.add(str, ex);
}

void petrov::FuncManage::rm_ex(std::ostream&, std::istream&, const std::string& str)
{
  if (!exercisesPool_.has(str))
  {
    throw std::runtime_error("Remove exercise invalid");
  }

  exercisesPool_.drop(str);
}

void petrov::FuncManage::show_ex(std::ostream& out, std::istream&, const std::string&)
{
  out << "<EXERCISE LIST>\n";
  if (exercisesPool_.empty())
  {
    return;
  }

  auto ex_it = exercisesPool_.cbegin();
  out << ex_it->key_ << ' '
      << ex_it->value_.muscleGroup_ << ' '
      << ex_it->value_.stamina_;
  ++ex_it;

  for (; ex_it != exercisesPool_.cend(); ++ex_it)
  {
    out << '\n'
        << ex_it->key_ << ' '
        << ex_it->value_.muscleGroup_ << ' '
        << ex_it->value_.stamina_;
  }
  out << '\n';
}

void petrov::FuncManage::add_ex_pool(std::ostream&, std::istream& in, const std::string& str)
{
  std::string pool_name = str, ex_name;
  in >> ex_name;
  if (!in || !exercisesPool_.has(ex_name))
  {
    throw std::runtime_error("Add exercise in pool invalid");
  }

  if (!pools_.has(pool_name))
  {
    pools_.add(pool_name, trainPool_t());
  }

  trainPool_t& p = pools_.get(pool_name);
  Exercise ex = exercisesPool_.get(ex_name);
  p.add(ex_name, ex);
}

void petrov::FuncManage::rm_pool(std::ostream&, std::istream&, const std::string& str)
{
  if (!pools_.has(str))
  {
    throw std::runtime_error("Remove pool invalid");
  }

  pools_.drop(str);
}

void petrov::FuncManage::show_pools(std::ostream& out, std::istream&, const std::string&)
{
  out << "<POOLS LIST>\n";
  if (pools_.empty())
  {
    return;
  }

  for (auto pool_it = pools_.cbegin(); pool_it != pools_.cend(); ++pool_it)
  {
    out << pool_it->key_ << '\n';
    for (auto ex_it = pool_it->value_.cbegin(); ex_it != pool_it->value_.cend(); ++ex_it)
    {
      out << ex_it->key_ << ' '
      << ex_it->value_.muscleGroup_ << ' '
      << ex_it->value_.stamina_ << '\n';
    }
  }
}
