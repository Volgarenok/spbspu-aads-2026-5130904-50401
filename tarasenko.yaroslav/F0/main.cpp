#include <iostream>
#include <fstream>
#include <istream>
#include <limits>
#include <stdexcept>
#include <string>
#include <sstream>
#include <utility>

#include "analyzer.hpp"
#include "robinHoodTable.hpp"
#include "vector.hpp"
#include "engine.hpp"
#include "game.hpp"
#include "move_generator.hpp"

using games_t = tarasenko::RobinHoodTable< std::string, chess::Game >;

struct SearchParams
{
  int depth = 10;
  int time_ms = 0;
  chess::EvaluationStrategy strategy = chess::EvaluationStrategy::BALANCED;
};

std::string readName(std::istream& in)
{
  std::istream::sentry s(in);
  if (!s)
  {
    throw std::logic_error("invalid arguments");
  }
  std::string name;
  in >> name;
  if (!in)
  {
    throw std::logic_error("invalid arguments");
  }
  return name;
}

bool isSpace(char c)
{
  return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool isNumber(const std::string& str)
{
  if (str.empty())
  {
    return false;
  }
  for (size_t i = 0; i < str.size(); ++i)
  {
    if (str[i] < '0' || str[i] > '9')
    {
      return false;
    }
  }
  return true;
}

int toInt(const std::string& str)
{
  int ans = 0;
  for (size_t i = 0; i < str.size(); ++i)
  {
    ans = ans * 10 + str[i] - '0';
  }
  return ans;
}

int secondsToMs(const std::string& str)
{
  std::stringstream stream(str);
  double seconds = 0;
  char extra = 0;
  stream >> seconds;
  if (!stream || (stream >> extra))
  {
    throw std::logic_error("invalid arguments");
  }
  int ms = static_cast< int >(seconds * 1000);
  if (ms <= 0)
  {
    throw std::logic_error("invalid arguments");
  }
  return ms;
}

bool setSearchLimit(const std::string& word, SearchParams& params)
{
  if (isNumber(word))
  {
    params.depth = toInt(word);
    params.time_ms = 0;
    return true;
  }
  if (word.size() < 2)
  {
    return false;
  }
  std::string value = word.substr(1);
  if (word[0] == 'd')
  {
    if (!isNumber(value))
    {
      throw std::logic_error("invalid arguments");
    }
    int number = toInt(value);
    if (number <= 0)
    {
      throw std::logic_error("invalid arguments");
    }
    params.depth = number;
    params.time_ms = 0;
    return true;
  }
  if (word[0] == 't')
  {
    params.time_ms = secondsToMs(value);
    return true;
  }
  return false;
}

std::pair< std::string, std::string > readFenAndName(std::istream& in)
{
  std::string line;
  std::getline(in, line);
  size_t end = line.find_last_not_of(" \t\r\n");
  if (end == std::string::npos)
  {
    throw std::logic_error("invalid arguments");
  }
  size_t name_start = end;
  while (name_start > 0 && !isSpace(line[name_start - 1]))
  {
    --name_start;
  }
  if (name_start == 0)
  {
    throw std::logic_error("invalid arguments");
  }
  size_t fen_end = line.find_last_not_of(" \t\r\n", name_start - 1);
  if (fen_end == std::string::npos)
  {
    throw std::logic_error("invalid arguments");
  }
  std::string fen = line.substr(0, fen_end + 1);
  std::string name = line.substr(name_start, end - name_start + 1);
  return {fen, name};
}

chess::Game& getGame(games_t& games, const std::string& name)
{
  if (games.count(name) == 0)
  {
    throw std::logic_error("no board with this name");
  }
  return games.at(name);
}

chess::Move readMove(std::istream& in)
{
  std::istream::sentry s(in);
  if (!s)
  {
    throw std::logic_error("invalid move");
  }
  chess::Move move;
  in >> move;
  if (!in)
  {
    throw std::logic_error("invalid move");
  }
  return move;
}

int readDepth(std::istream& in)
{
  std::string word = readName(in);
  if (!isNumber(word))
  {
    throw std::logic_error("invalid arguments");
  }
  int depth = toInt(word);
  if (depth <= 0)
  {
    throw std::logic_error("invalid arguments");
  }
  return depth;
}

SearchParams readSearchParams(std::istream& in)
{
  SearchParams params;
  std::string line;
  std::getline(in, line);
  std::stringstream stream(line);
  std::string first;
  std::string second;
  std::string extra;

  stream >> first;
  if (!stream)
  {
    return params;
  }
  if (setSearchLimit(first, params))
  {
    stream >> second;
    if (stream)
    {
      params.strategy = chess::getEvaluationStrategy(second);
    }
  }
  else
  {
    params.strategy = chess::getEvaluationStrategy(first);
  }
  stream >> extra;
  if (stream || params.depth <= 0 || params.time_ms < 0)
  {
    throw std::logic_error("invalid arguments");
  }
  return params;
}

std::pair< chess::Move, float > searchPosition(chess::Position& pos, const SearchParams& params)
{
  chess::Engine engine(chess::getEvaluationCoefficients(params.strategy));
  return engine.findBestMove(pos, params.depth, nullptr, params.time_ms);
}

void newGame(std::istream& in, std::ostream&, games_t& games)
{
  std::string name = readName(in);
  if (games.count(name))
  {
    throw std::logic_error("board with this name already exist");
  }
  games.insert({name, chess::Game()});
}

void removeGame(std::istream& in, std::ostream&, games_t& games)
{
  std::string name = readName(in);
  if (!games.count(name))
  {
    throw std::logic_error("no board with this name");
  }
  games.erase(name);
}

void setFen(std::istream& in, std::ostream&, games_t& games)
{
  auto parsed = readFenAndName(in);
  std::string fen = parsed.first;
  std::string name = parsed.second;
  if (games.count(name))
  {
    throw std::logic_error("board with this name already exist");
  }
  games.insert({name, chess::Game(chess::Position(fen.c_str()))});
}

void printGame(std::istream& in, std::ostream& out, games_t& games)
{
  std::string name = readName(in);
  getGame(games, name).print(out);
}

void listGames(std::istream&, std::ostream& out, games_t& games)
{
  for (auto it = games.begin(); it != games.end(); ++it)
  {
    out << it->first << "\n";
  }
}

struct MoveIO
{
  chess::Game& game;
};

struct LoadedGame
{
  std::string name;
  chess::Game game;
};

bool isEmptyLine(const std::string& line)
{
  return line.find_first_not_of(" \t\r\n") == std::string::npos;
}

std::istream& operator>>(std::istream& in, MoveIO&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  chess::Move move;
  in >> move;
  if (!in)
  {
    return in;
  }
  chess::MoveArray moves = chess::MoveGenerator::generateLegalMoves(dest.game.getPosition());
  try
  {
    dest.game.makeMove(chess::getMove(moves, move));
  }
  catch (const std::exception&)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

void readMoveLine(const std::string& line, chess::Game& game)
{
  std::stringstream stream(line);
  stream >> MoveIO{game};
  if (!stream)
  {
    throw std::logic_error("invalid file");
  }
  stream >> std::ws;
  if (stream.eof())
  {
    return;
  }
  stream >> MoveIO{game};
  if (!stream)
  {
    throw std::logic_error("invalid file");
  }
  stream >> std::ws;
  if (!stream.eof())
  {
    throw std::logic_error("invalid file");
  }
}

std::istream& operator>>(std::istream& in, LoadedGame& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }

  std::string name;
  std::string fen;
  std::getline(in, name);
  if (!std::getline(in, fen) || isEmptyLine(name) || isEmptyLine(fen))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  try
  {
    chess::Game game(chess::Position(fen.c_str()));
    std::string line;
    while (std::getline(in, line))
    {
      if (isEmptyLine(line))
      {
        break;
      }
      readMoveLine(line, game);
    }
    dest.name = name;
    dest.game = game;
  }
  catch (const std::exception&)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

void printSavedGame(std::ostream& out, const std::string& name, const chess::Game& game)
{
  out << name << "\n";
  out << game.getStartPosition().toFEN() << "\n";
  for (size_t i = 0; i < game.getMovesCount(); i += 2)
  {
    out << game.getMove(i);
    if (i + 1 < game.getMovesCount())
    {
      out << " " << game.getMove(i + 1);
    }
    out << "\n";
  }
  out << "\n";
}

void saveGames(std::istream& in, std::ostream&, games_t& games)
{
  std::string file_name = readName(in);
  std::string line;
  std::getline(in, line);
  std::stringstream stream(line);
  std::string name;
  std::string extra;
  chess::Game* game = nullptr;
  stream >> name;
  if (stream)
  {
    stream >> extra;
    if (stream)
    {
      throw std::logic_error("invalid arguments");
    }
    game = &getGame(games, name);
  }
  std::ofstream file(file_name);
  if (!file)
  {
    throw std::logic_error("invalid file");
  }
  if (game)
  {
    printSavedGame(file, name, *game);
    return;
  }
  for (auto it = games.begin(); it != games.end(); ++it)
  {
    printSavedGame(file, it->first, it->second);
  }
}

void loadGames(std::istream& in, std::ostream& out, games_t& games)
{
  std::string file_name = readName(in);
  std::ifstream file(file_name);
  if (!file)
  {
    throw std::logic_error("invalid file");
  }

  tarasenko::Vector< LoadedGame > loaded;
  while (file)
  {
    LoadedGame game;
    file >> game;
    if (file)
    {
      loaded.pushBack(game);
    }
    else if (!file.eof())
    {
      throw std::logic_error("invalid file");
    }
  }

  for (size_t i = 0; i < loaded.getSize(); ++i)
  {
    if (games.count(loaded[i].name))
    {
      games.at(loaded[i].name) = loaded[i].game;
    }
    else
    {
      games.insert({loaded[i].name, loaded[i].game});
    }
    out << loaded[i].name << "\n";
  }
}

void flipGame(std::istream& in, std::ostream&, games_t& games)
{
  std::string name = readName(in);
  getGame(games, name).flip();
}

void makeMove(std::istream& in, std::ostream&, games_t& games)
{
  std::string name = readName(in);
  chess::Game& game = getGame(games, name);
  chess::Move move = readMove(in);
  chess::MoveArray moves = chess::MoveGenerator::generateLegalMoves(game.getPosition());
  try
  {
    game.makeMove(chess::getMove(moves, move));
  }
  catch (const std::exception&)
  {
    throw std::logic_error("invalid move");
  }
}

void undoMove(std::istream& in, std::ostream&, games_t& games)
{
  std::string name = readName(in);
  getGame(games, name).undoMove();
}

void nextMove(std::istream& in, std::ostream&, games_t& games)
{
  std::string name = readName(in);
  getGame(games, name).nextMove();
}

void prevMove(std::istream& in, std::ostream&, games_t& games)
{
  std::string name = readName(in);
  getGame(games, name).prevMove();
}

void bestMove(std::istream& in, std::ostream& out, games_t& games)
{
  std::string name = readName(in);
  chess::Game& game = getGame(games, name);

  SearchParams params = readSearchParams(in);

  chess::Position pos = game.getPosition();
  chess::MoveArray moves = chess::MoveGenerator::generateLegalMoves(pos);
  if (moves.empty())
  {
    throw std::logic_error("no legal moves");
  }

  size_t move_number = game.getCurrentMove() / 2 + 1;
  if (game.getPosition().isWhiteToMove())
  {
    out << move_number << ".";
  }
  else
  {
    out << move_number << "...";
  }
  printMove(searchPosition(pos, params).first, pos, out);
  out << "\n";
}

void evaluate(std::istream& in, std::ostream& out, games_t& games)
{
  std::string name = readName(in);
  chess::Game& game = getGame(games, name);

  SearchParams params = readSearchParams(in);
  chess::Position pos = game.getPosition();
  out << searchPosition(pos, params).second << "\n";
}

void analyze(std::istream& in, std::ostream& out, games_t& games)
{
  std::string name = readName(in);
  chess::Game& game = getGame(games, name);

  SearchParams params = readSearchParams(in);
  chess::Analyzer analyzer(chess::getEvaluationCoefficients(params.strategy));
  out << analyzer.analyze(game, params.depth, params.time_ms) << "\n";
}

std::string getGameResult(const chess::Position& pos)
{
  chess::MoveArray moves = chess::MoveGenerator::generateLegalMoves(pos);
  if (!moves.empty())
  {
    return "draw";
  }
  if (chess::MoveGenerator::isCheck(pos))
  {
    return pos.isWhiteToMove() ? "black wins" : "white wins";
  }
  return "draw";
}

void playItself(std::istream& in, std::ostream& out, games_t& games)
{
  const int max_plies = 200;
  std::string name = readName(in);
  chess::Game& game = getGame(games, name);
  chess::EvaluationStrategy white_strategy = chess::getEvaluationStrategy(readName(in));
  chess::EvaluationStrategy black_strategy = chess::getEvaluationStrategy(readName(in));
  int white_depth = readDepth(in);
  int black_depth = readDepth(in);

  chess::Position pos = game.getPosition();
  if (chess::MoveGenerator::generateLegalMoves(pos).empty())
  {
    throw std::logic_error("no legal moves");
  }

  for (int ply = 0; ply < max_plies; ++ply)
  {
    if (chess::MoveGenerator::generateLegalMoves(game.getPosition()).empty())
    {
      break;
    }
    bool white_to_move = game.getPosition().isWhiteToMove();
    SearchParams params;
    params.depth = white_to_move ? white_depth : black_depth;
    params.strategy = white_to_move ? white_strategy : black_strategy;
    chess::Position cur = game.getPosition();
    chess::Move move = searchPosition(cur, params).first;
    size_t move_number = game.getCurrentMove() / 2 + 1;
    game.makeMove(move);
    if (white_to_move)
    {
      out << move_number << ".";
      printMove(move, cur, out);
    }
    else
    {
      out << " ";
      printMove(move, cur, out);
      out << "\n";
    }
  }
  if (game.getCurrentMove() % 2 == 1)
  {
    out << "\n";
  }

  game.print(out);
  out << "Result: " << getGameResult(game.getPosition()) << "\n";
}

int main()
{
  games_t games;

  using cmd_t = void(*)(std::istream&, std::ostream&, games_t&);
  tarasenko::RobinHoodTable< std::string, cmd_t > cmds;
  cmds["new"] = newGame;
  cmds["remove"] = removeGame;
  cmds["set_fen"] = setFen;
  cmds["print"] = printGame;
  cmds["list"] = listGames;
  cmds["flip"] = flipGame;
  cmds["make_move"] = makeMove;
  cmds["undo_move"] = undoMove;
  cmds["next_move"] = nextMove;
  cmds["prev_move"] = prevMove;
  cmds["best_move"] = bestMove;
  cmds["evaluate"] = evaluate;
  cmds["analyze"] = analyze;
  cmds["play_itself"] = playItself;
  cmds["save"] = saveGames;
  cmds["load"] = loadGames;

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      if (cmds.count(cmd) == 0)
      {
        throw std::logic_error("unknown command");
      }
      cmds.at(cmd)(std::cin, std::cout, games);
    }
    catch (const std::exception& e)
    {
      std::cout << "<INVALID COMMAND: " << e.what() << ">\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.clear();
      if (std::cin.peek() == ' ' || std::cin.peek() == '\t')
      {
        std::cin.ignore(toignore, '\n');
      }
    }
  }
}
