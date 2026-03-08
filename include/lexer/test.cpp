#include "lexer.hpp"
int main() {
    std::string source = "while true: x = 4e5; end\nx=5;\ny7= .5; if (true) u=x;end [u]=7*8; v=8.*6; x=A.*B; -.6e+34+4.e01;";
    std::vector<Lexeme> lexemes = maximal_munch(source);
    for (Lexeme lexeme: lexemes) {
        std::string str = source.substr(lexeme.start, lexeme.length);
        Token token = lexeme.tok;
        std::cout << str << " " << token_names[static_cast<int>(token)] << "\n";
    }
}
