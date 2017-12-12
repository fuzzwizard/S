#include "typedefs.h"
#include "util.h"

// typedef enum allocation_type {
//   AllocationType_NOOP = 0,
//   AllocationType_Alloc,
//   AllocationType_Realloc,
//   AllocationType_Free,
//   AllocationType_Reset,
// } Allocation_Type;

// #define MEMORY_INTERFACE(_Name)                                                \
//   void *_Name(void *data, size_t size, void *old_data, size_t old_size,        \
//               Allocation_Type type)
// typedef MEMORY_INTERFACE(Memory_Interface);

// MEMORY_INTERFACE(system_allocator) {
//   switch (type) {
//   case AllocationType_Alloc: {
//     void *result = malloc(size);
//     return result;
//   } break;
//   case AllocationType_Realloc: {
//     void *result = realloc(data, size);
//     return result;
//   } break;
//   case AllocationType_Free: {
//     free(data);
//   } break;
//   case AllocationType_Reset: {
//   } break;
//   default: { assert(false); } break;
//   }
//   return 0;
// }

enum Token_Type {
  TokenType_NONE,
  TokenType_open_paren,
  TokenType_close_paren,
  TokenType_open_bracket,
  TokenType_close_bracket,
  TokenType_open_brace,
  TokenType_close_brace,

  TokenType_comma,

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
  Token *tokens = nullptr;
  union {
    size_t write_index = 0;
    size_t count;
  };
  size_t capacity = 0;
};

using C_String = char *;

#define IsWhiteSpace(Char) ((Char) == ' ' || (Char) == '\n' || (Char) == '\t')
#define IsNumber(Char) ((Char) >= '0' && (Char) <= '9')
#define IsAlpha(Char)                                                          \
  (((Char) >= 'a' && (Char) <= 'z') && ((Char) >= 'A' && (Char) <= 'Z'))
#define IsBoundary(Char)                                                       \
  ((Char) == '{' || (Char) == '}' || (Char) == '(' || (Char) == ')' ||         \
   (Char) == '[' || (Char) == ']')

const char *token_type_get_string(Token_Type type) {
#define Case(Name)                                                             \
  case TokenType_##Name:                                                       \
    return #Name

  switch (type) {
    Case(NONE);
    Case(open_paren);
    Case(close_paren);
    Case(open_bracket);
    Case(close_bracket);
    Case(open_brace);
    Case(close_brace);
    Case(comma);
    Case(atom);
    Case(numeric_literal);
    Case(char_literal);
    Case(string_literal);
    Case(COUNT);
  }
}

size_t find_boundary_index(char *seek_ptr, size_t start_index) {
  assert(seek_ptr);
  size_t counter = start_index;
  for (char it = seek_ptr[counter];; counter++, it = seek_ptr[counter]) {
    if (IsWhiteSpace(it) || IsBoundary(it)) {
      break;
    }
  }
  return counter;
}

Token_Stream tokenize(C_String *program_text, size_t program_text_size) {
  const size_t initial_raw_token_count = program_text_size; // this is arbitrary
  Token_Stream stream;
  stream.tokens =
      new Token[initial_raw_token_count]; // TODO: when we have Program_Mem,
                                          // consider not doing this dynamically
  stream.capacity = initial_raw_token_count;

  for (int seek_index = 0; seek_index < program_text_size; seek_index++) {

    // resize the buffer if we need it
    if (stream.write_index >= stream.capacity) {
      auto old = stream.tokens;

      stream.capacity *= 2; // just double it for now
      stream.tokens = new Token[stream.capacity];
      util::memcpy(stream.tokens, old, stream.write_index * sizeof(Token));

      delete[] old;
    }

    const char the_char = (*program_text)[seek_index];
    if (IsWhiteSpace(the_char)) {
      continue;
    }

    if (the_char == '\0') {
      break;
    }
    printf("the_char: %c \n", the_char);

    Token next_token;
    switch (the_char) {
    case '(': {
      next_token.start_index = next_token.boundary_index = seek_index;
      next_token.type = TokenType_open_paren;
    } break;
    case ')': {
      next_token.start_index = next_token.boundary_index = seek_index;
      next_token.type = TokenType_close_paren;
    } break;
    case '{': {
      next_token.start_index = next_token.boundary_index = seek_index;
      next_token.type = TokenType_open_brace;
    } break;
    case '}': {
      next_token.start_index = next_token.boundary_index = seek_index;
      next_token.type = TokenType_close_brace;
    } break;
    case '[': {
      next_token.start_index = next_token.boundary_index = seek_index;
      next_token.type = TokenType_open_bracket;
    } break;
    case ']': {
      next_token.start_index = next_token.boundary_index = seek_index;
      next_token.type = TokenType_close_bracket;
    } break;

    case '\'': {
      next_token.start_index = seek_index;
      next_token.boundary_index = seek_index + 2;
      next_token.type = TokenType_char_literal;
    };
    case '"': {
      next_token.start_index = seek_index;
      next_token.boundary_index =
          find_boundary_index(*program_text, seek_index);
      next_token.type = TokenType_string_literal;
    };

    default: { // now, we're either a numeric literal, or an atom
      next_token.start_index = seek_index;
      next_token.boundary_index =
          find_boundary_index(*program_text, seek_index);

      if (IsNumber(the_char)) {
        next_token.type = TokenType_numeric_literal;
      } else {
        next_token.type = TokenType_atom;
      }
    } break;
    }

    // token is fully decorated
    stream.tokens[stream.count] = next_token;
    stream.count++;
  }

  return stream;
};

void token_stream_free(Token_Stream *stream) {
  delete[] stream->tokens;
  stream->capacity = 0;
  stream->write_index = 0;
}

void print_all_tokens(Token_Stream *stream) {
  size_t N = stream->write_index;
  for (size_t i = 0; i < N; i++) {
    auto token = stream->tokens + i;
    printf("Token { start: %u, end: %u, type: %s }\n", token->start_index,
           token->boundary_index, token_type_get_string(token->type));
  }
};

// struct Ast_Node {}; // TODO: Figure out how we structure the AST

// Ast_Node *lex(Tokenize_Result);

// struct Interpreter {};

// struct Evaluation_Output {};

// Evaluation_Output *eval(Ast_Node *);

// void handle_output(Evaluation_Output *);

int main(int argc, char *argv[]) {
  size_t program_size = 65536;
  C_String program_buffer = new char[program_size]; // arbitrary number
  defer(delete[] program_buffer);

  bool no_exit = true;
  while (no_exit) {
    util::memzero(program_buffer, program_size); // clear the buffer

    printf("> ");
    getline(&program_buffer, &program_size, stdin);

    auto token_stream = tokenize(&program_buffer, program_size);
    defer(token_stream_free(&token_stream));

    // debug
    print_all_tokens(&token_stream);

    // auto ast = lex(tokens);
    // auto output = eval(ast);
    // handle_output(output);
  }

  return 0;
}