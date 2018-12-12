#include <iostream>
#include <vector>

int main(){

  std::vector<char> numbers = {'a','b','c','d','e','f','g'};


  for (std::vector<char>::iterator it = numbers.begin()+1; it != numbers.end(); ++it){
    int i = distance(numbers.begin(), it);
    std::cout<<i<<'\n';
  }
  
}
