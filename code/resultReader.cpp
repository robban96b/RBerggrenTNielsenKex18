#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char * argv[]) {

  ofstream out;
  string sp = argv[1];
  sp = sp + "sum";
  out.open(sp);

  ifstream input;
  input.open (argv[1]);

  string s;

  vector<int> v;
  while(!input.eof()) {
    input >> s;
    if (s == "begin") {
      input >> s;
      if (s == "solution") {
        input >> s; //solutionNr;
        out << s << ", ";
        while(s != "=") {
          input >> s;
          out << s;
        }
        int fitnessScore;
        input >> fitnessScore;
        v.push_back(fitnessScore);
        out << fitnessScore << endl; //solution done
      }
    }
  }
  out << '[';
  for (uint i = 0; i < v.size() - 1; i++) {
    out << v[i];
    out << ", ";
  }
  out << v[v.size() - 1];
  out << ']';
  out.close();
  input.close();
  return 0;
}
