#include "application.hpp"

#define yell(...) printf("!!! @%s %d (%s)", __FILE__, __LINE__, __func__)

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

struct Token_Chunk {
  Token* tokens;
  size_t capacity;
  size_t count;
};

static inline Token_Chunk* new_token_chunk(size_t cap) {
  union {
    Token_Chunk* as_chunk;
    u8* as_bytes;
  };

  const size_t chunk_size = sizeof(Token_Chunk) + (sizeof(Token) * cap);
  as_bytes = platform_alloc<u8>(chunk_size);
  assert(as_bytes);

  as_chunk->count = 0;
  as_chunk->capacity = cap;
  as_chunk->tokens = (Token*)(as_bytes + sizeof(Token_Chunk));
  return as_chunk;
};

static inline void push_token(Token_Chunk* t, Token* token) {
  t->tokens[t->count] = *token;
  t->count++;
}

static inline void push_token(Token_Chunk* t, size_t start, size_t end,
                              Token_Type type) {
  t->tokens[t->count] = {start, end, type};
  t->count++;
}

Token_Chunk* tokenize_buffer(Buffer b) {
  auto character_count = b.occupied;
  auto result_chunk =
      new_token_chunk(character_count); // TODO: oversized, get a better metric
                                        // count the number of spaces?

  u8 current_char;
  for (size_t seek_index = 0; seek_index <= character_count && current_char;) {
    current_char = b.data[seek_index];

    switch (current_char) {
    case '(': {
      push_token(result_chunk, seek_index, seek_index + 1,
                 TokenType_open_paren);
      seek_index++;
    } break;
    case ')': {
      push_token(result_chunk, seek_index, seek_index + 1,
                 TokenType_close_paren);
      seek_index++;
    } break;
    // TODO: {} & []
    case '#': {
      size_t start_index = seek_index;
      while (current_char != '\n' && current_char) {
        seek_index++;
        current_char = b.data[seek_index];
      }
      push_token(result_chunk, start_index, seek_index, TokenType_comment);
    } break;

    default: {
      // ATOM OR LITERAL
      size_t start_index = seek_index;
      Token_Type type = TokenType_invalid;
      while (current_char != '\n' && current_char) {
        seek_index++;
        current_char = b.data[seek_index];
      }
      push_token(result_chunk, start_index, seek_index, TokenType_comment);
    } break;
    }
  }
}

return result_chunk;
}

void free_token_chunk(Token_Chunk* t) { platform_free(t); }

void print_token_chunk(Token_Chunk* t) {
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

// #include <stdarg.h>
// void sprintf(Buffer* buf, const char* fmt, ...) {
//   va_list args;
//   va_start(args, fmt);
//   unimplemented();
// }
// void format_into_buffer(Buffer* buf, const char* fmt, va_list vargs) {
//   unimplemented();
// };

int main(int argc, char* argv[]) {

  for (;;) {
    printf("Begin.\n");

    printf("Open file.\n");
    auto program_text = read_file_into_buffer("data/test.scheme");
    defer(buffer_free(&program_text));

    // tokenize
    printf("Lex.\n");
    auto t = tokenize_buffer(program_text);
    defer(free_token_chunk(t));

    // debug
    print_token_chunk(t);

    // auto ast = parse_ast(t);
    break;
  }

  return 0;
}
