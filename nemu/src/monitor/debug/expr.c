#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>
enum {
  TK_NOTYPE = 256, 
   /* TODO: Add more token types */
  TK_DEC, TK_HEX, TK_REG,
  TK_OR, TK_AND, TK_EQ, TK_NEQ,
  TK_ADD, TK_MIN, TK_MUL, TK_DIV,
  TK_NOT, TK_POI, TK_NEG,
  TK_LP, TK_RP
};
bool debug = 0;
static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  {"0x[0-9a-fA-F]+", TK_HEX}, // hexadecimal
  {"[1-9][0-9]*|0", TK_DEC}, // decimal
  {"\\$(eax|ecx|edx|ebx|esp|ebp|esi|edi|eip|ax|cx|dx|bx|sp|bp|si|di|al|cl|dl|bl|ah|ch|dh|bh)", TK_REG}, // register
  {"\\+", TK_ADD},      // plus
  {"-", TK_MIN},        // minus
  {"\\*", TK_MUL},      // multiply
  {"/", TK_DIV},        // divide
  {"\\(", TK_LP},       // (
  {"\\)", TK_RP},       // )
  {"!=", TK_NEQ},       // not equal
  {"==", TK_EQ},        // equal
  {"&&", TK_AND},       // and
  {"\\|\\|", TK_OR},        // or
  {"!", TK_NOT},        // not
  {" +", TK_NOTYPE},    // spaces
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        if (substr_len > 31) {
          printf("Error: the token is too long.\n");
          assert(0);
        }
        switch (rules[i].token_type) {
          case TK_ADD: {
            Token t;
            t.type = TK_ADD;
            t.str[0] = '+';
            t.str[1] = '\0';
            tokens[nr_token] = t;
            nr_token += 1;
            break;
          }
          case TK_MIN: {
            Token t;
            t.type = TK_MIN;
            t.str[0] = '-';
            t.str[1] = '\0';
            tokens[nr_token] = t;
            nr_token += 1;
            break;
          }
          case TK_MUL: {
            Token t;
            t.type = TK_MUL;
            t.str[0] = '*';
            t.str[1] = '\0';
            tokens[nr_token] = t;
            nr_token += 1;
            break;
          }
          case TK_DIV: {
            Token t;
            t.type = TK_DIV;
            t.str[0] = '/';
            t.str[1] = '\0';
            tokens[nr_token] = t;
            nr_token += 1;
            break;
          }
          case TK_LP: {
            Token t;
            t.type = TK_LP;
            t.str[0] = '(';
            t.str[1] = '\0';
            tokens[nr_token] = t;
            nr_token += 1;
            break;
          }
          case TK_RP: {
            Token t;
            t.type = TK_RP;
            t.str[0] = ')';
            t.str[1] = '\0';
            tokens[nr_token] = t;
            nr_token += 1;
            break;
          }
          case TK_DEC: {
            Token t;
            t.type = TK_DEC;
            int j;
            for (j = 0; j < substr_len; j++) {
              t.str[j] = *(substr_start + j);
            }
            t.str[j] = '\0';
            tokens[nr_token] = t;
            nr_token++;
            break;
          }
          case TK_HEX: {
            Token t;
            t.type = TK_HEX;
            int j;
            for (j = 0; j < substr_len; j++) {
              t.str[j] = *(substr_start + j);
            }
            t.str[j] = '\0';
            tokens[nr_token] = t;
            nr_token++;
            break;
          }
          case TK_REG: {
            Token t;
            t.type = TK_REG;
            int j;
            for (j = 0; j < substr_len; j++) {
              t.str[j] = *(substr_start + j);
            }
            t.str[j] = '\0';
            tokens[nr_token] = t;
            nr_token++;
            break;
          }
          case TK_NOT: {
            Token t;
            t.type = TK_NOT;
            t.str[0] = '!';
            t.str[1] = '\0';
            tokens[nr_token] = t;
            nr_token += 1;
            break;
          }
          case TK_EQ: {
            Token t;
            t.type = TK_EQ;
            t.str[0] = '=';
            t.str[1] = '=';
            t.str[2] = '\0';
            tokens[nr_token] = t;
            nr_token += 1;
            break;
          }
          case TK_NEQ: {
            Token t;
            t.type = TK_NEQ;
            t.str[0] = '!';
            t.str[1] = '=';
            t.str[2] = '\0';
            tokens[nr_token] = t;
            nr_token += 1;
            break;
          }
          case TK_AND: {
            Token t;
            t.type = TK_AND;
            t.str[0] = '&';
            t.str[1] = '&';
            t.str[2] = '\0';
            tokens[nr_token] = t;
            nr_token += 1;
            break;
          }
          case TK_OR: {
            Token t;
            t.type = TK_EQ;
            t.str[0] = '|';
            t.str[1] = '|';
            t.str[2] = '\0';
            tokens[nr_token] = t;
            nr_token += 1;
            break;
          }
          case TK_NOTYPE: {
            break;
          }
		  default: TODO();
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}
bool check_parentheses(int p, int q) {
  if(debug) printf("check_parentheses\n");
  // assert and ret false differs - refer to page 29 of instruction book
  if (tokens[p].type != TK_LP || tokens[q].type != TK_RP) {
//	2 * 3 will enter this part
    return false;
  }
  int t = p + 1;
  int count = 0;
  while(t < q) {
    if (tokens[t].type == TK_LP) {
      count++;
    }
    else if (tokens[t].type == TK_RP && count > 0) {
      count--;
    }
    else if (tokens[t].type == TK_RP && count == 0) {
      return false;
    }
    t++;
  }
  if (count != 0) {
	  printf("check_parentheses failed in part 2\n");
	  return false;
  }
  return true;
}
int find_right_parenthese(int p, int q) {
  if(debug) printf("find_right_parenthese\n");
  int tmp = p;
  int right = q;
  while(tmp < q && tokens[tmp].type != TK_RP) {
    if (tokens[tmp].type == TK_LP){
      right = find_right_parenthese(tmp+1, q);
      if(right == -1) {
        return -1;
      }
      tmp = right;
    }
    tmp++;
  }
  if (tmp == q && tokens[q].type != TK_RP) {
    printf("impossible to reach here! Incompatible parentheses.\n");
    assert(0);
	return -1;
  }
  else {
    return tmp;
  }
}
// true => op1 > op2, false => op1 < op2
bool compare_priority(int op1, int op2){
  if(debug) printf("compare_priority\n");
  if ((op1 == TK_EQ && op2 == TK_NEQ) || (op1 == TK_NEQ && op2 == TK_EQ)) {
    return true;
  }
  if ((op1 == TK_ADD && op2 == TK_MIN) || (op1 == TK_MIN && op2 == TK_ADD)) {
    return true;
  }
  if ((op1 == TK_MUL && op2 == TK_DIV) || (op1 == TK_DIV && op2 == TK_MUL)) {
    return true;
  }
  if (op1 == op2) {
    if ((op1 == TK_NEG || TK_POI || TK_NOT)) {return false;}
    else {return true;}
  }
  return op1 > op2;
}
int find_operator(int p, int q) {
  if(debug) printf("find_operator\n");
  int len = (q - p) + 1;
 // int* stack = (int*) malloc(sizeof(int)*len);
 //  memset(stack, 0, sizeof(int)*len);
  int t = p;
  int count = 0;
  int loc = p;
  int operand = 0;
  while(t <= q && count <= len) {
    if (tokens[t].type == TK_LP) {
      t = find_right_parenthese(t+1, q);
    }
    else if (tokens[t].type == TK_RP) {
      printf("impossible to reach here! RP should have already been eliminated.\n");
	  assert(0);
   	}
    else if(tokens[t].type != TK_DEC && tokens[t].type != TK_HEX 
    && tokens[t].type != TK_REG && tokens[t].type != TK_NOTYPE) {
      if(count == 0) {
      //  stack[count] = tokens[t].type;
        loc = t;
        count++;
        operand = tokens[t].type;
      }
      else {
        if (compare_priority(operand, tokens[t].type)) {
          loc = t;
          operand = tokens[t].type;
        }
	  }
	}
    t++;
  }
 // free(stack);
  return loc;
}
int eval(int p, int q) {
	if(debug) printf("eval\n");
  if (p > q) {
   // printf("Error: bad expression!\n");
   // assert(0);
   return 0;
   // it's tricky that 0 implicates cases like -1
  }
  else if (p == q) {
    int res = 0;
    if (tokens[p].type == TK_HEX) {
      int hex_len = strlen(tokens[p].str);
      int i = 2;
      while (tokens[p].str[i] == 0) {
        i++;
      }
      for (; i < hex_len; i++) {
        int radix = 1, iter = 0;
        while(iter < (hex_len - i - 1)) {
          radix *= 16;
          iter++;
        }
        // printf("%d, %d, %d\n", (int)(tokens[p].str[i]), iter, radix);
        res += ((int)(tokens[p].str[i]) - 48) * radix;
      }
    }
    else if (tokens[p].type == TK_DEC) {
      res = atoi(tokens[p].str);
    }
    else if (tokens[p].type == TK_REG) {
      char tmp[3] = {tokens[p].str[1], tokens[p].str[2], tokens[p].str[3]};
      for(int i = 0; i < 8; i++) {
        if (!strcmp(tmp, regsl[i])) {return cpu.gpr[i]._32;}
      }
      char tmp1[2] = {tokens[p].str[1], tokens[p].str[2]};
      for(int i = 0; i < 8; i++) {
        if (!strcmp(tmp1, regsw[i])) {return cpu.gpr[i]._16;}
      }
      for(int i = 0; i < 8; i++) {
        if (!strcmp(tmp1, regsb[i])) {return cpu.gpr[i%4]._8[i/4];}
      }
      if (!strcmp(tmp, "eip")) {return cpu.eip;}
      else { printf("Unknown register.\n"); assert(0);}
    }

    // printf("token_idx: %d, value: %s.\n", p, tokens[p].str);
    return res;
  }
  else if (check_parentheses(p, q) == true) {
	// just throw away the parentheses
    return eval(p + 1, q - 1);
  }
  else {
    // find the dominant operator
	// first get the value of 2 sub expr
	// second cal the two sub exprs
    int op = find_operator(p, q);// the position of dominant operator in the token expression
   	int val1 = eval(p, op - 1);
    int val2 = eval(op + 1, q);
    switch (tokens[op].type) {
      case TK_ADD: return val1 + val2;
      case TK_MIN: return val1 - val2;
      case TK_MUL: return val1 * val2;
      case TK_DIV: return val1 / val2;
      case TK_AND: {
        if (val1 != 0 && val2 != 0) {
          return true;
        }
        else {
          return false;
        }
      }
      case TK_OR: {
        if (val1 != 0 || val2 != 0) {
          return true;
        }
        else {
          return false;
        }
      }
      case TK_EQ: {
        if (val1 == val2){
          return true;
        }
        else {
          return false;
        }
      }
      case TK_NEQ: {
        if (val1 != val2) {
          return true;
        }
        else {
          return false;
        }
      }
      case TK_NEG: { return -1 * val2; }
      case TK_POI: { return vaddr_read(val2, 4); }
      case TK_NOT: { return !val2; }
      default: assert(0);
    }

  }
}

uint32_t expr(char *e, bool *success) {
	if(debug) printf("expr\n");
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  *success = true;
  for (int i = 0; i < nr_token; i++) {
    if (tokens[i].type == TK_MIN && (i == 0 || (tokens[i - 1].type != TK_DEC && tokens[i - 1].type  != TK_HEX && tokens[i - 1].type  != TK_REG))) {
        // diffrentiate the negative and minus
	   	tokens[i].type = TK_NEG;
    }
    else if (tokens[i].type == TK_MUL && (i == 0 || (tokens[i - 1].type != TK_DEC && tokens[i - 1].type  != TK_HEX && tokens[i - 1].type  != TK_REG && tokens[i - 1].type != TK_RP))) {
        // diffrentiate the pointer and multiply
		tokens[i].type = TK_POI;
    }
  }
  int p = 0, q = nr_token - 1;
  return eval(p, q);
}
