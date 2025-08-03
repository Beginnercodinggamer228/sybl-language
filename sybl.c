#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_VARS 100
#define MAX_LINES 1000
#define MAX_LINE_LEN 256
#define MAX_VAR_NAME 50

typedef enum {
    VAR_INT,
    VAR_FLOAT,
    VAR_STRING,
    VAR_BOOL
} VarType;

typedef struct {
    char name[MAX_VAR_NAME];
    VarType type;
    union {
        long int_val;
        double float_val;
        char str_val[256];
        int bool_val;
    };
} Variable;

typedef struct {
    Variable vars[MAX_VARS];
    int var_count;
    char lines[MAX_LINES][MAX_LINE_LEN];
    int line_count;
    int current_line;
} Interpreter;

void trim(char *str) {
    char *start = str;
    while (isspace(*start)) start++;
    if (*start == 0) {
        *str = 0;
        return;
    }
    char *end = start + strlen(start) - 1;
    while (end > start && isspace(*end)) end--;
    *(end + 1) = 0;
    memmove(str, start, end - start + 2);
}

Variable* find_var(Interpreter *interp, const char *name) {
    for (int i = 0; i < interp->var_count; i++) {
        if (strcmp(interp->vars[i].name, name) == 0) {
            return &interp->vars[i];
        }
    }
    return NULL;
}

double get_value(Interpreter *interp, const char *name) {
    if (isdigit(name[0]) || (name[0] == '-' && isdigit(name[1]))) {
        return atof(name);
    }
    Variable *var = find_var(interp, name);
    if (var) {
        switch (var->type) {
            case VAR_INT: return (double)var->int_val;
            case VAR_FLOAT: return var->float_val;
            case VAR_BOOL: return var->bool_val ? 1.0 : 0.0;
            default: return 0.0;
        }
    }
    return 0.0;
}

void set_var(Interpreter *interp, const char *name, VarType type, void *value) {
    Variable *var = find_var(interp, name);
    if (!var) {
        var = &interp->vars[interp->var_count++];
        strcpy(var->name, name);
    }
    var->type = type;
    switch (type) {
        case VAR_INT: var->int_val = *(long*)value; break;
        case VAR_FLOAT: var->float_val = *(double*)value; break;
        case VAR_STRING: strcpy(var->str_val, (char*)value); break;
        case VAR_BOOL: var->bool_val = *(int*)value; break;
    }
}

void parse_output(Interpreter *interp, char *text, char *result) {
    char *pos = text;
    char *out = result;
    
    while (*pos) {
        if (pos[0] == '$' && pos[1] == '%') {
            char *end = strstr(pos + 2, "%");
            if (end) {
                char var_name[MAX_VAR_NAME];
                int len = end - pos - 2;
                strncpy(var_name, pos + 2, len);
                var_name[len] = 0;
                
                Variable *var = find_var(interp, var_name);
                if (var) {
                    switch (var->type) {
                        case VAR_INT:
                            out += sprintf(out, "%ld", var->int_val);
                            break;
                        case VAR_FLOAT:
                            out += sprintf(out, "%g", var->float_val);
                            break;
                        case VAR_STRING:
                            out += sprintf(out, "%s", var->str_val);
                            break;
                        case VAR_BOOL:
                            *out++ = var->bool_val ? '+' : '-';
                            break;
                    }
                }
                pos = end + 1;
            } else {
                *out++ = *pos++;
            }
        } else {
            *out++ = *pos++;
        }
    }
    *out = 0;
    
    int len = strlen(result);
    if ((result[0] == '"' && result[len-1] == '"') || 
        (result[0] == '\'' && result[len-1] == '\'')) {
        memmove(result, result + 1, len - 2);
        result[len - 2] = 0;
    }
}

int evaluate_condition(Interpreter *interp, const char *condition) {
    char cond[256];
    strcpy(cond, condition);
    
    char *ops[] = {"==", "!=", ">=", "<=", ">", "<"};
    int op_count = 6;
    
    for (int i = 0; i < op_count; i++) {
        char *op_pos = strstr(cond, ops[i]);
        if (op_pos) {
            *op_pos = 0;
            char *left = cond;
            char *right = op_pos + strlen(ops[i]);
            trim(left);
            trim(right);
            
            double left_val = get_value(interp, left);
            double right_val = get_value(interp, right);
            
            switch (i) {
                case 0: return left_val == right_val;
                case 1: return left_val != right_val;
                case 2: return left_val >= right_val;
                case 3: return left_val <= right_val;
                case 4: return left_val > right_val;
                case 5: return left_val < right_val;
            }
        }
    }
    return 0;
}

