#include "application.hpp"

const char* token_type_get_string(Token_Type type) {
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

#define yell(m) printf("%s:%s\n", __func__, m)

struct Token_Stream {
  Token* tokens;
  size_t capacity;
  size_t count;
  char current_char;
  Token current_token;
};

void push_token(Token_Stream* t, Token* token) {
  t->tokens[t->count] = *token;
  t->count++;
}

void push_token(Token_Stream* t, size_t start, size_t offset, Token_Type type) {
  t->tokens[t->count] = {start, start + offset, type};
  t->count++;
}

Token_Stream* tokenize_buffer(Buffer b) {
  auto t = platform_alloc<Token_Stream>(1);
  auto N = b.occupied;

  u8 current_char;
  for (int seek_index = 0; seek_index < N;) {
    current_char = b.data[seek_index];

    switch (current_char) {
    case '(': {
      push_token(t, seek_index, seek_index + 1, TokenType_open_paren);
      seek_index++;
    } break;
    case ')': {
      push_token(t, seek_index, seek_index + 1, TokenType_close_paren);
      seek_index++;
    } break;
    case '#': {
      size_t start_index = seek_index;
      while (current_char != '\n') {
        assert(current_char);
        current_char = b.data[seek_index];
        seek_index++;
      }
      push_token(t, start_index, seek_index, TokenType_comment);
    } break;

    default: {}
      continue;
    }
  }

  return t;
}

void token_stream_free(Token_Stream* t) {
  platform_free(t->tokens);
  t->capacity = 0;
  t->count = 0;
}

void print_all_tokens(Token_Stream* t) {
  size_t N = t->count;
  for (size_t i = 0; i < N; i++) {
    auto token = t->tokens + i;
    printf("Token { start: %ld, end: %ld, type: %s }\n", token->start_index,
           token->boundary_index, token_type_get_string(token->type));
  }
};

// TODO: linear allocator

// Ast_Node* parse_all_tokens_into_ast_tree(Token_Stream*) { return nullptr; };

Buffer read_file_into_buffer(const char* name) {
  Buffer b;
  FILE* file = fopen(name, "r");
  defer(fclose(file));

  fseek(file, 0, SEEK_END);
  size_t final_file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  buffer_init(&b, final_file_size);
  fread(b.data, final_file_size, 1, file);
  b.occupied = final_file_size;

  return b;
}

#include <stdarg.h>
void sprintf(Buffer* buf, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  unimplemented();
}
void format_into_buffer(Buffer* buf, const char* fmt, va_list vargs) {
  unimplemented();
};

int main(int argc, char* argv[]) {

  // size_t program_size = 65536;
  // char* program_buffer = new char[program_size]; // arbitrary number
  // program_buffer[program_size] = 0;
  // defer(delete[] program_buffer);

  for (;;) {
    printf("Begin.\n");
    // clear the program buffer and take input
    // util::memzero(program_buffer, program_size);
    // printf("> ");
    // getline(&program_buffer, &program_size, stdin);

    // // check if the user exited
    // if (util::strncmp(program_buffer, "!exit", 5) == 0) {
    //   printf("Exiting.\n");
    //   break;
    // }

    printf("Open file.\n");
    auto program_text = read_file_into_buffer("data/test.scheme");
    defer(buffer_free(&program_text));

    // tokenize
    // printf("Lex.\n");
    auto t = tokenize_buffer(program_text);
    defer(token_stream_free(t));

    // debug
    print_all_tokens(t);

    // auto ast = parse_all_tokens_into_ast_tree(&token_stream);
    break;
  }

  return 0;
}
