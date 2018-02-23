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

Tokenizer* tokenize(Buffer program_buffer) {
  // Helpers
  auto find_boundary_index = [program_buffer](size_t start_index) {
    assert(start_index <= program_buffer.occupied);
    yell("find_boundary_index");
    size_t i = 0;
    u8* it = program_buffer.data + start_index;
    do {
      assert(i < 64);
      assert(start_index + i <= program_buffer.occupied);
      if (IsWhiteSpace(*it) || IsBoundary(*it)) {
        return start_index + i;
      }
      i++;
      it++;
    } while (it != (program_buffer.data + program_buffer.occupied));
    return start_index + i;
  };

  auto find_next_newline = [program_buffer](size_t start_index) {
    assert(start_index <= program_buffer.occupied);
    yell("find_next_newline");
    size_t i = 0;
    u8* it = program_buffer.data + start_index;
    do {
      assert(i < 64);
      assert(start_index + i <= program_buffer.occupied);
      if (*it == '\n') {
        return start_index + i;
      }
      i++;
      it++;
    } while (it != (program_buffer.data + program_buffer.occupied));
    return start_index + i;
  };

  Tokenizer* t = platform_alloc<Tokenizer>(1);
  const size_t initial_raw_token_count = program_buffer.data_size;
  auto push_token = [t](size_t start, size_t end, Token_Type type) {
    yell("push_token");

    auto it = t->tokens + t->count;
    it->start_index = start;
    it->boundary_index = end;
    it->type = type;
    t->count++;
  };
  t->tokens =
      platform_alloc<Token>(initial_raw_token_count); // TODO: when we have
                                                      // Token_Mem, dont do this
                                                      // dynamically
  t->capacity = initial_raw_token_count;
  t->count = 0;

  for (int seek_index = 0; seek_index < program_buffer.data_size;
       seek_index = t->current_token.boundary_index) {
    // resize the stream if we need it
    if (t->count >= t->capacity) {
      t->capacity *= 2; // just double it for now
      t->tokens = platform_realloc<Token>(t->tokens, t->capacity);
    }

    t->current_char = program_buffer.data[seek_index];
    if (IsWhiteSpace(t->current_char)) {
      push_token(seek_index, seek_index + 1, TokenType_whitespace);
      continue;
    }

    switch (t->current_char) {
    case '(': {
      push_token(seek_index, seek_index + 1, TokenType_open_paren);
    } break;
    case ')': {
      push_token(seek_index, seek_index + 1, TokenType_close_paren);
    } break;
    case '{': {
      push_token(seek_index, seek_index + 1, TokenType_open_brace);
      unimplemented();
    } break;
    case '}': {
      push_token(seek_index, seek_index + 1, TokenType_close_brace);
      unimplemented();
    } break;
    case '[': {
      push_token(seek_index, seek_index + 1, TokenType_open_bracket);
      unimplemented();
    } break;
    case ']': {
      push_token(seek_index, seek_index + 1, TokenType_open_bracket);
      unimplemented();
    } break;
    case ';': {
      push_token(seek_index, seek_index + 1, TokenType_semicolon);
      unimplemented();
    } break;
    case '\'': { // Char literal
      push_token(seek_index, seek_index + 2, TokenType_char_literal);
      unimplemented();
    } break;
    case '"': { // String literal
      push_token(seek_index, find_boundary_index(seek_index),
                 TokenType_string_literal);
      unimplemented();
    } break;
    case '#': { // Line comment
      push_token(seek_index, find_next_newline(seek_index), TokenType_comment);
    } break;

    default: { // numeric literal/atom
      size_t end_index = find_boundary_index(seek_index);
      auto type = IsNumber(t->current_char) ? TokenType_numeric_literal
                                            : TokenType_atom;
      push_token(seek_index, end_index, type);
    };
    }

    // token is fully decorated, advance the stream
  }

  // terminate the stream
  push_token(t->current_token.boundary_index, 0, TokenType_stream_terminator);
  return t;
};

void tokenizer_free(Tokenizer* t) {
  platform_free(t->tokens);
  t->capacity = 0;
  t->count = 0;
}

void print_all_tokens(Tokenizer* t) {
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
    auto t = tokenize(program_text);
    defer(tokenizer_free(t));

    // debug
    print_all_tokens(t);

    // auto ast = parse_all_tokens_into_ast_tree(&token_stream);
    break;
  }

  return 0;
}
