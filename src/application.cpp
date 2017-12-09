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
  TokenType_open_paren,
  TokenType_close_paren,
  TokenType_open_bracket,
  TokenType_close_bracket,
  TokenType_open_brace,
  TokenType_close_brace,

  TokenType_atom,
  TokenType_number_literal,
  TokenType_string_literal,

  TokenType_COUNT
};

struct Token {
  size_t start_index, end_index;
  Token_Type type;
};

struct Tokenize_Result {
  Token *token_allocation;
  size_t token_allocation_size;
};

template <typename T> struct Range {
  union {
    T min;
    T start;
  };
  union {
    T max;
    T end;
  };
};

Token make_token(Range<size_t> *lexeme_range) {}

#define IsWhiteSpace(Char) ((Char) == ' ' || (Char) == '\n' || (Char) == '\t')
size_t index_of_next_whitespace_after(char *target, size_t starting_index) {
  size_t result = starting_index;
  while (!IsWhiteSpace(target[result])) {
    ++target;
    ++result;
  }
  return result;
}

Tokenize_Result tokenize(char **program_text, size_t program_text_size) {
  int raw_token_count = program_text_size / 2;
  Token *tokens = new Token[raw_token_count];
  defer(delete[] tokens);

  for (int current_index = 0; current_index < program_text_size;
       ++current_index) {
    if (IsWhiteSpace((*program_text)[current_index]))
      continue;

    Range<size_t> current = {};

    current.start = current_index;
    current.end = index_of_next_whitespace_after(*program_text, current_index);
    Token token = make_token(&current);
  }

  return {};
};

void print_all_tokens(Tokenize_Result){};

struct Ast_Node {}; // TODO: Figure out how we structure the AST

Ast_Node *lex(Tokenize_Result);

struct Interpreter {};

struct Evaluation_Output {};

Evaluation_Output *eval(Ast_Node *);

void handle_output(Evaluation_Output *);

int main(int argc, char *argv[]) {
  static size_t N = 66536;
  char *program_buffer = new char[N]; // arbitrary number
  defer(delete[] program_buffer);

  bool no_exit = true;
  while (no_exit) {
    printf("> ");
    getline(&program_buffer, &N, stdin);
    // printf(program);
    size_t program_size = util::strlen(program_buffer);
    auto tokens = tokenize(&program_buffer, program_size);
    print_all_tokens(tokens);
    // auto ast = lex(tokens);
    // auto output = eval(ast);
    // handle_output(output);
  }

  return 0;
}