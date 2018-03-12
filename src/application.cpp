#include "application.hpp"

const char* token_type_as_string(Token_Type type) {
#define Case(Name)                                                             \
  case TokenType_##Name:                                                       \
    return #Name

  switch (type) {
    Case(invalid);
    Case(stream_terminator);
    Case(open_paren);
    Case(close_paren);
    Case(open_bracket);
    Case(close_bracket);
    Case(open_brace);
    Case(close_brace);
    Case(comma);
    Case(semicolon);
    Case(atom);
    Case(whitespace);
    Case(numeric_literal);
    Case(char_literal);
    Case(string_literal);
    Case(comment);
  default: { unreachable(); }
#undef Case
  };
  return "(invalid)";
}

Buffer read_file_into_read_buffer(const char* name) {
  Buffer b;
  FILE* file = fopen(name, "r");
  defer(fclose(file));

  fseek(file, 0, SEEK_END);
  size_t final_file_size = ftell(file) + 1;
  fseek(file, 0, SEEK_SET);

  b.initialize(final_file_size);
  fread(b.data, final_file_size, 1, file);

  return b;
}

int main(int argc, char* argv[]) {

  for (;;) {
    auto program_text = read_file_into_read_buffer("data/test.scheme");
    defer (program_text.free());

    // tokenize
    printf("Lex.\n");
    // auto t = tokenize_buffer(program_text);
    // defer(free_token_chunk(t));

    // debug
    // print_token_chunk(t);

    // auto ast = parse_ast(t);
    break;
  }

  return 0;
}
