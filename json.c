#include "json.h"
#include "btree.h"
#include "libft.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

typedef struct {
    char* key;
    void* val;
} t_keyval;

json_object* new_json_object()
{
    json_object* obj = calloc(1, sizeof(json_object));
    obj->type = JSON_OBJECT;
    obj->value = NULL;
    return obj;
}

void delete_keyval(void* kv)
{
    free(((t_keyval*)kv)->key);
    delete_json(((t_keyval*)kv)->val);
    free(kv);
}

void delete_json_object(json_object* obj)
{
    btree_apply_infix(obj->value, delete_keyval);
    btree_clear(&obj->value);
    free(obj);
}

json_array* new_json_array()
{
    json_array* obj = calloc(1, sizeof(json_array));
    obj->type = JSON_ARRAY;
    obj->value = NULL;
    return obj;
}

void delete_json_array(json_array* obj)
{
    ft_list_foreach(obj->value, delete_json);
    ft_list_clear(&obj->value);
    free(obj);
}

json_string* new_json_string(const char* str)
{
    json_string* obj = calloc(1, sizeof(json_string));
    obj->type = JSON_STRING;
    obj->value = ft_strdup(str);
    return obj;
}

void delete_json_string(json_string* obj)
{
    free(obj->value);
    free(obj);
}

void delete_json(void* json)
{
    json_unknown* obj = json;
    switch (obj->type)
    {
    case JSON_OBJECT:
        delete_json_object((json_object*)obj);
        break;
    case JSON_ARRAY:
        delete_json_array((json_array*)obj);
        break;
    case JSON_STRING:
        delete_json_string((json_string*)obj);
        break;
    default:
        free(obj);
    }
}

static int cmp(void* l, void* r)
{
    t_keyval* tl = (t_keyval*)l;
    t_keyval* tr = (t_keyval*)r;
    return ft_strcmp(tl->key, tr->key);
}

void json_object_insert(json_object* self, const char* key, void* val)
{
    t_keyval* tmp = calloc(1, sizeof(t_keyval));
    tmp->key = ft_strdup(key);
    tmp->val = val;
    btree_insert(&self->value, tmp, cmp);
}

void* json_object_find(json_object* self, char* key)
{
    t_btree* res = btree_find(self->value, key, &ft_strcmp);
    if (res) {
        return res->value;
    }
    return NULL;
}

///////////////////////////////////////////////
///////////////////////////////////////////////
///////////////////////////////////////////////

typedef enum s_tok_type {
    UNKNOWN,
    OBJ_OPEN,
    OBJ_CLOSE,
    ARR_OPEN,
    ARR_CLOSE,
    STR,
    COL,
    COM
} t_tok_type;

typedef enum s_error {
    SUCCESS,
    SYNTAX_ERROR,
    UNKNOWN_TOKEN,
    BAD_ALLOC,
    END
} t_error;

static const int MAX_VAL_SIZE = 256;
typedef struct s_token {
    t_tok_type type;
    char value[MAX_VAL_SIZE];
} t_token;

static struct {
    const char* start_pos;
    const char* current_pos;
} ctx;

static int is_delims(char c) {
    char* delims = " \n\t\r";
    return ft_strchr(delims, c) != NULL;
}

static int get_token(t_token* token)
{
    if (*ctx.current_pos == '\0') {
        return 0;
    }

    /* skip spaces */
    while (*ctx.current_pos && is_delims(*ctx.current_pos)) {
        ++ctx.current_pos;
    }

    ft_memset(token->value, 0, MAX_VAL_SIZE);
    switch (*ctx.current_pos)
    {
    case '{':
        token->type = OBJ_OPEN;
        token->value[0] = *ctx.current_pos++;
        break;
    case '}':
        token->type = OBJ_CLOSE;
        token->value[0] = *ctx.current_pos++;
        break;
    case '[':
        token->type = ARR_OPEN;
        token->value[0] = *ctx.current_pos++;
        break;
    case ']':
        token->type = ARR_CLOSE;
        token->value[0] = *ctx.current_pos++;
        break;
    case '"':
        token->type = STR;
        int i = 0;
        while (*(++ctx.current_pos) && *ctx.current_pos != '"') {
            token->value[i++] = *ctx.current_pos;
        }
        if (*ctx.current_pos)
            ctx.current_pos++;
        break;
    case ':':
        token->type = COL;
        token->value[0] = *ctx.current_pos++;
        break;
    case ',':
        token->type = COM;
        token->value[0] = *ctx.current_pos++;
        break;
    default:
        break;
    }
    return 1;
}

static int json_parse_private(void** obj);

static int parse_object(void** obj)
{
    json_object* n_obj = new_json_object();
    t_token token;
    t_error err = SUCCESS;
    while (token.type != OBJ_CLOSE) {
        t_token token_key;
        if (get_token(&token_key) == 0 || token_key.type != STR)
        {
            err = SYNTAX_ERROR;
            break;
        }
        if (get_token(&token) == 0 || token.type != COL)
        {
            err = SYNTAX_ERROR;
            break;
        }

        json_unknown* next_obj = NULL;
        int e = json_parse_private(&next_obj);
        if (e != SUCCESS) {
            err = e;
            break;
        }

        json_object_insert(n_obj, token_key.value, next_obj);
        if (get_token(&token) == 0 || (token.type != COM && token.type != OBJ_CLOSE))
        {
            err = SYNTAX_ERROR;
            break;
        }
    }
    if (err != SUCCESS) {
        delete_json_object(n_obj);
        return err;
    }
    *obj = n_obj;
    return SUCCESS;
}

static void* parse_array()
{

}

static int parse_string(void** obj, t_token* token)
{
    json_string* n_str = new_json_string(token->value);
    if (n_str == NULL) {
        return BAD_ALLOC;
    }
    *obj = n_str;
    return SUCCESS;
}

static int json_parse_private(void** obj)
{
    t_token token;
    json_unknown* unk_obj = NULL;
    while (get_token(&token)) {
        switch (token.type)
        {
        case OBJ_OPEN:
            return parse_object();
        case ARR_OPEN:
            return parse_array();
        case STR:
            return parse_string(&token);
        default:
            break;
        }
    }
    return NULL;
}

int json_parse(void** obj, const char* text)
{
    ctx.start_pos = ctx.current_pos = text;
    return json_parse_private(obj);
}