#include "application.hpp

#if 0
static void sb__maybe_realloc(String_Builder* sb, int amt) {
  int amount_needed = sb->current_buffer.occupied + amt;
  if (amount_needed > sb->current_buffer.data_size) {
    unimplemented();
  }
};

static inline void sb_push_char(String_Builder* sb, char c) {
  sb__maybe_realloc(sb, 1);
  sb->current_buffer.data[sb->current_buffer.occupied] = c;
  sb->current_buffer.occupied++;
}

static inline char sb_peek_head(String_Builder* sb) {
  return sb->current_buffer.data[sb->consumed];
}

static inline void sb_consume(String_Builder* sb) {
  sb->consumed++;
  assert(sb->consumed <= sb->occupied);
}

static inline char sb_consume_w_return(String_Builder* sb) {
  sb_consume(sb);
  return sb_peek_head(sb);
}

static inline bool sb_eat_whitespace(String_Builder* sb) {
  if (IsWhiteSpace(sb_peek_head(sb))) {
    sb_consume(sb);
    return true;
  }
  return false;
}

static inline bool sb_eat_whitespaces(String_Builder* sb) {
  bool result;
  while (result = sb_eat_whitespace(sb)) {
    /* This just keeps eating whitespace, until whitespace is not found */
  };
  return result;
}

static inline char sb_eat_char(String_Builder* sb) {
  char c = 0;
  if (IsAlpha(sb_peek_head(sb))) {
    c = sb_consume_w_return(sb);
  }
  return c;
}
#endif

enum Token_Type {
  TokenType_invalid,
  TokenType_open_paren,
  TokenType_close_paren,
  TokenType_open_bracket,
  TokenType_close_bracket,
  TokenType_open_brace,
  TokenType_close_brace,

  TokenType_comma,
  TokenType_semicolon,

  TokenType_atom,
  TokenType_numeric_literal,
  TokenType_char_literal,
  TokenType_string_literal,

  TokenType_COUNT
};

struct Token {
  size_t start_index = 0;
  size_t boundary_index = 0;
  Token_Type type = (Token_Type)0;
};

struct Token_Stream {
  Token* tokens = nullptr;
  size_t capacity = 0;
  size_t count = 0;
};

const char* token_type_as_string(Token_Type type) {
#define Case(Name)                                                             \
  case TokenType_##Name:                                                       \
    return #Name

  switch (type) {
    Case(invalid);
    Case(open_paren);
    Case(close_paren);
    Case(open_bracket);
    Case(close_bracket);
    Case(open_brace);
    Case(close_brace);
    Case(comma);
    Case(semicolon);
    Case(atom);
    Case(numeric_literal);
    Case(char_literal);
    Case(string_literal);
  default: { return "(invalid)"; }
#undef Case
  };
  return "(invalid)";
}

// WARNING: so unsafe!!!! dont use this anywhere else yet!
size_t find_boundary_index(Buffer* buf, size_t i) {
  char it = buf->data[i];
  for (;;) {
    if (IsWhiteSpace(it) || IsBoundary(it) || !it) {
      assert(i < 64);
      break;
    }
    i++;
    it = buf->data[i];
  }
  return i;
}

