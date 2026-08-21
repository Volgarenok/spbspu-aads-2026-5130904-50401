#ifndef LOGISTICREGRESSION_HPP
#define LOGISTICREGRESSION_HPP
#include "list.hpp"
#include <fstream>
#include <string>
#include <cmath>
#include "HashTable.hpp"
#include "pandas.hpp"

namespace petrov
{
  struct ListPairHashl
  {
    size_t operator()(const std::pair<List<double>, List<double>>& p) const
    {
      size_t hash = 0;
      auto hash_comb = [&hash](double v)
      {
        hash ^= std::hash<double>{}(v) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
      };
      for (auto it = p.first.begin(); it != p.first.end(); ++it)
      {
        hash_comb(*it);
      }
      for (auto it = p.second.begin(); it != p.second.end(); ++it)
      {
        hash_comb(*it);
      }
      return hash;
    }
  };
  struct ListPairEquall
  {
    bool operator()(const std::pair<List<double>, List<double>>& a, const std::pair<List<double>, List<double>>& b) const
    {
      return iseq(a.first, b.first) && iseq(a.second, b.second);
    }
  };
  struct LLListPairHashl
  {
    size_t operator()(const std::pair<List<List<double>>, List<double>>& p) const
    {
      size_t hash = 0;
      auto hash_comb = [&hash](double v)
      {
        hash ^= std::hash<double>{}(v) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
      };
      for (auto it = p.first.begin(); it != p.first.end(); ++it)
      {
        for (auto iit = (*it).begin(); iit != (*it).end(); ++iit)
        {
          hash_comb(*iit);
        }
      }
      for (auto it = p.second.begin(); it != p.second.end(); ++it)
      {
        hash_comb(*it);
      }
      return hash;
    }
  };
  struct LLListPairEquall
  {
    bool operator()(const std::pair<List<List<double>>, List<double>>& a, const std::pair<List<List<double>>, List<double>>& b) const
    {
      return iseq(a.first, b.first) && iseq(a.second, b.second);
    }
  };
  struct LogisticRegression
  {
    LogisticRegression()
    {
      W.push_back(0.1);
    }
    void add_we(double we)
    {
      W.push_back(we);
    }
    void Log_loss_model_init(dataset data)
    {
      while (W.getSize())
      {
        W.pop_back();
      }
      if (data.getC())
      {
        double wd = 0.1;
        while(W.getSize() < data.getC() - 1)
        {
          W.push_back(wd);
          wd += 0.1;
        }
      }
      else
      {
        throw std::logic_error("err");
      }
    }
    double model_predict(List<double> d)
    {
      if (h1.find({W, d}) != h1.end())
      {
        return h1.find({W, d})->second;
      }
      if (h1.find({d, W}) != h1.end())
      {
        return h1.find({d, W})->second;
      }
      if (d.getSize() == W.getSize())
      {
        double out = 0;
        LIter<double> di = d.begin();
        LIter<double> wi = W.begin();
        while (di != d.end())
        {
          out += (*di) * (*wi);
          ++di;
          ++wi;
        }
        h1.add({d, W}, 1/(1 + exp(-out)));
        return 1/(1 + exp(-out));
      }
      else
      {
        throw std::logic_error("err\n");
      }
    }
    List<double> model_predict(List<List<double>> inp)
    {
      if (h2.find({inp, W}) != h2.end())
      {
        return h2.find({inp, W})->second;
      }
      List<double> pred;
      for (LIter<List<double>> ii = inp.begin(); ii != inp.end(); ++ii)
      {
        LIter<double> wi = W.begin();
        double ou = 0;
        for (LIter<double> i1 = (*ii).begin(); i1 != (*ii).end(); ++i1, ++wi)
        {
          ou += (*i1) * (*wi);
        }
        pred.push_back(1/(1 + exp(-ou)));
      }
      h2.add({inp, W}, pred);
      return pred;
    }
    void model_fit(List<double> inp, double g, double pred, double a)
    {
      LIter<double> wi = W.begin();
      for (LIter<double> ii = inp.begin(); ii != inp.end(); ++ii)
      {
        (*wi) -= a * ((1 / (1 + exp(-(*ii) * (*wi)))) - g) * (*ii);
        ++wi;
      }
    }
    void model_fit(List<List<double>> inp, List<double> g, double a)
    {
      LIter<double> gi = g.begin();
      for (LIter<List<double>> ii = inp.begin(); ii != inp.end(); ++ii, ++gi)
      {
        LIter<double> wi = W.begin();
        for (LIter<double> i1 = (*ii).begin(); i1 != (*ii).end(); ++i1, ++wi)
        {
          (*wi) -= a * ((1 /1 + exp((-1) * (*i1) * (*wi))) - (*gi)) * (*i1);
        }
      }
    }
    void save_predict(std::istream& in, double pred)
    {
      std::ofstream out;
      std::string s;
      in >> s;
      bool isnorm = 0;
      for (size_t i = 0; i < s.size(); ++i)
      {
        if (s[i] == '.')
        {
          isnorm = 1;
          break;
        }
      }
      if (!isnorm)
      {
        out.open(s + ".predict");
      }
      else
      {
        out.open(s);
      }
      out << pred;
      out.close();
    }
    void save_model(std::istream& in)
    {
      std::ofstream out;
      std::string s;
      in >> s;
      bool isnorm = 0;
      for (size_t i = 0; i < s.size(); ++i)
      {
        if (s[i] == '.')
        {
          isnorm = 1;
          break;
        }
      }
      if (!isnorm)
      {
        out.open(s + ".model");
      }
      else
      {
        out.open(s);
      }
      for (LIter<double> wi = W.begin(); wi != W.end(); ++wi)
      {
        out << (*wi) << '\n';
      }
      out.close();
    }
    void save_model(std::string s)
    {
      std::ofstream out;
      bool isnorm = 0;
      for (size_t i = 0; i < s.size(); ++i)
      {
        if (s[i] == '.')
        {
          isnorm = 1;
          break;
        }
      }
      if (!isnorm)
      {
        out.open(s + ".model");
      }
      else
      {
        out.open(s);
      }
      for (LIter<double> wi = W.begin(); wi != W.end(); ++wi)
      {
        out << (*wi) << '\n';
      }
      out.close();
    }
    void load_model(std::istream& in)
    {
      std::ifstream i;
      std::string s;
      in >> s;
      i.open(s);
      LIter<double> wi = W.begin();
      for (; W.getSize(); ++wi)
      {
        W.pop_back();
      }
      for (; !i.eof();)
      {
        double q;
        i >> q;
        W.push_back(q);
      }
      W.pop_back();
    }
    void load_model(std::string s)
    {
      std::ifstream i;
      i.open(s);
      LIter<double> wi = W.begin();
      for (; W.getSize(); ++wi)
      {
        W.pop_back();
      }
      for (; !i.eof();)
      {
        double q;
        i >> q;
        W.push_back(q);
      }
      W.pop_back();
    }
    void model_info(std::ostream& out)
    {
      for (LIter<double> wi = W.begin(); wi != W.end(); ++wi)
      {
        out << (*wi) << ' ';
      }
    }
    void show_predict(List<double> inp)
    {
      double ou = 0;
      for (LIter<double> wi = W.begin(), ii = inp.begin(); wi != W.end(); ++wi, ++ii)
      {
        ou += (*wi) * (*ii);
      }
      std::cout << ou;
    }
    size_t getWsize()
    {
      return W.getSize();
    }
    private:
      List<double> W;
      HashTable<std::pair<List<double>, List<double>>, double, ListPairHashl, ListPairEquall> h1;
      HashTable<std::pair<List<List<double>>, List<double>>, List<double>, LLListPairHashl, LLListPairEquall> h2;
  };
}
#endif