void skip_to_endif(Interpreter *interp) {
    int depth = 1;
    while (interp->current_line < interp->line_count - 1 && depth > 0) {
        interp->current_line++;
        char *line = interp->lines[interp->current_line];
        if (strstr(line, "?(") && strstr(line, ")?")) {
            depth++;
        } else if (strcmp(line, "?end?") == 0) {
            depth--;
        }
    }
}

void execute_block_until(Interpreter *interp, const char *end_marker) {
    while (interp->current_line < interp->line_count) {
        char *line = interp->lines[interp->current_line];
        if (strcmp(line, end_marker) == 0) {
            return;
        }
        if (strlen(line) > 0 && strncmp(line, ";:", 2) != 0) {
            execute_line(interp, line);
        }
        interp->current_line++;
    }
}

void skip_to(Interpreter *interp, const char *marker) {
    while (interp->current_line < interp->line_count) {
        if (strcmp(interp->lines[interp->current_line], marker) == 0) {
            return;
        }
        interp->current_line++;
    }
}

void execute_line(Interpreter *interp, char *line) {
    if (strncmp(line, ">_", 2) == 0) {
        char text[512];
        strcpy(text, line + 2);
        trim(text);
        
        char *comment = strstr(text, ";:");
        if (comment) *comment = 0;
        trim(text);
        
        char output[512];
        parse_output(interp, text, output);
        printf("%s\n", output);
        return;
    }
    
    char loop_var[MAX_VAR_NAME];
    int limit;
    if (sscanf(line, "@(%s < %d)@", loop_var, &limit) == 2) {
        long zero = 0;
        set_var(interp, loop_var, VAR_INT, &zero);
        int loop_start = interp->current_line;
        
        while (get_value(interp, loop_var) < limit) {
            interp->current_line = loop_start + 1;
            execute_block_until(interp, "@end@");
            long current = (long)get_value(interp, loop_var) + 1;
            set_var(interp, loop_var, VAR_INT, &current);
        }
        
        skip_to(interp, "@end@");
        return;
    }
    
    char var_name[MAX_VAR_NAME], left[MAX_VAR_NAME], right[MAX_VAR_NAME], op;
    if (sscanf(line, "%s =$= %s %c %s", var_name, left, &op, right) == 4) {
        double left_val = get_value(interp, left);
        double right_val = get_value(interp, right);
        double result = 0;
        
        switch (op) {
            case '+': result = left_val + right_val; break;
            case '-': result = left_val - right_val; break;
            case '*': result = left_val * right_val; break;
            case '/': result = right_val != 0 ? left_val / right_val : 0; break;
        }
        
        if (result == (long)result) {
            long int_result = (long)result;
            set_var(interp, var_name, VAR_INT, &int_result);
        } else {
            set_var(interp, var_name, VAR_FLOAT, &result);
        }
        return;
    }
    
    if (line[0] == '?' && line[1] == '(' && strstr(line, ")?")) {
        char condition[256];
        sscanf(line, "?(%[^)])?", condition);
        if (!evaluate_condition(interp, condition)) {
            skip_to_endif(interp);
        }
        return;
    }
    
    char value_str[256];
    if (sscanf(line, "%s =$= %s", var_name, value_str) == 2) {
        long int_val = atol(value_str);
        set_var(interp, var_name, VAR_INT, &int_val);
    } else if (sscanf(line, "%s =$.$= %s", var_name, value_str) == 2) {
        double float_val = atof(value_str);
        set_var(interp, var_name, VAR_FLOAT, &float_val);
    } else if (sscanf(line, "%s =#= \"%[^\"]\"", var_name, value_str) == 2) {
        set_var(interp, var_name, VAR_STRING, value_str);
    } else if (sscanf(line, "%s =&= %s", var_name, value_str) == 2) {
        int bool_val = (value_str[0] == '+');
        set_var(interp, var_name, VAR_BOOL, &bool_val);
    }
}

void execute(Interpreter *interp, const char *code) {
    char *code_copy = strdup(code);
    char *line = strtok(code_copy, "\n");
    
    interp->line_count = 0;
    while (line && interp->line_count < MAX_LINES) {
        strcpy(interp->lines[interp->line_count], line);
        trim(interp->lines[interp->line_count]);
        interp->line_count++;
        line = strtok(NULL, "\n");
    }
    
    interp->current_line = 0;
    while (interp->current_line < interp->line_count) {
        char *current = interp->lines[interp->current_line];
        if (strlen(current) > 0 && strncmp(current, ";:", 2) != 0) {
            execute_line(interp, current);
        }
        interp->current_line++;
    }
    
    free(code_copy);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file.syb>\n", argv[0]);
        return 1;
    }
    
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Error opening file %s\n", argv[1]);
        return 1;
    }
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char *code = malloc(size + 1);
    fread(code, 1, size, file);
    code[size] = 0;
    fclose(file);
    
    Interpreter interp = {0};
    execute(&interp, code);
    
    free(code);
    return 0;
}