Token_Stream tokenize(Buffer program_buffer) {
  const size_t initial_raw_token_count =
      program_buffer.data_size; // this is arbitrary, but likely shouldn't be
  Token_Stream stream;
  stream.tokens = new Token[initial_raw_token_count]; // TODO: when we have
                                                      // Token_Mem, consider
                                                      // not doing this
                                                      // dynamically
  stream.capacity = initial_raw_token_count;

  Token next_token;
  for (int seek_index = 0; seek_index < program_buffer.data_size;
       seek_index = next_token.boundary_index) {
    // resize the stream if we need it
    if (stream.count >= stream.capacity) {
      auto old = stream.tokens;

      stream.capacity *= 2; // just double it for now
      stream.tokens = new Token[stream.capacity];
      util::memcpy(stream.tokens, old, stream.count * sizeof(Token));

      delete[] old;
    }

    const char the_char = program_buffer.data[seek_index];
    if (IsWhiteSpace(the_char)) {
      continue; // we're whitespace insensitive
    }

    // TODO: Remove this when we handle the program text better
    if (the_char == '\0') {
      break;
    }

    switch (the_char) {
    case '(': {
      next_token.start_index = seek_index;
      next_token.boundary_index = seek_index + 1;
      next_token.type = TokenType_open_paren;
    } break;
    case ')': {
      next_token.start_index = seek_index;
      next_token.boundary_index = seek_index + 1;
      next_token.type = TokenType_close_paren;
    } break;
    case '{': {
      next_token.start_index = seek_index;
      next_token.boundary_index = seek_index + 1;
      next_token.type = TokenType_open_brace;
      // TODO: Record literals
      unimplemented();
    } break;
    case '}': {
      next_token.start_index = seek_index;
      next_token.boundary_index = seek_index + 1;
      next_token.type = TokenType_close_brace;
      // TODO: Record literals
      unimplemented();
    } break;
    case '[': {
      next_token.start_index = seek_index;
      next_token.boundary_index = seek_index + 1;
      next_token.type = TokenType_open_bracket;
      // TODO: Array literals
      unimplemented();
    } break;
    case ']': {
      next_token.start_index = seek_index;
      next_token.boundary_index = seek_index + 1;
      next_token.type = TokenType_close_bracket;
      // TODO: Array literals
      unimplemented();
    } break;
    case ';': {
      next_token.start_index = seek_index;
      next_token.boundary_index = seek_index + 1;
      next_token.type = TokenType_semicolon;
      // Reserved token ';'
      unreachable();
    } break;
    case '\'': {
      next_token.start_index = seek_index;
      next_token.boundary_index = seek_index + 2;
      assert(program_buffer.data[next_token.boundary_index] ==
             '\''); // TODO: Shouldn't just assert but runtime check
      next_token.type = TokenType_char_literal;

      // TODO: character literals
      unimplemented();
    } break;
    case '"': {
      next_token.start_index = seek_index;
      next_token.boundary_index =
          find_boundary_index(&program_buffer, seek_index);
      next_token.type = TokenType_string_literal;
      assert(program_buffer.data[seek_index + 2] ==
             '\"'); // TODO: Shouldn't just assert but runtime check
      unimplemented();
    } break;

    default: { // now, we're either a numeric literal, or an atom
      next_token.start_index = seek_index;
      next_token.boundary_index =
          find_boundary_index(&program_buffer, seek_index);

      if (IsNumber(the_char)) {
        next_token.type = TokenType_numeric_literal;
      } else {
        next_token.type = TokenType_atom;
      }
    };
    }

    // token is fully decorated
    stream.tokens[stream.count] = next_token;
    stream.count++;
  }

  return stream;
};

void token_stream_free(Token_Stream* stream) {
  delete[] stream->tokens;
  stream->capacity = 0;
  stream->count = 0;
}

void print_all_tokens(Token_Stream* stream) {
  size_t N = stream->count;
  for (size_t i = 0; i < N; i++) {
    auto token = stream->tokens + i;
    printf("Token { start: %ld, end: %ld, type: %s }\n", token->start_index,
           token->boundary_index, token_type_as_string(token->type));
  }
};

// TODO: linear allocator
// TODO:

struct Data_Type {
  size_t size;
};

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
    printf("Lex.\n");
    // TODO: OSX hangs here
    auto token_stream = tokenize(program_text);
    defer(token_stream_free(&token_stream));

    // debug
    print_all_tokens(&token_stream);

    // lex
    // auto ast = parse_all_tokens_into_ast_tree(&token_stream);
    // defer(ast_recursive_free(&ast));
    // emit code

    // recursively_emit_code(&ast);
    break;
  }

  return 0;
}
